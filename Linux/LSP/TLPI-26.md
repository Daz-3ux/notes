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

