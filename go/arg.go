package cli

import (
	"fmt"
	"reflect"
	"strings"
)

type Arg struct {
	name  string
	value []string
}

func (arg *Arg) HasValue() bool {
	return len(arg.value) > 0
}

// 获取存入参数的值，使用空格拼接成字符串
func (arg *Arg) Value() string {
	return strings.Join(arg.value, " ")
}

// 获取存入参数的值，返回原始字符串数组
func (arg *Arg) Values() []string {
	return arg.value
}

func (arg *Arg) isOptional() bool {
	return arg.name == "..." || arg.name[0] == '['
}

// 创建一个必须的参数
func Argument(name string) *Arg {
	return &Arg{
		name:  fmt.Sprintf("<%s>", name),
		value: []string{},
	}
}

// 创建一个可选的参数
func Optional(name string) *Arg {
	return &Arg{
		name:  fmt.Sprintf("[%s]", name),
		value: []string{},
	}
}

// 创建剩余参数
func More() *Arg {
	return &Arg{
		name:  "...",
		value: []string{},
	}
}

// 创建必须参数和剩余参数
func Some(name string) *Arg {
	return &Arg{
		name:  name + "...",
		value: []string{},
	}
}

func argumentsOf(target any) []*Arg {
	switch known := target.(type) {
	case *FnCommand:
		return known.args
	case *Opt:
		return known.args
	}

	v := reflect.ValueOf(target).Elem()
	args := make([]*Arg, 0, v.NumField())
	for i := 0; i < v.NumField(); i++ {
		field := v.Field(i)
		if arg, ok := field.Interface().(*Arg); ok {
			args = append(args, arg)
		}
	}
	return args
}

func validateArguments(args []*Arg) {
	optional, more := false, false

	for _, arg := range args {
		if more {
			panic("more argument must be the last one")
		}

		if arg.isOptional() {
			optional = true
		} else if optional {
			panic("optional argument must be the last one")
		}

		if strings.HasSuffix(arg.name, "...") {
			more = true
		}
	}
}

func consumeArguments(args []string, cmd Command) (rest []string) {
	args = clearArgs(args)
	crgs := argumentsOf(cmd)
	_, simple := cmd.(*FnCommand)

	for len(crgs) > 0 {
		crg := crgs[0]
		if crg.name == "..." {
			crg.value = args
			break
		}

		if len(args) == 0 {
			if !crg.isOptional() {
				println("missing required argument: " + crg.name)
				return nil
			}
			continue
		}

		if strings.HasSuffix(crg.name, "...") {
			crg.value = args
			break
		}

		crg.value = []string{args[0]}
		crgs = crgs[1:]
		if simple {
			continue
		}

		args = args[1:]
	}

	return args
}

func clearArgs(args []string) []string {
	rest := make([]string, 0)
	for i := 0; i < len(args); i++ {
		if args[i] == "--" {
			rest = append(rest, args[i:]...)
			break
		}
		rest = append(rest, args[i])
	}
	return rest
}
