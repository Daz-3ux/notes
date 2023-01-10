#ifndef MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
#define MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H

// 定义 EchoServer class,不需要派生自任何基类

#include "muduo/net/TcpServer.h"

class EchoServer {
public:
  EchoServer(muduo::net::EventLoop* loop,
             const muduo::net::InetAddress& listenAddr);
  void start();  // calls server.start()

private:
  void onConnection(const muduo::net::TcpConnectionPtr& conn);

  void onMessage(const muduo::net::TcpConnectionPtr& conn,
                 muduo::net::Buffer* buf, muduo::Timestamp time);

  muduo::net::EventLoop* loop_;
  muduo::net::TcpServer server_;
};

#endif