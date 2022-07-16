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
    this->_connect = redisConnect(host.c_str(), port);
    if(this->_connect != NULL && this->_connect->err) {
      printf("connect error: %s\n", this->_connect->errstr);
      return false;
    }
    return true;
  }

  std::string get(std::string key)
  {
    this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());
    std::string str = this->_reply->str;
    freeReplyObject(this->_reply);
    return str;
  }

  void set(std::string key, std::string value)
  {
    redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());
  }



private:
  redisContext *_connect;
  redisReply *_reply;
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