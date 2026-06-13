# Singleton Design Pattern

## Resources

- [Refactoring Guru: Singleton](https://refactoring.guru/design-patterns/singleton)

## What is the Singleton Design Pattern?

The Singleton Design Pattern is a creational pattern that ensures a class has only one single instance throughout the application's lifecycle, while providing a global access point to that instance.

## What problem does it solve?

**The Problem:**

In many applications, certain objects should only exist once: a database connection pool, a configuration manager, a hardware device driver, or a logger. If multiple instances are created, it could lead to erratic behavior, resource leaks, or an inconsistent state. Furthermore, passing this single instance around everywhere through function arguments becomes extremely tedious and pollutes your APIs.

**The Solution:**

Hide the constructor of the class (make it private) and provide a static method that returns a reference to the single instance. The static method creates the instance the first time it is called and returns the cached instance on all subsequent calls.

## When should we use the Singleton Pattern?

- When a class must have exactly one instance available to all clients (e.g., a shared database connection or a configuration object).
- When you need stricter control over global variables. The Singleton guarantees that there is only one instance, unlike a standard global variable which can be easily overwritten.

*Note: Singleton is often considered an anti-pattern if overused, as it introduces global state and tight coupling, making unit testing difficult.*

## How to implement the Singleton Pattern?

### Implementation Steps

1. **Make the Constructor Private:** Prevent other objects from using the `new` operator (or equivalent) to instantiate the class directly.
2. **Delete Copy/Move Constructors:** Ensure the object cannot be copied or moved (in C++, delete the copy constructor and assignment operator).
3. **Create a Static Method:** Add a public static creation method (e.g., `getInstance()`) that acts as a constructor.
4. **Manage the Instance:** Inside `getInstance()`, create the instance lazily and store it. In modern C++ (C++11 and later), **Meyer's Singleton** (using a static local variable) is the preferred, thread-safe way to implement this without requiring manual mutex locks.

### Architecture

```text
    ┌─────────────────────────┐
    │         Client          │
    └────────────┬────────────┘
                 │ calls getInstance()
                 ▼
    ┌─────────────────────────┐
    │        Singleton        │
    ├─────────────────────────┤
    │ - instance: Singleton   │
    ├─────────────────────────┤
    │ - Singleton()           │
    │ + getInstance()         │
    │ + doSomething()         │
    └─────────────────────────┘
```
