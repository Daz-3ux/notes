```
MySQL 可以恢复到半个月内任何一秒的状态
```

- 日志模块: redo log
```
当有一条记录需要更新的时候，InnoDB 引擎就会先把记录写到 redo log 里面，并更新内存，这个时候更新就算完成了。同时，InnoDB 引擎会在适当的时候，将这个操作记录更新到磁盘里面，而这个更新往往是在系统比较空闲的时候做
```
  - WAL 技术: Write-Ahead-Logging
  - `InnoDB 引擎独有的技术`
    - InnoDB 依靠 redo log 实现 crash-safe
  - 当有一条记录需要更新,就先将记录写到 redo log,并更新内存,空闲后再写入磁盘
  - redo log的`大小是固定`的
    - 由 write pos 记录当前位置
    - 由 checkpoint 记录当前要擦除的位置
    - 环形
![redo log](https://static001.geekbang.org/resource/image/16/a7/16a7950217b3f0f4ed02db5db59562a7.png?wh=1142*856)
  - `innodb_flush_log_at_trx_commit` 设置为 1 时,表示每次事务的 redo log 都直接持久化到磁盘

- 日志模块: binlog
  - `server 层日志`: binlog
  - 与 redo log 的不同
    - redo log 为 InnoDB 独有
      - binlog 由 Server 层实现,所有引擎都可以使用
    - redo log 是物理日志,记录 "在某个数据页上做了什么修改"
      - binlog 是逻辑日志,记录语句的原始逻辑,比如 "给 ID=2 这一行的 c 字段加 1"
        - 物理日志就只有“我”自己能用，别人没有共享我的“物理格式”
        - 逻辑日志可以给别的数据库，别的引擎使用，已经大家都讲得通这个“逻辑”；
    - redo log 是循环写的,空间会固定用完
      - binlog 是追加写入,binlog 文件到了一定大小后会切换到下一个,并不会覆盖以前的日志

![process](https://static001.geekbang.org/resource/image/2e/be/2e5bff4910ec189fe1ee6e2ecc7b4bbe.png?wh=1142*1522)
  - 浅色为 InnoDB 内部, 深色为执行器中执行
- 两阶段提交
  - 为了让两份日志之间的逻辑一致
1. 写入 redo log, 处于 prepare 状态 [InnoDB 内部]
2. 写 binlog [执行器]
3. 提交事务,处于 commit 状态 [InnoDB] 内部
- 它们有一个共同的数据字段,叫XID。崩溃恢复的时候,会按照顺序扫描redo log:
  1. 如果碰到既有prepare、又有commit的redo log,就直接提交
  2. 如果碰到只有prepare、而没有commit的redo log,就拿着XID去binlog找对应的事务
- 奔溃恢复的时候，redo log prepare + binlog 完整，数据库会自动进行 commit 操作

## 小结
- redo log 保证 crash-safe 能力
- innodb_flush_log_at_trx_commit = 1 时, 事务的 redo log 直接写入磁盘
- sync_binlog = 1 时, 事务的 binlog 直接写入磁盘

---
2023-10-28

@2 10-30