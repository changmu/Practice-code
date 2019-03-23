package main

import (
	"testing"
)

func TestDollar(t *testing.T) {
	tests := []struct {
		input int
		want  Dollar
	}{
		{1, Dollar{2}},
		{1, Dollar{2}},
	}

	for _, test := range tests {
		d := Dollar{
			amount: test.input,
		}
		if got := d.Times(test.input); got != test.want {
			t.Errorf("Times(%v)=%v,want:%v", test.input, got, test.want)
		}
	}
}
