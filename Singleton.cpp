#include <iostream>
#include <mutex>
#include <cassert>
template <typename T>
class Singleton
{
public:
    static T *instance()
    {
        // static T *obj = nullptr;
        // std::call_once(m_onceFlag, [&]() -> void {
        //     obj = new T();
        //     assert(obj != nullptr);
        // });
        // return obj;
        static T obj;
        return &obj;
    }

    Singleton(const Singleton &) = delete;
    Singleton(Singleton &&) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton &operator=(Singleton &&) = delete;

protected:
    static std::once_flag m_onceFlag;
    Singleton() = default;
    virtual ~Singleton() = default;
};
template <typename Type>
std::once_flag Singleton<Type>::m_onceFlag;

class TestSingleton : public Singleton<TestSingleton>
{
public:
    void info()
    {
        std::cout << "This is TestSingleton" << std::endl;
    }

private:
    TestSingleton() = default;
    ~TestSingleton() = default;
    friend Singleton<TestSingleton>;
};

int main()
{
    TestSingleton::instance()->info();
    TestSingleton::instance()->info();
    TestSingleton::instance()->info();
}