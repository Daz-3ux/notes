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
- IPv6 `通配地址`: 0::0
    - 系统定义了常量`IN6ADDR_ANT_INIT`表示这个地址
```c
#define IN6ADDR_ANT_INIT { {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} }
```
- 在变量声明的初始化器中可以使用 IN6ADDR_ANY_INIT 常量，但无法在一个赋值语句的右边使用这个常量(C 语法不允许在赋值语句使用一个结构化的常量)

- 初始化方式如下图

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205241745599.png)

- IPv6 `环回地址`: ::1
- 其对应的常量与变量:`IN6ADDR_LOOPBACK_INIT` 和 `in6addr_loopback`

- IPv6 与 IPv4 中相应字段不同的是:IPv6的常量和变量初始化是网络字节序的,但开发人员仍需保证端口号是网络字节序的

## `sockaddr_storage` 结构
- 在 IPv6 socket API 中新引入了一个通用的 sockaddr_storage 结构，这个结构的空间足以**存储任意类型**的 socket 地址
- 允许透明地存储 IPv4 或 IPv6 socket 地址，从而删除了代码中的 IP 版本依赖性

- `sockaddr_storage`在Linux下的实现

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205241757500.png)

# 59.5 主机和服务转换函数概述
- 主机地址和端口的表示方法
    - 主机地址:`二进制值` 或 `符号主机名` 或 `展现格式`(IPv4是点分十进制, IPv6是十六进制字符串)
    - 端口号可以表示为一个`二进制值` 或 一个`符号服务名`

- 格式之间的转换工作可以通过各种库函数来完成(分`现代API` 和 `废弃的API`)

- 在二进制和人类可读的形式之间转换 IPv4 地址
- 在二进制和人类可读的形式之间转换 IPv6 地址
- 主机和服务名与二进制形式之间的转换

# 59.6 `inet_pton()` 和 `inet_btop()` 函数
- 允许在 IPv4 和 IPv6 地址的二进制形式和点分十进制表示法或十六进制字符串表示法之间进行转换

- 函数名中 `p` 为"**展现(presentati)**", `n` 为 "**网络(network)**"
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206071617793.png)


# 59.8 域名系统 DNS
- 使用 DNS 来维护 主机名 和 IP地址 之间的映射关系
- DNS 能够处理大规模的名空间，同时无需对名字进行集中管理

## DNS 的关键思想
- 将主机名组织在一个层级名空间中, DNS 层级中的每一个节点都有一个标签(名字), 该标签最多可包含 63 个字符. 层级的根是一个无名子的节点，即“匿名节点”。
- 一个节点的域名由该节点到根节点的路径中所有节点的名字连接而成，各个名字之间
用点（ .）分隔。如 google.com 是节点 google 的域名。
- 完全限定域名（ fully qualified domain name， FQDN），如 www.kernel.org.，标识出了层级中的一台主机。区分一个完全限定域名的方法是看名字是否已点结尾，但在**很多情况下这个点会被省略**
- **没有一个组织或系统会管理整个层级**, 相反，存在一个 DNS 服务器层级，每台服务
器管理树的一个分支（一个区域）。通常，每个区域都有一个主要主名字服务器。此外，还包含一个或多个从名字服务器（有时候也被称为次要主名字服务器），它们在主要主名字服务器崩溃时提供备份。
- 区域本身可以被划分成一个个单独管理的更小的区域。当一台主机被添加到一个区域中或主机名到 IP 地址之间的映射关系发生变化时，管理员负责更新本地名字服务器上的名字数据中的对应名字
- 当一个程序调用 `getaddrinfo()`来解析（即获取 IP 地址）一个域名时, getaddrinfo()会使用一组库函数来与本地的 DNS 服务器通信。如果这个服务器无法提供所需的信息，那么它就会与位于层级中的其他 DNS 服务器进行通信以便获取信息

## 递归和迭代的解析请求
- DNS解析请求分为两类: `递归` `迭代`
- 递归请求:请求者要求服务器处理整个解析任务,必要时与其他DNS服务器进行通信任务
- 迭代:如果本地 DNS 服务器自己并没有相关信息来完成解析，那么它就会迭代地解析这个域名

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205241823397.png)

## 顶级域
- 紧跟在匿名根节点下面的节点称为顶级域(TLD)
- 分为两类:通用的 / 国家的

# 59.9 /etc/services 文件
- 端口号和服务名会记录在文件/etc/services 中
- getaddrinfo()和 getnameinfo()函数会使用这个文件中的信息在服务名和端口号之间进行转换
- /etc/services 文件仅仅记录着名字到数字的映射关系
- 它不是一种预留机制： 在/etc/services 中存在一个端口号并不能保证在实际环境中特定的服务就能够绑定到该端口上

# 59.10 独立于协议的主机和服务转换
## 59.10.1 `getaddrinfo()`函数
- 给定一个主机名和服务器名， getaddrinfo()函数返回一个 socket 地址结构列表，每个结构都包含一个地址和端口号

![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202205241830327.png)

## 59.10.2 释放addrinfo列表:`freeaddrinfo()`
- getaddrinfo()函数会动态地为 result 引用的所有结构分配内存
- 调用者必须要在不再需要这些结构时释放它们


## 59.10.3 错误诊断: `gai_strerror()`
- 给定一个getaddrinfo()函数发生错误时返回的错误码， gai_strerror()函数会返回一个描述该错误的字符串

## 59.10.4 `getnameinfo()`函数
- getnameinfo()函数是 getaddrinfo()的逆函数
- 给定一个 socket 地址结构（ IPv4 或 IPv6），它会返回一个包含对应的主机和服务名的字符串
- 在无法解析名字时返回一个等价的数值

# 59.12 Internet domain socket 库
- `inetConnect()`函数
    - 根据给定的 socket type 创建一个 socket 并将其连接到通过 host 和 service 指定的地址
    - 这个函数可供需将自己的 socket 连接到一个服务器 socket 的 TCP 或 UDP 客户端使用

- `inetListen()函数`
    - 创建一个监听流socket， 该 socket 会被绑定到由 service指定的 TCP 端口的通配 IP 地址上
    - 这个函数被设计供 TCP 服务器使用

- `inetBind()函数`
    - 根据给定的 type 创建一个 socket 并将其绑定到由 service 和 type 指定的端口的通配 IP 地址上
    - 供 UDP 服务器和创建 socket 并将其绑定到某个具体地址上的客户端使用

# 59.13 过时的主机和服务转换API
- inet_aton()
- inet_ntoa()
- gethostbyname()
- gethostbyaddr()
- getserverbyname()
- getserverbyport()

# 59.14 UNIX 与 Internet domain socket 比较
- 编写只使用 Internet domain socket 的应用程序通常是最简单的做法， 因为这种应用程序既能运行于同一个主机上， 也能运行在网络中的不同主机上
- 选择UNIX domain socket的理由
    - 在一些实现上速度更快
    - 可以使用目录(在 Linux 上是文件)权限来控制对 UNIX domain socket 的访问
    - 可以传递 `打开的文件描述符` 和 `发送者的验证信息`

