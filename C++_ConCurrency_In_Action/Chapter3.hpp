#include "../Common.hpp"

#define BIGSIZE 1000
namespace Chapter3 {
// read-write mutex
class DnsEntry {};
class DnsCache {
public:
  DnsEntry findEntry(const std::string &domain) const {
    std::shared_lock<std::shared_mutex> sharedLock(_shareMutex);
    auto entry = _entries.find(domain);
    return entry == _entries.end() ? DnsEntry() : entry->second;
  }

private:
  std::unordered_map<std::string, DnsEntry> _entries;
  mutable std::shared_mutex _shareMutex;
};

// 递归锁： std::lock_guard<std::recursive_mutex> and std::unique_lock<std::recursive_mutex>;

// using std::lock() and std::lock_guard()
class SomeBigObject
{
    uint64_t data[BIGSIZE];
};

void swap(SomeBigObject &lhs, SomeBigObject &rhs){
}
class WrapData
{
public:
    WrapData(const SomeBigObject &sbo) : _object(sbo) {}
    friend void swap(WrapData &lhs, WrapData &rhs);

private:
    SomeBigObject _object;
    std::mutex _mutex;
};

void swap(WrapData &lhs, WrapData &rhs)
{
    if (&lhs == &rhs)
    {
        return;
    }
    std::lock(lhs._mutex, rhs._mutex);
    std::lock_guard<std::mutex> lockA(lhs._mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lockB(rhs._mutex, std::adopt_lock);
    swap(lhs._object, rhs._object);
    // c++17
    // std::scoped_lock guard(lhs._mutex, rhs._mutex);
    // swap(lhs._object, rhs._object);
}
}