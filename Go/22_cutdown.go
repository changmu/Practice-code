package main

import (
    "fmt"
    "time"
    "os"
)

func main() {
    tick := time.Tick(1 * time.Second)

    abort := make(chan struct{})
    go func() { os.Stdin.Read(make([]byte, 1)); abort <- struct{}{} }()

    ch := make(chan int, 1)
    for i := 0; i < 10; i++ {
        select {
        case x := <- ch:
            fmt.Println("x=", x)
        case ch <- i:
        }
    }

    for cutdown := 5; cutdown > 0; {
        select {
        case <- tick:
            fmt.Println(cutdown)
            cutdown--
        case <- abort:
            fmt.Println("Abort launched!")
            return
        }
    }

    fmt.Println("Launch!")
}
