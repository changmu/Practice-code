package main

import (
    "fmt"
    "os"
    "strings"
    "io/ioutil"
)

func main() {
    counts := make(map[string]int)
    for _, fname := range os.Args[1:] {
        data, err := ioutil.ReadFile(fname)
        if err != nil {
            fmt.Fprintf(os.Stderr, "%q\n", err)
            continue
        }
        for _, line := range strings.Split(string(data), "\n") {
            counts[line]++
        }
    }

    for str, times := range counts {
        if times > 1 {
            fmt.Printf("str:%v times:%v\n", str, times)
        }
    }
}
