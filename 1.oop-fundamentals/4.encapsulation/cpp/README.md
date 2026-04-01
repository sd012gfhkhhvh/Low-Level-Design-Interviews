# Encapsulation in C++ — A Complete Practical Guide

This guide explains encapsulation in C++ from fundamentals to advanced patterns, including:

- What is encapsulation and why it matters
- Access modifiers (private, protected, public)
- Getters and setters with validation
- Private helper methods
- Real-world design patterns
- Best practices and anti-patterns
- Comparison with other OOP languages

---

> Reference - https://www.geeksforgeeks.org/encapsulation-in-c/  
> Reference - https://en.cppreference.com/w/cpp/language/access

## 1. What Is Encapsulation?

**Encapsulation** is one of the four foundational principles of object-oriented programming (OOP). It's the practice of bundling data (attributes) and the methods that operate on that data into a single unit (class), while restricting direct access to some of the object's components.

### The Formula

> **Encapsulation = Data Hiding + Controlled Access**

### Mental Model

Think of encapsulation like a **car's dashboard**:

- You see the **interface** (steering wheel, pedals, dashboard)
- You don't see the **implementation** (engine internals, transmission)
- You **control** the car through a simple interface
- The car **validates** your input (can't shift to reverse while moving forward)

---

## 2. Why Encapsulation Matters

### Without Encapsulation ❌

```cpp
class BankAccount {
public:
    double balance;  // ❌ Public - anyone can modify!
};

BankAccount account;
account.balance = -1000000;  // ❌ Disaster! Negative balance
account.balance = 999999999; // ❌ Free money exploit!
```

### With Encapsulation ✅

```cpp
class BankAccount {
private:
    double balance;  // ✅ Hidden from outside

public:
    void deposit(double amount) {
        if (amount > 0) {  // ✅ Validation!
            balance += amount;
        }
    }

    double getBalance() const {
        return balance;  // ✅ Read-only access
    }
};
```

---

## 3. Access Modifiers in C++

C++ provides three access levels:

| Modifier        | Accessible From         | Use Case                       |
| --------------- | ----------------------- | ------------------------------ |
| **`private`**   | Only within the class   | Data members, internal helpers |
| **`protected`** | Class + derived classes | For inheritance hierarchies    |
| **`public`**    | Anywhere                | Public interface methods       |

### Example

```cpp
class Example {
private:
    int secretData;      // Only Example class can access

protected:
    int protectedData;   // Example + derived classes

public:
    int publicData;      // Anyone can access

    void publicMethod() {
        secretData = 10;      // ✅ OK
        protectedData = 20;   // ✅ OK
        publicData = 30;      // ✅ OK
    }
};

class Derived : public Example {
    void method() {
        // secretData = 10;     // ❌ Error: private
        protectedData = 20;     // ✅ OK: protected
        publicData = 30;        // ✅ OK: public
    }
};
```

---

## 4. Getters and Setters

**Getters** provide read access to private data.  
**Setters** provide controlled write access with validation.

### Read-Only Access (Getter Only)

```cpp
class Person {
private:
    std::string id;  // Should never change after creation

public:
    Person(const std::string& personId) : id(personId) {}

    // Getter only - no setter!
    std::string getId() const {
        return id;
    }
};
```

### Controlled Write Access (Setter with Validation)

```cpp
class Employee {
private:
    int age;
    double salary;

public:
    // Getter
    int getAge() const {
        return age;
    }

    // Setter with validation
    void setAge(int newAge) {
        if (newAge < 18 || newAge > 100) {
            throw std::invalid_argument("Invalid age");
        }
        age = newAge;
    }

    void setSalary(double newSalary) {
        if (newSalary < 0) {
            throw std::invalid_argument("Salary cannot be negative");
        }
        salary = newSalary;
    }
};
```

### Benefits of Getters/Setters

- ✅ **Validation**: Enforce business rules
- ✅ **Logging**: Track access or modifications
- ✅ **Lazy initialization**: Compute value when requested
- ✅ **Change implementation**: Modify internals without breaking code
- ✅ **Debugging**: Set breakpoints to track access

---

## 5. Private Helper Methods

Encapsulation isn't just about data—it's also about **hiding implementation details**.

```cpp
class PaymentProcessor {
private:
    std::string cardNumber;

    // Private helper - implementation detail hidden
    std::string maskCardNumber(const std::string& raw) {
        return "****-****-****-" + raw.substr(raw.length() - 4);
    }

    bool validateCard(const std::string& card) {
        // Complex validation logic
        return card.length() == 16;
    }

public:
    PaymentProcessor(const std::string& card) {
        if (!validateCard(card)) {
            throw std::invalid_argument("Invalid card");
        }
        cardNumber = maskCardNumber(card);
    }

    void processPayment(double amount) {
        // User doesn't need to know about masking
        std::cout << "Processing for card: " << cardNumber << "\n";
    }
};
```

**Why hide helper methods?**

- Implementation can change without affecting users
- Cleaner public interface
- Reduces coupling

---

## 6. Encapsulation Levels with Inheritance

```cpp
class Shape {
protected:
    // Protected - derived classes can access
    double area;
    std::string name;

private:
    // Private - only Shape can access
    int shapeId;

public:
    virtual void calculateArea() = 0;

    double getArea() const {
        return area;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {
        name = "Rectangle";
        // Can access protected 'area' and 'name'
        calculateArea();
    }

    void calculateArea() override {
        area = width * height;  // ✅ OK: protected
        // shapeId = 1;         // ❌ Error: private in base
    }
};
```

---

## 7. Real-World Examples

### Example 1: User Authentication

```cpp
class User {
private:
    std::string username;
    std::string passwordHash;  // Never store plain text!
    bool isActive;
    int loginAttempts;

    // Private helper: hash password
    std::string hashPassword(const std::string& password) const {
        // Use bcrypt, argon2, etc. in real world
        return "hash_" + password;
    }

public:
    User(const std::string& user, const std::string& pass)
        : username(user), isActive(true), loginAttempts(0) {
        passwordHash = hashPassword(pass);
    }

    bool login(const std::string& password) {
        if (!isActive) {
            return false;
        }

        if (hashPassword(password) == passwordHash) {
            loginAttempts = 0;
            return true;
        } else {
            loginAttempts++;
            if (loginAttempts >= 3) {
                isActive = false;  // Lock account
            }
            return false;
        }
    }

    // Only expose what's necessary
    std::string getUsername() const {
        return username;
    }

    // Never expose password or hash!
};
```

**Encapsulation benefits here:**

- Password is never exposed
- Hashing logic is hidden
- Account locking is automatic
- Users can't manipulate login attempts

### Example 2: Shopping Cart

```cpp
class ShoppingCart {
private:
    std::vector<std::string> items;
    std::vector<double> prices;
    double discount;

    // Private calculation
    double calculateSubtotal() const {
        double total = 0;
        for (double price : prices) {
            total += price;
        }
        return total;
    }

public:
    ShoppingCart() : discount(0.0) {}

    void addItem(const std::string& item, double price) {
        if (price <= 0) {
            throw std::invalid_argument("Invalid price");
        }
        items.push_back(item);
        prices.push_back(price);
    }

    void applyDiscount(double percent) {
        if (percent < 0 || percent > 100) {
            throw std::invalid_argument("Invalid discount");
        }
        discount = percent;
    }

    double getTotal() const {
        double subtotal = calculateSubtotal();
        return subtotal * (1.0 - discount / 100.0);
    }
};
```

**Encapsulation benefits:**

- Can't add negative-priced items
- Discount validation
- Internal calculation logic hidden
- Can change data structure without breaking code

---

## 8. Best Practices

### ✅ Do This

1. **Make data members private by default**

   ```cpp
   class Good {
   private:
       int value;  // ✅ Private by default
   public:
       int getValue() const { return value; }
   };
   ```

2. **Use const for getters**

   ```cpp
   double getBalance() const {  // ✅ Won't modify object
       return balance;
   }
   ```

3. **Validate in setters**

   ```cpp
   void setAge(int age) {
       if (age < 0 || age > 150) {  // ✅ Validation
           throw std::invalid_argument("Invalid age");
       }
       this->age = age;
   }
   ```

4. **Hide implementation details**

   ```cpp
   private:
       std::string hashPassword(const std::string& pass) {
           // ✅ Helper method hidden
       }
   ```

5. **Return const references for complex types**
   ```cpp
   const std::string& getName() const {  // ✅ Efficient
       return name;
   }
   ```

### ❌ Don't Do This

1. **Public data members**

   ```cpp
   class Bad {
   public:
       double balance;  // ❌ No encapsulation!
   };
   ```

2. **Getters that return non-const references**

   ```cpp
   std::vector<int>& getData() {  // ❌ Exposes internals!
       return data;
   }
   ```

3. **Setters without validation**

   ```cpp
   void setAge(int age) {
       this->age = age;  // ❌ No validation!
   }
   ```

4. **Exposing internal data structures**
   ```cpp
   std::map<string, int>& getInternalMap() {  // ❌ Bad!
       return internalData;
   }
   ```

---

## 9. Common Patterns

### Pattern 1: Read-Only Properties

```cpp
class Product {
private:
    const std::string id;  // Set once, never changes

public:
    Product(const std::string& productId) : id(productId) {}

    std::string getId() const {
        return id;  // No setter!
    }
};
```

### Pattern 2: Computed Properties

```cpp
class Rectangle {
private:
    double width, height;

public:
    // Getters
    double getWidth() const { return width; }
    double getHeight() const { return height; }

    // Computed property - not stored
    double getArea() const {
        return width * height;
    }

    double getPerimeter() const {
        return 2 * (width + height);
    }
};
```

### Pattern 3: Lazy Initialization

```cpp
class DataLoader {
private:
    mutable std::vector<int> data;  // mutable for lazy init
    mutable bool loaded = false;

    void loadData() const {
        if (!loaded) {
            // Load expensive data
            data = {1, 2, 3, 4, 5};
            loaded = true;
        }
    }

public:
    const std::vector<int>& getData() const {
        loadData();  // Load on first access
        return data;
    }
};
```

---

## 10. Comparison with Other Languages

| Feature                       | C++                              | Java                   | TypeScript              |
| ----------------------------- | -------------------------------- | ---------------------- | ----------------------- |
| **Default access**            | `private` (class)                | Package-private        | `public`                |
| **Access modifiers**          | `private`, `protected`, `public` | Same + package-private | Same (compile-time)     |
| **Const correctness**         | ✅ `const` methods               | ❌ No                  | ⚠️ `readonly` (limited) |
| **Property syntax**           | Manual getters/setters           | Manual getters/setters | Native `get`/`set`      |
| **Friend access**             | ✅ `friend` keyword              | ❌ No                  | ❌ No                   |
| **Encapsulation enforcement** | Compile-time                     | Compile + runtime      | Compile-time only       |

### C++ Unique Features

- **`const` correctness**: Mark methods that don't modify state
- **`friend` keyword**: Grant specific external access
- **`mutable`**: Allow modification in const methods
- **Compile-time enforcement**: No runtime overhead

---

## 11. Summary Table

| Concept            | Purpose                | C++ Syntax               |
| ------------------ | ---------------------- | ------------------------ |
| **Data hiding**    | Protect data integrity | `private: int value;`    |
| **Getter**         | Read access            | `int getValue() const`   |
| **Setter**         | Controlled write       | `void setValue(int v)`   |
| **Private helper** | Hide implementation    | `private: void helper()` |
| **Protected**      | For inheritance        | `protected: int data;`   |
| **Friend**         | Grant specific access  | `friend class Other;`    |

---

## 12. Final Takeaways

> **Encapsulation is about controlling access, not just hiding data.**

### Core Principles

1. **Default to private** - Only expose what's necessary
2. **Validate inputs** - Use setters to enforce rules
3. **Hide implementation** - Private helpers for internal logic
4. **Use const** - Mark getters that don't modify state
5. **Think interface first** - Design the public API, then implement

### Key Insight

> "A well-encapsulated class has a small, clear public interface and a complex, flexible private implementation. Users don't know (or care) how it works—they just use it."

**Benefits:**

- **Security**: Protect sensitive data (passwords, balances)
- **Integrity**: Enforce business rules (validation)
- **Maintainability**: Change internals without breaking code
- **Debugging**: Control points for logging/monitoring

---

## 13. When to Use Encapsulation

### Always Use ✅

- **Sensitive data**: Passwords, credit cards, personal info
- **Business rules**: Age limits, balance checks, stock levels
- **Complex state**: Multiple related properties that must stay in sync
- **Future flexibility**: Might need to change implementation later

### Consider Alternatives

- **Simple data structures**: Use `struct` for plain data
- **Performance-critical code**: Inlining may require public access
- **Internal classes**: Sometimes private classes in a module don't need strict encapsulation

---

## 14. References

- [cppreference: Access Specifiers](https://en.cppreference.com/w/cpp/language/access)
- [GeeksforGeeks: Encapsulation in C++](https://www.geeksforgeeks.org/encapsulation-in-c/)
- [C++ Core Guidelines: C.9](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-private)
- [Effective C++: Item 22](https://www.aristeia.com/books.html) - Declare data members private
