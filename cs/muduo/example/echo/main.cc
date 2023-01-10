#include "echo.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"

#include <unistd.h>

int main()
{
  LOG_INFO << "pid = " << getpid();
  muduo::net::EventLoop loop;
  muduo::net::InetAddress listenAddr(7890);
  EchoServer server(&loop, listenAddr);
  server.start();
  loop.loop();
}