package main

import "fmt"

func main() {
	aMap := map[string]int{
	}
	// 映射不再指向任何内存空间
	aMap = nil
	fmt.Println(aMap)
	aMap["test"] = 1
}