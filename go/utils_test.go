package cli

import (
	"reflect"
	"testing"
)

func Test_camelify(t *testing.T) {
	cases := []struct {
		in, want string
	}{
		{"Test", "Test"},
		{"test", "Test"},
		{"TEST", "TEST"},
		{"tEST", "TEST"},
		{"TestCase", "TestCase"},
		{"testcase", "Testcase"},
		{"testCase", "TestCase"},
		{"Testcase", "Testcase"},
	}
	for _, c := range cases {
		got := camelify(c.in)
		if got != c.want {
			t.Errorf("camelify(%q) == %q, want %q", c.in, got, c.want)
		}
	}
}

func Test_splitcamel(t *testing.T) {
	cases := []struct {
		in, want string
	}{
		{"Test", "test"},
		{"test", "test"},
		{"TEST", "t e s t"},
		{"tEST", "t e s t"},
		{"TestCase", "test case"},
		{"testcase", "testcase"},
		{"testCase", "test case"},
		{"Testcase", "testcase"},
	}
	for _, c := range cases {
		got := splitcamel(c.in)
		if got != c.want {
			t.Errorf("splitcamel(%q) == %q, want %q", c.in, got, c.want)
		}
	}
}

func Test_splitShorts(t *testing.T) {
	cases := []struct {
		in, want []string
	}{
		{[]string{"-a", "-bc", "--", "-d", "-ef"}, []string{"-a", "-b", "-c", "--", "-d", "-ef"}},
		{[]string{"-a", "-bc", "-d", "-ef"}, []string{"-a", "-b", "-c", "-d", "-e", "-f"}},
		{[]string{"-a", "-bc", "-d", "-ef", "--"}, []string{"-a", "-b", "-c", "-d", "-e", "-f", "--"}},
	}
	for _, c := range cases {
		got := splitShorts(c.in)
		if !reflect.DeepEqual(got, c.want) {
			t.Errorf("splitShorts(%q) == %q, want %q", c.in, got, c.want)
		}
	}
}
