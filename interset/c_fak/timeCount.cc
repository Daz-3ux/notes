#include <chrono>
#include <iostream>
#include <thread>

struct Timer {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<float> duration;

  Timer() { 
    start = std::chrono::high_resolution_clock::now(); 
  }

  ~Timer() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    float ms = duration.count() * 1000.0f;
    std::cout << "Timer took " << ms << "ms " << std::endl;
  }
};

void Function() {
  Timer timer;// 为整个作用域计时

  for(int i = 0; i < 100; i++){
    std::cout << "hello\n";
  }
}

int main(int argc, char **argv) {
  Function();

  std::cin.get();
}