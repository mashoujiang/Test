#include <thread>
#include <mutex>
#include <deque>
#include <condition_variable>
#include <exception>
#include <vector>
#include <iostream>

std::deque<int> taskQueue;
std::mutex g_mutex;
std::condition_variable g_condition;
bool g_stop = false;

void enqueue(int task)
{
    {
        std::lock_guard<std::mutex> lock(g_mutex);
        taskQueue.push_back(task);
        std::cout << "produce: " << task << std::endl;
    }
    g_condition.notify_one();
}

int dequeue()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    while (!g_stop && taskQueue.empty())
    {
        std::unique_lock<std::mutex> ul(g_mutex, std::defer_lock);
        g_condition.wait(ul);
    }
    if (g_stop)
    {
        throw std::runtime_error("process is stopped");
    }
    auto task = taskQueue.front();
    taskQueue.pop_front();
    return task;
}
void produce()
{
    for (int i = 0; i < 10; ++i)
    {
        enqueue(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
void consume()
{
    while (!g_stop)
    {
        int task = dequeue();
        std::cout << "consume: " << task << std::endl;
    }
}

int main()
{
    std::vector<std::thread> tv;
    int nThread = 3;
    tv.emplace_back(&consume);
    for (int i = 0; i < nThread; ++i)
    {
        tv.emplace_back(&produce);
    }

    for (auto &item : tv)
    {
        item.join();
    }
}