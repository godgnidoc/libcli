package cli

type version struct {
	Brief   string
	Verbose Opt
}

func (cmd *version) Run(args []string) int {
	if cmd.Verbose.IsOn() {
		println(AppName, " ", AppVersion, " by ", AppAuthor)
		return 0
	}
	println(AppVersion)
	return 0
}

var AppVersion string
var AppAuthor string
var Version = &version{
	Brief: "Show version information",
	Verbose: Opt{
		flags: []string{"-v", "--verbose"},
		doc:   "Show verbose output",
	},
}
