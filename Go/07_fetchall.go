package main

import (
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"strings"
	"time"
)

func main() {
	start_time := time.Now()
	ch := make(chan string)
	for _, url := range os.Args[1:] {
		if !strings.HasPrefix(url, "http://") {
			url = "http://" + url
		}
		go fetch(url, ch)
	}

	for range os.Args[1:] {
		fmt.Println(<-ch)
	}

	fmt.Printf("Total time used:%.2fs\n", time.Since(start_time).Seconds())
}

func fetch(url string, ch chan<- string) {
	start_time := time.Now()

	rsp, err := http.Get(url)
	if err != nil {
		ch <- fmt.Sprint(err)
		return
	}

	nbytes, err := io.Copy(ioutil.Discard, rsp.Body)
	rsp.Body.Close()
	if err != nil {
		ch <- fmt.Sprintf("while reading [%s], err:%q", url, err)
		return
	}

	ch <- fmt.Sprintf("time used:%.2fs, url:[%s], bytes:%d", time.Since(start_time).Seconds(), url, nbytes)
}
