#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

void DiscardServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf,
                              TimeStamp time) {
  std::string msg(buf->retriveAllAsString());
  LOG_INFO << conn->name() << " discards " << msg.size()
           << " bytes received at " << time.toString();
}