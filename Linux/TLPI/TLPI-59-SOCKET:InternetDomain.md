# intro
- 介绍如何在 `IPv4(AF_INET)` 和 `IPv6(AF_INET6)` domain中使用 socket 编程
- 介绍DNS

# 59.1 Internet domain socket
- Internet domain 流 socket 是基于 TCP 之上的，它们提供了可靠的双向字节流通信信道
- Internet domain 数据报 socket 是基于 UDP 之上:
    - UNIX domain 数据报 socket 是可靠的,但 UDP socket 是不可靠的
    - UNIX domain 数据报 socket 发送数据会在接受socket的数据队列为满时阻塞,而 UDP socket 会静默丢弃溢出的数据报

# 59.2 网络字节序
- IP地址 和 端口号 都是整数值
- 存储整数时先存储 `最高有效位`的被称为 `大端`,先存储 `最低有效位`的被称为 `小端`(X86)

- 一些硬件结构可以在这两种格式之间转换
- 在特定主机上使用的字节序被称作`主机字节序`

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205241312542.png)

- 网络中使用的标准字节序为 `网络字节序` , 是 `大端`

## 在 主机字节序 和 网络字节序 之间转换
- `htons()`、 `htonl()`、 `ntohs()`以及 `ntohl()`函数被定义（通常为宏）用来在主机和网络字节序之间转换整数

```c
#include <arpa/inet.h>

uint16_t htons(uint16_t host_uint16);
//return host_uint16 coverted to network byte order

uint32_t htonl(uint32_t host_uint32);
//return host_uint32 coverted to network byte order

uint16_t ntohs(uint16_t net_uint16);
//return net_uint16 coverted to host byte order

uint32_t ntohl(uint32_t net_uint32);
//return net_uint32 coverted to host byte order
```

- 程序员应该总是使用这些函数，这样程序就能够在不同的硬件结构之间移植了
- 在主机字节序与网络字节序相同的系统上, 这些函数只是简单返回传递给他们的参数

# 59.3 数据表示
- 由于在数据表现上存在差异，因此在网络中的`异构系统`之间交换数据的应用程序必须要采用一些`公共规则`来编码数据
- 一种通用方法:所有传输的数据编码成 `文本形式`，其中数据项之间使用`特定的字符`来分隔开，这个特定的字符通常是 `换行符`

- 网络编程是最有可能遇到需要处理异构系统间数据传输的场景

- 如果将在一个流 socket 上传输的数据编码成使用换行符分隔的文本，那么`定义`一个诸如 `readLine()` 之类的函数将是比较便捷的
```c
#include "readLine.h"

sszie_t readLine(int fd, void *buffer, size_t n);
//return number of bytes copied into buffer, 0 on EOF, -1 on error
```
- readLine()函数从文件描述符参数 fd 引用的文件中读取字节直到碰到换行符为止
- 返回的字符串总是以 null 结尾，因此实际上至多有（n– 1）个字节会返回

# 59.4 Internet socket 地址
- `IPv4` 与 `IPv6`

## IPv4 socket地址: `struct sockaddr_in`
- 一个IPv4地址存储于一个 `sockaddr_in` 结构中, 该结构定义于 `<netinet/in.h>`

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205241333058.png)

## IPv6 socket地址: `struct sockaddr_in6`
- 与 IPv4 地址一样，一个 IPv6 socket 地址包含一个 IP 地址和一个端口号，它们之间的差别在于 IPv6 地址是 `128位` 而不是 32位
- 存储于 `sockaddr_in6` 结构

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205241336887.png)

## 调配 和 回环 地址
- IPv4 与 IPv6 均有 通配与回环地址
- IPv6通配地址: 0::0
    - 系统定义了常量`IN6ADDR_ANT_INIT`表示这个地址
```c
#define IN6ADDR_ANT_INIT { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} }
```
- 在变量声明的初始化器中可以使用 IN6ADDR_ANY_INIT 常量，但无法在一个赋值语句的右边使用这个常量(C 语法不允许在赋值语句使用一个结构化的常量)

