package cli

type complete struct {
}

func (cmd *complete) Run(args []string) int {
	return 0
}

var Complete = &complete{}
