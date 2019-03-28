package main

import (
	"fmt"
)

func main() {
	natural_chan := make(chan int)
	square_chan := make(chan int)

	// counter
	go func(out chan<- int) {
		for i := 0; i < 100; i++ {
			out <- i
		}
		close(out)
	}(natural_chan)

	// square
	go func(in <-chan int, out chan<- int) {
		for x := range in {
			out <- x * x
		}
		close(out)
	}(natural_chan, square_chan)

	// print
	func(in <-chan int) {
		for v := range in {
			fmt.Printf("%d ", v)
		}
	}(square_chan)

	fmt.Println()
}
