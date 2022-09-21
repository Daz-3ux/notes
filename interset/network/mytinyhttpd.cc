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

#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: dazhttpd/0.1.0\r\n"

void *accept_request(void *);

// 编写顺序 : main -> startup -> accept_request -> execute_cgi -> ...

int startup(u_short *);
void error_die(const char *);
void *accept_request(void *);
int get_line(int, char*, int);
int unimplemented(int);
void not_found(int);
void server_file(int, const char *);
void execute_cgi(int, const char *, const char *, const char *);

/*
从套接字获取一行:行以换行符,回车符,CRLF组合结尾均可(Carriage-Return Line-Feed)
空字符用来终止读取到的字符串
如果缓冲区满还没读到换行符,字符串就以空值结束
参数:  套接字描述符
      保存数据的缓冲区
      缓冲区的大小
返回: 存储的字节数(不包括NULL)
*/
int get_line(int sock, char *buf, int size)
{
  // 读取套接字的一行，把回车换行等情况都统一为换行符结束
  int i = 0;
  char c = '\0';
  int n;

  while((i < size-1) && (c != '\n')) {
    n = recv(sock, &c, 1, 0);
    if(n > 0) { // 将'\r'当作换行符处理
      if(c == '\r') {
        // MSG_PEEK: 仅把tcp buffer中的数据读取到buf中
        // 并不把已读取的数据从tcp buffer中移除
        n = recv(sock, &c, 1, MSG_PEEK);
        if((n > 0) && (c == '\n')) {
          recv(sock, &c, 1, 0);
        }else {
          c = '\n';
        }
      }
      buf[i] = c;
      i++;
    }
  }
  buf[i] = '\0';

  return i;
}

/*
告知客户端其请求的web方法并没有被实现
参数: 客户端socket号
*/
void unimplemented(int client)
{

}

/*
给客户端发送错误信息: 404 not found
*/
void not_found(int client)
{

}

/*
发送一个常规文件给客户端
使用headers,以及报告如果发生的错误
参数: 指向从套接字文件描述符生成的文件结构的指针
      要服务的文件的名称
*/
void server_file(int client, const char *filename)
{

}

/*
执行一个CGI脚本
需要设置适当的环境变量
参数: socket
      CGI脚本部分
*/
void execute_cgi(int client, const char *path, const char *method,
                const char *query_string)
{

}


/*
此函数在一个特定端口启动监听网页连接的进程
如果初始端口号是0,将动态分配一个端口并且改变原始端口变量(传入传出参数),以反应真实的端口号
参数:指向包含要连接的端口的变量的指针
返回值:socket端口号
*/
int startup(u_short *port)
{
  int httpd = 0;
  httpd = socket(PF_INET, SOCK_STREAM, 0);
  if(httpd == -1) {
    error_die("socket");
  }

  struct sockaddr_in name;
  bzero(&name, sizeof(name));
  name.sin_family = AF_INET;
  name.sin_port = htons(*port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(httpd, (struct sockaddr*)&name, sizeof(name)) < 0) {
    error_die("bind");
  }

  if(*port == 0) {
    socklen_t namelen = sizeof(name);
    if(getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1) {
      error_die("getsockname");
    }
    *port = ntohs(name.sin_port);
  }

  if(listen(httpd, 5) < 0) {
    error_die("listen");
  }

  return httpd;
}

/*
一个请求导致服务器端口上的accept()调用返回:处理套接字上的http请求
在正确的时机处理请求
变量:连接到客户端的socket
*/
void *accept_request(void *client1)
{
  int client = *(int*)client1;
  char buf[1024];
  int numchars;
  char method[255];
  char url[255];
  char path[512];

  size_t i,j;
  struct stat st;
  int cgi = 0;// 标识位:当服务器确定这是一个CGI程序时转为真

  char *query_string = NULL;

  // 读取http请求的request line(第一行), 把请求方法存入method
  numchars = get_line(client, buf, sizeof(buf));
  i = 0;
  j = 0;
  // 去掉换行符
  while(!ISspace(buf[j]) && (i < sizeof(method) - 1)) {
    method[i] = buf[j];
    i++;
    j++;
  }
  method[i] = '\0';

  // 如果请求不是GET 或 POST 就发送response告知客户端没实现该方法
  if(strcasecmp(method, "GET") && strcasecmp(method, "POST")) {
    unimplemented(client);
    return NULL;
  }

  // POST方法
  if(strcasecmp(method, "POST") == 0) {
    cgi = 1;
  }
  i = 0;
  // 跳过所有空白字符
  while(ISspace(buf[j]) && (j < sizeof(buf))) {
    j++;
  }
  // 读取URL放入url数组
  while(!ISspace(buf[j]) && (i < sizeof(url)-1) && (j < sizeof(buf))) {
    url[i] = buf[j];
    i++;
    j++;
  }
  url[i] = '\0';

  // GET方法
  if(strcasecmp(method, "GET") == 0) {
    query_string = url;
    while((*query_string != '?') && (*query_string != '\0')) {
      query_string++;
    }
    // 如果是?就说明此请求需要调用cgi
    if(*query_string == '?') {
      cgi = 1;
      *query_string = '\0'; // 将url分成两段
      query_string++;
    }
  }
  
  // 处理第一段
  spraintf(path, "htdocs%s", url);

  // 如果path字符串最后一个字符为 /
  // 就拼接上"index.html"的字符串:首页
  if (path[strlen(path)-1] == '/') {
    strcat(path, "index.html");
  }

  // 查询文件是否存在
  // struct stat st;
  if(stat(path, &st) == -1) {
    // 不存在: read & discard head
    while((numchars > 0) && strcmp('\n', buf)) {
      numchars = get_line(client, buf, sizeof(buf));
    }
    // 给客户端返回一个 找不到文件 的response
    not_found(client);
  }else {
    // 文件存在,判断文件类型
    // S_IFMT:可以用来过滤出前四位表示的文件类型
    if((st.st_mode & S_IFMT) == S_IFDIR) {
      // S_IFDIR : 是否为目录, 如果是目录,就需要在path后拼接一个字符串
      // 用S_ISDIR(st.st_mode)更好
      strcat(path, "/index.html");
    }

    if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) ||
        (st.st_mode & S_IXOTH)) {
          // 只要是可执行文件,就将cgi置为1
          cgi = 1;
    }

    if(!cgi){
      // 如果不需要cgi机制
      server_file(client, path);
    }else {
      execute_cgi(client, path, method, query_string);
    }
  }

  close(client);
  return NULL;
}

/*
使用perror打印错误信息(针对系统错误;基于errno变量值确定系统调用错误)
以及当检测到错误时退出进程
*/
void error_die(const char *sc)
{
  perror(sc);
  exit(1);
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