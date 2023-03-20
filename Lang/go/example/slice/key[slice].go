package main

import (
	"fmt"
)

func k(list []string) string {
	fmt.Printf("%q\n", list)
	return fmt.Sprintf("%q", list)
}

func Add(m map[string]int, list []string) {
	m[k(list)]++
}

func Count(m map[string]int, list []string) int {
	return m[k(list)]
}

func addEdge(from, to string) {
	
}


func main() {
	var m = make(map[string]int)
	a := []string{"a", "b", "c", "d", "e", "f"}
	b := []string{"b", "c", "d"}
	Add(m, a)
	Add(m, b)
	Add(m ,b)
	count := Count(m, b)
	fmt.Println(count)


	graph := make(map[string]map[string]bool)

}