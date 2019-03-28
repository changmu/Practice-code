package main

import (
	"fmt"
)

func main() {
	var mp = make(map[string]int)
	fmt.Printf("mp.size=%v\n", len(mp))
	if mp["abc"] == 2 {

	}
	fmt.Printf("mp.size=%v\n", len(mp))
}
