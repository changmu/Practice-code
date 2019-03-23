package main

import (
	"fmt"
	"reflect"
)

func main() {
	x := .12
	fmt.Println("value.String:", reflect.ValueOf(x).String())
	fmt.Println("value:", reflect.ValueOf(x))
	fmt.Println("value.Type:", reflect.ValueOf(x).Type())
	fmt.Println("value.Kind:", reflect.ValueOf(x).Kind())

	v := reflect.ValueOf(&x)
	v.Elem().SetFloat(1.23)
	fmt.Println("x:", x)
}
