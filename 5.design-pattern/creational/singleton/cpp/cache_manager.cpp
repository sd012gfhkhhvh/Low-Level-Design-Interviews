// Thread-Safe in-memory cache

/*
=================================================
Practical Example: In-Memory Cache Manager
=================================================

# The Problem:
Lets say you are building an application where multiple components (HTTP
handlers, database layer, background jobs) all need to cache expensive data like
user profiles, configuration, and query results.

You want one shared cache so that any component's writes are immediately visible
to all others, without duplicate maps, stale reads, or wasted memory.

Without Singleton:
-------------------------------------------------------------------------
CacheManager cacheA = new CacheManager();
cacheA.put("user:42", userData);

CacheManager cacheB = new CacheManager();
cacheB.get("user:42"); // null! Different instance, different map

// Problems:
// - Duplicate HashMaps wasting memory
// - Writes in one component invisible to others
// - TTL cleanup duplicated across instances
--------------------------------------------------------------------------

# Requirements:
Design a CacheManager class so that all components access the single
CacheManager instance, which manages one shared map, handles TTL expiry on
reads, and synchronizes access internally.

- Single shared cache, no duplicate data or wasted memory
- Any component's put() is immediately visible to all others
- Thread-safe with internal synchronization
- TTL expiry handled in one place with lazy cleanup
- No need to pass cache references through constructors
*/

#include <chrono>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <unistd.h>
#include <unordered_map>

using namespace std;

class CacheManager {
private:
  struct CacheEntry {
    string value;
    chrono::steady_clock::time_point expiry;
    bool has_ttl;
  };

  unordered_map<string, CacheEntry> cache_;
  mutex mutex_;

  CacheManager() = default;

public:
  // Disable copy constructor and copy assignment operator
  CacheManager(const CacheManager &) = delete;
  CacheManager &operator=(const CacheManager &) = delete;

  // Disable move constructor and move assignment operator
  CacheManager(CacheManager &&) = delete;
  CacheManager &operator=(CacheManager &&) = delete;

  // Get Singleton Instance
  static CacheManager &getInstance() {
    static CacheManager instance;
    return instance;
  }

  // Add key-value pair to cache with optional TTL
  void put(const string &key, const string &value, long ttlSeconds = 0) {
    // Acquire lock to be thread safe
    lock_guard<mutex> lock(mutex_);
    // Create cache entry
    CacheEntry entry;
    entry.value = value;
    entry.has_ttl = ttlSeconds > 0;
    if (entry.has_ttl) {
      entry.expiry = chrono::steady_clock::now() + chrono::seconds(ttlSeconds);
    }
    cache_[key] = move(entry);
  }

  // Get value for key with automatic TTL expiry cleanup
  optional<string> get(const string &key) {
    // Acquire lock to be thread safe
    lock_guard<mutex> lock(mutex_);
    // Check if key exists
    auto it = cache_.find(key);
    if (it == cache_.end())
      return nullopt;
    // Check if TTL has expired
    if (it->second.has_ttl && chrono::steady_clock::now() > it->second.expiry) {
      cache_.erase(it);
      return nullopt;
    }
    // Return the value
    return it->second.value;
  }

  // Remove key from cache
  void remove(const string &key) {
    lock_guard<mutex> lock(mutex_);
    cache_.erase(key);
  }

  // Get current size of cache (removes expired entries)
  int size() {
    lock_guard<mutex> lock(mutex_);
    auto now = chrono::steady_clock::now();
    for (auto it = cache_.begin(); it != cache_.end();) {
      if (it->second.has_ttl && now > it->second.expiry)
        it = cache_.erase(it);
      else
        ++it;
    }
    return static_cast<int>(cache_.size());
  }
};

// --- Main ---
int main() {
  // Both references point to the same CacheManager instance
  CacheManager &cache1 = CacheManager::getInstance();
  CacheManager &cache2 = CacheManager::getInstance();

  // Check if both references point to the same instance
  cout << "Same instance? " << (&cache1 == &cache2) << endl; // 1 (true)

  // Component A caches data
  cache1.put("user:42", "{name: 'Alice'}", 5); // 5-second TTL
  cache1.put("config:theme", "dark");          // no expiry

  // Component B reads from the same cache
  auto user = cache2.get("user:42");
  cout << "user:42 = " << user.value_or("null") << endl; // {name: 'Alice'}
  auto theme = cache2.get("config:theme");
  cout << "config:theme = " << theme.value_or("null") << endl; // dark
  cout << "Cache size: " << cache2.size() << endl;             // 2

  // After 5 seconds, user:42 will expire
  sleep(6);
  user = cache2.get("user:42");
  cout << "user:42 = " << user.value_or("null") << endl; // null
  cout << "Cache size: " << cache2.size() << endl;       // 1

  return 0;
}