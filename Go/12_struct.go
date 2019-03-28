package main

import (
	"fmt"
	"math"
)

type Point struct {
	X float64
	Y float64
}

func (p *Point) Distance(q *Point) {
	distance := math.Hypot(p.X-q.X, p.Y-q.Y)
	fmt.Println("distance:", distance)
}

func main() {
	p := Point{X: 1, Y: 1}
	q := Point{4, 5}
	p.Distance(&q)
}
