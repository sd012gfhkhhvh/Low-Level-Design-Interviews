# Interface in C++ — A Complete Practical Guide

This guide explains interfaces in C++ from fundamentals to advanced patterns, including:

- Pure abstract classes as interfaces
- Multiple interface inheritance
- Interface Segregation Principle
- Polymorphism and dependency inversion
- Real-world design patterns
- Diamond problem resolution
- Comparison with Java and TypeScript

---

> Reference - https://www.geeksforgeeks.org/cpp/cpp-program-to-create-an-interface/  
> Reference - https://www.scaler.com/topics/cpp/abstract-class-in-cpp/

## 1. What Is an Interface?

An **interface** defines a contract—a set of operations that implementing classes **must** provide.

In C++, interfaces are implemented using **pure abstract classes**:

```cpp
class Drawable {
public:
    virtual ~Drawable() {}           // Virtual destructor
    virtual void draw() const = 0;   // Pure virtual function
};
```

### Key Characteristics

- Contains **only** pure virtual functions (`= 0`)
- Has **no data members** (convention)
- Has a **virtual destructor**
- **Cannot be instantiated** directly
- Forces derived classes to implement all methods

### When to Use

- Define contracts for unrelated classes
- Enable polymorphism
- Support dependency inversion
- Implement design patterns (Strategy, Observer, Command)

---

## 2. Basic Interface Implementation

```cpp
class Drawable {
public:
    virtual ~Drawable() {}
    virtual void draw() const = 0;
};

class Circle : public Drawable {
public:
    void draw() const override {
        std::cout << "Drawing a Circle ⭕️\n";
    }
};

class Rectangle : public Drawable {
public:
    void draw() const override {
        std::cout << "Drawing a Rectangle ▭\n";
    }
};
```

### Critical Rule: Virtual Destructor

✅ **Always include a virtual destructor in interfaces**

```cpp
class Interface {
public:
    virtual ~Interface() {}  // ✅ Ensures proper cleanup
};
```

❌ **Without it, deleting via base pointer causes undefined behavior**

```cpp
Drawable* shape = new Circle();
delete shape;  // Without virtual destructor: memory leak!
```

---

## 3. Multiple Interface Inheritance

Unlike Java (pre-Java 8), C++ natively supports **multiple inheritance** from interfaces.

```cpp
class Drawable { /* ... */ };
class Movable { /* ... */ };
class Resizable { /* ... */ };

// Implement multiple interfaces
class Shape : public Drawable, public Movable, public Resizable {
    void draw() const override { /* ... */ }
    void move(int x, int y) override { /* ... */ }
    void resize(double factor) override { /* ... */ }
};
```

### Why This Works

- Interfaces have **no data members** (by convention)
- Only pure virtual functions → no implementation conflicts
- No diamond problem when interfaces are pure

### Mental Model

> Interface = capability  
> Multiple interfaces = multiple capabilities  
> Shape can be Drawable AND Movable AND Resizable

---

## 4. Interface Segregation Principle (ISP)

**"Clients should not be forced to depend on methods they don't use"**

### ❌ Bad Design: Fat Interface

```cpp
class Animal {
public:
    virtual void walk() = 0;
    virtual void fly() = 0;
    virtual void swim() = 0;
};

class Dog : public Animal {
    void walk() override { /* OK */ }
    void fly() override { /* ??? Dog can't fly! */ }
    void swim() override { /* OK */ }
};
```

**Problems:**

- Forces unnecessary implementations
- Violates single responsibility
- Creates coupling
- Confusing API

### ✅ Good Design: Segregated Interfaces

```cpp
class Walkable {
public:
    virtual void walk() = 0;
};

class Flyable {
public:
    virtual void fly() = 0;
};

class Swimmable {
public:
    virtual void swim() = 0;
};

// Compose exactly what you need
class Dog : public Walkable, public Swimmable {
    void walk() override { /* ... */ }
    void swim() override { /* ... */ }
};

class Duck : public Walkable, public Flyable, public Swimmable {
    void walk() override { /* ... */ }
    void fly() override { /* ... */ }
    void swim() override { /* ... */ }
};
```

**Benefits:**

- Clear contracts
- No unnecessary methods
- Easy to extend
- Better testability

---

## 5. Polymorphism with Interfaces

Interfaces enable **runtime polymorphism**—the foundation of flexible design.

```cpp
void renderShapes(const std::vector<Drawable*>& shapes) {
    for (const auto* shape : shapes) {
        shape->draw();  // Polymorphic call
    }
}

// Usage
Circle circle;
Rectangle rect;
Triangle triangle;

std::vector<Drawable*> shapes = {&circle, &rect, &triangle};
renderShapes(shapes);  // Each calls its own draw()
```

### Power of Abstraction

✅ **Advantages:**

- Add new shapes without changing `renderShapes()`
- Program to interface, not implementation
- Loose coupling
- Open/Closed Principle

---

## 6. Real-World Pattern: Strategy (Payment Gateway)

Interfaces enable the **Strategy Pattern**—swapping algorithms at runtime.

```cpp
class PaymentGateway {
public:
    virtual ~PaymentGateway() {}
    virtual void initiatePayment(double amount) = 0;
    virtual std::string getProviderName() const = 0;
};

class StripePayment : public PaymentGateway {
    void initiatePayment(double amount) override {
        std::cout << "Processing via Stripe: $" << amount << "\n";
    }
    std::string getProviderName() const override { return "Stripe"; }
};

class RazorpayPayment : public PaymentGateway {
    void initiatePayment(double amount) override {
        std::cout << "Processing via Razorpay: ₹" << amount << "\n";
    }
    std::string getProviderName() const override { return "Razorpay"; }
};

class CheckoutService {
private:
    PaymentGateway* gateway_;
public:
    void processCheckout(double amount) {
        gateway_->initiatePayment(amount);
    }
    void setPaymentGateway(PaymentGateway* gateway) {
        gateway_ = gateway;
    }
};
```

### Usage

```cpp
StripePayment stripe;
RazorpayPayment razorpay;

CheckoutService checkout(&stripe);
checkout.processCheckout(99.99);  // Uses Stripe

checkout.setPaymentGateway(&razorpay);
checkout.processCheckout(1500.0);  // Uses Razorpay
```

### Benefits

- Swap payment providers at runtime
- Add new providers without changing `CheckoutService`
- Testable (mock payment gateways)
- Follows Dependency Inversion Principle

---

## 7. Diamond Problem and Interfaces

The **diamond problem** occurs with multiple inheritance when a class inherits the same base twice.

### Why Interfaces Avoid It

```cpp
class Printable {
public:
    virtual void print() const = 0;  // Pure virtual
};

class Serializable {
public:
    virtual std::string serialize() const = 0;  // Pure virtual
};

class Document : public Printable, public Serializable {
    void print() const override { /* ... */ }
    std::string serialize() const override { /* ... */ }
};
```

**No diamond problem because:**

1. No shared base class
2. No data members in interfaces
3. No implementation to inherit

### When Diamond Problem Occurs

Only when inheriting from classes with **data or implementations**:

```cpp
class Base { int x; };          // Has data
class A : public Base {};
class B : public Base {};
class C : public A, public B {}; // ❌ Diamond problem!
```

**Solution:** Use `virtual` inheritance (but avoid this complexity by using pure interfaces instead).

---

## 8. Best Practices and Anti-Patterns

### ✅ Best Practices

1. **Always use virtual destructors**

    ```cpp
    virtual ~Interface() {}
    ```

2. **Keep interfaces focused (ISP)**
    - One cohesive responsibility per interface
    - Small, composable contracts

3. **Use `override` keyword**

    ```cpp
    void draw() const override { /* ... */ }  // Catches errors
    ```

4. **Prefer interfaces over abstract classes**
    - Pure virtual functions only
    - No data members

5. **Program to interfaces, not implementations**
    ```cpp
    void process(Drawable* shape);  // ✅ Flexible
    void process(Circle* circle);    // ❌ Rigid
    ```

### ❌ Common Pitfalls

1. **Forgetting virtual destructor**

    ```cpp
    class Bad {  // ❌ No virtual destructor
    public:
        virtual void foo() = 0;
    };
    ```

2. **Adding data members to interfaces**

    ```cpp
    class NotAnInterface {  // ❌ Has data
        int data_;
    public:
        virtual void method() = 0;
    };
    ```

3. **Fat interfaces**

    ```cpp
    class TooManyMethods {  // ❌ Violates ISP
        virtual void method1() = 0;
        virtual void method2() = 0;
        // ... 20 more methods
    };
    ```

4. **Using raw pointers without ownership clarity**
    ```cpp
    // Better: Use smart pointers
    std::unique_ptr<Drawable> shape = std::make_unique<Circle>();
    ```

---

## 9. Comparison with Other Languages

| Feature                     | C++                 | Java                     | TypeScript               |
| --------------------------- | ------------------- | ------------------------ | ------------------------ |
| **Keyword**                 | Pure abstract class | `interface`              | `interface`              |
| **Multiple inheritance**    | ✅ Yes              | ✅ Yes (interfaces only) | ✅ Yes                   |
| **Virtual destructor**      | ✅ Required         | ❌ N/A (GC)              | ❌ N/A                   |
| **Default implementations** | ❌ No (pure)        | ✅ Yes (Java 8+)         | ✅ Yes                   |
| **Data members**            | ❌ No (convention)  | ❌ No                    | ✅ Yes (but discouraged) |
| **Runtime overhead**        | Virtual table       | Virtual table            | None (compile-time)      |

### C++ Unique Strengths

- **Zero-overhead abstraction** (when designed well)
- **Multiple inheritance** without restrictions
- **Value semantics** with interfaces via templates
- **Compile-time polymorphism** via templates (alternative to interfaces)

### When to Use Interfaces in C++

✅ **Use interfaces when:**

- Need runtime polymorphism
- Implementing design patterns
- Creating plugin architectures
- Defining public APIs

❌ **Consider alternatives when:**

- Compile-time polymorphism suffices (use templates)
- Performance is critical (templates can be faster)
- Single implementation (just use a concrete class)

---

## 10. Summary Table

| Concept                   | Purpose               | Example                              |
| ------------------------- | --------------------- | ------------------------------------ |
| **Pure virtual function** | Force implementation  | `virtual void f() = 0;`              |
| **Virtual destructor**    | Enable proper cleanup | `virtual ~Interface() {}`            |
| **Multiple inheritance**  | Compose capabilities  | `class X : public A, public B`       |
| **ISP**                   | Focused contracts     | Small, single-purpose interfaces     |
| **Polymorphism**          | Runtime flexibility   | `Base* ptr = new Derived();`         |
| **Strategy Pattern**      | Swap algorithms       | Payment gateways, sorting strategies |

---

## 11. Final Takeaways

> **Interfaces define WHAT, not HOW**

1. **Use pure abstract classes** to create interfaces in C++
2. **Always include virtual destructors** in interfaces
3. **Apply Interface Segregation Principle** for maintainable code
4. **Multiple inheritance works** when interfaces are pure
5. **Program to interfaces** for flexible, testable designs

### Core Insight

> C++ interfaces enable runtime polymorphism while maintaining zero-overhead abstraction—when used correctly, they provide flexibility without sacrificing performance.

When designed well, interfaces make your code:

- More flexible
- Easier to test
- Simpler to extend
- Better architected

---

## 12. References

- [GeeksforGeeks: C++ Interface](https://www.geeksforgeeks.org/cpp/cpp-program-to-create-an-interface/)
- [Scaler: Abstract Class in C++](https://www.scaler.com/topics/cpp/abstract-class-in-cpp/)
- [cppreference: Virtual Functions](https://en.cppreference.com/w/cpp/language/virtual)
- [Interface Segregation Principle](https://en.wikipedia.org/wiki/Interface_segregation_principle)
