package main

import (
	"container/heap"
	"fmt"
)

type heapFloat32 []float32

func (n *heapFloat32) Pop() interface{} {
	old := *n
	x := old[len(old)-1]
	*n = old[0 : len(old)-1]
	return x
}

func (n *heapFloat32) Push(x interface{}) {
	*n = append(*n, x.(float32))
}

func (n heapFloat32) Len() int {
	return len(n)
}

func (n heapFloat32) Less(i, j int) bool {
	return n[i] < n[j]
}

func (n heapFloat32) Swap(i, j int) {
	n[i], n[j] = n[j], n[i]
}

func main() {
	myHeap := &heapFloat32{1.2, 2.3, 3.4, 4.5}
	heap.Init(myHeap)
	size := len(*myHeap)
	fmt.Printf("Heap size: %d\n", size)
	fmt.Printf("%v\n", myHeap)
	myHeap.Push(float32(5.6))
	fmt.Printf("Heap size: %d\n", len(*myHeap))
	fmt.Printf("%v\n", myHeap)
	heap.Init(myHeap)
	fmt.Printf("%v\n", myHeap)
}
