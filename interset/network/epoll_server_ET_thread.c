#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>

typedef struct socketinfo {
  int fd;
  int epfd;
} socketInfo;

void *acceptConn(void *arg) {
  printf("accept connection: %ld", pthread_self());
  socketInfo *info = (socketInfo *)arg;
  int cfd = accept(info->fd, NULL, NULL);

  int flag = fcntl(cfd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(cfd, F_SETFL, flag);

  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = cfd;
  int ret = epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
  if (ret == -1) {
    perror("ctl error");
    exit(1);
  }

  free(info);
  return NULL;
}

void *communication(void *arg) {
  printf("communication: %ld", pthread_self());
  socketInfo *info = (socketInfo *)arg;
  int fd = info->fd;
  int epfd = info->epfd;
  char buf[5];
  char temp[4096];
  bzero(temp, sizeof(temp));
  while (1) {
    int len = recv(fd, buf, sizeof(buf), 0);
    if (len == -1) {
      if (errno == EAGAIN) {
        printf("数据接受完毕\n");
        int ret = send(fd, temp, strlen(temp)+1, 0);
        if(ret == -1) {
          perror("send error");
        }
        break;
      }
      perror("recv error");
      // exit(1);
      break;
    } else if (len == 0) {
      printf("客户端断开连接!\n");
      epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
      break;
    }
    printf("origin buf: %s\n", buf);
    for (int i = 0; i < len; i++) {
      buf[i] = toupper(buf[i]);
    }
    strncat(temp+strlen(temp), buf, len);
    write(STDOUT_FILENO, temp, len);
  }

  free(info);
  return NULL;
}

int main(int argc, char **argv) {
  int serverFd;
  int ret;
  socklen_t len;
  struct sockaddr_in serveraddr, clientaddr;
  char readBuf[1024] = {0};
  char ip[40] = {0};
  if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("socket error\n");
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
    printf("bind error\n");
    exit(1);
  }
  if ((ret = listen(serverFd, 128)) != 0) {
    close(serverFd);
    printf("listen error\n");
    exit(1);
  }

  int epfd = epoll_create(1);
  if (epfd == -1) {
    perror("create epoll");
    exit(1);
  }

  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = serverFd;
  ret = epoll_ctl(epfd, EPOLL_CTL_ADD, serverFd, &ev);
  if (ret == -1) {
    perror("epoll_ctl");
    exit(1);
  }

  struct epoll_event evs[1024];
  int size = sizeof(evs) / sizeof(evs[0]);

  while (1) {
    int num = epoll_wait(epfd, evs, size, -1);
    printf("%d\n", num);
    pthread_t tid;
    for (int i = 0; i < num; i++) {
      int fd = evs[i].data.fd;
      
      socketInfo *info = (socketInfo *) malloc(sizeof(socketInfo));
      info->fd = fd;
      info->epfd = epfd;
      // epoll系列函数为线程安全的,无需加锁
      if (fd == serverFd) { // 监听套接字
        pthread_create(&tid, NULL, acceptConn, info);
        pthread_detach(tid);
      } else { // 通信
        pthread_create(&tid, NULL, communication, info);
        pthread_detach(tid);
      }
    }
  }

  close(serverFd);

  return 0;
}
