#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
  // 套接字创建
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if(fd == -1){
    perror("socket");
    exit(1);
  }

  // 连接服务器
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(4321);
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
  int ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));

  if(ret == -1) {
    perror("connect");
    exit(1);
  }

  // communication
  char recvBuf[1024];
  while(1) {
    fgets(recvBuf, sizeof(recvBuf), stdin);
    write(fd, recvBuf, strlen(recvBuf)+1);
    read(fd, recvBuf, sizeof(recvBuf));
    printf("recvBuf: %s\n", recvBuf);
    //sleep(1);
  }

  close(fd);
  return 0;
}