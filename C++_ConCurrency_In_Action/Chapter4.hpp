#include "../Common.hpp"

namespace Chapter4 {
template <typename T> class ThreadSafeQueue {
public:
  ThreadSafeQueue() = default;
  ThreadSafeQueue(const ThreadSafeQueue &threadSafeQueue) {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    _queue = threadSafeQueue._queue;
  }

  void push(T value) {
    {
      std::lock_guard<std::mutex> lockGuard(_mutex);
      _queue.push(value);
    }
    _cond.notify_one();
  }

  void wait_and_pop(T &value) {
    std::unique_lock<std::mutex> uniqueLock(_mutex);
    _cond.wait(uniqueLock, [this]() { return !_queue.empty(); });
    value = _queue.front();
    _queue.pop();
  }

  std::shared_ptr<T> wait_and_pop() {
    std::unique_lock<std::mutex> uniqueLock(_mutex);
    _cond.wait(uniqueLock, [this]() { return !_queue.empty(); });
    auto ret = std::make_shared<T>(_queue.front());
    _queue.pop();
    return ret;
  }

  bool try_pop(T &value) {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    if (_queue.empty()) {
      return false;
    }
    value = _queue.front();
    _queue.pop();
    return true;
  }

  bool empty() {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    if (_queue.empty()) {
      return true;
    }
    return false;
  }

private:
  std::queue<T> _queue;
  mutable std::mutex _mutex;
  std::condition_variable _cond;
};
}