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

