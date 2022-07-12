# 7.1 在堆上分配内存
## 7.1.1 调整 program break： brk()和 sbrk()
- 在 program break 的位置抬升后，程序可以访问新分配区域内的任何内存地址，而此时`物理内存页尚未分配`,内核会在`进程首次`试图访问这些虚拟内存地址时自动分配新的物理内存页
```c
#include <unistd.h>

int brk(void *end_data_segment);
void *sbrk(intptr_t increment); 
```
- brk()将program break 设置为参数指定的位置(会四舍五入到下一个边界处)
- sbrk()将 program break 在原有地址上增加从参数 increment 传入的大小,返回`指向新分配内存起始位置`的`指针`

## 7.1.2 在堆上分配内存： malloc()和 free()
- C语言标准
- 易于在多线程中使用
- 接口简单,允许分配小块内存
- 允许随意释放内存块,其被维护于一张空闲内存列表,在后续内存分配调用时循环使用

- 一般情况下， free()并不降低 program break 的位置，而是将这块内存填加到空闲内存列表
中，供后续的 malloc()函数循环使用

- Linux中,`malloc(0)`的定义为:返回一小块可以用free()释放的内存
- 给free()传入空指针并不是`错误`代码,只是free什么都不会做
- double free是严重错误

- 总是显示调用free()的好处:
  - 可读性与可维护性高
  - 易于寻找内存泄漏的真正发生处

## 7.1.3 malloc()和 free()的实现
- malloc():
  - 它首先会扫描之前由 free()所释放的空闲内存块列表，以求找到尺寸大于或等于要求的一块空闲内存(`first-fit / best-fit`)
  - 如果在空闲内存列表中根本找不到足够大的空闲内存块，那么 malloc()会调用 sbrk()以分配更多的内存(为减少调用sbrk(), malloc()会以更大幅度提高program break)