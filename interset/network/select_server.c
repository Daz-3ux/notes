#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/select.h>

#define MAXBUFFER 256

pthread_mutex_t mutex;

typedef struct fdinfo {
  int fd;
  int *maxFd; // 传入传出参数
  fd_set *readSet;
} fdInfo;

void *acceptConn(void *arg) {
  printf("子线程ID: %ld\n", pthread_self());
  fdInfo *info = (fdInfo *)arg;
  int cfd = accept(info->fd, NULL, NULL);
  pthread_mutex_lock(&mutex);
  FD_SET(cfd, info->readSet);
  *info->maxFd = cfd > *info->maxFd ? cfd : *info->maxFd;
  pthread_mutex_unlock(&mutex);

  free(info);
  return NULL;
}

void *communication(void *arg) {
  printf("子线程ID: %ld\n", pthread_self());
  char buf[1024];
  fdInfo *info = (fdInfo *)arg;
  int len = recv(info->fd, buf, sizeof(buf), 0);
  if (len == -1) {
    perror("recv");
    free(info);
    return NULL;
  } else if (len == 0) {
    printf("客户端断开连接");
    pthread_mutex_lock(&mutex);
    FD_CLR(info->fd, info->readSet);
    pthread_mutex_unlock(&mutex);
    close(info->fd);
    free(info);
    return NULL;
  }
  printf("read buf = %s\n", buf);
  for (int i = 0; i < len; i++) {
    buf[i] = toupper(buf[i]);
  }
  printf("after buf = %s\n", buf);

  int ret = send(info->fd, buf, strlen(buf) + 1, 0);
  if (ret == -1) {
    perror("send error");
    exit(1);
  }

  free(info);
  return NULL;
}

int main(int argc, char **argv) {
  pthread_mutex_init(&mutex, NULL);
  int serverFd;
  int ret;
  socklen_t len;
  struct sockaddr_in serveraddr;
  char readBuf[MAXBUFFER] = {0};
  char ip[40] = {0};
  if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket error\n");
    exit(1);
  }
  setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, NULL, 0);
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(4321);
  inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
  if ((ret = bind(serverFd, (struct sockaddr *)&serveraddr,
                  sizeof(serveraddr))) != 0) {
    close(serverFd);
    perror("bind error\n");
    exit(1);
  }
  if ((ret = listen(serverFd, 64)) != 0) {
    close(serverFd);
    perror("listen error\n");
    exit(1);
  }

  fd_set readSet;
  FD_ZERO(&readSet);
  FD_SET(serverFd, &readSet);

  int maxFd = serverFd;
  while (1) {
    pthread_mutex_lock(&mutex);
    fd_set tmp = readSet;
    pthread_mutex_unlock(&mutex);
    int ret = select(maxFd + 1, &tmp, NULL, NULL, NULL);
    // 判断是不是监听fd
    if (FD_ISSET(serverFd, &tmp)) {
      // 创建子线程
      pthread_t tid;
      fdInfo *info = (fdInfo *)malloc(sizeof(fdInfo));
      info->fd = serverFd;
      info->maxFd = &maxFd;
      info->readSet = &readSet;
      pthread_create(&tid, NULL, acceptConn, info);
      pthread_detach(tid);
    }
    for (int i = 0; i <= maxFd; i++) {
      if (i != serverFd && FD_ISSET(i, &tmp)) {
        // 接收数据
        pthread_t tid;
        fdInfo *info = (fdInfo *)malloc(sizeof(fdInfo));
        info->fd = i;
        info->readSet = &readSet;
        pthread_create(&tid, NULL, communication, info);
        pthread_detach(tid);
      }
    }
  }

  close(serverFd);
  pthread_mutex_destroy(&mutex);

  return 0;
}
