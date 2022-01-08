# 动态内存分配

*有助于消除程序内部存在的限制*

## malloc() & free()

C库函数提供函数进行动态内存分配和释放
最基础：
```c
#include<stdlib.h>
void *malloc(size_t size);
void free(void *pointer);
```

- _malloc()_
    - malloc从内存池中提取一块合适的内存，并向程序返回一个指向这块内存的**指针**
    - 这块内存没有被**初始化**
    - malloc分配的是连续的字节
    - 分配失败则返回NULL
    - malloc返回的指针类型是 __void*__，可以自己动手进行强制类型转换
    - malloc返回的值总是符合字节对齐

- _free()_
    - free的参数要么是NULL，要么是一个从malloc、calloc、realloc返回的值

## plus：calloc()、realloc()
```c
void *calloc(size_t num_elements,size_t element_size);
void realloc(void *ptr,size_t new_size);
```

- _calloc()_
    - 在返回指向内存的指针前会把内存全部**初始化为0**
    - 合理使用会很方便，但切记不要画蛇添足
    - calloc的参数包括**所需元素数量**和**每个元素的字节数**

- _realloc()_
    - 用于修改一个原先已经分配的内存块的大小
    - 可以**扩**，也可以**减**
    - 扩：原内存块保留，**新内存块被添加到原内存块后面，且未被初始化**
    - 减：内存块**尾部**的内存被拿掉，剩余部分保留
    - 如果原内存块**无法被修改**，那么realloc会分配一块正确大小的内存并把原内容**复制**过来
    - 所以，在使用完realloc后，就**不能再使用指向旧内存的指针**，而是应该改用realloc返回的新指针
    - 如果realloc的第一个参数为NULL，则其行为**等同于malloc**

## 使用动态分配的内存及其常见错误
- 使用俺基本会
- 常见错误：
    - 对NULL进行解引用
    - 对分配的内存操作时越界
    - 释放**并非动态分配**的内存
    - 释放动态分配的内存的**一部分**（可以另辟蹊径使用realloc完成【虽然只能释放后面的内存】）
    - 内存在被释放后继续被使用

- **分配完后一定要写一个检查释放分配成功的函数**
```c
char *new_string;
new_string  = (char*)malloc(sizeof(char) * 100);
if(new_string == NULL){
    printf("动态内存分配内存失败");
    exit(1);
}
```
也可以使用宏定义实现一个不易发生错误的内存分配器（现在还不会，只懂皮毛，等会了发博客）

## 内存泄漏
- 只要不换，榨干可用内存，memory leak
- 是一个很严重的问题
- **记得free！！！**
