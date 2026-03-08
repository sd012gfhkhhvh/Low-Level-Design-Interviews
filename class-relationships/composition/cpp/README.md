# Composition in C++ — A Complete Practical Guide

This guide breaks down **Composition** in C++ from fundamental concepts to advanced memory management patterns, specifically tailored for robust system design.

- What is composition and how it fundamentally enforces strict ownership
- The concept of dependent lifecycles and lifecycle control
- Implementing composition using values vs `std::unique_ptr`
- "Composition over Inheritance" principle
- Real-world examples: House & Rooms, Car & Engine
- Best practices and common pitfalls

---

## 1. What Is Composition?

**Composition** is the strongest form of Association. It models a **"whole-part"** relationship characterized by **"strict ownership"** and **"dependent lifecycles"**.

### The Formula

> **Composition = "part-of" relationship + Strict Ownership + Dependent Lifecycles**

### Mental Model

Think of a **House and its Rooms**:

- A House (whole) is composed of Rooms (parts).
- The Rooms cannot exist floating around without the House.
- A Room belongs to exactly one House.
- If a wrecking ball destroys the House, the Rooms vanish with it.

In C++ code:

```cpp
class House {
    // 1. Direct object membership (Values) - Lifetimes are structurally unified
    std::vector<Room> rooms;

    // OR
    // 2. Strict pointer ownership
    std::unique_ptr<Roof> roof;
};
```

---

## 2. Why Composition Matters

### Benefits

1. **Guaranteed Cleanup**: Because lifecycles are joined, you don't face complex memory management bugs or dangling pointers. When the parent dies, children die.
2. **Encapsulation**: The parent tightly hides the children. The outside world doesn't need to know how the part works; it just interacts with the whole.
3. **Flexibility**: Composition is heavily favored in design ("Prefer Composition over Inheritance") because it allows swapping out behavior dynamically at runtime.

### The Big Three Relationships

| Relationship    | Concept   | Ownership | Lifetime    | Example                |
| --------------- | --------- | --------- | ----------- | ---------------------- |
| **Association** | "uses-a"  | None      | Independent | Player ↔ Enemy         |
| **Aggregation** | "has-a"   | Weak      | Independent | Department ↔ Professor |
| **Composition** | "part-of" | Strict    | Dependent   | House ↔ Room           |

---

## 3. UML Notation

Composition is represented in UML by a line with a **solid (filled) diamond** on the side of the "whole".

```text
Whole  ♦──────  Part
(House)        (Room)
```

---

## 4. Implementation 1: By Value

The simplest, safest, and most common way to represent Composition in C++ is by utilizing **value semantics**. C++ handles object lifecycles deterministically via RAII exactly for this scenario.

```cpp
class Room {
    std::string name;
public:
    Room(std::string n) : name(std::move(n)) {}
};

class House {
    std::string address;
    // Composition! The vector owns its elements.
    // When House is destructed, `rooms` is destructed, destroying all elements.
    std::vector<Room> rooms;

public:
    House(std::string addr) : address(std::move(addr)) {}

    void buildRoom(const std::string& name) {
        // The container creates the parts internally!
        rooms.emplace_back(name);
    }
};
```

---

## 5. Implementation 2: By \`std::unique_ptr\`

Sometimes value semantics won't work:

- The part is polymorphic (e.g., you aggregate a Base Class, but compose Derived Classes).
- The part is extremely large (avoiding large stack/value moves).
- The part type is incomplete (Pimpl Idiom).

When using pointers for Composition in C++, **always use `std::unique_ptr`**.

```cpp
class Engine {
public:
    void start() { std::cout << "Vroom!\n"; }
};

class Car {
    // std::unique_ptr signals strict, exclusive ownership.
    std::unique_ptr<Engine> engine;

public:
    Car() {
        // The Car constructs the Engine internally
        engine = std::make_unique<Engine>();
    }

    // Destructor automatically deletes the Engine when Car is destroyed.
    // No `delete engine;` needed!
};
```

---

## 6. Real-World Examples

### Example A: Document and Pages

Let's model a Document containing Pages.

```cpp
class Page {
    int pageNum;
    std::string text;
public:
    Page(int n, std::string t) : pageNum(n), text(std::move(t)) {
        std::cout << "Page " << pageNum << " created\n";
    }
    ~Page() {
        std::cout << "Page " << pageNum << " destroyed\n";
    }
};

class Document {
    std::string title;
    std::vector<Page> pages; // Strict ownership
public:
    Document(std::string t) : title(std::move(t)) {}

    void addPage(const std::string& txt) {
        pages.emplace_back(pages.size() + 1, txt);
    }
};

int main() {
    {
        Document doc("My Essay");
        doc.addPage("Intro...");
        doc.addPage("Conclusion...");
    } // doc goes out of scope -> Document dies -> Pages automatically die
}
```

### Example B: Order and Line Items

An e-commerce order strictly owns its line items. They cannot exist independently of that specific transaction order.

```cpp
class LineItem {
    std::string product;
    double price;
public:
    LineItem(std::string p, double pr) : product(std::move(p)), price(pr) {}
    // ...
};

class Order {
    std::string orderId;
    std::vector<LineItem> items; // Strict ownership by value!

public:
    Order(std::string id) : orderId(std::move(id)) {}

    void addItem(std::string product, double price) {
        items.emplace_back(std::move(product), price);
    }
};
```

---

## 7. Composition Over Inheritance

A major OOP design principle is **"Prefer Composition over Inheritance"**.

Inheritance creates a very rigid `is-a` coupling that can easily break down. Composition creates a flexible `has-a / uses-a-component` design.

**Bad (Inheritance):**

```cpp
class Bird { virtual void fly() {} };
class Ostrich : public Bird {
    // Wait, Ostriches don't fly! We have to override and throw exceptions.
};
```

**Good (Composition):**

```cpp
class FlyBehavior { virtual void execute() = 0; };
class NoFly : public FlyBehavior { void execute() {} };

class Bird {
    std::unique_ptr<FlyBehavior> flyLogic; // Composition
public:
    Bird(std::unique_ptr<FlyBehavior> f) : flyLogic(std::move(f)) {}
};

// Now you can compose birds with whatever precise behavior they need!
```

---

## 8. Best Practices and Common Anti-Patterns

### ✅ Do This

1. **Use Values `T` directly:** Default to direct object instances for parts, not pointers. It relies on standard RAII.
2. **Use `std::unique_ptr<T>` for heap semantics:** If you must use the heap, `unique_ptr` enforces the "one owner" rule of Composition at compile time.
3. **Instantiate Parts Internally:** The "Whole" class should be responsible for calling constructors for its parts. This asserts lifecycle control.

### ❌ Don't Do This

1. **Avoid `std::shared_ptr` in Composition**: `shared_ptr` implies shared ownership, which directly contradicts "strict singular ownership."
2. **Avoid Raw Pointers with manual `delete`**: While technically correct if you put `delete p;` in the destructor, in modern C++ this invites exception safety bugs. Use RAII/Smart Pointers.
3. **Do Not Inject Built Parts**:
   ```cpp
   // ❌ BAD! This is Dependency Injection / Aggregation.
   // Car doesn't control the lifecycle if you pass the pointer from outside!
   Car(Engine* externalEngine);
   ```

---

## 9. Summary Table & Key Comparison

| Feature                | Aggregation                           | Composition                     |
| ---------------------- | ------------------------------------- | ------------------------------- |
| **Symbol (UML)**       | Hollow Diamond `<>---`                | Solid Diamond `♦---`            |
| **Relationship**       | "has-a"                               | "part-of"                       |
| **Ownership**          | Weak / Shared                         | Strict / Exclusive              |
| **Lifecycle**          | Independent                           | Parent controls child           |
| **Sharing**            | Parts can belong to multiple wholes   | Parts belong to 1 whole         |
| **C++ Implementation** | Reference, `T*`, `std::shared_ptr<T>` | `T` value, `std::unique_ptr<T>` |
| **Instantiation**      | Passed in from outside                | Created internally              |

---

## 10. Final Takeaways

> **Composition is the safest, tightest relationship in OOP. When you compose objects, they live, operate, and perish as one unified entity.**

- Lean heavily on Composition. It simplifies memory management and mental load.
- It perfectly maps to C++'s greatest feature: **RAII** (Resource Acquisition Is Initialization).

### 📚 References

- [AlgoMaster: Composition in LLD](https://algomaster.io/learn/lld/composition)
- [CppCoreGuidelines: Rules for resource management](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-unique)
- [Visual Paradigm: Aggregation vs Composition](https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/)
