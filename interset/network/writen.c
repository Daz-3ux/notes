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

int main(int argc, char **argv)
{
    return 0;
}