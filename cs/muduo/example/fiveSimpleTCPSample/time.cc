#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

void TimeServer::onConnections(const muduo::TcpConnectionPtr &connection)
{
  LOG_INFO << "TimeServer - " << conn->peerAddress().toIpPort() << " -> "
  << conn->localAddress().toIpPort() << " is "
  << (conn->connected() ? "UP" : "DOWN");
  if(con->connected())
  {
    time_t now = ::time(NULL);
    int32_t be32 = sockets::hostToNetwork32(static_cast<int32_t>(now));
    conn->send(&be32, sizeof(be32));
    conn->shutdown();
  }
}