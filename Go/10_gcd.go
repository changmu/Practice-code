package main

import (
    "fmt"
)

func gcd(x, y int) int {
    for y != 0 {
        x, y = y, x % y
    }
    return x
}

func fib(n int) int {
    x, y := 0, 1
    for i := 0; i < n; i++ {
        x, y = y, x + y
    }
    return x
}

func main() {
    fmt.Println(gcd(15, 20))
    fmt.Println(gcd(9, 6))
    fmt.Println(gcd(9, 2))

    fmt.Println(fib(0))
    fmt.Println(fib(1))
    fmt.Println(fib(2))
    fmt.Println(fib(100))
    fmt.Println(fib(100000))
}
