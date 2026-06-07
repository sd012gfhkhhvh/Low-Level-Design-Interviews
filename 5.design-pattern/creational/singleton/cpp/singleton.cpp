/*
# Resources:
    - https://algomaster.io/learn/lld/singleton

# Singleton Pattern is a creational design pattern that guarantees a class
has only one instance and provides a global point of access to it.

# Two requirements define the pattern:

- Single instance: No matter how many times any part of the code requests it,
the same object is returned.
- Global access: Any component can reach the instance
without needing it passed through constructors or method parameters.

# Singleton is useful in scenarios like:

- Managing Shared Resources (database connections, thread pools, caches,
configuration settings)
- Coordinating System-Wide Actions (logging, print
spoolers, file managers)
- Managing State (user session, application state)

# Example: Global Logger class, DB connection pools, Local Cache Ojects, Thread
pools, File system

*/
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

/*
======================================
Lazy Initialization (Not Thread-Safe)
======================================
#Pros:
- Creates the instance only when needed.

# Cons:
- Not thread safe, simultenious access of multiple thread can create multiple
instances when the instance was null for the first time.
*/

class LazySingleton {
private:
  static LazySingleton *instance;
  LazySingleton() {};

public:
  static LazySingleton *getInstance() {
    if (instance == nullptr) {
      instance = new LazySingleton();
    }

    return instance;
  }
};

/*
===============================
Thread-Safe Lazy Initialization
===============================
#Pros:
- This approach extends lazy initialization by ensuring the Singleton is safe to
use in multi-threaded environments.
- Synchronization (or locking) ensures that only one thread can create the
object, while others wait.

# Cons:
- Low performance: every call to getInstance() acquires a lock, even after the
instance has been created. Once the instance exists, there is no reason to
synchronize.
*/

class ThreadSafeSingleton {
private:
  static ThreadSafeSingleton *instance;
  static mutex lock;

  ThreadSafeSingleton() {};

public:
  static ThreadSafeSingleton *getInstance() {
    lock_guard<mutex> guard(lock);
    if (instance == nullptr) {
      instance = new ThreadSafeSingleton();
    }
    return instance;
  }
};

/*
============================================
Double-Checked Locking (Lazy Initialization)
============================================
#Pros:
- Reduces the performance overhead by only synchronizing
during the first object creation. After the instance exists, threads skip the
lock entirely.
*/

class DoubleCheckedSingleton {
private:
  // Holds the single shared instance (needs safe publication in C++)
  static DoubleCheckedSingleton *instance;
  // Lock used only during first-time creation
  static mutex lock;

  // Private constructor prevents external instantiation
  DoubleCheckedSingleton() {}

public:
  // Global access point to get the Singleton instance
  static DoubleCheckedSingleton *getInstance() {

    // Fast path: first check without locking
    if (instance == nullptr) {
      // Lock only when the instance might need to be created
      lock_guard<mutex> guard(lock);
      // Second check inside the lock (prevents double creation)
      if (instance == nullptr) {
        instance = new DoubleCheckedSingleton();
      }
    }

    // Return the shared instance
    return instance;
  }
};

/*
================================================
Eager Initialization(Thred safe)
================================================
#Pros:
- Singleton instance is created as soon as the class/module is loaded, before
any thread can access it. That makes it inherently thread-safe without explicit
locks, because initialization happens once during load/initialization.

# Cons:
- It could potentially waste resources if the singleton instance is never used
by the client application.
- It is generally not recomended in mordern cpp because of memory leak issue and
resource waste.
*/

class EagerSingleton {
private:
  // Holds the single shared instance (created immediately during static
  // initialization)
  static EagerSingleton *instance;

  // Private constructor prevents creating objects from outside the class
  EagerSingleton() {}

public:
  // Global access point to get the Singleton instance
  static EagerSingleton *getInstance() {
    // Return the already-created shared instance
    return instance;
  }
};

// Eager initialization happens here
EagerSingleton *EagerSingleton::instance = new EagerSingleton();

/*
Even a better approach is to use reference instate of raw pointers to avoid
memory leak
*/
class EagerSingletonMemorySafe {
private:
  static EagerSingletonMemorySafe instance;
  EagerSingletonMemorySafe() {}

public:
  static EagerSingletonMemorySafe &getInstance() { return instance; }
};

// Eager initialization happens here
EagerSingletonMemorySafe EagerSingletonMemorySafe::instance;

/*
==============================================
Meyers' Singleton (Recommended)
==============================================
#Pros:
- In C++11 and later, the standard guarantees that static local variables are
initialized in a thread-safe manner. This makes the Meyers' Singleton the
idiomatic approach in modern C++:
- No explicite locking needed.
- Lazy initialization, no resource waste.
*/

class Singleton {
private:
  Singleton() {}

public:
  // Disable copy constructor and copy assignment operator
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  // Disable move constructor and move assignment operator
  Singleton(Singleton &&) = delete;
  Singleton &operator=(Singleton &&) = delete;

  // Global access point to get the Singleton instance
  static Singleton &getInstance() {
    // Thread-safe in C++11+
    static Singleton instance;

    return instance;
  }

  void doSomething() { std::cout << "Singleton operation" << std::endl; }
};

// Usage
int main() {
  Singleton::getInstance().doSomething();

  return 0;
}