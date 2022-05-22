# intro 


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
- accept()系统调用在文件描述符 sockfd 引用的监听流 socket 上接受一个接入连接
- 如果在调用 accept()时不存在未决的连接，那么调用就会阻塞直到有连接请求到达为止
```c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t addr);
```
- 它会创建一个新 socket， 并且正是这个新 socket 会与执行 connect()的对等socket 进行连接
