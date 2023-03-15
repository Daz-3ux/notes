>最近小组的大家都在选择想要参加的开源社区,在 GSoC 的组织里发现了 `criu` 这个很有趣的项目.  
>在初看之下,感觉 criu 的组成就是 `Linux kernel` + `Linux System Programming` + `Container`,用的语言是 `C` + `Golang`,这两个我最近很感兴趣的语言,所以在初见之下就对这个项目很感兴趣.  
>这篇博客主要记录自己对 criu 的入门

```
System: Linux archlinux 6.2.2-arch1-1 x86_64 GNU/Linux

CRIU:  
  Version: 3.17
  GitID: v3.17-177-gcd9680ce8

$SHELL: /usr/zsh
```

# OverView
- 项目主页:https://criu.org/Main_Page
- 仓库地址(GitHub):https://github.com/checkpoint-restore/criu
- 简介(Generate By ChatGPT)
>CRIU（Checkpoint/Restore In Userspace）是一个开源工具，用于在用户空间实现进程快照（checkpoint）和恢复（restore）。使用 CRIU 可以在不依赖虚拟化的情况下，将进程状态保存到文件中，然后在另一个时间点恢复该进程状态。这个功能可以在容器化环境、分布式系统、高可用系统等场景下发挥重要作用。
>
>CRIU 社区是由一群来自不同组织的开发者组成的社区，其中最知名的是 OpenVZ 和 Docker。社区致力于改进 CRIU 工具的性能和功能，同时提供文档、例子和交流平台，以帮助使用者更好地理解和应用 CRIU 工具。
>
>以下是 CRIU 社区的一些特点：
>
>  开源：CRIU 工具是完全开源的，任何人都可以查看、修改和分发源代码。这使得 CRIU 工具成为了容器化、分布式等场景下的标准工具之一。
>
>  跨平台：CRIU 工具可以在多种 Linux 发行版上运行，包括 Red Hat、Debian、Ubuntu 等。
>
>  功能强大：CRIU 工具可以保存和恢复多种进程状态，包括进程内存、文件描述符、网络连接等。
>
>  活跃的社区：CRIU 社区活跃，提供多种文档、例子和交流平台，帮助使用者更好地理解和应用 CRIU 工具。
>
>总之，CRIU 是一个功能强大的进程快照工具，可以在容器化、分布式等场景下发挥重要作用。CRIU 社区是一个活跃的开源社区，为用户提供了各种支持和帮助。

# 安装与编译
- 在开始之前先安装必要的依赖,照着报错装就好了


- 拉取代码并编译
```bash
git clone https://github.com/checkpoint-restore/criu
cd criu
make
```
![安装成功](https://i.imgur.com/dOHV92d.png)

- 运行并检查
```bash
./criu/criu --version

sudo ./criu/criu check
```

# 简单的使用示例
#### 为一个循环打印当前时间的`shell script`创建快照
1. 编辑 test.sh 文件
```bash
> vim test.sh
---

#!/bin/sh
while :; do
	sleep 1
	date
done
EOF

:wq
```

2. 设置执行权限
```bash
> chmod +x test.sh
```

3. 在特定会话运行 test.sh(消除对当前终端的依赖)
```bash
> setsid ./test.sh  < /dev/null &> test.log &
```
![](https://i.imgur.com/08cmeXA.png)
- 启动新会话并在后台运行脚本
- 标准输出重定向到 `/dev/null`
- 标准输出和标准错误保存到 `test.log` 文件

4. 获取 PID
```bash
> ps -C test.sh
```
![](https://i.imgur.com/6MZyF40.png)

5. Dump
```bash
> criu dump -t 141117 -vvv -o dump.log && echo OK
```
![](https://i.imgur.com/dxQlDVD.png)
- -t:指定 PID
- -vvv:输出详细的调试信息
- -o:指定转储文件输出路径

6. 检查 dump files
```bash
> ls
```
- ~~criu-daz 是我拉取的仓库~~  
![](https://i.imgur.com/QxtNVtr.png)

7. restore
```bash
> criu restore -d -vvv -o restore.log && echo OK
```
![](https://i.imgur.com/zbdkOoa.png)

# 基本原理
- criu 完全在用户空间实现,不依赖特定的 Linux 内核模块
![](https://i.imgur.com/j6kIuaO.png)  
- 采用了类似 `gdb` 的 debug 技术来控制,获取进程的信息,代码主要集中在
  - 进程中断
  - 寄生控制
  - 恢复
- `Dump 机制`
  - Parasite Code
    - receive file descriptor
    - dump memory content
    - Prctl(), sigaction, pending signals, timers, etc
  - Ptrace
    - freeze processes
    - Inject a parasite code
  - NetLink
    - Get Information about Sockets, netns
  - procfs
- `Restore 机制`
  - Collect shared object
  - Restore name-space
  - Create a process tree
    - Restore SID, PGID
    - Restore objects, which should be inherited
  - Files, sockets, pipes...
  - Restore per-task properties
  - Restore memory
  - Call sigreturn




# 代码组成
![](https://i.imgur.com/Di0Rkiq.png)

### compel
- 寄生代码注入

### crit
- 管理 checkpoint 存储的`镜像文件`

### criu
- 包含 CRIU 项目的`主要源代码文件`
- 执行 checkpoint 和 restore 过程的命令管理工具
#### crtools
- `main(int argc, char *argv[], char *envp[])` 函数在 `crtools.c` 文件中
- main():
  - 处理各种异常
  - 若用户输入格式错误,则 `goto` 至正确的 `usage` 页面
  - 解析命令行参数,获取返回值,根据返回值继续执行操作

### OTHER
- .circleci
- .github
- contrib
- coredump
- Documentation
- flog
- images
  - 包含检查点和恢复镜像，以及用于检查点和恢复进程的配置文件
- include
  - 包含 CRIU 的头文件，用于构建 CRIU 的库和应用程序
- lib
  - 包含 CRIU 的库文件，用于构建 CRIU 的应用程序
- plugins
- scripts
  - 包含 CRIU 的脚本文件，用于构建 CRIU 的应用程序和检查点和恢复镜像
- soccr
- test

# Refer
1. [MRAS 项目技术调研报告CRIU-Checkpoint/Restore in User-space](https://www.lizhaozhong.info/uploads/2015/03/CRIU.pdf)