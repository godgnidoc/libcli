package cli

import (
	"reflect"
)

// 选项定义
//
// 作为指令上下文的公开成员变量时可以被框架自动识别
type Opt struct {
	// 选项描述
	doc string

	// 选项标志，至少一个
	//
	// 短选项以单破折号开头，长选项以双破折号开头
	flags []string

	// 选项是否必须
	required bool

	// 选项是否可重复
	repeatable bool

	// 选项参数个数
	args []*Arg

	// 选项每次出现的参数，由框架自动设置
	repeats [][]string
}

// 选项是否被指定
func (opt *Opt) IsOn() bool {
	return len(opt.repeats) > 0
}

// 获取选项携带的所有参数
func (opt *Opt) AllArgs() []string {
	args := make([]string, 0, len(opt.repeats)*len(opt.args))
	for _, repeat := range opt.repeats {
		args = append(args, repeat...)
	}
	return args
}

// 获取选项被指定的次数
func (opt *Opt) Times() int {
	return len(opt.repeats)
}

// 获取选项第 i 次被指定时携带的参数
func (opt *Opt) ArgsAt(i int) []string {
	return opt.repeats[i]
}

// 设置选项描述
func (opt *Opt) Doc(doc string) *Opt {
	opt.doc = doc
	return opt
}

// 设置选项必须
func (opt *Opt) Required() *Opt {
	opt.required = true
	return opt
}

// 设置选项可重复
func (opt *Opt) Repeatable() *Opt {
	opt.repeatable = true
	return opt
}

// 设置选项参数
func (opt *Opt) Arg(name string) *Opt {
	opt.args = append(opt.args, Argument(name))
	return opt
}

// 设置选项为全局选项
func (opt *Opt) Global() *Opt {
	for _, flag := range opt.flags {
		global_options[flag] = opt
	}
	return opt
}

// 创建选项对象，至少需要一个标志
//
// 可以直接创建在命令上下文中作为公开成员变量
//
// 也可以通过命令对象的 Use 方法绑定到命令上下文
func Option(flag string, more_flags ...string) *Opt {
	return &Opt{
		flags: append([]string{flag}, more_flags...),
	}
}

func optionsOf(cmd Command) map[string]*Opt {
	switch simple := cmd.(type) {
	case *FnCommand:
		return simple.options
	}

	options := make(map[string]*Opt, 0)
	cmdv := reflect.ValueOf(cmd).Elem()

	for i := 0; i < cmdv.NumField(); i++ {
		field := cmdv.Field(i).Interface()
		if opt, ok := field.(*Opt); ok {
			if len(opt.flags) == 0 {
				panic("option must have at least one flag")
			}

			for _, flag := range opt.flags {
				options[flag] = opt
			}
		}
	}

	return options
}

func validateOption(opt *Opt) {
	if len(opt.flags) == 0 {
		panic("option must have at least one flag")
	}

	for _, flag := range opt.flags {
		if len(flag) < 2 {
			panic("flag must have at least two characters")
		}

		if flag == "--" {
			panic("flag must not be '--'")
		}

		if flag[0] != '-' {
			panic("flag must start with '-'")
		}

		if len(flag) > 2 && flag[1] != '-' {
			panic("long flag must start with '--'")
		}
	}

	validateArguments(opt.args)
}

func consumeOptions(opts map[string]*Opt, args []string) []string {
	rest := make([]string, 0)

	for _, opt := range opts {
		opt.repeats = make([][]string, 0)
	}

	for i := 0; i < len(args); i++ {
		key := args[i]
		if key == "--" {
			rest = append(rest, args[i:]...)
			break
		}

		opt, ok := opts[key]
		if !ok {
			rest = append(rest, key)
			continue
		}

		orgs := make([]string, 0, len(opt.args))
		for j := 0; j < len(opt.args); j++ {
			i++
			if i >= len(args) {
				println("option", key, "need", len(opt.args), "argument(s)")
				return nil
			}

			orgs = append(orgs, args[i])
		}

		opt.repeats = append(opt.repeats, orgs)
	}

	for flag, opt := range opts {
		if opt.required && !opt.IsOn() {
			println("option", flag, "is required")
			return nil
		}

		if !opt.repeatable && opt.Times() > 1 {
			println("option", flag, "can only be used once")
			return nil
		}
	}

	return rest
}

var global_options = make(map[string]*Opt)
