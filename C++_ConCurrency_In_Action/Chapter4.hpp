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

template <typename T> class my_packaged_task {};
// 4.8 特化版std::packaged_task<>
template <> class my_packaged_task<std::string(std::vector<char> *, int)> {
public:
  template <typename Callable> explicit my_packaged_task(Callable &&f);
  std::future<std::string> get_future();
  void operator()(std::string(std::vector<char> *, int));
};

// 4.9 std::packaged_task在GUI 多线程中的使用
namespace _4_9 {
bool _stop = false;
std::deque<std::packaged_task<void()>> _tasks;
std::mutex _mutex;
std::condition_variable _cv;
void GuiThread() {
  while (!_stop) {
    std::packaged_task<void()> task;
    std::unique_lock<std::mutex> uniqueLock(_mutex);
    _cv.wait(uniqueLock, []() { return !_tasks.empty(); });
    task = std::move(_tasks.front());
    _tasks.pop_front();
    task();
  }
}

template <typename Func> std::future<Func> PushTaskToGuiThread(Func f) {
  std::packaged_task<Func> task(f);
  auto future = task.get_future();
  {
    std::lock_guard<std::mutex> lockGuard(_mutex);
    _tasks.push_back(task);
  }
  _cv.notify_one();
  return future;
}
} // namespace _4_9

// 4.11 超时等待的条件变量
namespace _4_11 {
std::condition_variable _cv;
bool _done;
std::mutex _mutex;
bool wait_loop() {
  auto const timeout =
      std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
  std::unique_lock<std::mutex> uniqueLock(_mutex);
  while (!_done) {
    if (_cv.wait_until(uniqueLock, timeout) == std::cv_status::timeout) {
      break;
    }
  }
  return _done;
}
} // namespace _4_11

}