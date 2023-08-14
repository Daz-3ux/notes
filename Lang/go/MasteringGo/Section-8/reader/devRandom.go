package main

import (
	"encoding/binary"
	"fmt"
	"os"
)

func main() {
	f, err := os.Open("/dev/random")
	if err != nil {
		fmt.Println(err)
		return
	}
	defer f.Close()

	var seed int64
	binary.Read(f, binary.LittleEndian, &seed)
	fmt.Println("Seed:", seed)
}