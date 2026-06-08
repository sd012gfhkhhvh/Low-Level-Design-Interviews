/*
===================================
Exercise : Thread-Safe Counter
===================================
Implement Singleton Counter Class

Problem: Implement a Counter singleton that tracks a count across the
application. Multiple components should be able to increment the counter, and
all must see the same value.

Requirements:

increment() increases the count by 1
getCount() returns the current count
Thread-safe: concurrent increments must not lose updates
Calling the constructor/access method from different places returns the same
instance
*/

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

// ==========================================
// 1. Thread-Unsafe Counter (For comparison)
// ==========================================
class UnsafeCounter {
private:
  int _count = 0;

  UnsafeCounter() {}; // private constructor

public:
  // Disable copy and move operations
  UnsafeCounter(const UnsafeCounter &) = delete;
  UnsafeCounter &operator=(const UnsafeCounter &) = delete;
  UnsafeCounter(const UnsafeCounter &&) = delete;
  UnsafeCounter &operator=(const UnsafeCounter &&) = delete;

  // Meyers' Singleton
  static UnsafeCounter &getInstance() {
    static UnsafeCounter instance;
    return instance;
  }

  void increment() {
    // No mutex locking here, prone to data race
    _count++;
  }

  int getCount() { return _count; }
};

// ==========================================
// 2. Thread-Safe Counter
// ==========================================
class SafeCounter {
private:
  int _count = 0;
  mutex _mutex; // Added mutex for thread safety

  SafeCounter() {}; // private constructor

public:
  // Disable copy and move operations
  SafeCounter(const SafeCounter &) = delete;
  SafeCounter &operator=(const SafeCounter &) = delete;
  SafeCounter(const SafeCounter &&) = delete;
  SafeCounter &operator=(const SafeCounter &&) = delete;

  // Meyers' Singleton
  static SafeCounter &getInstance() {
    static SafeCounter instance;
    return instance;
  }

  void increment() {
    // Make thread-safe with lock_guard
    lock_guard<mutex> lock(_mutex);
    _count++; // thread-safe increment
  }

  int getCount() {
    // Thread-safe read
    lock_guard<mutex> lock(_mutex);
    return _count;
  }
};

// ==========================================
// Driver Code to Simulate Concurrent Access
// ==========================================

void simulateUnsafeCounter() {
  UnsafeCounter &counter = UnsafeCounter::getInstance();
  vector<thread> threads;

  // Spawn 100 threads, each incrementing the counter 10000 times
  for (int i = 0; i < 100; i++) {
    threads.push_back(thread([&counter]() {
      for (int j = 0; j < 10000; j++) {
        counter.increment();
      }
    }));
  }

  // Wait for all threads to finish
  for (auto &t : threads) {
    t.join();
  }

  cout << "Expected count: 1000000" << endl;
  cout << "Actual count  : " << counter.getCount() << endl;
  if (counter.getCount() != 1000000) {
    cout << "Result: Data Race Detected! (Count lost due to concurrent access)"
         << endl;
  } else {
    cout << "Result: Correct (This is rare without synchronization, but "
            "possible by chance)"
         << endl;
  }
}

void simulateSafeCounter() {
  SafeCounter &counter = SafeCounter::getInstance();
  vector<thread> threads;

  // Spawn 100 threads, each incrementing the counter 10000 times
  for (int i = 0; i < 100; i++) {
    threads.push_back(thread([&counter]() {
      for (int j = 0; j < 10000; j++) {
        counter.increment();
      }
    }));
  }

  // Wait for all threads to finish
  for (auto &t : threads) {
    t.join();
  }

  cout << "Expected count: 1000000" << endl;
  cout << "Actual count  : " << counter.getCount() << endl;
  if (counter.getCount() == 1000000) {
    cout << "Result: Thread-Safe! (No updates lost)" << endl;
  }
}

int main() {
  cout << "--- 1. Simulating Unsafe Counter ---" << endl;
  simulateUnsafeCounter();

  cout << "\n--- 2. Simulating Safe Counter ---" << endl;
  simulateSafeCounter();

  return 0;
}