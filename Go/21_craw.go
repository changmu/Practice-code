package main

import (
	"fmt"
	"net"
	"os"
)

func main() {
	worklist := make(chan []string)  // 可能有重复的URL列表
	unseenLinks := make(chan string) // 未访问过的URL列表

	go func() { worklist <- os.Args[1:] }()

	for i := 0; i < 20; i++ {
		go func() {
			for link := range unseenLinks {
				foundLinks := craw(link)
				go func() { worklist <- foundLinks }()
			}
		}()
	}

	seen := make(map[string]bool)
	for list := range worklist {
		for _, link := range list {
			if !seen[link] {
				seen[link] = true
				unseenLinks <- link
			}
		}
	}
}
