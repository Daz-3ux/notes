package main

import (
	"fmt"
)

type Node struct {
	value		int
	next 		*Node
}

var root = new(Node)

func addNode(t *Node, v int) int {
	if root == nil {
		t = &Node{v, nil}
		root = t
		return 0
	}
	if v == t.value {
		fmt.Println("Node already exists:", v)
		return -1
	}
	if t.next == nil {
		t.next = &Node{v, nil}
		return -2
	}

	return addNode(t.next, v)
}

func traverse(t *Node) {
	if t == nil {
		fmt.Println("-> Empty list")
		return
	}
	for t != nil {
		fmt.Printf("%d -> ", t.value)
		t = t.next
	}
	fmt.Println()
}

func lookupNode(t *Node, v int) bool {
	if root == nil {
		t = &Node{v, nil}
		root = t
		return false
	}
	if v == t.value {
		return true
	}
	if t.next == nil {
		return false
	}

	return lookupNode(t.next, v)
}

func size(t *Node) int {
	if t == nil {
		fmt.Println("-> Empty list")
		return 0
	}
	i := 0
	for t != nil {
		i++
		t = t.next
	}
	return i
}

func main() {
	fmt.Println(root)
	root = nil
	traverse(root)
	addNode(root, 1)
	addNode(root, -1)
	traverse(root)
	addNode(root, 10)
	addNode(root, 5)
	addNode(root, 45)
	addNode(root, 5)
	addNode(root, 5)
	traverse(root)
	addNode(root, 100)
	traverse(root)

	if lookupNode(root, 100) {
		fmt.Println("Node exists!")
	} else {
		fmt.Println("Node does not exist!")
	}

	if lookupNode(root, -100) {
		fmt.Println("Node exists!")
	} else {
		fmt.Println("Node does not exist!")
	}
}