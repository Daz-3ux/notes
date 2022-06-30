#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

void ForEach(const std::vector<int> &values, const std::function<void(int)> & func)
{
    for(int value : values) {
        func(value);
    }
}

int main(int argc, char **argv) 
{
    std::vector<int> values = {1,2, 3, 4, 5};
    int a = 5;
    // 值捕获
    auto lambda = [=](int value){ std::cout<<"value: " << a << std::endl; };

    ForEach(values, lambda);

    std::cin.get();
}