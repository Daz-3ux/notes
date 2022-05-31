- UNIX domain socket: 允许位于同一主机系统上的进程之间相互通信

# 57.1 UNIX domain socket 地址: `struct sockaddr_un`

# 57.2 UNIX domain 中的流socket

# 57.3 UNIX domain 中的数据报socket
- 使用数据报socket是不可靠的适用于通过 **网络** 传输的数据包
- 对于UNIX domain socket 来说:数据报的传输是在内核中发生的,所以其为可靠的, 所有消息都会按顺序被递送且不会发生重复的状况

# 57.4 UNIX domain socket 权限
- 要连接一个 UNIX domain 流 socket 需要在该 socket 文件上拥有写权限。
- 要通过一个 UNIX domain 数据报 socket 发送一个数据报需要在该 socket 文件上拥有
写权限
- 需要在存放 socket 路径名的`所有目录`上都拥有执行（搜索）权限

# 57.5 创建互联 socket 对： socketpair()
- 有时候让单个进程创建一对 socket 并将它们连接起来是比较有用的。这可以通过使用两个 socket()调用和一个 bind()调用以及对 listen()、 connect()、 accept()（用于流 socket）的调用或对 connect()（用于数据报 socket）的调用来完成:
- socketpair()系统调用创建一对相互连接的 UNIX domain socket。这样就无需调用多个系统调用来创建、绑定以及连接 socket。一个 socket 对的使用方式通常与管道类似：一个进程创建socket对，然后创建一个其引用 socket 对的描述符的子进程。然后这两个进程就能够通过这个socket 对进行通信了

- socketpair()系统调用则为这个操作提供了一个快捷方式
- socketpair()系统调用只能用在 UNIX domain 中

# 57.6 Linux 抽象 socket 名空间
- Linux 特有的一项特性
- 允许将一个 UNIX domain socket 绑定到一个名字上但不会在文件系统中创建该名字
    - 无需担心与文件系统中的既有名字产生冲突。
    - 没有必要在使用完 socket 之后删除 socket 路径名。 当 socket 被关闭之后会自动删除这个抽象名
    - 无需为 socket 创建一个文件系统路径名, 这对于 chroot 环境以及在不具备文件系统上的写权限时是比较有用的

