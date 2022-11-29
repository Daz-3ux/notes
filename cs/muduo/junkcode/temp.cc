#include <boost/noncopyable.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <iterator>
#include <vector>

// mutex类简单使用
class Counter : boost::noncopyable {
public:
  Counter() : value_(0) {}

  int64_t value() const;
  int64_t getAndIncrease();

private:
  int64_t value_;
  mutable MutexLock mutex_;
};

int64_t Counter::value() const {
  MutexLockGuard lock(mutex_);  // lock的析构会晚于返回对象的构造
  return value_;                // 有效保护了共享数据
}

int64_t Counter::getAndIncrease() {
  MutexLockGuard lock(mutex_);
  int64_t ret = value_++;
  return ret;
}

// 使用智能指针的简单Observer
class Observable  // not 100% thread safe!
{
public:
  void register_(
      std::weak_ptr<Observer> x);  // 参数还可以是: const weak_ptr<Observer>&
  void notifyObserver();

private:
  mutable MutexLock mutex_;
  std::vector<weak_ptr<Observer>> observers_;
  typedef std::vector<weak_ptr<Observer>>::iterator Iterator;
};
