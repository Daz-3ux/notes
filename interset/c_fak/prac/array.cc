#include <iostream>
#include <array>

// 不知道数组大小的情况下传入一个标准数组作为参数
template<typename T>
void PrintfArray(const T &data) {
    for(int i = 0; i < data.size(); i++) {
        std::cout << data[i] << std::endl;
    }
}

int main(int argc, char **argv) {
    std::array<int, 8> arr;
    for(auto i = 0; i < arr.size(); i++) {
        arr[i] = 1;
    }
    PrintfArray(arr);
}