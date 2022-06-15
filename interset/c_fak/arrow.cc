#include <iostream>
#include <string>

struct Vector3
{
    float x,y,z;
};

int main()
{
    long offset1 = (long)&((Vector3*)nullptr)->x;
    long offset2 = (long)&((Vector3*)nullptr)->y;
    long offset3 = (long)&((Vector3*)nullptr)->z;
    
    std::cout << offset1 << std::endl;
    std::cout << offset2 << std::endl;
    std::cout << offset3 << std::endl;
}