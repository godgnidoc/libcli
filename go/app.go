package cli

import "os"

// 将 optional_args 用作命令行参数，开始执行命令行应用程序
//
// optional_args 是命令行参数列表，省略则自动使用 os.Args[1:]
func Run(optional_args ...[]string) {
	args := os.Args[1:]
	if len(optional_args) > 0 {
		args = optional_args[0]
	}

	// 分割短选项
	args = splitShorts(args)

	// 扫描并消费全局选项
	args = consumeOptions(global_options, args)
	if args == nil {
		os.Exit(-1)
	}

	// 查找并确定要执行的命令
	args, selection := which(args)
	if len(selection) == 0 {
		println("Command not found")
		os.Exit(-1)
	} else if len(selection) > 1 {
		println("Ambiguous command")
		os.Exit(-1)
	}
	cmd := commands[selection[0]]
	if cmd == nil {
		println("Command not found")
		os.Exit(-1)
	}

	// 扫描并消费命令选项
	cmd_opts := optionsOf(cmd)
	args = consumeOptions(cmd_opts, args)
	if args == nil {
		os.Exit(-1)
	}

	// 从命令行消费具名参数
	args = consumeArguments(args, cmd)
	if args == nil {
		os.Exit(-1)
	}

	// 执行命令
	os.Exit(cmd.Run(args))
}

// 校验应用定义是否正确
func ValidateApp() {
	for _, opt := range global_options {
		validateOption(opt)
	}

	for _, cmd := range commands {
		validateCommand(cmd)
	}
}
