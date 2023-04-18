# 为什么不直接使用 Pod
- K8s 使用的是 RESTful API,把集群的各种资源都抽象为 HTTP 资源对象:使用面向对象的思想
- K8s 对象设计思路
  - `单一职责`
  - 组合优于继承

# 为什么要有 Job/CronJob
- 组合了 Pod,实现了对`离线业务的处理`
- K8s 的两大业务
  - 在线业务:Nginx
  - 离线业务:busybox
    - 特点为必定会退出
    - 只对内部用户有意义
    - 分为`临时任务`以及`定时任务`
      - 临时任务就是 API 对象 `Job`
      - 定时任务就是 API 对象 `CronJob`
      - 使用这两个对象就可以在 K8s 内调度管理任意的离线业务了

# 如何使用 YAML 描述 Job
- 几个控制离线作业的重要字段
  - activeDeadlineSeconds:设置 Pod 运行的超时时间
  - backoffLimit:设置 Pod 的失败重试次数
  - Completions:Job 完成需要多少个 Pod,默认为1
  - parellelism:允许并发运行的 Pod 数量

# outlook
- CronJob 使用定时规则控制 Job
- Job 使用并发数量控制 Pod
- Pod 再定义参数控制容器
- 容器再隔离控制进程
- 进程最终实现业务功能

---
- kubectl run 只能创建 Pod
- 创建 Pod 以外的对象需要使用 kubectl create