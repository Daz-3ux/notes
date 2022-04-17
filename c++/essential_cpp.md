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

# 面向过程的编程风格

## 2.1 撰写函数
与c无异
```cpp
int test_some[int abc];
```
## 2.2 调用函数
- 调用函数时会建立起一个“程序栈”(program stack)

- 传值与传址(pass by reference)
    - 面对reference的所有操作都像面对“reference所代表之对象”   

- 生存空间(scope)和生存范围(extent)

- 动态内存管理
    - 在堆区分配内存(heap memory)
    - 从heap中配置数组
```cpp
int *pia = new int[24];
//C++未提供任何语法让我们得以从heap配置数组的同时为其元素设定初值
```
    - 使用delete释放
    - 如果要删除数组所有对象：
```cpp
delete [] pia;
``` 
~~原世上无内存泄漏~~

## 2.3 提供默认参数
- c++允许我们为所有参数或部分参数设定默认值
    - 默认值的决议(resolve)操作由最右侧开始
    - 默认值只能够指定一次，或函数声明处，或函数定义处

## 2.4 使用局部静态对象
- local static object
- 和局部**非静态**对象不同的是，局部**静态**对象所处的空间，即使在不同的函数调用过程中，依然持续存在

## 2.5 声明一个inline函数
- 将函数声明为inline，表示要求编译器在每个**函数调用点**上将函数内容**展开**
```cpp
inline bool fibon_elem(int pos, int &elem);
```
- inline仅仅是一种请求而没有强制性，具体实现参考编译器
- 适用于 体积小，常被调用，所从事计算不复杂 的函数

## 2.6 提供重载函数
- Overloaded Functions
- 利用这一特性我们可以传入**不同类型甚至不同数量**的参数给display_message()
- 可以使参数表不相同的两个或多个函数拥有相同的函数名称
- 由编译器**根据具体参数**去确定该使用哪个函数(所以函数签名不可以相同)
- 编译器无法根据函数返回值来区分两个具有相同名称的函数

## 2.7 定义并使用Template Function
- 模板函数
- 利用这一特性我们可以将单一函数的内容与希望显示的各种型别绑定起来
```cpp
template <typename elemType>
```
- 每当用户使用这个模板产生函数时，其就必须提供确定的型别类型
- func tmeplate的参数表通常都由两类型别构成：
    - 明确的类型
    - 暂缓决定的类型

## 2.8 函数指针带来更大的弹性
- Pointers to Function

## 2.9 设定头文件
- .h 
- inline函数的定义必须置于头文件

# 泛型编程风格
STL由两种组件构成：
- 容器(vector,lisy,set,map...)
- 泛型算法(find(), sort(), replace()...) : 用以操作容器

## 3.1 指针的算术运算
- “哨兵”可以表示出数组最后一个元素的下一个地址，但不能读写

## 3.2 了解Iterators
- 泛型指针