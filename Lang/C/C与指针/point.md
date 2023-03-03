# 函数指针
```c
int f();        //一个返回整数的函数
int *f();       //一个返回值类型是 指向整型的指针 的函数
int (*f)();     //f是一个函数指针，它所指向的函数返回一个整形值
int *(*f)();    //f是一个函数指针，它所指向的函数返回一个整型指针
```

```c
int f[];        //整型数组
int *f[];       //整型指针数组
int f()[];      //错误：函数只能返回标量，不能返回数组
int f[]();      //错误：数组元素必须具有相同的长度，但不同的函数可能有不同的长度
```

```c
int (*f[])();   //将 f 声明为 指向返回int的函数 的 指针数组
int *(*f[])();  //将 f 声明为 指向返回整型指针的函数 的 指针数组
```

- 函数指针的主要用途有两处：
    - 作为参数传递给另外一个函数
    - 转换表
- 函数指针使用前必须 **初始化** 为指向某个函数
```c
int (*pf) (int) = &f;
```
- 上述初始化中`&`操作符是可选的：函数名被使用时编译器自动将其转换为函数指针，&操作符只是显示声明
    - 所以以下三种方式均正确
```c
int ans;

ans = f(25);
ans = (*pf)(25);
ans = pf(25);
```
- 第一句函数名f首先被转换为一个函数指针，该指针指向函数在内存中的位置，然后函数调用操作符调用该函数，执行这个地址的代码
- 对pf执行间接引用并非必要，编译器会在执行函数调用时又将其转换回去

## 回调函数
- 字符串和数组无法作为参数传递给函数，但指向它们的指针可以
- 使用此技巧的函数叫做回调函数

- demo：与类型无关的 链表查找函数
```c
#include <stdio.h>
#include <stdlib.h>

int compare(void const *a, void const *b)
{
    if( *(int*)a == *(int*)b ){
        return 0;
    }else{
        return 1;
    }
}

Node *search_node(Node *node, void const *value,int(*compare)(void const *, void const *))
{
    while(node != NULL){
        if(compare(&node->value, value) == 0){
            break;
        }
        node = node->next;
    }
    return node;
}

```

## 转移表
- jump table
- demo：计算器
```c
double add(double, double);
double sub(double, double);
double mul(double, double);
double div(double, double);

double (*oper_func[])(double, double) = {add, sub, mul, div};

result = oper_func[oper](op1, op2);
```
- oper从数组中选择正确的函数指针，而**函数调用操作符** 将执行这个函数