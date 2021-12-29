# string.h

- 因为C中没有显示的字符串数据类型，所以字符串以字符串常量形式存储在**字符数组**中

- 库函数strlen（）的返回值为size_t类型：
    - size_t是无符号整型，永远不会小于0
    - size_t定义在stddef.h头文件中

## 字符串函数：

***
### 不受限制的字符串函数

- 复制：strcpy()
    - 把后一个参数复制给前一个参数

- 连接：strcat()
    - 把后一个连接到前一个的后面
    - 被连接字符串可以是空字符串
    - 连接完后在末尾会加'\0'

- 复制与连接的返回值均为第一个参数的拷贝，即指向目标字符数组的指针
    - 技巧：
```c
strcat(strcpy(dst,a) , b);
```
只是花里胡哨和分开写比起来没啥优势，因此其返回值经常被忽略

- 比较：strcmp()
    - s1 < s2返回小于零的值
    - s1 > s2返回大于零的值
    - s1 == s2返回等于零的值

*** 
### 长度受限的字符串函数
```c
- char* strncpy(char* dst,char const* src,size_t len);
- char* strncat(char* dst,char const* src,size_t len);
- int strcmp(char const *s1,char const *s2,size_t len);
```

### 查找函数
- 查找一个函数：
    - strchr(char const *str, int ch):指向待字符第一次出现的位置的指针
    - strrchr(char  const *str,int ch):指向待查字符最后一次出现的位置的指针
    - 不存在则返回NULL 

- 查找任意几个字符：
    - strpbrk():查找一组字符中任意一个字符在字符串中第一次出现的位置的指针
    - 不存在则返回NULL

- 查找一个子串：
    ```c
    - strstr(char  const *s1, char const *s2)
    ```
    - 在s1中查找整个s2第一次出现的位置，并返回指向该位置的指针
    - 未找到则返回NULL，s2为空则返回s1；

### 高级字符串查找
- 简化了从一个字符串中查找和抽取一个子串的过程

- 查找一个字符串前缀  
```c
size_t strspn(char const *str,char const* group);
size_t strcspn(char const *str,char const* group);
```
    - 用于在字符串的起始位置对字符串计数。
    - group字符串包含一个或多个字符，strspn返回str起始部分匹配group中任意字符的字符数
    - strcspn与strspn相反，对不与group中任意字符匹配的字符进行计数
```c
        - ptr = buffer + strspn(buffer,"\n\r\f\t\v"); 
```
指向字符串中第一个非空白字符

- 查找标记
```c
char* strtok(char* str,char const* sep);
```
  - 从字符串中隔离各个单独的称为标记（token）的部分，并丢弃分隔符
  - sep参数为字符串，定义了用作**分隔符**的**字符集合**
  - str含有零至多个分隔符
  - 该函数返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针
  - 如果源字符串不能被修改，函数将返回**源函数的拷贝** 

### 错误信息
```c
char* strerror(int error_number);
```

### 字符操作
**ctype.h**

### 内存操作
- 对于含NULL的字符串很好用，因为字符串函数遇NULL则停止
```c
void *memcpy(char *dst,void const* src,size_t len);
void *memove(char *dst,void const* src,size_t len);
void *memcmp(char *a,void const* b,size_t len);
void *memchr(char *a,int ch,size_t len);
void *memset(void *a,int ch,size_t len);
```
- memcpy():从src起始位置复制len个字符到dst的内存初始位置，如果src与dst以任何方式出现重叠，其结果为**未定义**
- memmove()：与memcpy差不多，但源与目标可重叠
- memcmp()：比较len个字符，返回值同strcpy逻辑相同
- memchr()：从a的起始位置开始查找ch第一次出现的位置，并返回指向该位置的指针，共查len个字符
- memset()：从a的起始位置开始将len个字符都设置为**字符值ch**

# tips
- 不要重复造轮子
- 使用**字符分类**与**转换函数**可以提高函数的移植性