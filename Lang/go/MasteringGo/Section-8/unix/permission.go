package main

import (
	"fmt"
	"os"
)

func main() {
	arguments := os.Args
	if len(arguments) != 2 {
		fmt.Printf("usage:permission filename\n")
		return
	}

	filename := arguments[1]
	info, _ := os.Stat(filename)
	mode := info.Mode()
	fmt.Println(filename, "mode is", mode.String()[1:])
}