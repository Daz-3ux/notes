package main

import (
	"fmt"
	"os"
	"strconv"
)

func namedMinMax(x, y int) (min, max int) {
	if x > y {
		min, max = y, x
	} else {
		min, max = x, y
	}
	// 返回顺序与声明顺序一致
	return
}

func minMax(x, y int) (min, max int) {
	if x > y {
		min, max = y, x
	} else {
		min, max = x, y
	}
	return min, max
}

func main() {
	arguments := os.Args
	if len(arguments) < 3 {
		fmt.Println("The program needs at least 3 arguments!")
		return
	}

	a1, _ := strconv.Atoi(arguments[1])
	a2, _ := strconv.Atoi(arguments[2])

	fmt.Println(minMax(a1, a2))
	min, max := minMax(a1, a2)
	fmt.Println(min, max)

	fmt.Println(namedMinMax(a1, a2))
	min, max = namedMinMax(a1, a2)
	fmt.Println(min, max)
}
