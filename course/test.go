package main

import (
	"fmt"
	"sync"
)


func main() {
	ch := make(chan int, 3);
	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		defer wg.Done()
		for i := 0; i < 3; i++ {
			ch <- i
		}
		close(ch)
	}()

	for i := 0; i < 10; i++ {
		data, ok := <- ch
		fmt.Println(data, ok)
	}
}