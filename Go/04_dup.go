// 读取文件或者标准输入，统计汗文本个数
package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	counts := make(map[string]int)
	files := os.Args[1:]
	if len(files) == 0 {
		countLines(os.Stdin, counts)
	} else {
		for _, fname := range files {
			f, err := os.Open(fname)
			if err != nil {
				fmt.Fprintf(os.Stderr, "dup err, open %s failed, err:%q\n", fname, err)
				continue
			}
			countLines(f, counts)
			f.Close()
		}
	}

	for str, times := range counts {
		if times > 1 {
			fmt.Printf("str:%s, times:%d\n", str, times)
		}
	}
}

func countLines(f *os.File, counts map[string]int) {
	input := bufio.NewScanner(f)
	for input.Scan() {
		counts[input.Text()]++
	}
}
