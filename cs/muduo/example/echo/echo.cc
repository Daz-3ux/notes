#include "echo.h"

#include "muduo/base/Logging.h"

/*
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
*/

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

// 在构造函数中注册回调函数
EchoServer::EchoServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
    : loop_(loop), server_(loop, listenAddr, "EchoServer") {
  server_.setConnectionCallback(std::bind(&EchoServer::onConnection, this, _1));
  server_.setMessageCallback(
      std::bind(&EchoServer::onMessage, this, _1, _2, _3));
}

// 实现 EchoServer::onConnection() 和 EchoServer::onMessage()
void EchoServer::onConnection(const muduo::net::TcpConnectionPtr& conn) {
  LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? " UP " : "DOWN ");
}

void EchoServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf, muduo::Timestamp time) {
  muduo::string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " echo " << msg.size() << " byets, "
           << "data received at " << time.toString();
  conn->send(msg);
}