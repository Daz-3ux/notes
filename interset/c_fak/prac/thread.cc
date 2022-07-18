#include <iostream>
#include <thread>

static bool s_Finished = false;

void doWork()
{
  using namespace std::literals::chrono_literals;
  std::cout << "started thread id: " << std::this_thread::get_id() << std::endl;

  while(!s_Finished) {
    std::cout << "Working...\n";
    std::this_thread::sleep_for(1s);
  }
}

int main(int argc, char **argv)
{
  std::thread worker(doWork);// 接受函数指针

  std::cin.get();
  s_Finished = true;

  worker.join();// wait fot exit: 在当前线程上等待这个线程完成工作
  std::cout << "started thread id: " << std::this_thread::get_id() << std::endl;
  std::cout << "Finished" << std::endl;

  std::cin.get();
}