
# 56.1 概述
- `socket` 是一种IPC方法(进程间通信):它允许位于同一主机或使用网络连接起来的不同主机上的应用程序之间交换数据

在一个典型的`服务端/客户端`场景中,应用程序使用socket进行通信的方法如下:
- 各个应用程序创建一个socket
- 服务器将自己的socket绑定到一个众所周知的地址上使得客户端可以定位到它的位置

## 通信domain
- socket 存在于一个 `通信domain` 中，它确定：
    - 识别出一个 socket 的方法（即 socket“地址”的格式）；
    - 通信范围

- 现代操作系统至少支持下列 domain
    - UNIX (AF_UNIX) domain 允许在同一主机上的应用程序之间进行通信
    - IPv4 (AF_INET) domain 允许在使用因特网协议第 4 版网络连接起来的主机上的应用程序之间进行通信
    - IPv6 (AF_INET6) domain 允许在使用因特网协议第 6 版网络连接起来的主机上的应用程序之间进行通信
    - IPv4 仍是主流

- `AF`表示"`地址族(address family)`" / `PF`表示"`协议族(protocol family)`"
    - 没有哪一个 PF 可以支持多个已被定义的 AF 族

- socket domain

|Domain|执行的通信|应用程序间的通信|地址格式|地址结构|
|--|--|--|--|---|
|AF_UNIX|内核中|同一主机|路径名|`socketaddr_un`|
|AF_INET|通过IPv4|通过IPv4网络连接起来的主机|32位IPv4地址+16位端口|`sockrt_in`|
|AF_INET6|通过IPv6|通过IPv6网络连接起来的主机|128位IPv4地址+16位端口|`socket_in6`|


## socket类型
- 流
- 数据报

![](https://s3.bmp.ovh/imgs/2022/05/22/e845f23f6e904a18.png)

### 流 socket
- TCP socket(`传输控制协议`)
- 流 `socket(SOCK_STREAM)` 提供了一个 `可靠的` `双向的` `字节流` 通信信道
    - 可靠的：表示可以保证发送者传输的数据会完整无缺地到达接收应用程序（假设网络
链接和接收者都不会崩溃）或收到一个传输失败的通知
    - 双向的：表示数据可以在两个 socket 之间的任意方向上传输。
    - 字节流：表示与管道一样不存在消息边界的概念
- 一个流 socket 类似于使用一对允许在两个应用程序之间进行双向通信的管道，它们之间
的差别在于（ Internet domain） socket 允许在网络上进行通信。
- 面向连接: 流 socket 的正常工作需要一对 **相互连接** 的 socket
- 一个流 socket 只能与一个对等 socket 进行连接:
    - **对等地址**:该socket的地址
    - **对等应用程序**:利用这个对等socket的应用程序

### 数据报 socket
- UDP sockt(`用户数据报协议`)
- 数据报 `socket(SOCK_DGRAM)` 允许数据以被称为数据报的消息的形式进行交换。在数
据报 socket 中，消息边界得到了保留，但数据传输是不可靠的
- 数据报 socket 是更一般的无连接 socket 概念的一个示例。与流 socket 不同，一个数据报socket 在使用时无需与另一个 socket 连接

## socket系统调用
- 关键的 socket 系统调用包括以下几种
    - `socket()` 系统调用创建一个新 socket。
    - `bind()` 系统调用将一个 socket 绑定到一个地址上。通常，服务器需要使用这个调用来将其 socket 绑定到一个众所周知的地址上使得客户端能够定位到该 socket 上
    - `listen()` 系统调用允许一个 流socket 接受来自其他 socket 的接入连接
    - `accept()` 系统调用在一个 监听流socket 上接受来自一个对等应用程序的连接， 并可选地返回对等 socket 的地址
    - `connect()` 系统调用建立与另一个 socket 之间的连接

# 56.2 创建一个socket:`socket()`
```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
//return file descriptor on success, or -1
```
- domain:指定socket的通信domain
- type:
    - 流socket:`SOCK_STREAM`
    - 数据报socket:`SOCK_DGRAM`
- protocol:一般置0

# 56.3 将socket绑定到地址:`bind()`
```c
#include <sys/socket.h>

int bind(int sockfd, const struct socladdr *addr, socket_t addrlen);
// 0 on success, or -1
```
- socketfd:socket()的返回值
- addr:指向该socket绑定到的地址的结构
- addrlen:指定地址结构的大小(socket_t为整数类型)

- 一般来讲，会将一个服务器的 socket 绑定到一个众所周知的地址

# 56.4 通用的socket地址结构:struct sockaddr
-  bind()之类的系统调用适用于所有 socket domain,因此它们必须要能够接受任意类型的地址结构:为支持这种行为， socket API 定义了一个通用的地址结构 `struct sockaddr`
```c
struct sockaddr{
    sa_family_t sa_family;      // Address family
    char        sa_data[14];    // Socket address
}
```

# 56.5 流socket
- 流 socket 的运作与电话系统类似。
1. socket()系统调用将会创建一个 socket，这等价于安装一个电话。为使两个应用程序能够通信，每个应用程序都必须要创建一个 socket
2. 通过一个流 socket 通信类似于一个电话呼叫。 一个应用程序在进行通信之前必须要将
其 socket 连接到另一个应用程序的 socket 上。两个 socket 的连接过程如下:
    - 一个应用程序调用 bind()以将 socket 绑定到一个众所周知的地址上，然后调用
listen()通知内核它接受接入连接的意愿。这一步类似于已经有了一个为众人所知
的电话号码并确保打开了电话，这样人们就可以打进电话了
    - 其他应用程序通过调用 connect()建立连接，同时指定需连接的 socket 的地址。这类似于拨某人的电话号码
    - 调用 listen()的应用程序使用 accept()接受连接。这类似于在电话响起时拿起电话。如果在对等应用程序调用 connect()之前执行了 accept()，那么 accept()就会`阻塞`(“等待电话”)
3. 一旦建立了一个连接之后就可以在应用程序之间（类似于两路电话会话）进行双向数据
传输直到其中一个使用 close()关闭连接为止。通信是通过传统的 read()和 write()系统调用或通过一些提供了额外功能的 socket 特定的系统调用（如 send()和 recv()）来完成的

![](https://s3.bmp.ovh/imgs/2022/05/22/375e213ef34cc9aa.png)

## 主动socket 和 被动socket
- 主动socket:可执行主动的打开
- 被动socket(`监听socket`):可执行被动的打开

- **服务器会执行被动式打开 , 而客户端会执行主动式打开**

## 56.5.1 监听接入:listen()
- 将 sockfd 引用的 流socket 标记为 `被动`
```c
#include <sys/socket.h>

int listen(int sockfd, int backlog);
//return 0 on success, or -1
```
- backlog 限制`未决连接的数量`(在这个限制之内的连接请求会立即成功)

- 无法在一个已连接的socket()上执行listen()

## 56.5.2 接受连接:accept()
- accept()系统调用在文件描述符 sockfd 引用的 监听流socket 上接受一个接入连接
- 如果在调用 accept()时不存在未决的连接，那么调用就会阻塞直到有连接请求到达为止
```c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t addr);
```
- 它会创建一个新 socket， 并且正是这个新 socket 会与执行 connect()的对等socket 进行连接
- accept()返回的函数是已连接的socket的文件描述符
- `addr` 参数指向了一个用来返回 socket地址的结构:传入 accept()的剩余参数会返回对端 socket 的地址
- `addrlen`参数是一个值:执行accept()之前为addr指向的缓冲区的大小,执行accept()后为实际复制进缓冲区中的数据的字节数
- addr与addrlen可设置为`NULL 和 0`

## 56.5.3 连接到对等的socket:`connect()`
```c
#include <sys.socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
//return 0 on success, or -1
```
- connect()系统调用将文件描述符 `sockfd` 引用的 `主动socket` 连接到地址通过 addr 和 addrlen指定的 `监听socket` 上
- 如果 connect()失败并且希望重新进行连接，那么 SUSv3 规定完成这个任务的可移植的方法是关闭这个 socket，创建一个新 socket，在该新 socket 上重新进行连接

## 56.5.4 流socket I/O
- 一对连接的流 socket 在两个端点之间提供了一个双向通信信道

![](https://s3.bmp.ovh/imgs/2022/05/23/ac418a2f8d04e99a.png)


## 56.5.5 连接终止:close()
- 如果多个文件描述符引用了同一个socket，那么当`所有`描述符被关闭之后连接就会终止
- `shutdown()`可以提供更加精细的关闭流的操作

# 56.6 数据报socket
- 类似于邮政系统
1. `socket()`系统调用等价于创建一个邮箱(取信和送信都是在邮箱中发生的）所有需要发送和接收数据报的应用程序都需要使用 socket()创建一个数据报 socket。
2. 为允许另一个应用程序发送其数据报（信），一个应用程序需要使用 `bind()`将其socket 绑定到一个众所周知的地址上。一般来讲，一个服务器会将其 socket 绑定到一个众所周知的地址上，而一个客户端会通过向该地址发送一个数据报来发起通信(在一些 domain中——特别是 UNIX domain——客户端如果想要接受服务器发送来的数据报的话可能还
需要使用 bind()将一个地址赋给其 socket)
3. 要发送一个数据报，一个应用程序需要调用 `sendto()`，它接收的其中一个参数是数据
报发送到的 socket 的地址。这类似于将收信人的地址写到信件上并投递这封信
4. 为接收一个数据报，一个应用程序需要调用 `recvfrom()`，它在没有数据报到达时会阻塞,由于 recvfrom()允许获取发送者的地址，因此可以在需要的时候发送一个响应
5. 当不再需要 socket 时，应用程序需要使用 `close()`关闭 socket

- 无法保证数据按发送顺序到达
- 甚至无法保证到达
- 数据有可能多次到达

![](https://s3.bmp.ovh/imgs/2022/05/23/09c19c80a10574b3.png)

## 56.6.1 交换数据报:`recvfrom()` / `sendto()`
```c
#include <sys/socket.h>

ssize_t recvfrom(int sockfd, void *buf, size_t length, int flags,
                    struct sockaddr *src_addr, socklen_t addrlen);
//return number of bytes received,0 on EOF, -1 on error

ssize_t sendto(int sockfd, void *buf, size_t length, int flags,
                    struct sockaddr *src_addr, socklen_t addrlen);
// return number of bytes sent, -1 on error
```
- 前三个参数与 read()和 write()中的返回值和相应参数一样
- `flag` 指定I/O特性
- `src_addr` 和 `addrlen` 参数被用来获取或指定与之通信的对等 socket 的地址
- 如果不关心发送者的地址，那么可以将 src_addr 和 addrlen 都指定为 `NULL`
- 不管 length 的参数值是什么， recvfrom()只会从一个数据报 socket 中读取一条消息. 如果消息的大小超过了 length 字节，那么消息会被`静默地截断`为 length 字节
- Linux上可以使用sendto()发送长度为0的数据报

## 56.6.2 在数据报socket上使用`connect()`
- 在数据报 socket 上调用 connect()会导致内核记录这个 socket 的对等 socket 的地址
- 为一个数据报 socket 设置一个对等 socket，这种做法的一个明显优势是在该 socket 上传输数据时可以使用更简单的 I/O 系统调用，即无需使用指定了 dest_addr 和 addrlen 参数的sendto()，而只需要使用 write()即可