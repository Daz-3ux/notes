#include <iostream>
#include <vector>

void PrintValue(int value)
{
    std::cout << "value: " << value << std::endl;
}


void ForEach(const std::vector<int>& values, void(*func)(int)) 
{
    for(int value : values) {
        func(value);
    }
}


int main(int argc, char **argv)
{
    std::vector<int> values = {1, 2, 3, 4, 5};
    ForEach(values, PrintValue);

    std::cin.get();
}