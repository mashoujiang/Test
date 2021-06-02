#include "../Common.hpp"

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
}