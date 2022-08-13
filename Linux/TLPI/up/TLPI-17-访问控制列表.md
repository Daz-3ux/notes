# 17-1 概述
- `ACL`
- 利用ACL可以在任意数量的用户和组之中,为`单个`用户或组指定文件权限
- 一个ACL由一系列`ACL记录(简称ACE)`组成,每条记录都针对单个用户或用户组定义了对文件的访问权限

![](https://i.imgur.com/okIwDn6.png)

# 17-5 `getfacl` 和 `setfacl` 命令
- 在 shell 中运行 getfacl 命令,可查看到应用于文件的 ACL
- setfacl 命令可用来修改文件的 ACL

