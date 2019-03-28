package main

import (
	"fmt"
	"os"
)

func main() {
	var s, sep string
	for idx, arg := range os.Args[1:] {
		s += sep + arg
		sep = " "
		fmt.Println("idx:", idx, "arg:", arg)
	}
	fmt.Println(s)
	fmt.Println(os.Args[1:])
}
