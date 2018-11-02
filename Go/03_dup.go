package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
    a := 14
    s := "abc"
    fmt.Printf("%#X, T=%T, v=%v c=%c\n", a, a, a, a, s)

	counts := make(map[string]int)
	input := bufio.NewScanner(os.Stdin)
	for input.Scan() {
		counts[input.Text()]++
	}

	for str, n := range counts {
		if n > 1 {
			fmt.Printf("str:[%s], count:[%d]\n", str, n)
		}
	}
}
