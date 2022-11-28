#include <iostream>
#include <boost/noncopyable.hpp>
#include <mutex>

class Counter : boost::noncopyable
{
public:
  Counter() : value_(0) {}

  int64_t value() const;
  int64_t getAndIncrease();
private:
  int64_t value_;
  mutable MutexLock mutex_;
};

int64_t Counter::value() const {
  MutexLockGuard lock(mutex_); // lock的析构会晚于返回对象的构造
  return value_;               // 有效保护了共享数据
}

int64_t Counter::getAndIncrease() {
  MutexLockGuard lock(mutex_);
  int64_t ret = value_++;
  return ret;
}