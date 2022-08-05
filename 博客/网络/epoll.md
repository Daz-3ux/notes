![](https://img-blog.csdnimg.cn/img_convert/54e60afdf2764a07539da3136f3ce3e4.png)
本作品采用[知识共享署名-非商业性使用-相同方式共享 4.0 国际许可协议](https://creativecommons.org/licenses/by-nc-sa/4.0/)进行许可。

---

# 概述
- `e`vent`poll`
- 高效：
  - 底层为`红黑树`
  - 使用`回调机制`而不是线性扫描,处理效率不会随着集合的变大而下降
  - 使用`共享内存`(内核与用户区之间),避免了频繁拷贝
- 没有最大文件描述符限制:将用户所关心的文件描述符上的事件放在内核的一个事件表中,同时epoll需要一个`额外的文件描述符`来唯一标识内核中的这个事件表
- `线程安全`,无需进行共享资源管理

# 函数
```c
#include <sys/epoll.h>
// 创建epoll实例
int epoll_create(int size);
```
- `size`参数无实际意义
- 返回内核中epoll`事件表`的文件描述符

```c
#include <sys/epoll.h>
// 管理epoll红黑树(添加, 修改, 删除)
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```
- fd:要操作的文件描述符
- op:option
  - `EPOLL_CTL_ADD`: 向事件表中注册fd上的事件
  - `EPOLL_CTL_MOD`: 修改fd上的注册事件
  - `EPOLL_CTL_DEL`: 删除fd上的注册事件
- event:
```c
struct epoll_event
{
  __uint32_t events;  // epoll事件
  epoll_data_t data;  // 用户数据
}
```
  - events:描述用户事件
    - EPOLLIN:读事件
    - EPOLLET:边缘触发模式
    - EPOLLONESHTO:在注册了此事件的文件描述符上最多触发`一个`可读/可写/异常事件(进一步减少可读,可写和异常等事件被触发的次数)
  - data:用于存储用户数据:其为`联合体`,有四种方式描述用户,最常用的为fd
```c
typedef union epoll_data
{
  void *ptr;
  int fd;
  uint32_t u32;
  uint43_t u64;
}epoll_data_t;
```

```c
#include <sys/epoll.h>
// 检测epoll树中是否有就绪的文件描述符
int epoll_wait(int epfd, struct epoll_event *event, int maxevents, int timeout);
```
- 主要接口
- epoll_wait如果检测到事件,就将所有`就绪的事件`从内核表中复制到第二个参数event指向的数组中(这个数组只用于输出就绪事件),极大提高了效率
- 处理epoll返回的就绪文件描述符:
```c
int ret = epoll_wait(epfd, events, MAX_EVENT_NUMBER, -1);
// 仅遍历ret个文件描述符
for(int i = 0; i < ret; i++) {
  int sockfd = events[i].data.fd;
}
```


# 工作模式
## 水平模式
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

## 边缘触发
- edge trigger(`ET`)
- 高速工作模式,只支持`no-block socket`
- 通知次数少(`只有新事件才会通知`,降低了同一事件被重复触发的次数),编写较难,但效率高
- 读事件:
  - 当读缓冲区有新的数据进入,读事件触发`一次`,没有新数据不会触发事件
  - 如果数据没有被全部取走,`并且没有新数据进入`,读时间不会再次出发,只通知一次
  - 如果数据被全部取走或只取走一部分,`此时有新数据进入`,读事件触发,并且只通知一次
- 写事件:
  - 当写缓冲区可写,写事件只触发`一次`
  - 写缓冲区从不满到被写满,期间写事件只触发一次
  - 写缓冲区从满到不满,状态变为可写,写事件只会被触发一次

---
# refer
- 《TLPI》
- 《高性能服务器编程》--游双
- 爱编程的大丙