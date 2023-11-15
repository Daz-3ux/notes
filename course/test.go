package main

import (
    "context"
    "fmt"
    "sync"
    "time"
)

func PrintOdd(ctx context.Context, wg *sync.WaitGroup, num int, odd, even chan struct{}) {
    defer wg.Done()
    for {
        select {
        case <-ctx.Done():
            return
        case <-odd:
            fmt.Println(num)
            num += 2
            time.Sleep(time.Millisecond * 500)
            even <- struct{}{}
        }
    }
}

func PrintEven(ctx context.Context, wg *sync.WaitGroup, num int, odd, even chan struct{}) {
    defer wg.Done()
    for {
        select {
        case <-ctx.Done():
            return
        case <-even:
            fmt.Println(num)
            num += 2
            time.Sleep(time.Millisecond * 500)
            odd <- struct{}{}
        }
    }
}

func main() {
    var wg sync.WaitGroup
    num1 := 1
    num2 := 0
    oddchan := make(chan struct{}, 1)
    evenchan := make(chan struct{}, 1)
    ctx, cancel := context.WithTimeout(context.Background(), time.Second*3)
    defer cancel()

    wg.Add(2)
    go PrintEven(ctx, &wg, num2, oddchan, evenchan)
    go PrintOdd(ctx, &wg, num1, oddchan, evenchan)

    evenchan <- struct{}{}
    wg.Wait()

}
