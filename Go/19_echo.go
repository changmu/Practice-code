package main

import (
    "flag"
    "net"
    "log"
    "fmt"
    "bufio"
    "time"
    "strings"
)

var g_port = flag.String("port", "8080", "conn port")

func main() {
    flag.Parse()
    listener, err := net.Listen("tcp", "localhost:"+*g_port)
    if err != nil {
        log.Fatal(err)
    }

    for {
        conn, err := listener.Accept()
        if err != nil {
            log.Print(err)
            continue
        }

        go handleConn(conn)
    }
}

func handleConn(conn net.Conn) {
    defer conn.Close()
    input := bufio.NewScanner(conn)
    for input.Scan() {
        go echo(conn, input.Text(), 1 * time.Second)
    }
}


func echo(conn net.Conn, shout string, delay time.Duration) {
    fmt.Fprintln(conn, "\t", strings.ToUpper(shout))
    time.Sleep(delay)
    fmt.Fprintln(conn, "\t", shout)
    time.Sleep(delay)
    fmt.Fprintln(conn, "\t", strings.ToLower(shout))
}
