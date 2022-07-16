# 8.1 密码文件:/etc/passwd
- UNIX广泛使用的密码加密算法是DES
- 单击系统中所有密码都存储在`etc/passwed`
```
realdaz:x:1000:1000::/home/realdaz:/bin/zsh
```
`登录名:加密的密码:用户ID:组ID:注释:主目录:登陆shell`

# 8.2 shadow 密码文件:/etc/shadow
- 为了防止加密算法被破解
- 非敏感信息放在人人可读的密码文件中,经过加密的密码有shadow密码文件单独维护,仅供特权程序读取



# 8.3 组文件:/etc/group
`realdaz:x:1000:`
组名:加密密码:组ID:用户列表

# 8.4 获取用户和组的信息
## 从密码文件获取记录
## 从组文件获取记录
## 扫描密码文件和组文件中的所有记录

# 8.5 密码加密和用户认证
- UNIX采用单向加密算法对密码进行加密
- 加密算法封装于crypt()函数之中
```c
#define _XOPEN_SOURCE
#include <unistd.h>

char *crypt(const char *key, const char *salt);
// return pointer on success, or NULL on error
```
- 接受最长`8字符`的密码
- `salt`为一个`两字符`的字符串
- 读取密码的程序应立即加密密码,并尽快将密码的明文从内存中抹去。只有这样,才能基本杜绝如下事件的发生:恶意之徒借程序崩溃之机,读取内核转储文件以获取密码

