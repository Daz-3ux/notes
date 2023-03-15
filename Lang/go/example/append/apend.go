package main

import (
	"fmt"
)

func appendInt(x []int, i int) []int {
	var z []int
	zlen := len(x) + 1
	if zlen < cap(x) {
		z = x[:zlen]
	} else {
		zcap := zlen
		if zcap < len(x)*2 {
			zcap = len(x) * 2
		}
		z = make([]int, zlen, zcap)
		copy(z, x)
	}
	z[len(x)] = i

	return z
}

func main() {
	x := []int{1, 2, 3}
	z := appendInt(x, 4)
	fmt.Println(z)
}