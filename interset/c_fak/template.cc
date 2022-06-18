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