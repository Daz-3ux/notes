/*关于linux的知识*/

第一次开始于：
```
2021.?.?
```

没错，开始于：
```
2022.1.1
```

学习于：
1. https://www.bilibili.com/video/BV13U4y1p7kB
2. https://github.com/shihyu/MyTool/blob/master/GDB/Debugging.with.gdb%20%E4%B8%AD%E6%96%87.pdf
3. https://seisman.github.io/how-to-write-makefile/overview.html
4. https://101.lug.ustc.edu.cn/

# 常见名词解释

- GUN：Gun's NO Unix,口号
- GPL：General Public License
    - 还有类似于LGPL，BSD
    - 通用公共许可：版权协议
- FSF：自由软件基金会
- POSIX：开发标准，对操作系统调用的接口进行标准化，保证可移植性

# LINUX发行版：
- debian：ubuntu，mint
- redhat：dedora，centod

# LINUX内核
- 进程调度 **SCHED**
    - SCHED_OTHER:分时调度策略（默认）
    - SCHED——FIFO：实时调度策略，针对**实时性要求高**，**运行时间短**的进程的策略
    - SCHED_RR：实时调度策略，针对**实时性要求高**，**运行时间长**的进程的策略
- 内存管理 **MMU**
    - CPU中的一个单元，将虚拟内存单元中的数据映射到物理内存再进行调度
- 虚拟文件系统 **VFS**
    - 最常用是ext2，ext3
- 网络接口
    - 网络协议
    - 驱动程序
- 进程间通信  
    - 管道
    - 信号
    - 消息队列
    - 共享内存
    - 套接字

![kernel](https://mmbiz.qpic.cn/mmbiz_png/K0TMNq37VN2JhvhMn471iaWUhZErdXDDP3WJjiayEfxUQ9enY2HyLtvCqtH5ydyicwNQNOm6IlWnOYhic8agibVyCwA/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)


***

# LINUX目录
- 根目录是‘/’，目录结构类似于树
- 子目录各有各的功能

看完了p7

```

```
