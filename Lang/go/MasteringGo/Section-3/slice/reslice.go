package main

import (
	"fmt"
)

func main() {
	s1 := make([]int, 5)
	reSilce := s1[1:3]
	fmt.Println(s1)
	fmt.Println(reSilce)
	reSilce[0] = -100
	reSilce[1] = 123456
	fmt.Println(s1)
	fmt.Println(reSilce)
}
