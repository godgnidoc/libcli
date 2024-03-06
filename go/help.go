package cli

import (
	"strings"
)

type help struct {
	More  *Arg
	Brief string
	Doc   string
}

func (cmd *help) Run(args []string) int {
	if len(args) > 0 {
		rest, selection := which(args)
		if len(selection) != 1 || len(rest) != 0 {
			println("Help: Command '" + strings.Join(args, " ") + "' does not exist")
			return -1
		}
		cmd := commands[selection[0]]
		documentCommand(cmd, false)
	} else {
		if main != "" {
			main := commands[main]
			documentCommand(main, true)
		} else {
			println("Usage: " + AppName + " [global-options] <command> [command-options] [arguments]")
			println()
			println(AppUsage)
			println()
		}

		println("Commands:")
		briefCommands()
	}

	if len(global_options) > 0 {
		println("Global Options:")
		briefOptions(global_options)
		println()
	}

	return 0
}

func documentCommand(cmd Command, omit_main bool) {
	print("Usage: " + AppName + " ")

	for key, value := range commands {
		if value != cmd {
			continue
		}

		if key != main || !omit_main {
			print(splitcamel(key) + " ")
		}
		break
	}

	options := optionsOf(cmd)
	if len(options) > 0 {
		print("[command-options] ")
	}

	for _, arg := range argumentsOf(cmd) {
		print(arg.name + " ")
	}

	println()
	println()

	doc := docOf(cmd)
	if doc == "" {
		doc = briefOf(cmd)
	}

	if doc != "" {
		println(doc)
		println()
	}

	if len(options) > 0 {
		println("Options:")
		briefOptions(options)
		println()
	}
}

func briefCommands() {
	max_width := 0
	for name := range commands {
		max_width = max(max_width, len(name))
	}

	start := (max_width + 6 + 7) / 8 * 8
	for name, cmd := range commands {
		brief := briefOf(cmd)
		if brief == "" {
			println("  " + splitcamel(name))
		} else {
			doc := "  " + splitcamel(name) + strings.Repeat(" ", start-len(name)) + brief
			doc = strings.ReplaceAll(doc, "\n", "\n"+strings.Repeat(" ", start))
			println(doc)
		}
	}
}

func briefOptions(options map[string]*Opt) {
	sorted := make(map[*Opt]string)
	max_width := 0
	for flag, opt := range options {
		if sorted[opt] == "" {
			sorted[opt] = "  " + flag
		} else {
			sorted[opt] += ", " + flag
		}
		max_width = max(max_width, len(sorted[opt]))
	}

	start := (max_width + 4 + 7) / 8 * 8
	for opt, flags := range sorted {
		gap := start - len(flags)
		doc := flags + strings.Repeat(" ", gap) + opt.doc
		doc = strings.ReplaceAll(doc, "\n", "\n"+strings.Repeat(" ", start))
		println(doc)
	}
}

var AppName = "App name here"
var AppUsage = "Describe usage here when no main command is specified"
var Help = &help{
	More:  More(),
	Brief: "Show help",
	Doc:   "Show help, specify a command to show detailed help",
}
