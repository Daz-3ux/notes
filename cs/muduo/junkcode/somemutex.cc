#include <bits/stdc++.h>

#include <boost/noncopyable.hpp>

class MutexLock : boost::noncopyable {
public:
  MutexLock() : holder_(0) { pthread_mutex_init(&mutex_, NULL); }

  ~MutexLock() {
    assert(holder_ == 0);
    pthread_mutex_destroy(&mutex_);
  }

  bool isLockByThisThread() { return holder_ == CurrentThread::tid(); }

  void assertLocked() { assert(isLockedByThisThread()); }

  void lock() {  // only for mutexlockguard
    pthread_mutex_lock(&mutex_);
    holder_ = CurrentThread::tid();
  }

  void unlock() {  // only for mutexlockguard
    holder_ = 0;
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t *getPthreadMutex() {  // only for Condition
    return &mutex_;
  }

private:
  pthread_mutex_t mutex_;
  pid_t holder_;
};

class MutexLockGuard : boost::noncopyable {
public:
  explicit MutexLockGuard(MutexLock &mutex) : mutex_(mutex) { mutex.lock(); }

  ~MutexLockGuard() { mutex_.unlock(); }

private:
  MutexLock &mutex_;
};

#define MutexLockGuard(x) static_assert(false, "missing mutex guard var name")

class Condition : boost::noncopyable {
public:
  explicit Condition(MutexLock &mutex) : mutex_(mutex) {
    pthread_cond_init(&pcond_, NULL);
  }

  ~Condition() { pthread_cond_destroy(&pcond_); }

  void wait() { pthread_cond_wait(&pcond_, mutex_.getPthreadMutex()); }
  void notify() { pthread_cond_signal(&pcond_); }
  void notifyAll() { pthread_cond_broadcast(&pcond_); }

private:
  MutexLock &mutex_;
  pthread_cond_t pcond_;
}