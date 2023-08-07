package main

import(
	"fmt"
)

func main() {
	var myInt interface{} = 123
	// var myInt interface{} = true
	k, ok := myInt.(int)
	if ok {
		fmt.Println("success:", k)
	}
	v, ok := myInt.(float64)
	if ok {
		fmt.Println(v)
	} else {
		fmt.Println("Failed without pancking!")
	}

	j := myInt.(int)
	fmt.Println("No checking:", j)
	k := myInt.(bool)
	fmt.Println(k)
}