```
2022.4.10
```

开始学c++，深入学的那种，在这个过程中把C语言也进阶巩固一下吧

编程环境：Linux_Mint
第一本书：《Essential c++》

- 首先就是经典的Hello,World！
```cpp
#include<iostream>
using namespace std;

cout << "Hello,World";

return 0;
```


# c++编程基础

## 1.1 BASE

### 类(class)
- 用户自定的数据型别
- class的定义一般分为两部分
    - 头文件(header file)，声明class提供的各种操作行为
    - 程序代码文件(program text)，包含操作行为的实现内容

### std
- 标准程序库所驻之命名空间(namespace)
- 命名空间是一种将程序库名称封装起来的方法

## 1.2 对象的定义与初始化
- 命名**不能**以数字开头！(同C)
- int num = 0； **==** int num(0)；

## 1.3 撰写表达式
nothing new

## 1.4 调价与循环
nothing new

## 1.5 数组(Arrays)和向量(Vectors)
### array
- array的尺度(array所能储存的元素个数) **必须** 是一个常量表达式(constant expression)

### vector
- 定义vector object
```cpp
    #include<vector>
    const int seq_size = 18;
    vector<int>pell_seq (seq_size);
```
- 初始化vector
    1. 一个一个指定
    2. 利用已初始化的 array 作为 vector 的初值
```cpp
int test_array[array_size] = { 
    1,2,3,
    4,5,6
};
vector<int>test_vector{test_array, test_array + array_size};
```

- `test_vector.size()`会返回test_vector这个vector所包含的元素个数

## 1.6 指针带来的弹性
- 我们可以操纵指针，而不是直接操控对象，更加透明化
    - 增加了**弹性**
    - 也增加了**复杂度**
- 一定要防范**野指针**
- 一种定义
```cpp
vector<int> *seq_addrs[seq_cnt] = {//seq_addrs是一个array
    &a,&b,&c,
    &d,&e,&f
};
```

## 文件的读写
- 文件读写需头文件：
`#include<fstream>`

- cerr：standard error(无缓冲)