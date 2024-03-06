package cli

import (
	"reflect"
	"strings"
)

// Command 是一个命令行命令的接口
//
// 可以为命令定义 Brief 和 Doc 参数来提供命令的简要和详细描述
//
// 可以为命令上下文定义若干 Option 字段来设计指令选项
type Command interface {
	// 运行命令，args 是剔除全局选项和命令选项后的参数列表
	Run(args []string) int
}

// 注册命令，cmd 对象被用作命令上下文来查找命令选项
//
// 命令的类型名会按照驼峰被拆解，转换为全小写的单词串用作命令名
//
// ！！！注意确保使用指针传参
func Cmd(cmd Command) {
	name := reflect.TypeOf((cmd)).Elem().Name()
	commands[camelify(name)] = cmd
}

// 注册主要命令，cmd 对象被用作命令上下文来查找命令选项
//
// 命令的类型名会按照驼峰被拆解，转换为全小写的单词串用作命令名
// 当命令行参数未指定命令时，会执行主要命令
//
// ！！！注意确保使用指针传参
func Main(cmd Command) {
	name := reflect.TypeOf((cmd)).Elem().Name()
	main = camelify(name)
	commands[main] = cmd
}

func validateCommand(cmd Command) {
	for _, opt := range optionsOf(cmd) {
		validateOption(opt)
	}

	args := argumentsOf(cmd)
	validateArguments(args)
}

func briefOf(cmd Command) string {
	switch simple := cmd.(type) {
	case *FnCommand:
		return simple.brief
	}

	brief := reflect.ValueOf(cmd).Elem().FieldByName("Brief")
	str, ok := brief.Interface().(string)
	if ok {
		return str
	}
	return ""
}

func docOf(cmd Command) string {
	switch simple := cmd.(type) {
	case *FnCommand:
		return simple.doc
	}

	doc := reflect.ValueOf(cmd).Elem().FieldByName("Doc")
	str, ok := doc.Interface().(string)
	if ok {
		return str
	}
	return ""
}

func which(args []string) (rest []string, selection []string) {
	rest = make([]string, 0)
	selection = make([]string, 0, len(commands))
	for key := range commands {
		selection = append(selection, key)
	}

	accept := ""
	for i := 0; i < len(args); i++ {
		test := accept + camelify(args[i])
		narrow := make([]string, 0)
		for j := range selection {
			if strings.HasPrefix(selection[j], test) {
				narrow = append(narrow, selection[j])
			}
		}

		if len(narrow) == 0 {
			rest = append(rest, args[i])
			continue
		}

		selection = narrow
		accept = test
	}

	if len(selection) > 1 && len(selection) == len(commands) && main != "" {
		selection = []string{main}
	}

	return rest, selection
}

var commands = make(map[string]Command)
var main string
