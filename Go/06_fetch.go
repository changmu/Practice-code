package main

import (
	"fmt"
	"io"
	"os"
	"strings"
	// "io/ioutil"
	"net/http"
)

/*
// asd
*/
func main() {
	for _, url := range os.Args[1:] {
		if !strings.HasPrefix(url, "http") {
			url = "http://" + url
			fmt.Println("add http://")
		}
		rsp, err := http.Get(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "err:%q\n", err)
			os.Exit(1)
		}

		fmt.Printf("status:%q\n", rsp.Status)

		// data, err := ioutil.ReadAll(rsp.Body)
		// if err != nil {
		//     fmt.Fprintf(os.Stderr, "err:%q\n", err)
		//     os.Exit(1)
		// }

		// data.Clear()
		// fmt.Printf("%s", data)

		io.Copy(os.Stdout, rsp.Body)

		rsp.Body.Close()
	}
}
