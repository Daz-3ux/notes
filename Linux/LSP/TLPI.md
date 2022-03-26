```
2022.3.24
在看书，但是知识不进脑子，还是得靠做一些笔记来辅助阅读
电子pdf实在是太冷冰冰了
目标:希望自己可以慢慢完善笔记，形成一个框架
```

```
book:《Linux/Unix系统编程手册》
参考：
- https://github.com/hsf1002/linux-note
- https://github.com/sunhuiquan/tlpi-learn
```

# （上-24） 进程的创建
- 本章概述了fork(),exit(),wait(),execve(),具体讲述了fork函数

## fork()
- 创建新进程
- 父子进程执行相同的文本段，但却拥有各自不同的栈段，数据段和堆段**拷贝**
- 成功了会**返回两次**，子进程返回0，父进程返回子进程ID
- 失败则返回-1
- 父子进程可以存在文件共享：子进程会获得父进程的文件描述符的副本
- fork()后常伴随exec()，所以对父进程的复制有一系列操作去避免浪费（比较高深）
- fork()与wait()的组合可以控制一个进程的内存需求(暂时还不太懂)

## vfork()
- 怪异版fork()，为了提升性能而生
- 但能用到的地方很少
- 尽量不要使用

## fork()后的竞争状态
- 无论父，子谁先访问cpu，都差别不大，听内核的话
- 不要对fork()之后的执行顺序做任何假设
- 想要特定的话需采取同步技术
- 同步技术的一种:**使用signal**

# (上-25) 进程的终止
- 调用exit() 或 _exit()以终止一个进程
- 运用exit handler在进程调用exit()时自动执行清理动作
- fork(),stdio缓存区,exit()之间的交互

## 进程的终止
- 两种终止方式：
    - 异常终止（可能产生core dump）
    - 正常终止

- _exit()
```c
#include<unistd.h>

void _exit(int status);
```
-
    - status参数定义了进程的终止状态（父进程可调用wait()以获取该状态）
    - 父进程只可使用status的低八位
    - 调用_exit()的程序**总会成功终止**(_exit()从不返回)
    - 非0值表示异常退出
    - 0值为“功成身退”

- exit()
```c
#include<stdlib.h>

void exit(int status);
```
-
    - 程序一般不调用_exit(),而是使用**库函数**exit()
    - exit()调用前会执行以下动作：
    1. 调用退出处理程序（exit handler）
    2. 刷新stdio流缓存区
    3. 使用由status提供的值执行_exit()**系统调用**
    - return n 等同于执行 exit(n)
    - return未指定返回值或无return时C89与C99标准对其处理结果不同
        - C89:status为随机值
        - C99:等同于调用exit(0)

## 进程终止的细节
很丰富，很细节，很看不懂
慢慢就会懂得
we will know

## 退出处理程序
- exit handler
- 由程序设计者提供
- 于进程生命周期的任意时点注册，在程序调用exit()正常终止时**自动执行**
- 程序异常终止则不调用

### 注册退出处理程序
GUN C语言函数库提供两种方式来注册退出处理程序
#### atexit()
```c
#include<stdlib.h>

int atexit( void(*func)(void));
```
-
    - atexit()将func加到一个函数列表中，进程终止时调用该函数列表的所有函数
    - 出错时返回非零值
    - func()应无参数无返回值、
```c
void func(void)
{
    //do something
}
``` 
- 
   - 可以注册多个退出处理程序（包括一个函数多次注册）
   - 处理函数执行顺序与注册顺序相反
   - 有一个处理函数出错就不会再调用剩余的处理函数
   - 程序应避免在退出处理程序内部使用exit()
   - fork()出的子进程会继承父进程的退出处理函数
   - exec()会移除掉已注册的退出处理函数 
- atexit()注册的退出处理程序受到的的限制：
    - 退出处理程序无法获知传递给exit()的状态
    - 无法给退出处理程序指定参数

#### on_exit():atexit()的非标准替代
```c
#define _BSD_SOURCE
#include<stdlib.h>

int on_exit( void(*func)(int,void *),void *arg);
```
```c
void func(int status,void *arg)
{
    //perform cleanup action
}
```
- func()的参数:
    - status：提供给exit()
    - arg：注册时供给on_exit()的一份arg参数拷贝
- 出错时返回非0值
- 使用atexit()和on_exit()注册的函数位于同一函数列表
- 若需确保可移植性则不要使用此函数

### fork(),stdio缓存区以及_exit()之间的交互
- 混合使用stdio函数和系统调用 对同一文件 进行IO处理时，需特别注意：
    - fork()之前应该强制使用fflush()刷新stdio缓存区，或者使用setbuf()关闭stdio缓存区
    - 调用_exit()而非exit()可以避免刷新stdio缓冲区
    - 一个通用原则：父进程使用exit()终止，子进程使用_exit()终止，从而确保只有一个进程调用退出程序并刷新stdio缓冲区

# （上-26） 监控子进程
两种监控子进程的技术：
- 系统调用wait()
- 信号SIGCHLD

## 等待子进程
父进程经常需要检测子进程的**终止时间**和**过程**

### 系统调用wait()
等待调用进程的任一子进程终止，同时在参数status所指向的缓存区中返回该子进程的终止状态
```c
#include<sys/wait.h>

pid_t wait(int *status);
```
系统调用wait后执行如下操作：
- 如果进程所有子进程都在运行，则阻塞
- 如果一个子进程已经终止，正等待父进程获取其终止状态，则取得该子进程的终止状态且立即返回
- 如果没有子进程，则出错返回-1并将errno置为ECHILD

### 系统调用waitpid()
waitpid()的诞生是为了突破wait()存在的诸多限制
- 如果父进程拥有多个子进程，wait()无法等待某个**特定**子进程，只能按顺序等待
- 如果没有子进程，wait()总是保持阻塞
- wait()只能发现那些已经终止的子进程

```c
#include<sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int option);
```
pid表示需要等待的具体子进程，可对应多种意义：
- pid > 0:pid为需要等待的具体子进程的ID
- pid = 0:等待与调研进程(父进程)同一个process group的所有子进程
- pid < -1:等待进程组标识符与pid绝对值相对的所有子进程
- pid = -1:等待任意子进程
option为一个位掩码，可包含0至多个标识

### 等待状态值
- wait()与waitpid()返回的status值，可用来区不同的子进程事件
- 可用标准宏来处理status值

### 从信号处理程序中终止进程
捕获某些可以终止进程的信号，在进程终止前执行一些清理步骤（类似退出处理程序）

### 系统调用waitid()
```c
#include<sys/wait.h>

int waitid(idtype_t idtype,id_t id,siginfo_t *infop, int options);
```
- idtype为P_ALL，则等待任何子进程，同时忽略id值
- idtype为P_PID，则等待 进程ID为id 的进程的子进程
- idtype为P_PGID，则等待 进程组ID为id 的各进程的所有子进程

waitid()相较于waitpid()可实现更精确的控制子进程事件

### wait3()与wait4()
- 类似waitpid()
- 可返回子进程的资源使用情况

## 孤儿进程与僵尸进程
父进程与子进程的生命周期一般都不相同

### 孤儿进程
- init的进程ID为1，为众进程之祖
- 某一进程的父进程终止后会成为orphan，由init接管

### 僵尸进程
- 内核将先于wait()终止的子进程转为僵尸进程，确保父进程总是可以使用wait()
- 僵尸进程无法杀死，只可调用wait()从系统中移除

## SIGCHLD信号
- 无论一个信号于何时终止，系统都会向其父进程发送SIGCHLD信号
- 可以采用SIGCHLD信号的**处理程序**来避免特定情况下wait()与waitpid()造成的浪费

### 为SIGCHLD信号建立信号处理程序
- 为保证可移植性应在任何子进程创建之前就设置好SIGCHLD处理程序

### 向已停止的子进程发送SIGCHLD信号
- 若未使用SA_NOCLDSTOP标志，系统会在子进程停止时向父进程发送SIGCHLD信号
- 若使用了此标志，子进程停止时就不会向父进程发送SIGCHLD信号
- 当信号SIGCHLD导致已停止的子进程恢复执行时，也向父进程发送SIGCHLD信号

### 忽略终止的子进程
将对SIGCHLD的处置显式置为SIG_IGN，系统从而将其**后**终止的所有子进程立即删除，避免转换为僵尸进程


# (上-27) 程序的执行
介绍execve()与system()

## 执行新程序：execve()
```c
#include<unistd.h>

int execve(const char *pathname,char *const argv[],char *const envp[]);
```
- Never return onsuccess, returns -1 on error
- pathname为新程序的路径名（绝对相对都可以）
- argv为命令行参数
- envp为新程序的环境列表
- 将新程序加载到某一**进程**的内存空间，丢弃旧有程序，进程的栈，数据以及堆段会被新程序的相应部件替换
- 最频繁的用法：由fork（）生成的子进程对execve()进行调用
- 调用execve()后进程ID不变

## exec()库函数
- 为执行exec()提供了多种API选择，均构建于execve()之上

### 环境变量PATH
- PATH的值是一个以冒号分隔，由多个目录名（路径前缀）组成的字符串

### 解释器脚本
- 解释器：能够读取并执行文本格式命令的程序
- 脚本需满足两点要求
    - 必须赋予脚本文件可执行权限
    - 文件的起始行必须指定运行脚本解释器的路径名
```
//initial line格式
#! interpreter-path [option-arg]
```
- path一般采用相对路径
- arg的用途之一是为解释器指定命令行参数

## 文件描述符与exec()
