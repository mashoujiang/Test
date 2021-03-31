#include <future>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <memory>
#include <functional>

class ThreadPool
{
public:
    ThreadPool(std::string poolName, size_t nThreads, size_t capacity = 20);
    ~ThreadPool();
    template <typename Func, typename... Args>
    auto enqueueTask(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>;

private:
    void enqueue(std::function<void()> &&task, bool isUrgent);
    std::function<void()> dequeueTask();
    void createThreads();
    void exitAllThreads();

private:
    std::string m_poolName;
    size_t m_nThreads{0};
    size_t m_capacity{0};
    bool m_stop{false};
    std::mutex m_mutex;
    std::condition_variable m_notFull;
    std::condition_variable m_notEmpty;
    std::list<std::function<void()>> m_waitList;
    std::list<std::thread> m_workers;
};

ThreadPool::ThreadPool(std::string poolName, size_t nThreads, size_t capacity)
    : m_poolName(std::move(poolName)), m_nThreads(nThreads), m_capacity(capacity)
{
    createThreads();
}

ThreadPool::~ThreadPool()
{
    exitAllThreads();
    for (auto &t : m_workers)
    {
        if (t.joinable())
        {
            t.join();
        }
    }
}

void ThreadPool::exitAllThreads()
{
    m_stop = true;
    m_notEmpty.notify_all();
}

template <typename Func, typename... Args>
auto ThreadPool::enqueueTask(Func &&func, Args &&...args) -> std::future<decltype(func(args...))>
{
    using retType = decltype(func(args...));
    auto task = std::make_shared<std::packaged_task<retType()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
    try
    {
        enqueue([=]() { (*task)(); }, false);
    }
    catch (std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    return task->get_future();
}

void ThreadPool::enqueue(std::function<void()> &&task, bool isUrgent)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    while (!m_stop && m_waitList.size() >= m_capacity)
    {
        std::unique_lock<std::mutex> ulock(m_mutex, std::defer_lock);
        m_notFull.wait(ulock);
    }
    if (m_stop)
    {
        throw std::runtime_error("Failed to enqueue task, thread pool is stopped");
    }
    if (isUrgent)
    {
        m_waitList.push_front(task);
    }
    else
    {
        m_waitList.push_back(task);
    }
    m_notEmpty.notify_one();
}

std::function<void()> ThreadPool::dequeueTask()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    while (!m_stop && m_waitList.empty())
    {
        std::unique_lock<std::mutex> ulock(m_mutex, std::defer_lock);
        m_notEmpty.wait(ulock);
    }
    if (m_stop)
    {
        throw std::runtime_error("dequeue task failed, thread pool is stopped");
    }
    auto task = std::move(m_waitList.front());
    m_waitList.pop_front();
    m_notFull.notify_one();
    return task;
}

void ThreadPool::createThreads()
{
    for (size_t i = 0; i < m_nThreads; ++i)
    {
        m_workers.emplace_back([=]() {
            try
            {
                while (!m_stop)
                {
                    auto task = dequeueTask();
                    task();
                }
            }
            catch (std::exception &e)
            {
                std::cout << e.what() << std::endl;
            }
        });
    }
}

int main()
{
    ThreadPool threadPool{"test", 3};
    for (int i = 0; i < 100; ++i)
    {
        auto future = threadPool.enqueueTask([]() {
            std::cout << "--------\n";
            for (int i = 0; i < 10; ++i)
            {
                std::cout << i << std::endl;
            }
            std::cout << "--------\n";
        });
        // future.wait() and get() make the async to sync
        future.wait();
        future.get();
    }
}