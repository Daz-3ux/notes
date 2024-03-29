# 18-1 目录与硬链接
## 目录
- 在文件系统中,目录的存储方式类似于普通文件
  - 在其 i-node 条目中,会将目录标记为一种不同的文件类型
  - 目录是经特殊组织而成的文件:本质上说就是一个表格,包含`文件名`和`i-node 编号`

![](https://i.imgur.com/yAAyn0m.png)

## 硬链接
- 文件i-node中存储的信息列表并未包含文件名,而仅通过目录列表内的一个映射来定义文件名称
- 其好处是:能够在相同或者不同目录中创建多个名称,每个均指向相同的i-node节点,也将这些名称称为链接,有时也称之为`硬链接`

- 可在shell中使用`ln`命令为一个已经存在的文件创建新的硬链接

- 关于删除:
  - 仅当 i-node 的链接计数降为 0 时,也就是移除了文件的所有名字时,才会删除(释放)文件的 i-node 记录和数据块。总结如下:rm 命令从目录列表中删除一文件名,将相应 i-node的链接计数减一,若`链接计数`因此而降为 0,则将释放该文件名所指代的 i-node 和数据块
  - (智能指针)

- 同意文件的所有链接地位平等

- 限制:
  - 硬链接必须与其指代的文件驻留在同一文件系统
    - 硬链接对文件的指代采用了 i-node 编号,而 i-node 编号的唯一性仅在一个文件系统之内才能得到保障
  - 不能为目录创建硬链接
    - 避免出现令诸多系统程序陷于混乱的链接环路

# 18-2 软链接(符号链接)
- 特殊的文件类型:数据是另一文件的名称
- 符号链接既可以是绝对路径也可以是相对路径
- 文件的链接计数不包含符号链接
- 存在`悬空链接`的情况
- ![](https://i.imgur.com/wH0e0Qy.png)
- 可以用来链接不同文件系统中的一个文件

# 18.3 创建和移除(硬)链接:link()和 unlink()

# 18.4 更改文件名:rename()

# 18.5 使用符号链接:symlink()和 readlink()

# 18.6 创建和移除目录:mkdir()和 rmdir()

# 18.7 移除一个文件或目录:remove()

# 18.8 读目录:opendir()和 readdir()

# 18.9 文件树遍历:nftw()

# 18.10 进程的当前工作目录
- 进程可使用 `getcwd()` 来获取当前工作目录
- `chdir()` 系统调用将调用进程的当前工作目录改变为由 pathname 指定的相对或绝对路径名

# 18.11 针对目录文件描述符的相关操作

# 18.12 改变进程的根目录:chroot()
- 每个进程都拥有一个`根目录`和一个`当前工作目录`,分别作为解释`绝对路径`和`相对路径`的参照点

# 18.13 解析路径名:realpath()
- `realpath()` 库函数对 pathname(以空字符结尾的字符串)中的所有`符号链接`一一解除引用,并解析其中所有对/.和/..的引用,从而生成一个以空字符结尾的字符串,内含相应的绝对路径名

# 18.14 解析路径名字符串:dirname()和 basename()
- `dirname()` 和 `basename()` 函数将一个路径名字符串分解成目录和文件名两部分