package main

import (
    "fmt"
    // "os"
    "flag"
    "strings"
)

var s = flag.String("s", " ", "string arg")
var n = flag.Bool("b", false, "boolean var.")

func main() {
    flag.Parse()
    fmt.Println(strings.Join(flag.Args(), *s))
    if !*n {
        fmt.Println()
    }
}
