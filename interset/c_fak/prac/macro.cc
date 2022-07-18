#include <iostream>

#ifdef PR_DEBUG
#define LOG(X) std::cout << X << std::endl;
#else
#define LOG(X)
#endif

int main() {
    LOG("Hello, world!");
    std::cin.get();
}