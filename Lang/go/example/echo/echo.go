// package main

// import (
// 	"fmt"
// 	"os"
// )

// func main() {
// 	var s, sep string
// 	for i := 0; i < len(os.Args); i++ {
// 		if i == 0 {
// 			fmt.Println(os.Args[i])
// 		} else {
// 			s += sep + os.Args[i]
// 			sep = " "
// 		}
// 	}
// 	fmt.Println(s)
// }