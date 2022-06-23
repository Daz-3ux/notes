/*
    readn() 的参数与read()相同
    循环使用了read()系统调用
    确保请求的字节数总是能够得到全部传输
    ssize_t readn(int fd, void *buffer, size_t count);
    return number of bytes read, 0 on EOF, -1 on failure
*/

#include <unistd.h>
#include <sys/socket.h>

ssize_t readn(int fd, char *buf, size_t size) {
    char *p = buf;
    int count = size;
    while (count > 0) {
        int len = recv(fd, p, count, 0);
        if(len == -1) {
            return -1;
        }else if(len == 0) {
            return size-count;
        }
        p += len;
        count -= len;
    }
    return size;
}

