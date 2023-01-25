#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

void DaytimeServer::onConnection(const TcpConnection &conn)
{
  LOG_INFO << "DaytimeServer - " << conn
}