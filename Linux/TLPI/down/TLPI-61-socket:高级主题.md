# 61.1 流式套接字上的部分读和部分写
- 部分 I/O 现象：
    - read():
        - 可用数据比read()请求的数据少,此时read()返回可用的字节数
    - write():
        - write()传输了部分请求的字节后被信号处理进程中断
        - 套接字工作在非阻塞模式下(`O_NONBLOCK`),`可能`只传输一部分
        - 在部分请求的字节已经完成传输后出现了一个异步错误
- 有时候需要重新调用系统调用来完成全部数据的传输

- `readen()`和`writen()`
    - 使用`循环`来重新启用这些系统调用,确保了请求的字节数总是可以得到`全部传输`
    - 自己编写


# 61.2 shutdown()系统调用
- 在套接字上调用close()会将双向通信通道的`两端都关闭`
- shutdown()可以只关闭一端
```c
#include <sys/socket.h>

int shutdown(int sockfd, int how);
//return 0 on success, -1 on error
```
- 系统调用 shutdown()可以根据参数 how 的值选择关闭套接字通道的一端还是两端
- shutdown()最常用的操作是`SHUT_WR`:关闭连接的写端, 称其为`半关闭套接字`
- shutdown()不会关闭文件描述符,要关闭文件描述符只能另外调用close()

# 61.3 专用于套接字的 I/O 系统调用： `recv()` 和 `send()`
- recv()和 send()系统调用可在已连接的套接字上执行 I/O 操作
- 提供了专属于套接字的功能

# 61.4 sendfile()系统调用
- 像 Web 服务器和文件服务器这样的应用程序常常需要将磁盘上的文件内容`不做修改地通过（已连接）套接字传输出去`
    - `循环`read() + write() (在传输大文件时不够高效)
    - 使用sendfile()
- 当应用程序调用 sendfile()时，`文件内容会直接传送到套接字上，而不会经过用户空间`
- 这种技术被称为`零拷贝传输（ zero-copy transfer）`
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205271753085.png)

```c
#include <sys/sendfile.h>

sszie_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```
- 我们可以使用sendfile()`将数据从文件传到套接字`,但反过来不可以

# 61.5 获取套接字地址
- `getsockname()`:返回`本地`套接字地址
- `getpeername()`:返回`对端`套接字地址

# 61.6 深入探讨TCP协议
## 61.6.1 TCP报文的格式

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205271806063.png)
- 源端口号（ source port number）:TCP 发送端的端口号
- 目的端口号（ destination port number）:TCP 接收端的端口号
- 序列号（sequence number）:报文的序列号，标识从 TCP发端 向 TCP 收端 发送的数据字节流，它表示在这个报文段中的第一个数据字节上
- 确认序号（ acknowledgement number:如果设定了 ACK 位，那么这个字段包含了 接收方 期望从 发送方 接收到的下一个数据字节的序列号
- 首部长度（ header length）：该字段用来表示 TCP 报文首部的长度，首部长度单位是32 位。由于这个字段只占 4 个比特位，因此首部总长度最大可达到 60 字节（ 15 个字长）该字段使得 TCP 接收端可以确定变长的选项字段（ options）的长度，以及数据域的起始点
- 保留位（ reserved）:该字段包含 4 个未使用的比特位（必须置为 0）
- 控制位（ control bit）:该字段由 8 个比特位组成，能进一步指定报文的含义
- 窗口大小（ window size）：该字段用在接收端发送 ACK 确认时提示自己可接受数据的空间大小
- 校验和（ checksum ）： 16 位的检验和包括 TCP 首部和 TCP 的数据域
- 紧急指针（ Urgent pointer）：如果设定了 URG 位，那么就表示从发送端到接收端传输的数据为紧急数据
- 选项（ Options）：这是一个变长的字段，包含了控制 TCP 连接操作的选项
- 数据（ Data）：这个字段包含了该报文段中传输的用户数据。如果报文段没有包含任何数据的话，这个字段的长度就为 0

## 61.6.2 TCP 序列号和确认机制
- 每个通过 TCP连接 传送的字节都由TCP协议分配了一个逻辑序列号
- 当传送一个报文时，该报文的序列号字段被设为该传输方向上的报文段数据域第一个字节的逻辑偏移
    - 这样 TCP 接收端就可以按照正确的顺序对接收到的报文段重新组装，并且当发送一个确认报文给发送端时就表明自己接收到的是哪一个数据

- 要实现可靠的通信， TCP 采用了主动确认的方式：
    - 当一个报文段被成功接收后，TCP接收端会发送一个确认消息(ACK)给TCP发送端
    - 该消息的确认序号字段被设置为接收方所期望接收的下一个数据字节的逻辑序列号
    - 当 TCP 发送端发送报文时会设置一个定时器，如果在定时器超时前没有接收到确认报文， 那么该报文会重新发送

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205271818912.png)

## 61.6.3 TCP 协议状态机以及状态迁移图
-  TCP结点以状态机的方式来建模
-  TCP有多种状态且可互相迁移
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205271820925.png)

## 61.6.4 TCP 连接的建立
- 在套接字 API 层，两个流式套接字通过以下步骤来建立连接
    - 服务器调用 listen()在套接字上执行被动打开，然后调用 accept()阻塞服务器进程直到连接建立完成
    - 客户端调用 connect()在套接字上执行主动打开，以此来同服务器端的被动打开套接字之间建立连接

- TCP 协议建立连接执行 `三次握手` (在两个 TCP 结点间有 3 个报文需要传递)
    - connect()调用导致客户端 TCP 结点发送一个 SYN 报文到服务器端 TCP 结点:这个报文将告知服务器有关客户端 TCP 结点的初始序列号,这一信息是必要的，因为序列号不会从0开始
    -  服务器端 TCP 结点必须确认客户端发送来的 TCP SYN 报文， 并告知客户端自己的初始序列号
    -  客户端 TCP 结点发送一个 ACK 报文来确认服务器端 TCP 结点的 SYN 报文
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205271830875.png)

## 61.6.5 TCP 连接的终止
- TCP其中一端的应用程序执行close():我们说这个应用程序正在执行一个主动关闭; 稍后,连接另一端的应用程序也执行一个close, 被称为被动关闭

- TCP协议执行的相关步骤:(假设客户端发起主动关闭)
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205271838723.png)

## 61.6.6 在 TCP 套接字上调用 shutdown()
- 对于可移植的 TCP 应用程序来说，应该避免使用 SHUT_RD 操作

## 61.6.7 TIME_WAIT 状态
- TIME_WAIT状态的存在主要基于两个目的
    - 实现可靠的连接终止
    - 让老的重复的报文段在网络中过期失效,在建立新连接时不再接收他们

# 61.7 监视套接字： netstat
- netstat 程序可以显示系统中 Internet 和 UNIX 域套接字的状态
- 强大的调试工具

# 61.8 使用 tcpdump 来监视 TCP 流量
- 可以让超级用户监视网络中的实时流量,实时生成文本信息
- 强大的调试工具

# 61.9 套接字选项
- 系统调用 `setsockopt()`和 `getsockopt()`是用来设定和获取套接字选项的

# 61.10 SO_REUSEADDR 套接字选项
- 可以取消`TIME_WAIT`状态

# 61.11 在 accept()中继承标记和选项
- 由 accept()返回的新的描述符继承了大部分套接字选项，这些选项可以通过setsockopt()来设定
- 为了满足可移植性，可能需要显式地在 accept()返回的新套接字上
重新设定这些属性

# 62.12 TCP vs.UDP
- 一些选择UDP而不是TCP的原因
    - UDP 传送单条消息的开销比使用 TCP 要小:UDP 服务器能从多个客户端接收数据报（并可以向它们发送回复），而不必为每个客户端创建和终止连接
    - 对于简单的请求—响应式通信， UDP 的速度比 TCP 要快

- 使用 UDP 但又需要`可靠性保证`的应用程序必须`自行`实现可靠性保障功能:至少需要序列号、确认机制、丢包重传以及重复报文检测。 

# 61.13 高级功能
- 带外数据(不提倡使用)
- sendmsg()和 recvmsg()系统调用
- 传递文件描述符
- 接收发送端的凭据
- 顺序数据包套接字
- SCTP 以及 DCCP 传输层协议:两个新的传输层协议，有可能在将来变得越来越普及