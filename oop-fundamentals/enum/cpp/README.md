# Enums in C++ — A Complete Practical Guide

This note explains enums in C++ from the ground up, including:

- classic enums
- `enum class`
- attaching behavior
- usage in OOP
- Java-style singleton enum emulation
- why that idea exists
- why it is usually a bad fit in C++

---

> Reference - https://www.geeksforgeeks.org/cpp/enum-classes-in-c-and-their-advantage-over-enum-datatype/

## 1. What Is an Enum?

An enum represents a **closed set of named values**.

Use enums when:

- Values are known at compile time
- Only predefined values are valid
- Values represent **labels**, not objects

---

## 2. Unscoped Enums (Legacy)

```cpp
enum Color { Red, Green, Blue };
```

### Problems

- Names leak into global scope
- Implicit conversion to `int`
- Weak type safety

```cpp
int x = Red;// allowed
if (Red ==0) { }// allowed
```

⚠️ **Avoid in modern C++**

---

## 3. Scoped Enums (`enum class`) — Best Practice

```cpp
enum class Coin {
    Penny,
    Nickel,
    Dime,
    Quarter
};
```

### Advantages

- Strong typing
- Scoped names (`Coin::Dime`)
- No implicit conversion
- Prevents accidental arithmetic

```cpp
// int x = Coin::Dime;  // ❌ compile error
```

### Mental Model

> enum class = distinct value type, not an integer
> 

---

## 4. Attaching Behavior to Enums (C++ Way)

C++ enums cannot have methods.

Instead, behavior is added using **free functions**.

```cpp
constexpr int coinValue(Coin coin);
```

### Why This Is Ideal

- Preserves value semantics
- Zero runtime overhead
- Can be evaluated at compile time
- Keeps enum simple

This matches C++'s philosophy:

> Don’t pay for what you don’t use
> 

---

## 5. Enums in Object-Oriented Code

Enums work naturally as **domain values** inside classes.

Example:

```cpp
class Wallet {
	void addCoin(Coin coin);
};
```

Key idea:

- Enums define *what* something is
- Classes define *how* it is used

This separation improves:

- Maintainability
- Testability
- Correctness

---

## 6. Java-Style Singleton Enum Emulation

C++ *can* emulate Java enums using singleton objects:

```cpp
class CoinObject {
public:
	static const CoinObject Dime;
};
```

### Why This Idea Comes Up

This pattern comes from:

- Java enums being real objects
- TypeScript lacking rich enums
- Developers wanting behavior on enum values

It looks attractive because:

```cpp
CoinObject::Dime.value();
```

Feels object-oriented.

---

## 7. Why This Is Usually Wrong in C++

Although valid, this approach is **not ideal** for C++:

### ❌ Reasons

1. **Coins have no identity**
    - A dime is a value, not an entity
2. **Unnecessary object semantics**
    - Lifetime
    - Initialization rules
    - Linkage concerns
3. **More code, less clarity**
    - Static members
    - Constructors
    - Definitions
4. **Violates value-based design**
    - C++ favors cheap, copyable value types

### Rule of Thumb

> If it behaves like a value, model it as a value.
> 

Coins are values.

---

## 8. When Object-Based Enums Make Sense

Use objects **only when** enum values:

- Have complex behavior
- Require polymorphism
- Act as strategies
- Hold runtime state

Example:

- State machines
- Algorithms
- Command objects

Not denominations.

---

## 9. Summary Table

| Feature | enum | enum class | Singleton Objects |
| --- | --- | --- | --- |
| Type safety | ❌ | ✅ | ✅ |
| Value semantics | ✅ | ✅ | ❌ |
| Object overhead | ❌ | ❌ | ✅ |
| Methods | ❌ | ❌ | ✅ |
| Recommended for coins | ❌ | ✅ | ❌ |

---

## 10. Final Takeaways

- Prefer `enum class` over `enum`
- Attach behavior via free functions
- Use enums as domain values in OOP
- Avoid Java-style enum emulation unless behavior demands it

### Core Insight

> C++ enums are about correctness and zero-cost abstraction — not object identity.
> 

When modeled correctly, enums make your code:

- Safer
- Faster
- Easier to reason about