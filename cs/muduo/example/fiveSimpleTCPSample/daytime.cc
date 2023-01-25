#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

void DaytimeServer::onConnection(const TcpConnection &conn) {
  LOG_INFO << "DaytimeServer - " << conn->peerAddress.toIpPort() << " -> "
           << conn->localAddress.toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");

  if (conn->connected()) {
    conn->send(Timestamp::now().toFormattedString + "\n");
    conn->shutdown();
  }
}