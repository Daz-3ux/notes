// package main

// import (
// 	"bufio"
// 	"fmt"
// 	"os"
// )

// func euqalSlice(x, y map[string]int) bool {
// 	if len(x) != len(y) {
// 		return false
// 	}
// 	for k, xv := range x {
// 		if yv, ok := y[k]; !ok || yv != xv {
// 			return false
// 		}
// 	}

// 	return true
// }

// func slicePassSet(x map[string]bool) {
// 	input := bufio.NewScanner(os.Stdin)
// 	for input.Scan() {
// 		line := input.Text()
// 		if !x[line] {
// 			x[line] = true
// 		}
// 	}

// 	if err := input.Err(); err != nil {
// 		fmt.Fprintf(os.Stderr, "deduo: %v\n", err)
// 		os.Exit(1)
// 	}
// }

// func showSet(x map[string]bool) {
// 	for k, ok := range x {
// 		if ok == true {
// 			fmt.Println(k)
// 		}
// 	}
// }

// func main () {
// 	x := map[string]int {
// 		"x": 1,
// 		"y": 2,
// 		"z": 3,
// 	}
// 	y := map[string]int {
// 		"x": 1,
// 		"y": 2,
// 		"z": 3,
// 	}
// 	z := map[string]int {
// 		"x": 3,
// 		"y": 2,
// 		"z": 1,
// 	}

// 	age, ok := x["bob"]
// 	if !ok { // 区分 已经存在的0 和 不存在而返回零值的0
// 		fmt.Println(age)
// 	}

// 	same1 := euqalSlice(x, y)
// 	same2 := euqalSlice(x, z)

// 	fmt.Println(same1, same2)

// 	set := make(map[string]bool);
// 	slicePassSet(set);
// 	showSet(set);
// }