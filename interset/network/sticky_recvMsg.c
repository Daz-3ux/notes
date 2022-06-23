#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int recvMsg(int cfd, char **msg) {
    // 首先接受报头
    int len = 0;
    readn(cfd, (char*)&len, 4);
    len = ntohl(len);
    printf("需接收的数据包大小: %d\n", len);

    // 根据大小分配内存
    char *buf = (char *)malloc(len + 1);
    int ret = readn(cfd, buf, len);
    if(ret != len) {
        close(cfd);
        free(buf);
        return -1;
    }
    buf[len] = '\0';

    *msg = buf;

    return ret;
}