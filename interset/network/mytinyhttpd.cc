/*
根据tinyhttpd仿写的httpd实现
*/

#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

#define Isspace(x) isspace((int)(x))
#define SERVER_STRING "Server: dazhttpd/0.1.0\r\n"

void *accept_request(void *);

// 编写顺序 : main -> startup -> accept_request -> execute_cgi -> ...

int startup(u_short *);
void error_die(const char *);
void *accept_request(void *);


/*
此函数在一个特定端口启动监听网页连接的进程
如果初始端口号是0,将动态分配一个端口并且改变原始端口变量(传入传出参数),以反应真实的端口号
参数:指向包含要连接的端口的变量的指针
返回值:socket端口号
*/
int startup(u_short *port)
{

}

/*
一个请求导致服务器端口上的accept()调用返回
在正确的时机处理请求
变量:连接到客户端的socket
*/
void *accept_request(void *client1)
{

}

/*
使用perror打印错误信息(针对系统错误;基于errno变量值确定系统调用错误)
以及当检测到错误时退出进程
*/
void error_die(const char *sc)
{

}


int main(int argc, char *argv[])
{
  int server_sock = -1;
  u_short port = 0;
  int client_sock = -1;
  struct sockaddr_in client_name;
  socklen_t client_name_len = sizeof(client_name);
  pthread_t newthread;

  server_sock = startup(&port); // 初始化
  std::cout << "httpd running on prot " << port << std::endl;

  while(1) {
    client_sock = accept(server_sock, (struct sockaddr *)&client_name, &client_name_len);
    if(client_sock == -1) {
      error_die("accept");
    }
    if(pthread_create(&newthread, NULL, accept_request,
                      (void *)&client_sock) != 0) {
      perror("pthread_create");
    }
  }
  
  close(server_sock);

  return 0;
}