# Redis 简介
## 用途
- 主数据库
  - primary database
- 辅助数据库
  - auxiliary database

## 持久化
- 两种方式
1. 时间点转储
  - "指定时间段内有指定数量的写操作"
  - 调用 dump-to-disk 命令中的任意一条
2. 将所有修改了数据库的命令都写入一个只追加文件中
  - 从不同步
  - 每秒同步一次
  - 每一个命令同步一次

## 五种常用的
- string
- list
- set
- hash
- zset
  - 键被称为`成员`(member)
  - 值被称为`分值`(score),必须为浮点数