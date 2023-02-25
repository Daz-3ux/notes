# 配置/定制应用

- 容器的两种管理配置文件的方式
  - 编写 Dockerfile:用 COPY 指令把配置文件打包到镜像
  - 运行时使用 docker cp / docker run -v:将本机文件拷贝到文件
  - 存在缺陷:笨拙

- K8s 中管理信息的两种对象:`ConfigMap` 和 `Secret`
  - 本质都是`字符串`
  - `ConfigMap`:明文配置
  - `Secret`:机密配置

# ConfigMap
- 内部数据都是 Key-Value 结构


