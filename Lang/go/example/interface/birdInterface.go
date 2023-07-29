package main

import (
	"fmt"
)

type Bird interface {
	Fly()
	Type() string
}

type Eagle struct {
	Name string
}

func (e *Eagle) Fly() {
	fmt.Println("Eagle can fly")
}

func (e *Eagle) Type() string {
	return e.Name
}

type Crow struct {
	Name string
}

func (c *Crow) Fly() {
	fmt.Println("Crow can fly")
}

func (c *Crow) Type() string {
	return c.Name
}

func LetItFly(b Bird) {
	fmt.Printf("Let %s Fly!\n", b.Type())
	b.Fly()
}

func main() {
	LetItFly(&Eagle{Name: "Eagle"})
	LetItFly(&Crow{Name: "Crow"})
}
