- 日志模块: redo log
  - WAL 技术: Write-Ahead-Logging
  - InnoDB 引擎独有的技术
    - InnoDB 依靠 redo log 实现 crash-safe
  - 当有一条记录需要更新,就先将记录写到 redo log,并更新内存,空闲后再写入磁盘
  - redo log的大小是固定的
    - 由 write pos 记录当前位置
    - 由 checkpoint 记录当前要擦除的位置
    - 环形
  - `innodb_flush_log_at_trx_commit` 设置为 1 时,表示每次事务的 redo log 都直接持久化到磁盘

- 日志模块: binlog
  - server 层日志: binlog
  - 与 redo log 的不同
    - redo log 为 InnoDB 独有,binlog 由 Server 层实现,所有引擎都可以使用

- 两阶段提交
  - 为了让两份日志之间的逻辑一致
1. 写入 redo log, 处于 prepare 状态 [InnoDB 内部]
2. 写 binlog [执行器]
3. 提交事务,处于 commit 状态 [InnoDB] 内部
- 它们有一个共同的数据字段,叫XID。崩溃恢复的时候,会按照顺序扫描redo log:
  1.如果碰到既有prepare、又有commit的redo log,就直接提交
  2.如果碰到只有prepare、而没有commit的redo log,就拿着XID去binlog找对应的事务
