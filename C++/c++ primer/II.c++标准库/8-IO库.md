# 8.1 IO类

- 三个独立头文件
1. `iostream`：读写 **流** 类型(处理控制台IO)
2. `fstream`：读写 **命名文件** 类型
3. `sstream`：读写 **内存string对象** 类型

- 标准库使我们能忽略不同类型之间的流之间的差异：通过 **继承机制** 实现

- 不能拷贝或对IO对象赋值
- 进行IO操作的函数通常以引用方式传递和返回流(不能const：读写一个IO会改变其状态)

- `endl`：换行+显示刷新缓冲区
- `ends`：空字符+显示刷新缓冲区
- `flush`：显示刷新缓冲区

- `unitbuf`操作符：
```cpp
cout << unitbuf;//所有输出操作后都立即刷新缓冲区
//无缓冲
cout << nounitbuf;//回到正常缓冲模式
```

# 8.2 文件输入输出
头文件`fstream`支持文件IO函数：
- `ifstream`：从给定文件读数据
- `ofstream`：从给定文件写入数据
- `fstream`：读写给定文件

- 文件模式：
    - in：以读模式打开
    - out：以写模式打开
    - app：写操作前均定位到文件结尾
    - ate：打开后立即定位到文件结尾
    - trunc：截断文件
    - binary：以二进制方式IO

- `以out模式打开文件会丢弃已有数据`，保存数据唯一方法是 **显示指定app或in模式**
- `每次调用open时都会确定文件模式`

# 8.3 string流
- `istringstream`：从string读取数据
- `ostringstream`：向strin写入数据
- `stringstream`：可读可写