# 什么是 CGroups
- Control Groups
- Linux 内核`限制`,`记录`,`隔离`进程组(process groups)所使用的物理资源的机制
- `Linux Containers(LXC)` 的基础
- 由 Google 工程师提出,后整合入 `Linux 内核`
- 为 `Docker` 以及 `K8s` 提供底层支持

# CGroups 可以做什么
- 为资源管理提供一个统一的框架,既整合已有的可管理系统物理资源的`子系统`,又可以为新开发的子系统提供接口
- 适用于从**单个进程的资源控制**到**操作系统层次的虚拟化**
- 提供以下功能
  - 限制进程组可以使用的资源数量
  - 进程组的优先级控制
  - 记录进程组使用的资源数量
  - 进程组隔离
  - 进程组控制

# CGroups 概念及关系
- 概念
  - 任务(task):系统的一个进程
  - 控制族群(control group):按照一组按照某种标准划分的进程
  - 层级(hierarchy):控制族群可以组织成 hierachical 的形式,即一颗控制族群树
  - 子系统(subsystem):一个子系统就是一个资源管理器,必须附加到一个层级上才可以起作用
    - `blkio`:为块设备设定输入/输出限制
    - `cpu`:提供对 CPU 的 cgroup 任务访问
    - `cpuacct`:自动生成 CGroup 中任务所使用的 CPU 报告
    - `cpuset`:为 CGroup 中的任务分配独立 CPU(多核系统中)和内存节点
    - `devices`:允许或拒绝 CGroup 中的任务访问设备
    - `freezer`:挂起或恢复 CGroup 中的任务
    - `memory`:设定 CGroup 中任务使用的内存限制,并自动生成由那些任务使用的内存资源报告
    - `net_cls`:使用等级识别符标记网络数据包
    - `ns`:名称空间子系统

- 相互关系
  - 每次在创建新层级时,该系统中的所有任务都是那个层级的 root CGroup(root CGroup 在创建层级时自动创建,后续在该层级中创建的 CGroup 都是该 CGroup 的后代) 的初始成员
  - 一个子系统最多添加到一个层级
  - 一个层级可以附加多个子系统
  - 一个任务可以是多个 CGroup 的成员,但这些 CGroup 必须在不同的层级
  - 系统中的进程在创建子进程时,该子进程自动成为其父进程所在的 CGroup 的成员

# CGroups 文件系统
- CGroup 通过 `VFS(Virtual File System)` 将功能暴露给用户态
  - CGroup 与 VFS 之间的衔接部分称之为 `CGroups 文件系统`

- `VFS`
  - VFS 可以将具体文件系统的细节隐藏起来,给用户态提供一个统一的文件系统 API 接口
  - 采用通用文件系统设计,具体的文件系统只要实现了 VFS 的实际接口就可以注册到 VFS 中
  - VFS 通用文件模型的四种元数据结构
    - 超级块对象(superblock object):用于存放已经注册的文件系统的信息
      - ext2,ext3等基础磁盘文件系统
      - 用于读写 socket 的 socket 文件系统
      - 用于读写 cgroup 配置信息的 cgroup 文件系统
    - 索引节点对象(inode object):用于存放具体文件的信息
      - 一般磁盘文件:存放文件在硬盘中的存储块等信息
      - socket 文件系统:存放 socket 相关属性
      - cgroups 文件系统:存放 CGroup 节点相关的属性信息
      - `inode_operations`结构体,定义了具体文件系统中创建文件,删除文件等的具体实现
    - 文件对象(file object):一个文件对象对应进程中打开的一个文件,文件对象存放在进程的文件描述符里
      - `file_operation`:定义了具体的文件读写实现
    - 目录项对象(directory entry object):内核在查找某一个路径中的文件时,会为内核路径上的每一个分量都生成一个目录项文件
      - 通过目录项对象能够找到对应的 inode 对象
      - 目录项对象一般会被缓存,提高内核查找速度
```
目录项文件是一种特殊的文件，它只包含目录项的相关信息，而不包含实际的文件数据。在Linux系统中，目录项文件通常以"."或者".."命名。例如，当你访问路径"/home/user/Documents"时，内核会为每个路径分量（"/"、"home"、"user"、"Documents"）分别创建目录项文件，以便后续访问这些文件或目录。
```

# refer
- [Linux资源管理之cgroups简介](https://tech.meituan.com/2015/03/31/cgroups.html)
- [CGroups介绍.pdf](http://files.cnblogs.com/files/lisperl/cgroups%E4%BB%8B%E7%BB%8D.pdf)