#include <iostream>
#include <string>

template<typename T>
void Print(T value)
{
    std::cout << value << std::endl;
}

int main(int argc, char** argv)
{
    Print("5");
    Print("Hello,World");
    Print("5.5f");

    std::cin.get();
}