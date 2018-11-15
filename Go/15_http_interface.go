package main

import (
    "fmt"
    "log"
    "net/http"
)

func main() {
    db := database {
        "shoes": 50,
        "socks": 20,
    }
    log.Fatal(http.ListenAndServe("localhost:8080", db))
}

type dollars float32
func (d dollars) String() string { return fmt.Sprintf("$%.2f", d) }

type database map[string]dollars

func (db database) ServeHTTP(w http.ResponseWriter, r *http.Request) {
    switch r.URL.Path {
    case "/list":
        for item, price := range db {
            fmt.Fprintf(w, "item:%s, price:%s\n", item, price)
        }

    case "/price":
        item := r.URL.Query().Get("item")
        price, ok := db[item]
        if !ok {
            w.WriteHeader(http.StatusNotFound)
            fmt.Fprintf(w, "no such item:%s", item)
            return
        }
        fmt.Fprintf(w, "item:%s, price:%s\n", item, price)
        
    default:
        w.WriteHeader(http.StatusNotFound)
        fmt.Fprintf(w, "no such page:%s", r.URL)
    }
}
