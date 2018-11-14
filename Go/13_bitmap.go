package main

import (
    "fmt"
)

type BitMap struct {
    words []uint64
}

// 判断x是否存在
func (this *BitMap) Has(x int) bool {
    idx, bits := x / 64, uint(x%64)
    return idx < len(this.words) && this.words[idx]&(1<<bits) != 0
}

// 添加x
func (this *BitMap) Add(x int) {
    idx, bits := x / 64, uint(x%64)
    for len(this.words) <= idx {
        this.words = append(this.words, 0)
    }
    this.words[idx] |= 1<<bits
}

// 合并两个位图
func (this *BitMap) Union(that *BitMap) {
    for i, v := range that.words {
        if i < len(this.words) {
            this.words[i] |= that.words[i]
        } else {
            this.words = append(this.words, v)
        }
    }
}

func main() {
    var bit_map BitMap
    bit_map.Add(98)
    fmt.Println("96:", bit_map.Has(96))
    fmt.Println("98:", bit_map.Has(98))
}
