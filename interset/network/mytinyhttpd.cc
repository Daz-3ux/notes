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