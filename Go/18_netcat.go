package main

import (
	"flag"
	"fmt"
	"io"
	"log"
	"net"
	"os"
)

var g_port = flag.String("port", "8080", "conn port")

func main() {
	flag.Parse()
	conn, err := net.Dial("tcp", "localhost:"+*g_port)
	if err != nil {
		log.Fatal(err)
	}

	done_chan := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn)
		fmt.Println("done")
		done_chan <- struct{}{}
	}()
	mustCopy(conn, os.Stdin)

	conn.Close()
	<-done_chan
}

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
