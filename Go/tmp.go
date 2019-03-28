package main

import (
	"fmt"
)

func main() {
	s := fmt.Sprintf("%d", 123)
	fmt.Printf("%T => %v\n", s, s)
	fmt.Println("-5 % 10 =", -5%10)
	fmt.Println("5 % -10 =", 5%-10)
}
