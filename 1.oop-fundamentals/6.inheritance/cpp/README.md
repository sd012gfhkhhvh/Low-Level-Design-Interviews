# Inheritance in C++ — A Complete Practical Guide

This guide explains inheritance in C++ from fundamentals to advanced patterns, including:

- What is inheritance and why it matters
- Types of inheritance (Single, Multi-level, Hierarchical, Multiple)
- Access specifiers and inheritance modes
- The Diamond Problem and virtual inheritance
- Constructor/destructor chains
- Polymorphism through inheritance
- Real-world examples and best practices

---

> Reference - https://www.geeksforgeeks.org/inheritance-in-c/  
> Reference - https://en.cppreference.com/w/cpp/language/derived_class  
> Reference - https://algomaster.io/learn/lld/inheritance

## 1. What Is Inheritance?

**Inheritance** allows one class (child/derived/subclass) to inherit properties and behaviors from another class (parent/base/superclass). It's one of the four pillars of OOP.

### The Formula

> **Inheritance = Code Reuse + "is-a" Relationship + Hierarchy**

### Mental Model

Think of inheritance like a **family tree**:

- Children inherit traits from parents (eye color, height)
- Children can have unique traits too (different personality)
- Grandchildren inherit from both parents and grandparents
- Traits pass down through generations

In programming:

```cpp
class Animal { };        // Parent/Base
class Dog : public Animal { };  // Child/Derived (Dog IS-A Animal)
```

---

## 2. Why Inheritance Matters

### Benefits

1. **Code Reusability**: Write common logic once in base class
2. **Logical Hierarchy**: Models real-world "is-a" relationships
3. **Ease of Maintenance**: Change once, affects all derived classes
4. **Enables Polymorphism**: Runtime behavior based on actual type

### Example: Without vs With Inheritance

**Without Inheritance** ❌ (Code Duplication):

```cpp
class Dog {
    void eat() { /* eating logic */ }
    void sleep() { /* sleeping logic */ }
    void bark() { /* barking logic */ }
};

class Cat {
    void eat() { /* same eating logic! */ }
    void sleep() { /* same sleeping logic! */ }
    void meow() { /* meowing logic */ }
};
```

**With Inheritance** ✅ (Code Reuse):

```cpp
class Animal {
    void eat() { /* eating logic */ }
    void sleep() { /* sleeping logic */ }
};

class Dog : public Animal {
    void bark() { /* barking logic */ }
};

class Cat : public Animal {
    void meow() { /* meowing logic */ }
};
```

---

## 3. Basic Syntax

```cpp
class Base {
protected:
    int value;
public:
    void display() { }
};

class Derived : public Base {  // Public inheritance
private:
    int extra;
public:
    void show() {
        display();  // Can call base method
        value = 10; // Can access protected member
    }
};
```

### Inheritance Modes

```cpp
class Derived : public Base { };     // Most common
class Derived : protected Base { };  // Rare
class Derived : private Base { };    // Very rare
```

| Base Member | Public Inheritance | Protected Inheritance | Private Inheritance |
| ----------- | ------------------ | --------------------- | ------------------- |
| `public`    | `public`           | `protected`           | `private`           |
| `protected` | `protected`        | `protected`           | `private`           |
| `private`   | Not accessible     | Not accessible        | Not accessible      |

**Rule of Thumb:** Use `public` inheritance 99% of the time.

---

## 4. Types of Inheritance

### 1. Single Inheritance

**One child, one parent** (most common):

```cpp
class Animal {
public:
    void eat() { std::cout << "Eating\n"; }
};

class Dog : public Animal {
public:
    void bark() { std::cout << "Barking\n"; }
};

Dog dog;
dog.eat();  // From Animal
dog.bark(); // From Dog
```

### 2. Multi-level Inheritance

**Chain of inheritance** (grandparent → parent → child):

```cpp
class Vehicle {
public:
    void start() { }
};

class Car : public Vehicle {
public:
    void honk() { }
};

class ElectricCar : public Car {
public:
    void charge() { }
};

ElectricCar tesla;
tesla.start();  // From Vehicle
tesla.honk();   // From Car
tesla.charge(); // From ElectricCar
```

### 3. Hierarchical Inheritance

**Multiple children, one parent**:

```cpp
class Shape {
public:
    virtual double getArea() = 0;
};

class Circle : public Shape {
    double getArea() override { return 3.14 * r * r; }
};

class Rectangle : public Shape {
    double getArea() override { return w * h; }
};
```

### 4. Multiple Inheritance

**One child, multiple parents** (C++ only, not in Java/C#):

```cpp
class Flyable {
public:
    void fly() { }
};

class Swimmable {
public:
    void swim() { }
};

class Duck : public Animal, public Flyable, public Swimmable {
public:
    // Duck can eat(), fly(), and swim()
};
```

---

## 5. The Diamond Problem

The **Diamond Problem** occurs with multiple inheritance when a class inherits from two classes that share a common ancestor.

### The Problem

```
      Animal
     /      \
   Bird    Aquatic
     \      /
       Duck
```

Without virtual inheritance, `Duck` gets **TWO copies** of `Animal`!

```cpp
class Animal {
public:
    int age;
};

class Bird : public Animal { };
class Aquatic : public Animal { };

class Duck : public Bird, public Aquatic { };

Duck d;
// d.age = 5;  // ❌ Error: ambiguous! Which age? Bird's or Aquatic's?
```

### The Solution: Virtual Inheritance

```cpp
class Animal {
public:
    int age;
};

// Use 'virtual' keyword
class Bird : virtual public Animal { };
class Aquatic : virtual public Animal { };

class Duck : public Bird, public Aquatic { };

Duck d;
d.age = 5;  // ✅ OK: Only ONE Animal instance!
```

**Key Rules:**

- Virtual base class must be initialized by **most derived class**
- Only **one instance** of virtual base exists
- Prevents ambiguity

---

## 6. Constructor and Destructor Chain

### Construction Order

Base classes are constructed **before** derived classes:

```cpp
class Base {
public:
    Base() { std::cout << "1. Base constructor\n"; }
};

class Middle : public Base {
public:
    Middle() { std::cout << "2. Middle constructor\n"; }
};

class Derived : public Middle {
public:
    Derived() { std::cout << "3. Derived constructor\n"; }
};

Derived d;
// Output:
// 1. Base constructor
// 2. Middle constructor
// 3. Derived constructor
```

### Destruction Order

Derived classes are destroyed **before** base classes (reverse order):

```cpp
class Base {
public:
    ~Base() { std::cout << "3. Base destructor\n"; }
};

class Middle : public Base {
public:
    ~Middle() { std::cout << "2. Middle destructor\n"; }
};

class Derived : public Middle {
public:
    ~Derived() { std::cout << "1. Derived destructor\n"; }
};

{
    Derived d;
} // Object goes out of scope
// Output:
// 1. Derived destructor
// 2. Middle destructor
// 3. Base destructor
```

### Virtual Destructor (Critical!)

```cpp
class Base {
public:
    virtual ~Base() { }  // ✅ Always virtual!
};

class Derived : public Base {
public:
    ~Derived() { }
};

Base* ptr = new Derived();
delete ptr;  // Without virtual destructor, only Base destructor called! Memory leak!
```

---

## 7. Polymorphism Through Inheritance

Inheritance enables **runtime polymorphism**:

```cpp
class Animal {
public:
    virtual void makeSound() {
        std::cout << "Some sound\n";
    }
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void makeSound() override {
        std::cout << "Woof!\n";
    }
};

class Cat : public Animal {
public:
    void makeSound() override {
        std::cout << "Meow!\n";
    }
};

// Polymorphic behavior
Animal* animals[] = {new Dog(), new Cat()};

for (Animal* a : animals) {
    a->makeSound();  // Calls correct version based on actual type!
}
// Output:
// Woof!
// Meow!
```

---

## 8. Real-World Example: Notification System

```cpp
class Notification {
protected:
    std::string recipient;
    std::string message;
    std::string timestamp;

public:
    Notification(const std::string& r, const std::string& m)
        : recipient(r), message(m) {
        // Set timestamp
    }

    virtual void send() {
        std::cout << "Sending to: " << recipient << "\n";
    }

    virtual ~Notification() = default;
};

class EmailNotification : public Notification {
private:
    std::string subject;

public:
    EmailNotification(const std::string& r, const std::string& m,
                      const std::string& s)
        : Notification(r, m), subject(s) { }

    void send() override {
        std::cout << "📧 EMAIL\n";
        std::cout << "To: " << recipient << "\n";
        std::cout << "Subject: " << subject << "\n";
        std::cout << "Body: " << message << "\n";
    }
};

class SMSNotification : public Notification {
private:
    std::string phoneNumber;

public:
    SMSNotification(const std::string& r, const std::string& m,
                    const std::string& phone)
        : Notification(r, m), phoneNumber(phone) { }

    void send() override {
        std::cout << "📱 SMS to " << phoneNumber << "\n";
        std::cout << message << "\n";
    }
};

// Usage
void sendNotification(Notification* notify) {
    notify->send();  // Polymorphic!
}

EmailNotification email("user@example.com", "Hello", "Greeting");
SMSNotification sms("Bob", "Hi", "+1-555-0123");

sendNotification(&email);  // Sends email
sendNotification(&sms);    // Sends SMS
```

---

## 9. Best Practices

### ✅ Do This

1. **Always use virtual destructor in base classes**

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
       void method() override { }  // ✅ Clear intent
   };
   ```

3. **Follow "is-a" relationship**

   ```cpp
   class Dog : public Animal { };  // ✅ Dog IS-A Animal
   ```

4. **Use protected for inherited members**

   ```cpp
   class Base {
   protected:
       int value;  // ✅ Accessible to derived classes
   };
   ```

5. **Keep hierarchies shallow**

   ```cpp
   // ✅ Good: 2-3 levels
   Base → Derived

   // ❌ Bad: Too deep
   Level1 → Level2 → Level3 → Level4 → Level5
   ```

### ❌ Don't Do This

1. **Forget virtual destructor**

   ```cpp
   class Base {
   public:
       ~Base() { }  // ❌ Not virtual! Memory leaks!
   };
   ```

2. **Use inheritance for "has-a"**

   ```cpp
   class Car : public Engine { };  // ❌ Car HAS-A Engine, not IS-A!
   ```

3. **Deep inheritance hierarchies**

   ```cpp
   // ❌ Too many levels
   A → B → C → D → E → F
   ```

4. **Multiple inheritance without virtual**
   ```cpp
   class D : public B, public C { };  // ❌ Diamond problem!
   ```

---

## 10. Inheritance vs Composition

| Aspect           | Inheritance     | Composition   |
| ---------------- | --------------- | ------------- |
| **Relationship** | "is-a"          | "has-a"       |
| **Coupling**     | Tight           | Loose         |
| **Flexibility**  | Compile-time    | Runtime       |
| **Reusability**  | Vertical        | Horizontal    |
| **When to use**  | Clear hierarchy | Mix behaviors |

### Example Comparison

**Inheritance** (is-a):

```cpp
class Dog : public Animal {
    // Dog IS-A Animal
};
```

**Composition** (has-a):

```cpp
class Car {
private:
    Engine engine;  // Car HAS-A Engine
    Wheels wheels;  // Car HAS-A Wheels
};
```

**Rule:** Prefer composition over inheritance unless there's a clear "is-a" relationship.

---

## 11. When to Use Inheritance

### Use Inheritance When ✅

- Clear **"is-a"** relationship (Dog is an Animal)
- Shared **behavior/data** across classes
- Want **code reuse** within same family
- Need **polymorphism**
- Hierarchy is **shallow** (2-3 levels)

### Avoid Inheritance When ❌

- Relationship is **"has-a"** (Car has an Engine)
- Need **runtime flexibility** (composition better)
- Want to **mix behaviors** dynamically
- Hierarchy would be **deep** (>3 levels)
- **Tight coupling** is a concern

---

## 12. Common Pitfalls

### 1. Slicing Problem

```cpp
class Base {
public:
    int x = 1;
};

class Derived : public Base {
public:
    int y = 2;
};

Derived d;
Base b = d;  // ❌ Slicing! y is lost!
```

**Solution:**

```cpp
Base* ptr = &d;  // ✅ Use pointers/references
```

### 2. Hiding Base Methods

```cpp
class Base {
public:
    void display() { }
    void display(int x) { }
};

class Derived : public Base {
public:
    void display() { }  // Hides both Base::display methods!
};

Derived d;
// d.display(5);  // ❌ Error: no matching function
```

**Solution:**

```cpp
class Derived : public Base {
public:
    using Base::display;  // ✅ Bring base methods into scope
    void display() { }
};
```

---

## 13. Comparison with Other Languages

| Feature                   | C++                   | Java                   | TypeScript      |
| ------------------------- | --------------------- | ---------------------- | --------------- |
| **Single inheritance**    | ✅ Yes                | ✅ Yes                 | ✅ Yes          |
| **Multiple inheritance**  | ✅ Yes                | ❌ No                  | ❌ No           |
| **Interface inheritance** | Pure abstract class   | `interface`            | `interface`     |
| **Virtual keyword**       | ✅ Required           | Default                | Not needed      |
| **Override keyword**      | Optional (`override`) | Optional (`@Override`) | Not needed      |
| **Virtual destructor**    | ✅ Required           | N/A (GC)               | N/A (GC)        |
| **Diamond problem**       | Virtual inheritance   | Interfaces only        | Interfaces only |

---

## 14. Summary Table

| Concept                 | Syntax                         | Purpose               |
| ----------------------- | ------------------------------ | --------------------- |
| **Single inheritance**  | `class D : public B`           | One parent            |
| **Multi-level**         | `A → B → C`                    | Chain of parents      |
| **Hierarchical**        | `B, C : public A`              | Multiple children     |
| **Multiple**            | `class D : public B, public C` | Multiple parents      |
| **Virtual inheritance** | `virtual public Base`          | Solve diamond problem |
| **Virtual destructor**  | `virtual ~Base()`              | Proper cleanup        |
| **Override**            | `void f() override`            | Clear intent          |

---

## 15. Final Takeaways

> **Inheritance models "is-a" relationships and enables code reuse and polymorphism.**

### Core Principles

1. **Use for "is-a"** - Dog is an Animal
2. **Virtual destructors** - Always in base classes
3. **Prefer composition** - When in doubt
4. **Keep shallow** - 2-3 levels maximum
5. **Use `override`** - Clarity and safety

### Key Insight

> "Inheritance is a powerful tool, but it creates tight coupling. Use it when there's a genuine 'is-a' relationship and you need polymorphism. Otherwise, prefer composition for flexibility."

**Benefits:**

- **Code Reuse**: Write once, use many
- **Polymorphism**: Runtime behavior
- **Hierarchy**: Logical organization
- **Maintainability**: Change once, affects all

**Drawbacks:**

- **Tight Coupling**: Child depends on parent
- **Fragile Base Class**: Changes ripple down
- **Deep Hierarchies**: Hard to understand
- **Inflexibility**: Fixed at compile time

---

## 16. References

- [cppreference: Derived Classes](https://en.cppreference.com/w/cpp/language/derived_class)
- [GeeksforGeeks: Inheritance in C++](https://www.geeksforgeeks.org/inheritance-in-c/)
- [AlgoMaster: Inheritance in LLD](https://algomaster.io/learn/lld/inheritance)
- [C++ Core Guidelines: C.126-C.130](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)
