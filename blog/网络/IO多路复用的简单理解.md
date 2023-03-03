# 三种方式:
- select()
    - 底层为线性表
    - `跨平台`通用
- poll()
    - Linux Api, Windows中对应为WSAPoll()
    - 底层为线性表
- epoll()
    - 底层为`红黑树`
    - `Linux`专属
    - 效率最高

## 做了什么
- 将本该由程序员处理的缓冲区维护工作交给了内核,由内核`监测`缓冲区上的事件

# select()
```c
#include <sys/select.h>

struct timeval {
    time_t          tv_sec;  //秒
    suseconds_t     tv_usec; //微秒
};

itn select(int nfds, fd_set *reabufs, fd_set *writefds,
            fd_set *exceptfds, struct timeval *timeout);
```
- nfds:委托内核监测的文件描述符数量
    - 三个集合中最大的文件描述符+1 / 直接`1024`(上限值)
    - 内核需要线性遍历这些集合中的文件描述符,`+1`这个值是循环结束的条件
    - Windows中无效,指定为`-1`即可
- readfds:内核只检测这个集合中文件描述符的`读缓冲区`
    - 传入传出参数
    - 常用
- writefds:内核只检测这个集合中文件描述符对应的`写缓冲区`
    - 传入传出参数
    - 不常用,不需要使用的话可以指定为`NULL`
- execptfds:内核只检测这个集合中文件描述符是否有`异常状态`
    - 传入传出参数
- timeout:超时时长,用来强制接触select()的阻塞
    - NULL:检测不到`就绪`的文件描述符就一直阻塞
    - 固定秒数:一直检测不到就绪的文件描述符,指定时长后解除阻塞,返回0
    - 0:不等待,不阻塞
- 返回值:
    - 大于0:成功,返回已就绪的文件描述符的个数
    - 等于-1:调用失败
    - 等于0:超时,没有检测到就绪的文件描述符

## tips
- fd_set是一个`1024bit`大小的东西,nfds的上限1024就是由此得来,fd_set拥有1042个`标志位`,每一个位都对应着`0 / 1`,代表着这个位对应的文件描述符的状态,由内核维护每一个位的状态

## 基于select处理服务器端并发的操作流程
- 对fd_set标志位处理( `FD_ZERO()` \ `FD_SET()` )
- 轮询检测指定fd( `FD_ISSET()` )

# poll()
- 不具备可移植性,性能又弱于epoll,一般不使用此函数

# epoll()
## 概述
- eventpoll
- 高效：
  - 底层为`红黑树`
  - 使用`回调机制`而不是线性扫描,处理效率不会随着集合的变大而下降
  - 使用`共享内存`(内核与用户区之间),避免了频繁拷贝
- `没有`最大文件描述符限制(取决于硬件)

## 函数
```c
#include <sys/epoll.h>

// 创建epoll实例
int epoll_create(int size);
// 管理epoll红黑树(添加, 修改, 删除)
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// 检测epoll树中是否有就绪的文件描述符
int epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout);
```

## 工作模式
### 水平模式
- level trigger(`LT`)
- `默认`工作方式,支持`block`与`no-block socket`
- 通知次数多,易于编写,但效率低
- 读事件:
  - 文件描述符对应的`读缓冲区`还有`数据`,读事件就会被触发,`epoll_wait()`解除阻塞
  - 读缓冲区一次没有读完,读事件一直触发
  - 读数据是被`被动`的,必须通过读事件才知道有数据到达了,因此对于读事件的检测是`必须`的
- 写事件:
  - 如果文件描述符对应的写缓冲区可写,写事件就会被触发,epoll_wait()解除阻塞
  - 如果写缓冲区没有写满,写事件一直触发
  - 写数据是`主动`的,并且写缓冲区一般都是可写的(未满),所以对写事件的检测`不是必须`的

### 边缘触发
- edge trigger(`ET`)
- 高速工作模式,只支持`no-block socket`
- 通知次数少(`只有新事件才会通知`),编写较难,但效率高
- 读事件:
  - 当读缓冲区有新的数据进入,读事件触发`一次`,没有新数据不会触发事件
  - 如果数据没有被全部取走,`并且没有新数据进入`,读时间不会再次出发,只通知一次
  - 如果数据被全部取走或只取走一部分,`此时有新数据进入`,读事件触发,并且只通知一次
- 写事件:
  - 当写缓冲区可写,写事件只触发`一次`
  - 写缓冲区从不满到被写满,期间写事件只触发一次
  - 写缓冲区从满到不满,状态变为可写,写事件只会被触发一次

#### ET模式下注意事项
- 一个程序如果使用了ET模式,就应该使用非阻塞的fd,避免在read或write时使其中一个任务被"锁死"
- 如何编程:
  - 使用`非阻塞的fd`
  - 在read或write返回`EAGAIN`时才进入epoll_wait的调用
- EAGAIN:
  - nonblock fd read 返回 `EAGAIN`：缓冲区数据被读完
  - nonblock fd write 返回 `EAGAIN`：表示缓冲区被写满(待数据发送出去之后,缓冲重新进入可写状态,会触发`EPOLLOUT`事件,不会造成 epoll_wait被`挂起`)

### SHOW ME THE CODE
- 设置非阻塞fd
```c
int cfd = accept(serverFd, NULL, NULL);\

int flag = fcntl(cfd, F_GETFL);
flag |= O_NONBLOCK;
fcntl(cfd, F_SETFL, flag);
```

- 设置ET模式
```c
ev.events = EPOLLIN | EPOLLET;
```

- 设置`EAGIAN`的判定
```c
while (1) {
  int len = recv(fd, buf, sizeof(buf), 0);
  if (len == -1) {
    if (errno == EAGAIN) { // 判断是异常终止或数据接收完毕
      printf("数据接受完毕\n");
        break;
    }
    perror("recv error");
    exit(1);
  } else if (len == 0) {
    printf("客户端断开连接!\n");
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    break;
  }
}
```

-------
# 参考
1. [Linux 教程](https://subingwen.cn/linux/)
2. [epoll 相关问题简单说明](https://blog.csdn.net/frodocheng/article/details/105568116)