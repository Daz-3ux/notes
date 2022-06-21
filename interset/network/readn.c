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

int main(int argc, char **argv)
{
    return 0;
}