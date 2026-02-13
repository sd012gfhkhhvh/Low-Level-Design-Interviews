# Polymorphism in C++ — A Complete Practical Guide

This guide explains polymorphism in C++ from fundamentals to advanced patterns, including:

- What is polymorphism and why it matters
- Compile-time polymorphism (Function/Operator Overloading)
- Runtime polymorphism (Virtual Functions, Method Overriding)
- vtable and vptr mechanism
- Pure virtual functions and abstract classes
- Object slicing problem
- Real-world examples and best practices

---

> Reference - https://www.geeksforgeeks.org/polymorphism-in-c/  
> Reference - https://en.cppreference.com/w/cpp/language/virtual  
> Reference - https://algomaster.io/learn/lld/polymorphism

## 1. What Is Polymorphism?

**Polymorphism** (from Greek: "many forms") allows the same interface to work with different underlying forms (data types).

### The Formula

> **Polymorphism = Same Interface + Different Behaviors**

### Mental Model

Think of a **universal remote control**:

- One "Power" button (same interface)
- Works with TV, AC, Sound System (different behaviors)
- You press the same button, different devices respond differently

In programming:

```cpp
Shape* shape = new Circle();  // Circle is handled as Shape
shape->draw();                // Calls Circle's draw() at runtime!
```

---

## 2. Why Polymorphism Matters

### Benefits

1. **Code Reusability**: Write generic code once, works with many types
2. **Flexibility**: Add new types without changing existing code (Open/Closed Principle)
3. **Loose Coupling**: Depend on abstractions, not concrete implementations
4. **Extensibility**: Easily plug in new implementations

### Example: Without vs With Polymorphism

**Without Polymorphism** ❌:

```cpp
void drawCircle(Circle* c) { c->draw(); }
void drawRectangle(Rectangle* r) { r->draw(); }
void drawTriangle(Triangle* t) { t->draw(); }

// Need new function for every shape!
```

**With Polymorphism** ✅:

```cpp
void draw(Shape* shape) {
    shape->draw();  // Works with ANY shape!
}

// One function handles all current and future shapes
```

---

## 3. Types of Polymorphism in C++

| Type                      | Resolved At  | Mechanism            | Example                                  |
| ------------------------- | ------------ | -------------------- | ---------------------------------------- |
| **Compile-time** (Static) | Compile time | Function overloading | `add(int, int)` vs `add(double, double)` |
| **Compile-time** (Static) | Compile time | Operator overload    | `Point operator+(Point)`                 |
| **Compile-time** (Static) | Compile time | Templates            | `template<typename T>`                   |
| **Runtime** (Dynamic)     | Runtime      | Virtual functions    | `virtual void draw()`                    |

---

## 4. Compile-Time Polymorphism: Function Overloading

**Function overloading** allows multiple functions with the same name but different parameters.

```cpp
class Calculator {
public:
    // Overload based on number of parameters
    int add(int a, int b) {
        return a + b;
    }

    int add(int a, int b, int c) {
        return a + b + c;
    }

    // Overload based on parameter types
    double add(double a, double b) {
        return a + b;
    }

    // Overload with different type
    std::string add(const std::string& a, const std::string& b) {
        return a + b;
    }
};

Calculator calc;
calc.add(2, 3);           // Calls add(int, int)
calc.add(2, 3, 4);        // Calls add(int, int, int)
calc.add(2.5, 3.5);       // Calls add(double, double)
calc.add("Hello", " World"); // Calls add(string, string)
```

### Rules for Function Overloading

✅ **Different parameter count**
✅ **Different parameter types**
✅ **Different parameter order** (if types differ)

❌ **Different return type only** (not enough!)
❌ **Different const-ness only** (not enough in most cases)

---

## 5. Compile-Time Polymorphism: Operator Overloading

**Operator overloading** redefines how operators work for custom types.

```cpp
class Point {
private:
    double x, y;

public:
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    // Overload + operator
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    // Overload == operator
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Overload << operator (friend function)
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

Point p1(3, 4);
Point p2(1, 2);
Point p3 = p1 + p2;          // Uses operator+
bool same = (p1 == p2);      // Uses operator==
std::cout << p3 << std::endl; // Uses operator<<
```

---

## 6. Runtime Polymorphism: Virtual Functions

**Virtual functions** enable runtime polymorphism through dynamic dispatch.

```cpp
class Shape {
public:
    // Virtual function - can be overridden
    virtual void draw() const {
        std::cout << "Drawing a shape\n";
    }

    // Virtual destructor - ALWAYS use in base classes!
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    void draw() const override {  // Override base virtual function
        std::cout << "⭕ Drawing Circle\n";
    }
};

class Rectangle : public Shape {
public:
    void draw() const override {
        std::cout << "▭ Drawing Rectangle\n";
    }
};

// Polymorphic behavior
Shape* shape1 = new Circle();
Shape* shape2 = new Rectangle();

shape1->draw();  // Calls Circle::draw()
shape2->draw();  // Calls Rectangle::draw()
```

### Key Points

- Use `virtual` keyword in base class
- Use `override` keyword in derived class (C++11+)
- Resolved at **runtime**, not compile time
- Requires pointers or references to base class

---

## 7. How Runtime Polymorphism Works: vtable & vptr

When a class has virtual functions, C++ creates:

1. **vtable** (virtual table): Array of function pointers for each class
2. **vptr** (virtual pointer): Hidden pointer in each object pointing to its vtable

### Mechanism

```cpp
class Base {
public:
    virtual void func1() { }
    virtual void func2() { }
};

class Derived : public Base {
public:
    void func1() override { }  // Overrides func1
    // func2 not overridden, uses Base version
};
```

**Memory Layout:**

```
Base vtable:         Derived vtable:
+----------+         +----------+
| &Base::func1 |     | &Derived::func1 |
| &Base::func2 |     | &Base::func2    |
+----------+         +----------+

Base object:         Derived object:
+------+            +------+
| vptr |---> Base vtable    | vptr |---> Derived vtable
+------+            +------+
```

**Runtime Dispatch:**

```cpp
Base* ptr = new Derived();
ptr->func1();  // 1. Follow ptr's vptr to Derived vtable
               // 2. Look up func1 in vtable
               // 3. Call Derived::func1
```

---

## 8. Pure Virtual Functions & Abstract Classes

**Pure virtual function**: Declared with `= 0`, makes class abstract.

**Abstract class**: Has at least one pure virtual function, cannot be instantiated.

```cpp
class Animal {  // Abstract class
protected:
    std::string name;

public:
    Animal(const std::string& n) : name(n) {}

    // Pure virtual function
    virtual void makeSound() const = 0;

    // Regular virtual function
    virtual void sleep() const {
        std::cout << name << " is sleeping\n";
    }

    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    Dog(const std::string& n) : Animal(n) {}

    // MUST implement pure virtual function
    void makeSound() const override {
        std::cout << name << " says: Woof!\n";
    }
};

// Animal a("Generic");  // ❌ Error: cannot instantiate abstract class
Dog dog("Buddy");        // ✅ OK: Dog implements all pure virtual functions
Animal* ptr = &dog;      // ✅ OK: Base pointer to derived object
ptr->makeSound();        // Calls Dog::makeSound()
```

### When to Use Abstract Classes

✅ Define interface contract (what must be implemented)  
✅ Provide some shared implementation  
✅ Prevent instantiation of base class  
✅ Force derived classes to implement specific methods

---

## 9. Virtual Destructor (Critical!)

**ALWAYS make destructor virtual in base classes** to prevent memory leaks.

### The Problem

```cpp
class Base {
public:
    ~Base() {  // ❌ Not virtual!
        std::cout << "~Base()\n";
    }
};

class Derived : public Base {
private:
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() {  // Won't be called!
        delete[] data;
        std::cout << "~Derived()\n";
    }
};

Base* ptr = new Derived();
delete ptr;  // ❌ Only calls ~Base()! Memory leak from data!
```

### The Solution

```cpp
class Base {
public:
    virtual ~Base() {  // ✅ Virtual destructor!
        std::cout << "~Base()\n";
    }
};

class Derived : public Base {
private:
    int* data;
public:
    Derived() { data = new int[100]; }
    ~Derived() override {  // Will be called!
        delete[] data;
        std::cout << "~Derived()\n";
    }
};

Base* ptr = new Derived();
delete ptr;  // ✅ Calls ~Derived() then ~Base()
// Output:
// ~Derived()
// ~Base()
```

---

## 10. Object Slicing Problem

**Object slicing** occurs when assigning a derived object to a base object.

### The Problem

```cpp
Circle circle("Red", 5);

// ❌ Slicing: Circle-specific data lost!
Shape shape = circle;  // Copies only Shape part
shape.draw();          // Calls Shape::draw(), NOT Circle::draw()
```

### The Solution

Use **pointers** or **references**:

```cpp
Circle circle("Red", 5);

// ✅ Polymorphism with pointer
Shape* shapePtr = &circle;
shapePtr->draw();  // Calls Circle::draw()

// ✅ Polymorphism with reference
Shape& shapeRef = circle;
shapeRef.draw();   // Calls Circle::draw()
```

**Rule:** Runtime polymorphism requires pointers or references to base class!

---

## 11. Function Overriding vs Overloading

| Aspect         | Overloading                | Overriding                  |
| -------------- | -------------------------- | --------------------------- |
| **Type**       | Compile-time polymorphism  | Runtime polymorphism        |
| **Location**   | Same class                 | Base and derived classes    |
| **Signature**  | Different parameters       | Same signature              |
| **Keywords**   | None                       | `virtual`, `override`       |
| **Resolution** | Compile time               | Runtime                     |
| **Purpose**    | Same name, different tasks | Redefine inherited behavior |

### Example

```cpp
class Base {
public:
    // Overloading (same class, different parameters)
    void show(int x) { }
    void show(double x) { }

    // Virtual function for overriding
    virtual void display() {
        std::cout << "Base display\n";
    }
};

class Derived : public Base {
public:
    // Overriding (same signature as base)
    void display() override {
        std::cout << "Derived display\n";
    }
};
```

---

## 12. Real-World Example: Payment System

```cpp
class PaymentMethod {
protected:
    std::string accountId;

public:
    PaymentMethod(const std::string& id) : accountId(id) {}

    virtual bool processPayment(double amount) = 0;
    virtual std::string getType() const = 0;
    virtual ~PaymentMethod() = default;
};

class CreditCard : public PaymentMethod {
private:
    std::string cardNumber;

public:
    CreditCard(const std::string& id, const std::string& card)
        : PaymentMethod(id), cardNumber(card) {}

    bool processPayment(double amount) override {
        std::cout << "💳 Processing $" << amount << " via Credit Card\n";
        return true;
    }

    std::string getType() const override {
        return "Credit Card";
    }
};

class PayPal : public PaymentMethod {
private:
    std::string email;

public:
    PayPal(const std::string& id, const std::string& email)
        : PaymentMethod(id), email(email) {}

    bool processPayment(double amount) override {
        std::cout << "💰 Processing $" << amount << " via PayPal\n";
        return true;
    }

    std::string getType() const override {
        return "PayPal";
    }
};

// Usage: Polymorphic function
void processTransaction(PaymentMethod* method, double amount) {
    std::cout << "Payment Type: " << method->getType() << "\n";
    method->processPayment(amount);
}

CreditCard cc("ACC001", "1234****");
PayPal pp("ACC002", "user@example.com");

processTransaction(&cc, 100.00);  // Works with CreditCard
processTransaction(&pp, 50.00);   // Works with PayPal
```

---

## 13. Best Practices

### ✅ Do This

1. **Always virtual destructor in base classes**

   ```cpp
   class Base {
   public:
       virtual ~Base() = default;  // ✅ Critical!
   };
   ```

2. **Use `override` keyword**

   ```cpp
   class Derived : public Base {
   public:
       void func() override { }  // ✅ Clear intent, compile-time check
   };
   ```

3. **Use pointers/references for polymorphism**

   ```cpp
   Base* ptr = new Derived();  // ✅ Enables polymorphism
   ```

4. **Program to abstractions**

   ```cpp
   void process(Shape* shape) {  // ✅ Accepts any Shape
       shape->draw();
   }
   ```

5. **Use pure virtual for interfaces**
   ```cpp
   class Interface {
   public:
       virtual void method() = 0;  // ✅ Must implement
       virtual ~Interface() = default;
   };
   ```

### ❌ Don't Do This

1. **Forget virtual destructor**

   ```cpp
   class Base {
   public:
       ~Base() { }  // ❌ Memory leak risk!
   };
   ```

2. **Assign derived to base object**

   ```cpp
   Derived d;
   Base b = d;  // ❌ Slicing!
   ```

3. **Call virtual functions in constructor**
   ```cpp
   class Base {
   public:
       Base() {
           init();  // ❌ Calls Base::init(), not Derived::init()!
       }
       virtual void init() { }
   };
   ```

---

## 14. Comparison: Compile-time vs Runtime

| Aspect          | Compile-time            | Runtime                         |
| --------------- | ----------------------- | ------------------------------- |
| **Binding**     | Early (static)          | Late (dynamic)                  |
| **Resolution**  | Compile time            | Runtime                         |
| **Performance** | Faster (no overhead)    | Slight overhead (vtable lookup) |
| **Flexibility** | Less flexible           | More flexible                   |
| **Examples**    | Overloading, templates  | Virtual functions               |
| **Type safety** | Checked at compile time | Checked at runtime              |

---

## 15. When to Use Each Type

### Use Compile-Time Polymorphism When:

✅ Performance is critical (no runtime overhead)  
✅ Types known at compile time  
✅ Need different behavior for different parameter types  
✅ Building template libraries

### Use Runtime Polymorphism When:

✅ Types determined at runtime  
✅ Need to add types without recompiling  
✅ Building extensible systems  
✅ Implementing design patterns (Strategy, Factory, Observer)

---

## 16. Summary Table

| Concept                | Syntax                           | Purpose                     |
| ---------------------- | -------------------------------- | --------------------------- |
| **Function overload**  | `void f(int)` + `void f(double)` | Same name, different params |
| **Operator overload**  | `T operator+(const T&)`          | Redefine operators          |
| **Virtual function**   | `virtual void f()`               | Enable override             |
| **Pure virtual**       | `virtual void f() = 0`           | Abstract method             |
| **Override**           | `void f() override`              | Redefine virtual function   |
| **Virtual destructor** | `virtual ~Class()`               | Proper cleanup              |

---

## 17. Final Takeaways

> **Polymorphism enables writing code that works with many types while allowing each type to behave differently.**

### Core Principles

1. **Compile-time polymorphism** - Resolved before program runs (fast, inflexible)
2. **Runtime polymorphism** - Resolved during execution (flexible, slight overhead)
3. **Virtual functions** - Enable runtime polymorphism through vtable
4. **Virtual destructor** - Always use in base classes
5. **Pointers/References** - Required for runtime polymorphism

### Key Insight

> "The power of polymorphism lies in writing generic code once that works with both current and future types, without modification."

**Benefits:**

- **Flexibility**: Add new types easily
- **Extensibility**: Open for extension, closed for modification
- **Maintainability**: Change once, affects all
- **Reusability**: Generic code works with many types

**Trade-offs:**

- **Runtime overhead**: vtable lookup (minimal)
- **Memory overhead**: vptr in each object
- **Complexity**: Understanding vtable mechanism

---

## 18. References

- [cppreference: Virtual Functions](https://en.cppreference.com/w/cpp/language/virtual)
- [GeeksforGeeks: Polymorphism in C++](https://www.geeksforgeeks.org/polymorphism-in-c/)
- [GeeksforGeeks: vtable and vptr](https://www.geeksforgeeks.org/cpp/vtable-and-vptr-in-cpp/)
- [AlgoMaster: Polymorphism in LLD](https://algomaster.io/learn/lld/polymorphism)
- [C++ Core Guidelines: C.128-C.140](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)
