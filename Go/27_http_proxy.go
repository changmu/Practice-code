// http[s] proxy
package main

import (
	// "bytes"
	"fmt"
	"io"
	"log"
	"net"
	"net/url"
	"strings"
)

func main() {
	// 格式化日志
	log.SetFlags(log.LstdFlags | log.Lshortfile)
	// 绑定端口
	listener, err := net.Listen("tcp", ":8081")
	panicIf(err)
	log.Print("listening *:8081")
	// 接受服务
	for {
		client, err := listener.Accept()
		panicIf(err)

		go handleClientRequest(client)
	}
}

func panicIf(err error) {
	if err != nil {
		log.Panic(err)
	}
}

func handleClientRequest(client net.Conn) {
	if client == nil {
		return
	}
	defer client.Close()

	log.Print("get client: ", client.RemoteAddr())

	// 读取请求头
	var buf [1024]byte
	nbytes, err := client.Read(buf[:])
	if err != nil {
		log.Print(err)
		return
	}

	// 解析请求头
	var srv_addr string
	var method, host string
	fmt.Sscanf(string(buf[:]), "%s%s", &method, &host)
	hostPortUrl, err := url.Parse(host)
	if err != nil {
		log.Print(err)
		return
	}
	if hostPortUrl.Opaque == "443" {
		srv_addr = hostPortUrl.Scheme + ":443"
	} else {
		if strings.Index(hostPortUrl.Host, ":") == -1 {
			srv_addr = hostPortUrl.Host + ":80"
		} else {
			srv_addr = hostPortUrl.Host
		}
	}

	// 开始拨号
	srv, err := net.Dial("tcp", srv_addr)
	if err != nil {
		log.Print(err)
		return
	}
	if method == "CONNECT" {
		fmt.Fprintf(client, "HTTP/1.1 200 Connection established\r\n\r\n")
	} else {
		srv.Write(buf[:nbytes])
	}

	// 透传
	go io.Copy(srv, client)
	io.Copy(client, srv)
}
