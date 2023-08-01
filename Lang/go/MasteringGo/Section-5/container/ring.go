package main

import (
	"container/ring"
	"fmt"
)

var size int = 10

func main() {
	myRing := ring.New(size + 1)
	fmt.Println("Empty ring", *myRing)

	for i := 0; i < myRing.Len()-1; i++ {
		myRing.Value = i
		myRing = myRing.Next()
	}
	myRing.Value = 999

	sum := 0
	myRing.Do(func(x interface{}) {
		t := x.(int)
		sum += t
	})
	fmt.Printf("Sum: %d\n", sum)

	// 只能通过 ring.Len() 函数安全取到环的大小
	for i := 0; i < myRing.Len()+2; i++ {
		myRing = myRing.Next()
		fmt.Print(myRing.Value, " ")
	}
	fmt.Println()
}