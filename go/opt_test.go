package cli

import "testing"

type options struct {
	Verbose *Opt
	Action  *Opt
}

func (cmd *options) Run(args []string) int {
	return 0
}

func Test_optionsOf(t *testing.T) {
	cmd := &options{
		Verbose: &Opt{
			flags: []string{"-v", "--verbose"},
		},
		Action: &Opt{
			flags: []string{"-a", "--action"},
		},
	}
	opts := optionsOf(cmd)
	if len(opts) != 4 {
		t.Error("optionsOf failed")
	}
	if v, found := opts["-v"]; !found || v != cmd.Verbose {
		t.Error("optionsOf failed")
	}
	if v, found := opts["--verbose"]; !found || v != cmd.Verbose {
		t.Error("optionsOf failed")
	}
	if v, found := opts["-a"]; !found || v != cmd.Action {
		t.Error("optionsOf failed")
	}
	if v, found := opts["--action"]; !found || v != cmd.Action {
		t.Error("optionsOf failed")
	}
}

func Test_validateFlag(t *testing.T) {
	cases := []struct {
		in    string
		panic bool
	}{
		{"", true},
		{"-", true},
		{"--", true},
		{"ab", true},
		{"-ab", true},
		{"--ab", false},
		{"-a", false},
	}

	for _, c := range cases {
		t.Run(c.in, func(t *testing.T) {
			defer func() {
				if r := recover(); (r != nil) != c.panic {
					t.Errorf("validateFlag failed")
				}
			}()
			validateOption(&Opt{flags: []string{c.in}})
		})
	}
}
