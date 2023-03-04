**指针的意义只是单纯的指向空间**

2021.12.15（补）

# 线性表
- 顺序表
- 链表

## 顺序表：
- 每一次增删太麻烦，但可以很快定位元素，一般用于查找
- {数组}，一组地址连续的存储单元

## 链表
- 适合增删操作多的

- 一个节点：元素+指向下一个节点的指针{数据域+指针域}】
- 单向链表：
    - 带头结点（常）
    - 不带头结点
- 单向循环链表
- 双向链表
- 双向循环链表

- 头指针：指向第一个节点的指针
- 头结点：一般不存放数据域的第一个节点
- 首元节点：存放第一个元素的节点

**带头结点**形式： 头指针 -> 头结点 -> 首元节点

- 删除节点：先指向，再free；一定要记得free，因为malloc分配的空间不会随被调函数的执行完毕而释放