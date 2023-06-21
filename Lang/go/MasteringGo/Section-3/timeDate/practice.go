package main

import (
	"fmt"
	//"time"
)

const (
	Zero = iota
	One
	Two
	Three
)

const (
	Sunday  = 1 + iota
	Monday
	Tuesday
	Wednesday
	Thursday
	Friday
	Saturday
)


func main() {
	fmt.Println(Zero)
	fmt.Println(Wednesday)

	myArray := [...]int{1, 2, 3, 4, 5}
	myMap := map[int]int{}
	for k, v := range(myArray) {
		myMap[k] = v
	}
	fmt.Println(myMap)
}