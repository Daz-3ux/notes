# intro
- redis是用C语言写的,所以想使用C++调用的话最好写个类封装一下
- 需要下载`redis`以及`hiredis`
- redis程序在编译时要确保使用了`redis-server`命令并且redis服务器已经运行在了终端

# code
## redis.h
```cpp
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include "hiredis/hiredis.h"

class Redis
{
public:
  Redis() {}
  ~Redis() {
    this->_connect = NULL;
    this->_reply = NULL;
  }

  bool connect(std::string host, int port) 
  {
    // 获取连接状态
    this->_connect = redisConnect(host.c_str(), port);
    // 判断连接是否正常
    if(this->_connect != NULL && this->_connect->err) {
      printf("connect error: %s\n", this->_connect->errstr);
      return false;
    }
    return true;
  }

  std::string get(std::string key)
  {
    // 获取redis命令的回复
    this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
    std::string str = this->_reply->str;
    // 释放内存
    freeReplyObject(this->_reply);
    return str;
  }

  void set(std::string key, std::string value)
  {
    // 设置键值对命令
    redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
  }



private:
  redisContext *_connect; // 获取连接(连接句柄?)
  redisReply *_reply;     // 获取redis命令的回复
};
```

## redis.cc
```cpp
#include "redis.h"

int main(int argc, char **argv)
{
  Redis *db = new Redis;
  if(!db->connect("127.0.0.1",6379)) {
    std::cout << "connect error" << std::endl;
    //perror("connect error");
    exit(1);
  }

  db->set("name", "daz");
  std::cout << "Get the name is " << db->get("name") << std::endl;

  delete db;
  return 0;
}
```

# 运行效果
![](https://raw.githubusercontent.com/Daz-3ux-Img/Img-hosting/master/202207161146730.png)