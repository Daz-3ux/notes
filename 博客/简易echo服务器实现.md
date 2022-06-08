![](https://img-blog.csdnimg.cn/img_convert/54e60afdf2764a07539da3136f3ce3e4.png)
本作品采用[知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议](https://creativecommons.org/licenses/by-nc-sa/4.0/)进行许可。

---
# intro
- 本文将详细讲解一个简易echo服务器的实现
- 本文实现的简易echo服务器仅支持将用户在`客户端`的输入显示在`服务器`
- 使用C语言编写
- 文章中代码为追求简洁，不附带错误检查,详细源码[在这里]()

- `echo服务器`就像是网络编程的`Hello World!`,是一定要掌握的
- 我的编码环境:
    - OS:GUN/Linux Mint发行版
    - IDE:vscode
    - COMPILER:GCC

# 实现逻辑与效果
## 实现逻辑
![](https://s3.bmp.ovh/imgs/2022/05/22/375e213ef34cc9aa.png)

## 效果预览
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202206081849492.png)


# 头文件与主要函数
## 头文件
- head.h
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXBUFFER 256
```

## 主要函数
- socket():系统调用创建一个新 socket
- bind():系统调用将一个 socket 绑定到一个地址上。通常，服务器需要使用这个调用来将其 socket 绑定到一个众所周知的地址上使得客户端能够定位到该 socket 上
- listen():系统调用允许一个 流socket 接受来自其他 socket 的接入连接
- accept():系统调用在一个 监听流socket 上接受来自一个对等应用程序的连接， 并可选地返回对等 socket 的地址
- connect():系统调用建立与另一个 socket 之间的连接
- close():终止一个流 socket 连接

- bzero():从 BSD 衍生出来的 bzero()函数是一个可以用来取代memset()对一个结构的内容进行清零的函数,但SUSv4规范已将其删除,本文为追求简单就使用它了
- inet_pton():
    - `p`为展现(presentation),`n`为网络(network)
    - 展现形式是人类可读的形式,IPv4中点分十进制地址即为"展现的",例如`127.0.0.1`
    - 网络形式即为网络字节序的二进制IP地址,只有电脑可以理解

# SHOW ME THE CODE
- server.c
```c
#include "head.h"

int main(int argc, char **argv)
{
    int serverFd, connfd;
    int ret;
    socklen_t len;
    // 定义服务器结构体与客户端结构体
    struct sockaddr_in serveraddr, clientaddr;
    // 将缓冲区全部置0
    char readBuf[MAXBUFFER] = {0};
    // 存放ip地址
    char ip[40] = {0};
    // 使用socket()创建一个新的套接字
    (serverFd = socket(AF_INET, SOCK_STREAM, 0));
    // 重置服务器结构体
    bzero(&serveraddr, sizeof(serveraddr));
    // 设置结构体参数
    serveraddr.sin_family = AF_INET;//IPv4协议
    serveraddr.sin_port = htons(1234);//端口号
    // 将点分十进制ip地址转换为机器可读的二进制格式
    inet_pton(AF_INET, "127.0.0.1" , &serveraddr.sin_addr);
    /* 
    将socket绑定到指定地址
    SA*是为了将IPv4的结构体格式转换为通用的结构体格式
    */
    bind(serverFd, (struct sockaddr *)&serveraddr,sizeof(serveraddr));
    // 开始监听地址,允许其他socket接入连接
    listen(serverFd, 5);
    // 清空客户端结构体
    bzero(&clientaddr, sizeof(clientaddr));

    len = sizeof(clientaddr);
    while(1){//循环,一直处理客户端连接
        // 建立与其他socket的连接
        connfd = accept(serverFd, (struct sockaddr *)&clientaddr, &len);
        printf("%s 连接到服务器\n", inet_ntop(AF_INET, &clientaddr.sin_addr,ip, sizeof(ip)));
        // 打印客户端输入
        while((ret = read(connfd, readBuf, MAXBUFFER))){
            // 一定要加`\n`,因为终端是`行缓冲模式`
            printf("%s\n", readBuf);
        }
        // 一个客户端退出后关闭与其的连接
        close(connfd);
    }
    close(serverFd);
    return 0;
}
```

- client.c
```c
#include "head.h"

int main(int argc, char **argv)
{
    int clientFd;
    struct sockaddr_in serveraddr;
    char buf[MAXBUFFER];
    clientFd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(1234);
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
    connect(clientFd, (struct sockaddr *)&serveraddr,
    sizeof(serveraddr));
    while(1){
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        write(clientFd, buf, sizeof(buf));
    }

    close(clientFd);
    return 0;
}
```

## 编译运行
```
gcc server.c -o server
gcc client.c -o client
./server

<Ctrl+T>
./client
```

- 通过修改demo中的ip地址,也可实现在手机中通过运行`Termux`将服务器放置在手机中,支持将从电脑中输入的字符打印在手机上,非常有趣

---
参考:
- 《TLPI》
- 《UNIX网络编程卷1》



