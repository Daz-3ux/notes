```
2021.12.31
```


# 二叉排序树（二叉查找树 二叉搜索树）

## 性质：
- 如果左子树不空，那么左子树的值都**小于**_根节点_
- 如果右子树不空，那么右子树的值都**大于**_根节点_
- 左小右大
- 若插入时数字大小一样：加一个当前元素个数，一样了就（当前元素个数++），最后输出（当前元素个速）遍当前元素
    - 【遇事不决，加个变量】

- **中序排列**(左中右)完成后为从小到大顺序
- 数组有序后查找元素也就方便，效率了
- 查找：二分查找 
    - O(h):时间复杂度（树的高度）
    - 二叉排序树的缺点：当成了斜树时：O(h) --->>  O(n)，由此平衡二叉树诞生
- 插入：同构建操作一样
- 删除：
    - 删除的是叶子就很普通
    - 被删除节点只有一个孩子（左或右）：只需将 待删除节点 的孩子链接到其父节点
    - 被删除节点有两个孩子：找`左子树`最**大**值，或`右子树`最**小**值（都是叶子节点）替换他


```
看到了28min
```

```
跨年前夕
```

很难从逻辑到代码，今天就先搞懂删除吧

```
2022.1.1
```

新年的第一天从找不到bug开始，慢慢来吧

```
2022.1.2
```
新年的第二天，终于找到bug了，起飞

DONE