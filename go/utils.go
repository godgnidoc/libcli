package cli

import "strings"

func camelify(name string) string {
	return strings.ToUpper(name[:1]) + name[1:]
}

func splitcamel(name string) string {
	words := make([]string, 0)
	start := 0
	for i := 1; i < len(name); i++ {
		if name[i] >= 'A' && name[i] <= 'Z' {
			words = append(words, name[start:i])
			start = i
		}
	}
	words = append(words, name[start:])
	return strings.ToLower(strings.Join(words, " "))
}

func splitShorts(args []string) []string {
	rest := make([]string, 0, len(args))
	for i := 0; i < len(args); i++ {
		if args[i] == "--" {
			rest = append(rest, args[i:]...)
			break
		}

		if len(args[i]) <= 2 || args[i][0] != '-' || args[i][1] == '-' {
			rest = append(rest, args[i])
			continue
		}

		for j := 1; j < len(args[i]); j++ {
			rest = append(rest, "-"+string(args[i][j]))
		}
	}
	return rest
}
