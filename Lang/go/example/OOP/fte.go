package main

import (
	"fmt"
)

type Person struct {
}

func (p *Person) SayHello() {
	fmt.Println("Hello, I'm a person!")
}

type Employee struct {
	Name string
	Age int
}

func (e *Employee) Work() {
	fmt.Println("Working......")
}

type FullTimeEmployee struct {
	*Person
	*Employee
}

func main() {
	fte := FullTimeEmployee {
		Person: &Person{},
		Employee: &Employee{
			Name: "Daz",
			Age: 20,
		},
	}

	fte.SayHello()
	fte.Work()
}