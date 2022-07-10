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


# select
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
    - Windows中无效,指定为-1即可
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