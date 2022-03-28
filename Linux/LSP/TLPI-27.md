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
默认由exec()的调用程序所打开的文件描述符在exec()执行时会保持打开状态，且在新程序中依然有效，shell利用这一特性为其所执行的程序处理**IO重定向**
```
ls /tmp > dir.txt
```
以上shell命令执行以下步骤：
1. 调用fork()创建子进程
2. 子shell以描述符1(标准输出)打开文件用于输出
3. 子shell执行程序ls，ls将其结果输出到标准输出，亦即文件中
```
描述符0：标准输入，指代为进程提供输入的文件
描述符1：标准输出，指代为进程写入输出的文件
描述符2：标准错误，指代供进程写入错误信息或异常通知的文件
在shell或程序中指向终端
在stdio函数库中与stdin，stdout，stderr对应
```

### 执行时关闭(close-on-exec)标志(FD_CLOEXEC)
- 执行(exec) 时 关闭(fd)
- 在执行exec()之前，从安全编程角度出发，程序有时需要确保关闭某些特定的文件描述符
- 如果文件描述符有**执行时关闭标志**，在成功执行exec()时，会自动关闭该文件描述符，若exec()执行失败，文件描述符保持开启状态
- 使用dup()、dup2()或fcntl()为一文件描述符创建副本时，总会清除副本描述符的执行时关闭标志

## 信号与exec()
- exec()会将现有进程的文本段丢掉，也包含调用进程创建的信号处理程序
- 内核会将对所有已设信号的处置重置为SIG_DFL，而对其他信号的处置则保持不变
- 在调用exec()期间，进程型号掩码以及挂起(pending)信号的设置均得以保存

## 执行shell命令：system()
- 程序可通过调用system()函数来执行任意的shell命令
- 可运用fork(),exec(),wait(),exit()来实现system()

```c
#include<stdlib.h>

int system(const char *command);
```
- 函数sysytem()创建一个子进程来运行shell，并以之执行命令command
- 效率较低
- 如果所有的系统调用都成功，system()返回执行command的子shell的终止状态
```
e.g.:
system("ls | wc");
```

### 在设置set_user-ID和set-group-ID程序中避免使用system()
shell对操作的控制依赖于各种环境变量，此时使用system()会不可避免的给系统带来安全隐患

## system()的实现
简化版：
```c
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int system(char *command)
{
    int status;
    pid_t childPid;

    switch(childPid = fork()){
        case -1://error
            return -1;
        case 0://child
            execl("/bin/sh","sh","-c",command,(char*) NULL);
            _exit(127);
        default://parent
            if(waitpid(childPid,&status,0) == -1){
                return -1;
            }else{
                return status;
            }
    }
```

### 在system()内部正确处理信号
- 给system()的实现带来复杂性的是对信号的正确处理
- 进阶版system():**下次一定**