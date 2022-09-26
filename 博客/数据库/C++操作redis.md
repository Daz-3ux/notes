# 在C++下借助hiredis使用redis数据库

## 简介
- 本文为我在编写C++聊天室项目时使用redis的经验之谈,主要讲解如何使用`C++`去调用redis数据库,并将其封装为一个类,方便程序随时调用
- 因为项目并没有使用到redis的订阅发布模式,所以本文所提及的均为redis的键值命令,大概如下:
  - key
  - string
  - hash
  - list
  - set
- 调用redis的方法是借助hiredis接口使用redis命令进操作


## 关于hiredis
- `hiredis`是redis数据库的简约`C客户端库`，是redis官方的C语言客户端，支持所有命令(command set)，管道(pipelining)，时间驱动编程(event driven programming)
- `hiredis项目地址`: github地址：https://github.com/redis/hiredis

## 编写为C++类

### 头文件
```cpp
#include <hiredis/hiredis.h>

#include <iostream>
#include <list>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
```

### 类的主要结构
```cpp
class Redis 
{
public:
  // redis的构造函数
  Redis() {
    m_redis = NULL;
    init();
  }
  // redis的析构函数
  ~Redis() {
    if (m_redis != NULL) {
      redisFree(m_redis);
      std::cout << "redis存储完毕" << std::endl;
    }
  }

  bool setString(std::string key, std::string value)
  {
    // SET命令
  }

  std::string getString(std::string key)
  {
    // GET命令
  }

  bool delKey(std::string key)
  {
    // DEL命令
  }

  bool setVector(std::string key, std::vector<int> value)
  {
    // RPUSH命令
  }

  std::vector<int> getVector(std::string key)
  {
    // LRANGE命令
  }

  bool setHash(std::string key, std::string field, std::string value)
  {
    // HSET命令
  }

  int hashExist(std::string key, std::string field)
  {
    // HEXIST命令
  }

  std::string getHash(std::string key, std::string field)
  {
    // HGET命令
  }

  std::vector<std::string> getHashKey(std::string key)
  {
    // HKEYS命令
  }

  bool hashDel(std::string key, std::string field)
  {
    // HDEL命令
  }

  bool saddValue(std::string key, std::string value)
  {
    // SADD命令
  }

  int sismember(std::string key, std::string value)
  {
    // SISMEMBER命令
  }

  bool srmmember(std::string key, std::string value)
  {
    // SREM命令
  }

  int hlen(std::string key)
  {
    // HLEN命令
  }

  int slen(std::string key)
  {
    // SLEN命令
  }

  std::vector<std::string> smembers(std::string key)
  {
    // SMEMBERS命令
  }

  int lpush(std::string key, std::string value)
  {
    // LPUSH命令
  }

  int llen(std::string key)
  {
    // LLEN命令
  }

  redisReply **lrange(std::string key)
  {
    // LRANGE命令
  }

  redisReply **lrange(std::string key, int a, int b)
  {
    // LRANGE命令:指定范围
  }

  bool ltrim(std::string key)
  {
    // LTRIM命令
  }

  bool lrem(std::string key, std::string value) {
    // LREM命令
  }

  std::string lpop(std::string key) {
    // LPOP命令
  }

private:
  void init() {
    // 调用hiredis接口完成初始化
    struct timeval timeout = {1, 50000};  // 连接等待时间为1.5秒
    m_redis = redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if (m_redis->err) {
      // 自己编写的错误处理函数
      my_error("RedisTool : Connection error", __FILE__, __LINE__);
    } else {
      std::cout << "init redis success" << std::endl;
    }
  }

  redisContext *m_redis;
};
```

### 调用demo
- 在我的聊天室项目中,`只有服务器连接了redis`,主要用来存储和获取用户数据

```cpp
#include <myredis.hpp>

int main()
{
  // 初始化自定义的redis类
  Redis myredis;

  // 查询当前用户名是否已经注册
  int flag = myredis.hashExist(UserMap, json["name"]);
  if(flag) {
    std::cout << "用户名已注册" << std::endl;
  }else {
    std::cout << "用户名未注册" << std::endl;
  }
  /*
  int hashExist(std::string key, std::string field) {  // HEXISTS
    redisReply *reply;
    reply = (redisReply *)redisCommand(m_redis, "HEXISTS %s %s", key.c_str(),
                                       field.c_str());
    if (reply == NULL) {
      redisFree(m_redis);
      m_redis = NULL;
      return -1;
    }
    return reply->integer;
  }
  */
}
```

## 完整代码
- 完整的代码在我的Github仓库,附带有简单注释
- 地址:https://github.com/Daz-3ux/tasks/blob/master/chat/include/REDIS.hpp