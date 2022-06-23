#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>


int sendMsg(int cfd, char* msg, int len)
{
    // 错误处理
    if(cfd <= 0 || msg == NULL || len <= 0){
        return -1;
    }

    // 分配空间
    char *data = (char *)malloc(len+4);
    /* 
    字符串没有字节序问题
    数据头为整型,存在字节序问题
    需要将数据头转换网络字节序
    */
    int bigLen = htonl(len);

    // 写入数据
    memcpy(data, &bigLen, 4);
    memcpy(data+4, msg, len);

    // 发送数据
    int ret = writen(cfd, data, len+4);

    // 释放内存
    free(data);

    return ret;
}