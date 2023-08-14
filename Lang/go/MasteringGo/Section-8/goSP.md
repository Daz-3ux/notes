# 系统编程
- Go 是一门`严格的系统编程语言`
- 进程分为三类
  - 用户进程
  - 守护进程
    - 运行在用户态
  - 内核进程

- C 使用 `fork()`, Go 使用 `gorountines`


# io.Reader 与 io.Writer
- 实现 io.Reader 需要实现 Read() 方法
- 实现 io.Writer 需要实现 Write() 方法

# bufio 包
