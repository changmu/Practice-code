package main

import "fmt"

// import "strings"
import "unicode"
import "math"

func myAtoi(str string) int {
	startPos, mark := func(str string) (int, int) {
		mark := 1
		for i := 0; i < len(str); i++ {
			if unicode.IsSpace(rune(str[i])) {
				continue
			}

			if str[i] == '-' {
				mark = -1
				i++
			} else if str[i] == '+' {
				i++
			}
			return i, mark
		}
		return len(str), mark
	}(str)

	overflowByMark := func(mark int) int {
		if mark > 0 {
			return math.MaxInt32
		}
		return math.MinInt32
	}

	minq := math.MinInt32 / 10
	minr := math.MinInt32 % 10

	ret := 0
	for i := startPos; i < len(str); i++ {
		if unicode.IsDigit(rune(str[i])) {
			v := int(int8('0' - str[i]))
			if ret < minq || (ret == minq && v <= minr) {
				return overflowByMark(mark)
			}
			ret = ret*10 + v
		} else {
			return -ret * mark
		}
	}

	return -ret * mark
}

func main() {
	fmt.Printf("str=%v\n", myAtoi("42"))
	fmt.Printf("str=%v\n", myAtoi("  -42"))
	fmt.Printf("str=%v\n", myAtoi("4193 with words"))
	fmt.Printf("str=%v\n", myAtoi("words and 987"))
	fmt.Printf("str=%v\n", myAtoi("-91283472332"))
	fmt.Printf("str=%v\n", myAtoi("2147483646"))
}
