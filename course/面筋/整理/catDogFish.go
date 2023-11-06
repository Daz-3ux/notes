package 整理

import (
    "fmt"
    "sync"
)

package main

import (
"fmt"
"sync"
)

func main() {
    var wg sync.WaitGroup
    dogChan := make(chan struct{}, 1)
    catChan := make(chan struct{}, 1)
    fishChan := make(chan struct{}, 1)
    var dogtimes int
    var cattimes int
    var fishtimes int
    var totalCount int

    wg.Add(3)
    go dog(&wg, dogtimes, totalCount, dogChan, catChan)
    go cat(&wg, cattimes, catChan, fishChan)
    go fish(&wg, fishtimes, fishChan, dogChan)
    dogChan <- struct{}{}
    wg.Wait()
}

func dog(wg *sync.WaitGroup, times, total int, dogChan, catChan chan struct{}) {
    for {
        if times >= 100 {
            wg.Done()
        }
        <-dogChan
        fmt.Println(total)
        total++
        fmt.Println("dog")
        times++
        catChan <- struct{}{}
    }
}

func cat(wg *sync.WaitGroup, times int, catChan, fishChan chan struct{}) {
    for {
        if times >= 100 {
            wg.Done()
        }
        <-catChan
        fmt.Println("cat")
        times++
        fishChan <- struct{}{}
    }
}

func fish(wg *sync.WaitGroup, times int, fishChan, dogChan chan struct{}) {
    for {
        if times >= 100 {
            wg.Done()
        }
        <-fishChan
        fmt.Println("fish")
        times++
        dogChan <- struct{}{}
    }
}
