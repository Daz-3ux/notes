# 23.1 间隔计时器
- interval timer
- `settimer()`:
  - 创建一个间隔计时器
- `gettimer():`
  - 可在任何时间调用
  - 返回定时器当前状态,距离下次到期的剩余时间

- `alarm()`:
  - 更简单的定时器接口

# 23.2 定时器的调度及精度
- 现代Linux内核可以选择支持高分辨率定时器

# 23.3 为阻塞操作设置超时
- 实时定时器的用途之一是为某个阻塞系统调用设置其处于阻塞状态的时间上限

# 23.4 暂停运行(休眠)一段固定时间
- 使用休眠函数比使用定时器更简单

# 23.4.1 低分辨率休眠:sleep()
# 23.4.2 高分辨率休眠:nanosleep()

# 23.5 POSIX时钟
- POSIX时钟提供的时钟访问 API 可以支持纳秒级的时间精度,其中表示纳秒级时间值的 timespec 结构同样也用于`nanosleep()`调用

- POSIX时钟API的主要系统调用:
  - 返回当前值:clock_gettime()
  - 返回时钟分辨率:clock_gettres()
  - 更新时钟:clock_settime()

- 高分辨率休眠的改进版:clock_nanosleep()

# 23.6 POSIX 间隔式定时器
## 23.6.1 创建定时器:timer_create()
## 23.6.2 配备和解除定时器:timer_settime()
## 23.6.3 获取定时器的当前值:timer_gettime()
## 23.6.4 删除定时器:timer_delete()

# 23.7 利用文件描述符进行通知的定时器:timerfd API
- Linux 特有的 timerfd API 提供了一组创建定时器的接口,与 POSIX 定时器 API 相类似,但允许从文件描述符中读取定时器通知
- 还可使用 select()、poll()和 epoll()来监控这些描述符