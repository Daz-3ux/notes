# command

## 字符串
- 可存储类型
  - 字节串(byte dance)
  - 整数
  - 浮点数

- 自增 / 自减
![](https://i.imgur.com/Z3I12ht.png)
  - 仅针对 十进制整数/浮点数
  - 对于 不存在的键/保存了空串的键, 默认将其当作0看待

- 处理字串和二进制位
![](https://i.imgur.com/W8DpDd9.png)
  - SETRANGE / SETBIT 写入
    - 如果字符串当前的长度不够, Redis 会自动使用 null 对其扩展
  - GETRANGE / GETBIT 读取
    - 超出字符串末尾的数据被视为空串
    - 超出二进制串末尾的二进制位被视为 0
      - Redis 存储的二进制位是按照偏移量`从高到低`排列的
```bash
> setbit key 2 1
(integer) 0
> setbit key 7 1
(integer) 0
> get key
"!" # ! 为 ASCII 33
```