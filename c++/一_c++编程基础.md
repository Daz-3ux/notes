- 首先就是经典的Hello,World！
```cpp
#include<iostream>
using namespace std;

cout << "Hello,World";

return 0;
```
# 1.1 BASE

## 类(class)
- 用户自定的数据型别
- class的定义一般分为两部分
    - 头文件(header file)，声明class提供的各种操作行为
    - 程序代码文件(program text)，包含操作行为的实现内容

## std
- 标准程序库所驻之命名空间(namespace)
- 命名空间是一种将程序库名称封装起来的方法

# 1.2 对象的定义与初始化
- 命名**不能**以数字开头！(同C)
- int num = 0； **==** int num(0)；
- 