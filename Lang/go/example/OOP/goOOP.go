package main

import "fmt"

type Bird struct {
	Type string
}

func (bird *Bird) Class() string {
	return bird.Type
}

type Birds interface {
	Name() string
	Class() string
}

type Canary struct {
	Bird
	name string
}

func (canary *Canary) Name() string {
	return canary.name
}

type Crow struct {
	Bird
	name string
}

func (crow *Crow) Name() string {
	return crow.name
}

func newCanary(name string) *Canary {
	return &Canary{
		Bird: Bird{
			Type: "Canary",
		},
		name: name,
	}
}

func newCrow(name string) *Crow {
	return &Crow{
		Bird: Bird{
			Type: "Crow",
		},
		name: name,
	}
}

func BirdInfo(bird Birds) {
	fmt.Printf("I'm %s, I belong to %s bird class!\n", bird.Name(), bird.Class())
}

func main() {
	canary := newCanary("Yellow")
	crow := newCrow("Black")

	BirdInfo(canary)
	BirdInfo(crow)
}
