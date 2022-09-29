#include <iostream>
#include <cstring>


template<typename T>
class myVector
{
public:
  // 无参构造初始化
  myVector() : p(NULL), capacity(0), size(0) {}

  // 有参构造初始化
  myVector(int size, T data) {
    this->capacity = size+20;
    this->size = size;
    this->p = new T[capacity];
    for (int i = 0; i < size; i++) {
      this->p[i] = data;
    }
  }

  // 析构函数
  ~myVector() {
    if(p != NULL){
      delete[] p;
    }
  }

  // 拷贝构造函数
  myVector(const myVector &v) {
    this->capacity = v.capacity;
    this->size = v.size;
    this->p = new T[this->capacity];
    memcpy(this->p, v.p, this->size*sizeof(T));
  }

  // 打印容器内容
  void print() {
    for(int i = 0; i < this->size; i++) {
      std::cout << this->p[i] << " ";
    }
    std::cout << std:: endl;
  }

  // 扩容
  void expand() {
    T *new_p = new T[this->capacity*2];
    memcpy(new_p, p, this->size*sizeof(T));
    this->capacity *= 2;
    delete[] p;
    p = new_p;
  }

  // 插入
  void push_back(T data) {
    // 如果vector是无参构造的
    if(this->p == NULL) {
      this->capacity = 20;
      this->size = 0;
      T *p = new T[this->capacity];
    }

    // 如果容量已满
    if(this->size == this->capacity) {
      expand();
    }

    this->p[this->size] = data;
    this->size++;
  }

  // 删除最后一个元素
  void pop_back()
  {
    if(this->size > 1) {
      this->p[this->size-1] = 0;
      this->size--;
    }
  }

  // 插入
  void insert(int pos, T data) {
    if(pos >= 0 && pos < this->size) {
      if(this->size == this->capacity) {
        expand();
      }
      
      for(int i = this->size; i > pos; i--) {
        this->p[i] = this->p[i-1];
      }

      this->p[pos] = data;
      this->size++;
    }
  }

  // 清除
  void clear() {
    for (int i = 0; i < this->size; i++) {
      this->p[i] = 0;
    }
    this->size = 0;
  }

  // 重载[]运算符
  T &operator[] (int index) {
    if(index > 0 && index < this->size) {
      // 返回的是对数据的引用
      return this->p[index];
    }else{

    }
  }

  // 重载赋值= (和拷贝构造函数一样)
  void operator=(const myVector &v) {
    if(this->p != NULL) {
      delete[] this->p;
      this->capacity = 0;
      this->size = 0;
      // ?
      this->p = NULL;
    }
    this->capacity = v.capacity;
    this->size = v.size;
    this->p = new T[this->capacity];
    memcpy(this->p, v.p, this->size*sizeof(T));
  }

public:
  T *p;
  int capacity;   // 容量
  int size;       // 大小
};

void test()
{
    myVector<int> v(3,1);
    v.print();
    std::cout<<"size: "<<v.size<<" capacity: "<<v.capacity<< std::endl;
    for(int iter = 0;iter<11;iter++)
    {
        v.push_back(iter);
    }
    v.print();
    std::cout << "pop_back * 3" << std::endl;
    v.pop_back();
    v.pop_back();
    v.pop_back();
    v.print();
    v[3] = 10086;
    v.print();

    std::cout << "拷贝构造v2" << std::endl;
    myVector<int> v2(v);
    v2.print();

    std::cout << "重载赋值v3" << std::endl;
    myVector<int> v3 = v;
    v3.print();

    std::cout << "v3 size: " << v3.size << " " << "v3 capacity: "<< v3.capacity << std::endl;
    v3.insert(1,11111);
    v3.insert(0,22222);
    v3.insert(v3.size-1,33333);
    v3.insert(v3.size,44444);
    std::cout << "赋值后打印v3" << std::endl;
    v3.print();
    v3.clear();
    std::cout << "清零后打印v3" << std::endl;
    v3.print();
}

int main(int argc, char ** argv) 
{
  test();

  return 0;
}