package main

import (
	"fmt"
	"time"
)

func main() {
	time_begin := time.Now()
	go spinner(100 * time.Millisecond)
	const N = 45
	fibN := fib(N)
	fmt.Printf("\rfib(%d)=%d,time used:[%.2fs]\n", N, fibN, time.Since(time_begin).Seconds())
}

func spinner(delay time.Duration) {
	for {
		for _, c := range `-\|/` {
			fmt.Printf("\r%c", c)
			time.Sleep(delay)
		}
	}
}

func fib(n int) int {
	if n < 2 {
		return n
	}
	return fib(n-1) + fib(n-2)
}
