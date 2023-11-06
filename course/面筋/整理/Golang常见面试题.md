## 基础
1. golang 中 make 和 new 的区别？（基本必问）
- make 返回对类型的引用, new 返回类型的指针
- make 会分配内存并初始化, new 分配内存但赋零值
  - 零值: 0, "", false, nil
- make 只能用于 slice, map, channel 的初始化
  - make 是三者的工厂函数
  - make 对三者有分别的makemap, makeslice, makechan

2. 数组和切片的区别 （基本必问）
- 数组是
  - 值类型
  - 固定长度
  - 指针指向数组的第一个元素的地址
- 切片
  - 引用类型
  - 底层是数组, 不定长
  - 指针指向第一个 slice 元素对应的底层数组元素的地址
    - slice 的第一个元素不一定是底层数组的第一个元素
    - 因为切片的起始索引值可以是任意的，而不是固定为0。当你从一个切片中创建另一个切片时，新切片的起始索引和结束索引可以是原始切片的任意子集
```go
type silice struct {
  array unsafe.Pointer
  len int
  cap int
}

array := [5]int{1, 2, 3, 4, 5}
slice1 := make([]int, 5)
slice2 := []int{1, 2, 3, 4, 5}
```

3. for range 的时候它的地址会发生变化么？for 循环遍历 slice 有什么问题？
- for range 时,地址不会变化
  - 在循环时,会创建一个变量,每次都会把地址赋给同一个变量,导致循环结束后,拷贝永远是最后一个
```go
	for _, v := range studs {
		tmp := v
		gl[v.Name] = &tmp
		fmt.Printf("%s addr: %v\n", v.Name, v.Age)
	}
```
- range 对 slice 做遍历的时候,实际上是先构造一个原 slice 的拷贝,在对拷贝做遍历
```go
    var x = []string{"A", "B", "C"}

    for i, s := range x {
        print(i, s, ",")
        x[i+1] = "M"
        x = append(x, "Z")
        x[i+1] = "Z"
    }
```
输出结果为 0A, 1M, 2C


4. go defer，多个 defer 的顺序，defer 在什么时机会修改返回值？（for defer）&& defer recover 的问题？(主要是能不能捕获)
- 多个 defer: 栈的顺序, 都在 return 之后
-  defer recover 这种机制只是针对当前函数和以及直接调用的函数可能产生的 panic，它无法处理其调用产生的其它协程的 panic，这一点和 try catch 机制不一样
   -  recover 只能捕获一层
```go
func example() {
    defer func() {
        if r := recover(); r != nil {
            fmt.Println("Recovered from panic:", r)
        }
    }()
    panic("Something went wrong")
}

func main() {
    example()
    fmt.Println("Program continues to run after panic")
}
```

5. uint 类型溢出
```go
	var a uint8 = 1
	var b uint8 = 255
	fmt.Println("减法：", a-b) // 2
	fmt.Println("加法：", a+b) // 0
	fmt.Println("乘法：", a*b) // 255
```

6. 介绍 rune 类型
- 其实就是 int32, 四字节
- 表示单个 Unicode 字符,可以存储任意的 Unicode 码点
- 在字符串中,每个字符都可以用一个 rune 值表示
```go
	var r rune = '中'  		  // '中' 是一个Unicode字符，用rune类型表示
	fmt.Printf("%c\n", r)  // 输出：中
	fmt.Printf("%d\n", r)  // 输出：20013（'中'的Unicode码点）
```

7. golang 中解析 tag 是怎么实现的？反射原理是什么？(问的很少，但是代码中用的多)
- 标签通过反射实现
  - 标签可存储元数据,比如字段的 Json 名称,数据库列名
- 反射基于接口实现
  - 反射三大法则
    - 从 interface{} 变量可以反射出反射对象
    - 从反射对象可以获取 interface{} 变量
    - 要修改反射对象，其值必须是“addressable”(可寻址的)

8. 调用函数传入结构体时，应该传值还是指针？
- 值传递: 传递的是值的副本,函数内部修改不会影响原值
- 址传递: 传递的是指针的,函数内部可以修改原值
- Golang 中`只有值传递`, 址接收器是另外一回事

9. silce 遇到过哪些坑？
- slice底层结构并没有使用加锁的方式,不支持并发读写
- 切片的底层数组是共享的
- 切片的扩容机制
  - 切片有长度和容量两个属性。长度表示切片当前包含的元素个数，容量表示切片的底层数组中能够容纳的元素个数。当切片的长度超过容量时，切片会重新分配底层数组，可能导致之前的引用失效
    - 可以使用深拷贝,直接指向两个不同的底层数组
    - `copy(newSlice, oldSlice)`
    - GO1.17版本及之前
      - 当新切片需要的容量cap大于两倍扩容的容量，则直接按照新切片需要的容量扩容
      - 当原 slice 容量 < 1024 的时候，新 slice 容量变成原来的 2 倍
      - 当原 slice 容量 > 1024，进入一个循环，每次容量变成原来的1.25倍,直到大于期望容量
    - GO1.18之后
      - 当新切片需要的容量cap大于两倍扩容的容量，则直接按照新切片需要的容量扩容
      - 当原 slice 容量 < threshold 的时候，新 slice 容量变成原来的 2 倍
      - 当原 slice 容量 > threshold，进入一个循环，每次容量增加（旧容量+3*threshold）/4
```go
func main() {
	slice := make([]int,0,3)
	slice = append(slice,1,2,3)
	TestSlice(slice)
	fmt.Println(slice)
}

func TestSlice(slice []int)  {
  // 对slice进行了追加操作，但由于底层数组容量不够，切片会重新分配底层数组。所以，TestSlice函数内部的修改不会影响原始切片的值
	slice = append(slice,4)
  // 虽然这里是引用,但底层数组已经换了
	slice[0] = 10
}
```
- 切片的截取： 当使用切片的[start:end]截取操作时，如果end超出了切片的长度，会引发panic
- 空切片 和 nil 切片不同
  -  空切片`emptySlice := []int{}` 是一个长度为0的切片
    - 有底层数组
  -  nil切片 `var nilSlice []T`或者`var nilSlice = []T(nil)` 没有底层数组
    - 无底层数组

10. go struct 能不能比较？
  - 可比较: 
    - Integer，Floating-point，String，Boolean，Complex(复数型)，Pointer，Channel，Interface，Array 
  - 不可比较
    - Slice，Map，Function
  - 同一个 struct 的两个实例可比较也不可比较，当结构不包含不可直接比较成员变量时可直接比较，否则不可直接比较

11. Go 闭包
- Closure
```go
func(入参) {
    // 函数体
}(调用,放置入参)
```
- 闭包 = 函数+引用环境
- 保留状态： 闭包可以保存外部函数中的局部变量的状态。当外部函数执行完毕后，闭包仍然可以访问和修改这些变量，保留了它们的状态。这使得闭包可以在函数之间传递状态，而不需要使用全局变量(配合defer 使用)
- 闭包的生命周期不由它的作用域决定，而是由它是否被引用决定。闭包会一直存在，直到它不被引用，被 GC 回收

## Context
1. context 结构是什么样的？
```go
type Context interface {
    Deadline() (deadline time.Time, ok bool)
    Done() <-chan struct{}
    Err() error
    Value(key interface{}) interface{}
}
```
- Deadline(): 返回 context 的截止时间,没有的话就设置截止时间
- Done(): 返回一个 channel, 当 context 被取消或者超时时, channel 会被关闭
  - 是一个只读的 channel, 当子协程读到这个 cahnnel(零值)后, 就可以做一些清理工作了
- Err(): 返回 context 结束的原因, 如果上下文没有被取消，该方法会返回 nil
- Value(): 返回 context 中与 key 对应的 value, 如果没有对应的 key, 返回 nil

2. context 使用场景和用途？（基本必问）
- 用于在函数之间传递上下文信息
  - 足球模式: 整个调用链把一个足球从最上层的调用者踢到最下层的被调用者，然后再一层层踢回来
- 控制并发操作
  - context是并发安全的
- 处理超时和取消操作
- 应该放到 ctx 的信息:
  - Use context values only for request-scoped data that transits processes and API boundaries, not for passing optional parameters to functions.
```go
func handleLoginRequest(ctx context.Context, request *http.Request) {
    // 从请求中获取用户身份信息，例如用户名和密码
    username := request.FormValue("username")
    password := request.FormValue("password")

    // 进行身份验证，如果验证成功，将用户信息存储到context中
    user, err := authenticateUser(username, password)
    if err != nil {
        // 处理身份验证失败的情况
        http.Error(w, "Authentication failed", http.StatusUnauthorized)
        return
    }

    // 将用户信息存储到context中
    ctx = context.WithValue(ctx, "user", user)

    // 在后续的函数调用中，可以通过context获取用户信息
    // 调用其他函数进行权限检查等操作，不需要显式传递user参数
    someOtherFunction(ctx)
}

func someOtherFunction(ctx context.Context) {
    // 从context中获取用户信息
    user := ctx.Value("user").(User)
    
    // 进行权限检查等操作
    // ...
}
```

## Channel 相关：
1. Channel 特点
   - 两种类型:
     - 有缓存
     - 无缓存
   - 三种模式
     - 写操作模式(单向通道)
     - 读操作模式(单向通道)
     - 读写操作模式(双向通道)
   - 三种状态
     - 未初始化
	 - 已初始化
	 - 已关闭
   - 一个channel 不可以多次关闭, 会 panic
   - 如果多个 gorountine 都监听同一个 channel, 那么只有一个 gorountine 会被`随机`选中, 从而获取到 channel 中的数据
   -  如果多个goroutine监听同一个channel，如果这个channel被关闭，则`所有`goroutine都能收到退出信号
   - 无缓冲:一个送信人去你家送信，你不在家他不走，你一定要接下信，他才会走。
     - 无缓存的 chan 有发送者就必须有接收者,否则会报错,发生阻塞
   - 有缓冲:一个送信人去你家送信，扔到你家的信箱转身就走，除非你的信箱满了，他必须等信箱有多余空间才会走

2. channel 是否线程安全？锁用在什么地方？
- 是的, 为了保证数据一致性
- channel的底层实现中, hchan 结构体中采用 Mutex 锁来保证数据读写安全
  - 在对循环数组buf中的数据进行入队和出队操作时，必须先获取互斥锁，才能操作 channel 数据

3. go channel 的底层实现原理 （数据结构）
  - 底层实现:
	- 队列: 先进先出
	- 负责协程之间的通信(Go语言提倡不要通过共享内存来通信，而要通过通信来实现内存共享)
	- 通过var声明或者make函数创建的channel变量是一个存储在函数栈帧上的指针，占用8个字节，指向堆上的hchan结构体
	  ![](https://img-blog.csdnimg.cn/4c5a4b62c9dd4522abeae054e11456f1.png)
	- buf: 用来保存 gorountine 之间传递数据的循环数组
	- sendx 和 recvx: 分别记录当前循环数组当前的读写位置
	- sendq 和 recvq: 分别保存向该 chan 发送数据和接收数据的被阻塞 gorountine 队列
	- lock: 用来保证 channel 的数据读写安全

4. nil. 关闭的 channel. 有数据的 channel，再进行读. 写. 关闭会怎么样？（各类变种题型）
   -  nil channel
	  - 读: 会一直阻塞
	  - 写: 会一直阻塞
	  - 关闭: panic
   - 关闭的 channel
     - 读: 会一直读到 channel 中的数据被读完, 读完后, 会返回 channel 的零值, 并且第二个返回值为 false
     - 写: 会 panic
     - 关闭: panic
   - 有数据的 channel
	 - 读: 会读到 channel 中的数据, 并且第二个返回值为 true
	 - 写: 如果已经满了, 会一直阻塞
	 - 关闭: 仍可读,但不能再写,同 2
5. 向 channel 发送数据和从 channel 读数据的流程是什么样的？
```go
ch := make(chan int) // 创建一个无缓冲的int类型channel
go func() {
	ch <- 42 // 向channel发送数据
}()
data := <-ch // 从channel中接收数据，不会阻塞
```

## Map 相关:
1. map 使用注意的点，并发安全？
   1. map 的 key 必须是可比较的, 不能是 slice, map, function(不可比较的类型)
   2. map 在并发情况下, 只读是线程安全的, 读写是线程不安全的
2. map 循环是有序的还是无序的？
   1. 无序的
      1. 无序写入
      2. 成倍扩容迫使元素重新分布
   2. 如何有序?
      1. 使用 `orderedmap` 包
      2. 将每个 key 同时存入 slice, 遍历 slice
3. map 中删除一个 key，它的内存会释放么？
   1. map 删除 key 后不会释放内存,只会修改标记,等待下次 GC
4. 怎么处理对 map 进行并发访问？有没有其他方案？ 区别是什么？
   1. 使用 `sync.Mutex` || 使用 `sync.RWMutex`
   2. 使用 `sync.Map`
   3. 使用 `channel`
5. nil map 和空 map 有何不同？
   1. nil map 是一个未初始化的 map, 不能对其进行写操作, 但可以读
      1. if map == nil
   2. 空 map 是一个初始化的 map, 可以进行读写操作
6. map 的数据结构是什么？是怎么实现扩容？
   1. map 的底层数据结构是哈希表(hmap)
      1. hmap 里面维护者若干个 bucket, bucket 中每个元素都是 bmap
      2. 每个 bmap 中 八个 kv 对, 8 个满了后, 新的落到溢出桶 (通过 overflow 连接)
   2. 两种扩容
      1. 等量扩容
         1. 由于map中不断的put和delete key，桶中可能会出现很多断断续续的空位，这些空位会导致连接的bmap溢出桶很长
         2. 导致扫描时间边长。这种扩容实际上是一种整理，把后置位的数据整理到前面。这种情况下，元素会发生重排，但不会换桶
         ![](https://pic1.zhimg.com/v2-2ecbb8a8c52d395061f67e41620b1504_r.jpg)
	  2. 2 倍扩容
         1. 桶数组不够用了, 元素重排 + 可能换桶 
         2. 装载因子 = map中元素的个数 / map中当前桶的个数 (平均每个桶中的元素个数)
         3. 扩容条件1：装载因子 > 6.5
         4. 扩容条件2：溢出桶的数量过多
         	1. 当 B < 15 时，如果overflow的bucket数量超过 2^B
			2. 当 B >= 15 时，overflow的bucket数量超过 2^15
         5. 在我们的hmap结构中有一个oldbuckets吗，扩容刚发生时，会先将老数据存到这个里面
			每次对map进行删改操作时，会触发从oldbucket中迁移到bucket的操作【非一次性，分多次】
			在扩容没有完全迁移完成之前，每次get或者put遍历数据时，都会先遍历oldbuckets，然后再遍历buckets。
7. map 取一个 key，然后修改这个值，原 map 数据的值会不会变化
   1. 不会
   2. `val := m["a"]` 只是拷贝
