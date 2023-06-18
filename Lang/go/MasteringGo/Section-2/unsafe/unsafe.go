package main

import (
	"fmt"
	"unsafe"
)

// 使用32-bit的指针无法存一个64-bit的整数型
func main() {
	var value int64 = 5
	var p1 = &value
	var p2 = (*int32)(unsafe.Pointer(p1))

	fmt.Println("*p1:", *p1)
	fmt.Println("*p2:", *p2)
	*p1 = 5434123412312431212
	fmt.Println(value)
	fmt.Println("*p2:", *p2)
	*p1 = 54341234
	fmt.Println(value)
	fmt.Println("*p2:", *p2)
}