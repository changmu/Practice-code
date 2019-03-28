package main

import (
	"bytes"
	"fmt"
	"io"
	"os"

	// "io/ioutil"
	"net/http"
)

func main() {
	url := "http://localhost:8080/api/v1/services/default/qrw/qrw_conf_default/model_conf[1].fetch_keys[+]"
	req, err := http.NewRequest("POST", url, bytes.NewBuffer([]byte("12")))
	if err != nil {
		fmt.Fprintf(os.Stderr, "err:%q\n", err)
		os.Exit(1)
	}

	client := &http.Client{}
	rsp, err := client.Do(req)
	if err != nil {
		fmt.Fprintf(os.Stderr, "err:%q\n", err)
		os.Exit(1)
	}

	fmt.Printf("status:%q\n", rsp.Status)

	io.Copy(os.Stdout, rsp.Body)

	rsp.Body.Close()
}
