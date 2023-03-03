# 什么是套接字的部分I/O
- 在许多情况下，当在流式套接字上执行 `I/O` 操作时会出现`部分读取`和`部分写入`的现象
- 执行 `read()` 和 `write()` 系统调用时，在某些情况下, 可能会出现 被传输的数据 少于 请求的数据

# 什么时候会出现这种现象
- read():
    - 可用数据比read()请求的数据少,此时read()返回可用的字节数
- write():
    - write()传输了部分请求的字节后被信号处理进程中断
    - 套接字工作在非阻塞模式下(`O_NONBLOCK`),`可能`只传输一部分
    - 在部分请求的字节已经完成传输后出现了一个异步错误

# 如何解决
- 可以重新调用系统调用来完成对全部数据的传输
- 我们对`read()`与`write()`构造其各自对应的包裹函数,其可以循环调用对应的系统调用,确保请求的字节数总是可以得到全部传输
    -除非 如果出现错误 或 read()检测到了EOF 

# SHOW ME THE CODE
- readn()
```c
/*
    readn() 的参数与read()相同
    循环使用了read()系统调用
    确保请求的字节数总是能够得到全部传输
    ssize_t readn(int fd, void *buffer, size_t count);
    return number of bytes read, 0 on EOF, -1 on failure
*/

#include <unistd.h>
#include <errno.h>

//ssize_t read(int fildes, void *buf, size_t nbyte);
ssize_t readn(int fd, void *buffer, size_t n)
{
    /*
    ssize_t : signed long \ signed类型仅仅是为了处理read返回-1的情况,无其他意义
    size_t  : unsigned long
    */
    ssize_t numRead;        //  * of Bytes fetched by last read()
    size_t totRead;         // Total * of bytes read so far
    char *buf;

    buf  = buffer;
    for(totRead = 0; totRead < n; ){
        numRead = read(fd, buf, n-totRead);

        if(numRead == 0){   // EOF
            return totRead; // may be 0 if this is first read()
        }
        if(numRead == -1){
            if(errno == EINTR){
                continue;   // interrupted -> restart read()
            }else{
                return -1;  // some other error
            }
        }
        totRead += numRead;
        buf += numRead;     // offset
    }

    return totRead;         // must be 'n' Bytes if we get here
}
```

- writen()
```c
/*
    writen() 的参数与write()相同
    循环使用了write()系统调用
    确保请求的字节数总是能够得到全部传输
    ssize_t writen(int fd, void *buffer, size_t count);
    return number of bytes written, -1 on failure
*/

#include <unistd.h>
#include <errno.h>

//ssize_t write(int fildes, const void *buf, size_t nbyte);
ssize_t writen(int fd, void *buffer, size_t n)
{
    /*
    ssize_t : signed long
    size_t  : unsigned long
    */
    ssize_t numWritten;         //  * of Bytes fetched by last read()
    size_t totWritten;          // Total * of bytes read so far
    char *buf;

    buf = buffer;
    for(totWritten = 0; totWritten < n; ){
        numWritten = read(fd, buf, n-totWritten);

        if(numWritten <= 0){
            if(numWritten == -1 && errno == EINTR){
                continue;       // Interrupted -> restart write()
            }else{
                return -1;
            }
        }

        totWritten += numWritten;
        buf += numWritten;
    }

    return totWritten;         // must be 'n' Bytes if we get here
}
```

# 参考
- 《TLPI》