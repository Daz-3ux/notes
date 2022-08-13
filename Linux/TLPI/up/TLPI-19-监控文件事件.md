# 19.1 概述
- Linux提供`inotify` 与 `dnotify`
- 都是Linux专有机制
- 后者比较陈旧,主要使用`inotify`允许`应用程序`监控`文件事件`

- ~~非常像epoll那一套~~
- 可使用 select()、poll()、epoll 以及由信号驱动的 I/O来监控inotify文件描述符

# 19.2 inotify API
- inotify_init()
- inotify_add_watch()
- inotify_rm_watch()

# 19.3 inotify 事件
- 使用 inotify_add_watch()删除或修改监控项时,位掩码参数 mask 标识了针对给定路径名(pathname)而要监控的事件

# 19.4 读取 inotify 事件
- 将监控项在监控列表中登记后,应用程序可用 read()从 inotify 文件描述符中读取事件,以判定发生了哪些事件
- 若时至读取时尚未发生任何事件,read()会阻塞下去,直至有事件产生(除非对该文件描述符设置了 O_NONBLOCK 状态标志,这时若无任何事件可读, read()将立即失败,并报错 EAGAIN)

# 19.5 队列限制和/proc 文件 