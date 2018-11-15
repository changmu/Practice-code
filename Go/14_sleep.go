package main

import (
    "fmt"
    "flag"
    "time"
)

var g_period = flag.Duration("period", 1*time.Second, "sleep period")

func main() {
    flag.Parse()
    begin_time := time.Now()
    fmt.Printf("Sleeping for %v\n", *g_period)
    time.Sleep(*g_period)
    fmt.Printf("sleep done:%v\n", time.Since(begin_time))
}
