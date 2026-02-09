# Classes and Objects in C++ — A Complete Practical Guide

This guide explains classes and objects in C++ from fundamentals to advanced patterns, including:

- Class definition and object instantiation
- Constructors and destructors
- The `this` pointer
- Access specifiers and encapsulation
- Static members
- Const correctness
- Object lifecycle and memory management
- Copy semantics
- Real-world design patterns

---

> Reference - https://www.geeksforgeeks.org/c-classes-and-objects/  
> Reference - https://en.cppreference.com/w/cpp/language/classes

## 1. What Are Classes and Objects?

A **class** is a blueprint or template that defines the structure and behavior of objects. It encapsulates data (attributes) and functions (methods) that operate on that data.

An **object** is an instance of a class—a concrete entity created from the class blueprint.

```cpp
class Car {          // Class: Blueprint
    std::string brand;
    int speed;
};

Car myCar;           // Object: Instance of Car
```

### Mental Model

> Class = Architectural blueprint  
> Object = Actual house built from that blueprint

Multiple houses (objects) can be built from the same blueprint (class), each with its own characteristics.

---

## 2. Basic Class Definition

A class contains:

- **Data members** (attributes/properties) - typically `private`
- **Member functions** (methods) - typically `public`

```cpp
class BankAccount {
private:
    std::string accountNumber;  // Data member
    double balance;              // Data member

public:
    void deposit(double amount) {  // Member function
        balance += amount;
    }

    double getBalance() const {    // Member function
        return balance;
    }
};
```

### Access Specifiers

- **`public`**: Accessible from anywhere
- **`private`**: Accessible only within the class (default)
- **`protected`**: Accessible within class and derived classes

---

## 3. Constructors and Destructors

### Constructor

A special member function called automatically when an object is created.

```cpp
class Car {
public:
    // Default constructor
    Car() : brand("Unknown"), speed(0) {
        std::cout << "Car created\n";
    }

    // Parameterized constructor
    Car(const std::string& b, int s) : brand(b), speed(s) {
        std::cout << "Car " << brand << " created\n";
    }

private:
    std::string brand;
    int speed;
};
```

### Member Initializer List

✅ **Preferred way** to initialize data members:

```cpp
Car(const std::string& b, int s) : brand(b), speed(s) { }
```

Why use initializer lists:

- More efficient (direct initialization vs assignment)
- Required for `const` and reference members
- Required for members without default constructors

### Destructor

A special member function called when an object is destroyed.

```cpp
class Resource {
private:
    int* data;

public:
    Resource() : data(new int(0)) { }

    ~Resource() {  // Destructor
        delete data;  // Cleanup
    }
};
```

**When destructors are called:**

- Object goes out of scope
- `delete` called on pointer to object
- Program ends

---

## 4. The `this` Pointer

`this` is a pointer to the current object. It's an implicit parameter to all non-static member functions.

### Use Case 1: Resolve Naming Conflicts

```cpp
class Rectangle {
private:
    int width, height;

public:
    Rectangle(int width, int height) {
        this->width = width;    // Member variable
        this->height = height;  // Parameter
    }
};
```

### Use Case 2: Method Chaining

```cpp
class Builder {
public:
    Builder& setWidth(int w) {
        width = w;
        return *this;  // Return reference to current object
    }

    Builder& setHeight(int h) {
        height = h;
        return *this;
    }
};

// Usage: method chaining
Builder b;
b.setWidth(10).setHeight(20);  // Fluent interface
```

### Use Case 3: Pass Object to Functions

```cpp
void processObject(const Object& obj);

class Object {
    void doSomething() {
        processObject(*this);  // Pass current object
    }
};
```

---

## 5. Access Specifiers and Encapsulation

**Encapsulation** = Hiding internal details, exposing only necessary interface

### Why Use Private Members?

```cpp
class BankAccount {
private:
    double balance;  // ✅ Private - controlled access

public:
    void deposit(double amount) {
        if (amount > 0) {  // Validation
            balance += amount;
        }
    }

    double getBalance() const {
        return balance;
    }
};
```

❌ **Without encapsulation:**

```cpp
account.balance = -1000;  // Direct access - no validation!
```

✅ **With encapsulation:**

```cpp
account.deposit(-1000);  // Validation prevents invalid state
```

### Getter and Setter Pattern

```cpp
class Person {
private:
    std::string name;
    int age;

public:
    // Getter (accessor)
    std::string getName() const { return name; }
    int getAge() const { return age; }

    // Setter (mutator) with validation
    void setAge(int a) {
        if (a >= 0 && a <= 150) {
            age = a;
        }
    }
};
```

---

## 6. Static Members

**Static members** belong to the class itself, not to individual objects.

### Static Data Members

Shared across all instances:

```cpp
class Counter {
private:
    static int count;  // Static data member
    int id;

public:
    Counter() : id(++count) { }

    static int getCount() {  // Static member function
        return count;
    }
};

// Definition outside class (required)
int Counter::count = 0;
```

### Usage

```cpp
Counter c1, c2, c3;
std::cout << Counter::getCount();  // 3 (shared across all objects)
```

### Static Member Functions

- Can only access static members
- Called using class name: `ClassName::functionName()`
- No `this` pointer

**Use cases:**

- Factory methods
- Utility functions
- Counters and statistics

---

## 7. Const Correctness

**Const correctness** ensures that functions don't unintentionally modify data.

### Const Member Functions

```cpp
class Data {
private:
    int value;

public:
    int getValue() const {  // ✅ Const - won't modify object
        return value;
    }

    void setValue(int v) {  // ❌ Not const - modifies object
        value = v;
    }
};
```

### Why It Matters

```cpp
void printData(const Data& d) {
    std::cout << d.getValue();  // ✅ OK - getValue() is const
    // d.setValue(10);          // ❌ Error - setValue() is not const
}
```

### Const Parameters

```cpp
void process(const std::string& data) {  // ✅ Const reference
    // data cannot be modified
    // Avoids costly copy
}

void accelerate(int speed) {  // ✅ Small type by value
    // Simple and efficient for primitives
}
```

### Best Practices Summary

| Type                          | How to Pass                   | Example                        |
| ----------------------------- | ----------------------------- | ------------------------------ |
| Small types (int, char, bool) | By value                      | `void f(int x)`                |
| Large types (string, vector)  | By const reference            | `void f(const std::string& s)` |
| Modify needed                 | By reference                  | `void f(std::string& s)`       |
| Pointers                      | By value (for pointer itself) | `void f(int* ptr)`             |

**Golden Rule:**

> Use `const` wherever modification is not intended

---

## 8. Object Lifecycle

Objects in C++ have a well-defined lifecycle: creation, copying, and destruction.

### Stack vs Heap Allocation

```cpp
// Stack allocation (automatic lifetime)
Car stackCar("Toyota", "Camry");  // Destroyed when out of scope

// Heap allocation (manual lifetime)
Car* heapCar = new Car("Honda", "Accord");
delete heapCar;  // Must manually delete
```

### Copy Constructor

Called when an object is copied:

```cpp
class Resource {
private:
    int* data;

public:
    // Constructor
    Resource(int val) : data(new int(val)) { }

    // Copy constructor (deep copy)
    Resource(const Resource& other) {
        data = new int(*other.data);  // Allocate new memory
    }

    // Destructor
    ~Resource() {
        delete data;
    }
};
```

### Copy Assignment Operator

```cpp
Resource& operator=(const Resource& other) {
    if (this != &other) {  // Self-assignment check
        delete data;
        data = new int(*other.data);
    }
    return *this;
}
```

### The Rule of Three

> If you define one of: destructor, copy constructor, or copy assignment  
> You should define all three

---

## 9. Best Practices and Anti-Patterns

### ✅ Best Practices

1. **Use member initializer lists**

    ```cpp
    Car(const std::string& b) : brand(b) { }  // ✅ Efficient
    ```

2. **Make data members private**

    ```cpp
    class Good {
    private:
        int value;  // ✅ Encapsulated
    public:
        int getValue() const { return value; }
    };
    ```

3. **Use const correctness**

    ```cpp
    int getSize() const { return size; }  // ✅ Won't modify
    ```

4. **Prefer smart pointers** over raw pointers

    ```cpp
    std::unique_ptr<Car> car = std::make_unique<Car>();  // ✅ RAII
    ```

5. **Initialize all members**
    ```cpp
    class Safe {
        int x = 0;  // ✅ In-class initialization
        int y = 0;
    };
    ```

### ❌ Common Pitfalls

1. **Forgetting to initialize members**

    ```cpp
    class Bad {
        int value;  // ❌ Uninitialized!
    };
    ```

2. **Public data members**

    ```cpp
    class Bad {
    public:
        int balance;  // ❌ No encapsulation
    };
    ```

3. **Memory leaks**

    ```cpp
    class Leak {
        int* data = new int(0);
        // ❌ No destructor to delete data!
    };
    ```

4. **Shallow copy with pointers**

    ```cpp
    // ❌ Default copy constructor does shallow copy
    // Both objects share same pointer!
    ```

5. **Not using const for read-only functions**
    ```cpp
    int getValue() { return val; }  // ❌ Should be const
    ```

---

## 10. Real-World Examples

### Example 1: Bank Account with Validation

```cpp
class BankAccount {
private:
    std::string accountNumber;
    double balance;

public:
    BankAccount(const std::string& acc, double initial)
        : accountNumber(acc), balance(initial) { }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
        }
    }

    bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const { return balance; }
};
```

### Example 2: Method Chaining for Fluent APIs

```cpp
class QueryBuilder {
private:
    std::string query;

public:
    QueryBuilder& select(const std::string& cols) {
        query += "SELECT " + cols + " ";
        return *this;
    }

    QueryBuilder& from(const std::string& table) {
        query += "FROM " + table + " ";
        return *this;
    }

    QueryBuilder& where(const std::string& condition) {
        query += "WHERE " + condition;
        return *this;
    }

    std::string build() const { return query; }
};

// Usage
std::string sql = QueryBuilder()
    .select("*")
    .from("users")
    .where("age > 18")
    .build();
```

---

## 11. Comparison with Other Languages

| Feature                  | C++                | Java                    | TypeScript                 |
| ------------------------ | ------------------ | ----------------------- | -------------------------- |
| **Syntax**               | `class Car { };`   | `class Car { }`         | `class Car { }`            |
| **Constructor name**     | Same as class      | Same as class           | `constructor()`            |
| **Destructor**           | ✅ `~ClassName()`  | ❌ No (GC)              | ❌ No (GC)                 |
| **Access default**       | `private`          | Package-private         | `public`                   |
| **Memory management**    | Manual (RAII)      | Automatic (GC)          | Automatic (GC)             |
| **Multiple inheritance** | ✅ Yes             | ❌ No (interfaces only) | ❌ No (interfaces only)    |
| **`this` keyword**       | Pointer (`this->`) | Reference (`this.`)     | Reference (`this.`)        |
| **Static members**       | ✅ Yes             | ✅ Yes                  | ✅ Yes                     |
| **Const correctness**    | ✅ Yes (`const`)   | ✅ Partial (`final`)    | ✅ TypeScript (`readonly`) |

### C++ Unique Strengths

- **Deterministic destruction** (RAII pattern)
- **Stack allocation** (no heap overhead)
- **Const correctness** (compile-time safety)
- **Zero-overhead abstraction**

---

## 12. Summary Table

| Concept         | Purpose                   | Syntax              |
| --------------- | ------------------------- | ------------------- |
| **Class**       | Blueprint for objects     | `class Name { };`   |
| **Object**      | Instance of class         | `Name obj;`         |
| **Constructor** | Initialize object         | `Name() { }`        |
| **Destructor**  | Cleanup resources         | `~Name() { }`       |
| **`this`**      | Pointer to current object | `this->member`      |
| **private**     | Restrict access           | `private: int x;`   |
| **public**      | Allow access              | `public: void f();` |
| **static**      | Class-level member        | `static int count;` |
| **const**       | Read-only guarantee       | `void f() const;`   |

---

## 13. Final Takeaways

> **Classes define structure, objects provide instances**

1. **Use constructors** for initialization
2. **Use destructors** for cleanup (RAII)
3. **Encapsulate** with private members
4. **Mark const** what doesn't modify
5. **Initialize** all members
6. **Follow Rule of Three** when managing resources

### Core Insight

> C++ classes provide zero-overhead abstraction with deterministic resource management—when designed correctly, they're both safe and performant.

When used well, classes make your code:

- More organized
- Easier to maintain
- Safer from bugs
- More reusable

---

## 14. References

- [cppreference: Classes](https://en.cppreference.com/w/cpp/language/classes)
- [GeeksforGeeks: C++ Classes and Objects](https://www.geeksforgeeks.org/c-classes-and-objects/)
- [cppreference: The this pointer](https://en.cppreference.com/w/cpp/language/this)
- [cppreference: Constructors](https://en.cppreference.com/w/cpp/language/constructor)
- [cppreference: Destructors](https://en.cppreference.com/w/cpp/language/destructor)
