package cli

import (
	"reflect"
	"runtime"
	"strings"
)

// 简易命令由框架提供通用上下文结构
type FnCommand struct {
	fn      func([]string) int
	options map[string]*Opt
	args    []*Arg
	doc     string
	brief   string
}

// 运行命令，args 是剔除全局选项和命令选项后的参数列表
func (cmd *FnCommand) Run(args []string) int {
	return cmd.fn(args)
}

// 为命令设置详细描述
func (cmd *FnCommand) Doc(doc string) *FnCommand {
	cmd.doc = doc
	return cmd
}

// 为命令设置简要描述
func (cmd *FnCommand) Brief(brief string) *FnCommand {
	cmd.brief = brief
	return cmd
}

// 为命令绑定选项
func (cmd *FnCommand) Use(opt *Opt) *FnCommand {
	for _, flag := range opt.flags {
		cmd.options[flag] = opt
	}
	return cmd
}

// 为命令添加参数
func (cmd *FnCommand) Arg(name string) *FnCommand {
	cmd.args = append(cmd.args, Argument(name))
	return cmd
}

// 为命令添加可选参数
func (cmd *FnCommand) Opt(name string) *FnCommand {
	cmd.args = append(cmd.args, Optional(name))
	return cmd
}

// 为命令添加可重复参数
func (cmd *FnCommand) More() *FnCommand {
	cmd.args = append(cmd.args, More())
	return cmd
}

// 为命令添加必须剩余参数
func (cmd *FnCommand) Some(name string) *FnCommand {
	cmd.args = append(cmd.args, Some(name))
	return cmd
}

// 注册命令，fn 为命令的执行函数
func CmdFn(fn func([]string) int) *FnCommand {
	name := runtime.FuncForPC(reflect.ValueOf(fn).Pointer()).Name()
	name = name[strings.LastIndex(name, ".")+1:]
	cmd := &FnCommand{fn: fn, options: make(map[string]*Opt)}
	commands[camelify(name)] = cmd
	return cmd
}

// 注册主要命令，fn 为命令的执行函数
func MainFn(fn func([]string) int) *FnCommand {
	name := runtime.FuncForPC(reflect.ValueOf(fn).Pointer()).Name()
	name = name[strings.LastIndex(name, ".")+1:]
	cmd := &FnCommand{fn: fn, options: make(map[string]*Opt)}
	main = camelify(name)
	commands[main] = cmd
	return cmd
}
