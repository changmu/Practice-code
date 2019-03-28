package main

import "fmt"

const (
	_ = 1 << (iota * 10)
	_
	_
	_
	_
	_
	_
	_
	_
	_
	k1
)

func main() {
	a, b := 1, 2
	const k_v = 123
	// a, b = b + 1, a + 2
	println(a, b)
	println("v_v:", k1)
	fmt.Println("Hello, 世界。", "你好，世界。", 'a', 'b')
}
