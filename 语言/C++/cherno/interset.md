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
- 拷贝还是很费时间的,可以使用`const引用`优化
    - `通过const引用来传递对象`
- `浅拷贝`:复制指针,两个指针对象指向完全相同的内存缓冲区
- `深拷贝`:实际上复制了整个对象

- `拷贝构造函数`:
    - 可使用`delete`禁用
    - 使用其手动执行深拷贝

# C++的箭头操作符
- `->`
- 可以重载
- 可以获取变量的偏移量
```cpp
#include <iostream>
#include <string>

struct Vector3
{
    float x,y,z;
};

int main()
{
    int offset1 = (int)&((Vector3*)nullptr) -> x;
    int offset2 = (int)&((Vector3*)nullptr) -> y;
    int offset3 = (int)&((Vector3*)nullptr) -> z;
    
    std::cout << offset1 << std::endl;
    std::cout << offset2 << std::endl;
    std::cout << offset3 << std::endl;
}
```

# C++动态数组
- vector
- 叫做向量很奇怪,准确叫应该是ArrayList才对
- 动态数组是内存连续的数组,一条高速缓存线
- vector的扩容策略与编译器有关,没有标准化
- vector的简单优化即为`revise()`和`emplace_back()`

# C++中使用库(静态链接)
- 在编译时发生
- 静态链接会更快
- 用引号包含头文件会先检查相对路径

# C++中使用动态库
- 在运行时发生
- 头文件同时支持静态编译与动态编译

# C++处理多返回值
- `结构体`
- 值-结果参数
- string / array(只有在相同类型情况下才有效)
- vector
- C++
    - tuple(元组):一个类,包含x个变量,但它不关心类型
    - pair

# C++的模板
- templates
- 允许定义一个可以根据用途进行编译的模板:让编译器为你写代码
- 模板并不真实存在,只有当我们实际调用他的时候,模板才会被真正构建
- `typename`与class都可以用于指定模板名
- 可以使用模板,但记得当个人
```c
#include <iostream>
#include <string>

template<typename T>
void Print(T value)
{
    std::cout << value << std::endl;
}

template<typename T, int N>
class Array
{
private:
    T m_Array[N];
public:
    int GetSize() const { return N; }
};

int main(int argc, char** argv)
{
    Print(5);
    Print<int>(6);
    Print("Hello,World");
    Print(5.5f);

    Array<std::string, 1008611> array;
    std::cout << array.GetSize() << std::endl;

    std::cin.get();
}
```
