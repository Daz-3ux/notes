package main

import "fmt"

func main() {
	iMap := make(map[string]int)
	iMap["keyOne"] = 10
	iMap["keyTwo"] = 20
	fmt.Println("iMap:", iMap)

	anotherMap := map[string]int{
		"key1":  11,
		"key2":  22,
	}
	fmt.Println("anotherMap:", anotherMap)
	// 多次调用 delete() 去删除同一个元素并不会导致异常或者警告
	delete(anotherMap, "key1")
	delete(anotherMap, "key1")
	delete(anotherMap, "key1")
	fmt.Println("anotherMap:", anotherMap)

	_, ok := iMap["doseItExist"]
	if ok {
		fmt.Println("Exists!")
	} else {
		fmt.Println("Does NOT exist!")
	}

	for key, value := range iMap {
		fmt.Println(key, ":", value)
	}

}