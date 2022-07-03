# 三种方式:
- select()
    - 底层为线性表
    - 跨平台通用
- poll()
    - Linux Api, Windows中对应为WSAPoll()
    - 底层为线性表
- epoll()
    - 底层为`红黑树`
    - `Linux`专属