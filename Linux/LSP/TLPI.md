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
- 成功了会返回两次，子进程返回0，父进程返回子进程ID
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
    1. 调用退出处理程序
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