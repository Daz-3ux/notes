package main

import (
	"fmt"
)

func f() *int {
	v := 1
	return &v
}

func main() {
	var p = f()
	fmt.Println(p)
	fmt.Println(*p)
}