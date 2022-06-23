/*
    writen() 的参数与write()相同
    循环使用了write()系统调用
    确保请求的字节数总是能够得到全部传输
    ssize_t writen(int fd, void *buffer, size_t count);
    return number of bytes written, -1 on failure
*/

#include <unistd.h>
#include <sys/socket.h>

ssize_t writen(int fd, const char *msg, size_t size)
{   
    // 只读,所以const
    const char *buf = msg;
    int count = size;
    // 循环处理, 直至全部发送
    while(count > 0) {
        // 专用于套接字的 I/O 系统调用
        int len = send(fd, buf, count, 0);
        if(len == -1) {
            close(fd);
            return -1;
        }else if (len == 0){
            continue;
        }
        // offset
        buf += len;
        // 计算剩余待发送量
        count -= len;
    }

    return size;
}