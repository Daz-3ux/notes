#include <iostream>
// #include <vector>
// using std::vector;

struct vector2
{
  float x,y;
};

struct vector4
{
  union
  {
    struct{
      float x,y,z,w;
    };
    struct{
      vector2 a,b;
      // a和x,y的内存是一样的
      // b和z,w的内存是一样的
    };
  };
};

void printVector2(const vector2 &vector) 
{
  std::cout << vector.x << "," << vector.y << std::endl;
}

int main()
{
  vector4 vector = {1.0f, 2.0f, 3.0f, 4.0f};
  printVector2(vector.a);
  printVector2(vector.b);
  vector.z = 1314.0f;
  std::cout << "-------------------------" << std::endl;
  printVector2(vector.a);
  printVector2(vector.b);
}