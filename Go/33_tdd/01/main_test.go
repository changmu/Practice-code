package main

import (
	"testing"
)

func TestAdd(t *testing.T) {
	var tests = []struct {
		a, b int
		want int
	}{
		{1, 2, 3},
		{10, 29, 39},
		{10, 29, 1},
		{120, 29, 1},
	}

	for _, test := range tests {
		if got := Add(test.a, test.b); got != test.want {
			// assertEqual(Add(test.a, test.b), test.want)
			t.Errorf("Add(%v,%v)=%v, want %v", test.a, test.b, got, test.want)
		}
	}
}
