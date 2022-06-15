# new

- heap
- 属于操作符
- 分配一块`连续`地址
- 寻找空地址是是借助 `空闲链表` (还有其他方法,OS相关) 来寻找,并非扫描内存
- 不仅`分配内存`, 还`调用构造函数`
    - new就是malloc+调用构造函数
- must delete
- 还有placement new,以后学

# 隐式转换与`explicit`关键字
- 要意识到对于构造函数,存在隐式类型转换这个东西
- c++只做一次隐式转换,两次的话就需要手动处理一下了
- 尽量避免隐式类型转换,不易阅读
- explicit放在构造函数之前:表示`禁用`隐式类型转换,必须强制转换

# C++运算符和运算符重载
- `operator`
```c
//重载 + demo
Vector Add(const Vector2& other) const
{
    return Vector2(x + other.x, y + other.y);
}

Vector  operator+(const Vector2& other) const
{
    return Add(other);
}

```

# this关键字
- 可通过其访问成员函数
- 就是代表着这个类

# C++栈作用域生存期
- 在栈上的东西出了域就会消逝
    - 在局部创建基于栈的变量然后尝试返回指向它的指针就很搞笑
- 可以利用栈的特点包装一个自动delete的作用域指
    - 自动构造,自动析构,基于栈的变量,离开作用域自动销毁 
```cpp
class ScopePtr
{
private:
    Entity *m_ptr;
public:
    ScopePtr(Entity *ptr) : m_ptr(ptr){}

    ~ScopePtr(){
        delete m_ptr;
    }
}

int main()
{
    {
        ScopePtr e(new Entity());
        //ScopePtr e = new Entity();
    }
}

```

# 智能指针
- 自动化的 new+delete
- `#include<memory>`
- `unique_pre`是`explicit`的
- `shared_ptr`基本上都使用`引用计数`
    - 需要分配另一块内存,叫做`控制块`,用来存储引用计数
    - make_shared可以组合`分配new的内存`与`分配控制块内存`,效率更高
    - 单纯栈大小不够可以使用unique_ptr来分配内存
```cpp
int main() 
{
    {
        std::shared_ptr<Entity> e0;
        {
            std::shared_ptr<Entity>  sharedEntity = std::make_shared<Entity>();
            e0 = sharedEntity;
        }
    }
}
```
- `weak_ptr`:将一个shared_ptr赋值给weak_ptr,不会增加引用计数

- 先考虑 `unique_ptr`, 在考虑 `shared_ptr`

# C++中复制与拷贝构造函数