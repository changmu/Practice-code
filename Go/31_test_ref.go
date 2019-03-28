package main

import (
	"fmt"
)

func main() {
	s := []int{
		1, 2, 3,
	}
	fmt.Println(s)

	f0(s)

	fmt.Println(s)

	f1 := func() {
		s[0] *= 10
		s = append(s, 4)
	}
	f1()

	fmt.Println(s)
}

func f0(s []int) {
	s[0] *= 10
	s = append(s, 4)
}
