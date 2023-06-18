# 垃圾回收
- 垃圾回收是释放掉那些不再被使用的内存空间的过程
- 在 Go 程序运行中以`并发`的方式进行的

>The GC runs concurrently with mutator threads, is type accurate (aka precise), allows multiple GC thread to run in parallel. It is a concurrent mark and sweep that uses a write barrier. It is non-generational and non-compacting. Allocation is done using size segregated per P allocation areas to minimize fragmentation while eliminating locks in the common case.
> 垃圾回收是和go线程同时运行的,它是类型精确的,而且多个垃圾回收线程可以并行运行。它是一种使用了写屏障的并发标记清除的垃圾回收方式。它是非分代和非压缩的。使用按P分配区域隔离的大小来完成分配,以最小化碎片,同时消除常见情况下的锁。

# 三色算法
- go 垃圾回收器的操作都是基于 `三色算法`(三色标记清除算法 - tricolor mark-and-sweep algorithm) 
- 其可以和程序一起并发工作并且使用`写屏障`(write barrier)
  - `黑色`:确保没有任何指针指向白色集合[`自身被标记,成员变量也被标记`]
  - `白色`:允许有指针指向黑色集合[`未被标记对象`]
  - `灰色`:可能会有指针指向白色集合里的对象(白色集合中的对象是垃圾回收的候选对象)[`自身和成员变量均已标记完成`]"`黑`去连接`白`会被染成`灰`"
  - 算法开始时,从根对象(程序可以`直接访问`的对象,包括全局变量以及栈中的东西)出发,所有对象标记为白色,随着算法的继续进行,白色对象移到了其他两种颜色集合的一种里面,最后留在白色集合里面的对象会被清理

- Go垃圾回收器是一个实时的垃圾回收器 ，它是和其他goroutines一起并发运行的


