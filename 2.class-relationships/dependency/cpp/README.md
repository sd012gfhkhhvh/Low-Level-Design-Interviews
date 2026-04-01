# Dependency in C++ — A Complete Practical Guide

This guide breaks down **Dependency** in C++ from fundamental concepts to Dependency Injection patterns, showing how it serves as the lightest architectural relationship.

- What is dependency and how it differs from association
- The 4 forms of dependencies in code
- Dependency Injection (DI) and its benefits
- Real-world example: Event Ticketing System
- Best practices and common anti-patterns

---

## 1. What Is Dependency?

A **Dependency** exists when one class relies on another class to fulfill a specific responsibility, but does so **without retaining a permanent reference** to it.

### The Formula

> **Dependency = "uses-a" temporarily + No Fields Stored + Short-lived Scope**

### Mental Model

Imagine a **Chef preparing a meal**:

- The chef picks up a **Knife** to chop vegetables.
- Once the chopping is done, the knife is put away.
- The chef doesn't permanently store the knife in their pocket (no class fields).
  This represents a dependency. The chef depends on the knife _only_ during the cooking process.

In C++ code:

```cpp
class Printer {
public:
    // Printer uses Document temporarily. It has no fields.
    void print(const Document& doc) {
        std::cout << doc.getText();
    }
};
```

---

## 2. Why Dependency Matters

### Benefits

1. **Extreme Loose Coupling**: Classes aren't structurally bound to one another.
2. **High Testability**: Since behaviors aren't hardcoded into instance variables, mocking dependencies injected into methods is effortless.
3. **Single Responsibility Enabler**: By relying on other focused classes transiently, a service class can coordinate tasks without accumulating bloat.

### The Relationship Hierarchy

| Relationship    | Concept            | Ownership | Lifetime    | Field Stored? |
| --------------- | ------------------ | --------- | ----------- | ------------- |
| **Dependency**  | "uses temporarily" | None      | Independent | ❌ **No**     |
| **Association** | "uses-a"           | None      | Independent | ✅ Yes        |
| **Aggregation** | "has-a"            | Weak      | Independent | ✅ Yes        |
| **Composition** | "part-of"          | Strict    | Dependent   | ✅ Yes        |

---

## 3. UML Notation

Dependency is represented in UML class diagrams by a **dashed arrow** (`..>`) pointing from the dependent class to the class it depends on.

```text
Dependent  ..>  Dependency Target
(Printer)        (Document)
```

---

## 4. The Four Forms of Dependency

Recognizing dependencies is critical for code review and refactoring. They appear in 4 main ways:

### 1. As Method Parameters (Most Common)

The class receives another class explicitly as an argument.

```cpp
void ReportGenerator::generate(const DataSource& source) { ... }
```

### 2. As Local Variables

The class creates another class inside a method, uses it, and discards it before the method ends.

```cpp
void OrderProcessor::process() {
    JsonFormatter formatter; // Created and destroyed locally
    formatter.format();
}
```

### 3. As Return Types

The class acts as a factory, building and returning a type it doesn't store itself.

```cpp
User UserFactory::createUser(std::string name) {
    return User(name);
}
```

### 4. As Static Method Calls

The class relies on global/static logic from another component without ever instantiating it.

```cpp
void PasswordService::secure(std::string pass) {
    std::string hash = HashUtils::hash(pass);
}
```

---

## 5. Dependency Injection (DI)

When a class instantiates its own dependencies internally (e.g. creating a concrete `EmailSender` inside its functions), it binds the code rigidly. Attempting to test this, or attempting to swap the `EmailSender` for an `SmsSender`, requires physically editing the service.

**Dependency Injection** solves this: Provide dependencies from the outside (via arguments/constructors) relying on abstract `interfaces`.

### The Solution: Inject from Outside

```cpp
// Interface
class ISender { virtual void send(string msg) = 0; };

class EmailSender : public ISender { void send(string msg) override { /*...*/ } };
class SmsSender : public ISender { void send(string msg) override { /*...*/ } };

class NotificationService {
public:
    // Any sender can be passed here! Highly modular!
    void notify(const std::string& msg, const ISender& sender) {
        sender.send(msg);
    }
};
```

This ensures:

1. **Mockability** (Pass a FakeSender in tests)
2. **Swappable implementations** (Swap Email for SMS dynamically)

---

## 6. Real-World Example: Event Ticketing System

Let's model an event booking architecture. The `TicketBookingService` coordinates validations, payments, notifications, and QR generation, but owns absolutely none of them.

```cpp
class TicketBookingService {
public:
    // Pure Coordinator! Zero internal fields stored!
    void bookTicket(int seat,
                    const SeatValidator& validator,
                    const PaymentProcessor& paymentProcessor,
                    const QRCodeGenerator& qrGenerator,
                    const ISender& emailService) const
    {
        if (!validator.validate(seat)) return;
        if (!paymentProcessor.processPayment(199.0)) return;

        std::string qr = qrGenerator.generate(seat);
        emailService.send("Ticket confirmed! Code: " + qr);
    }
};
```

**Why this is brilliant**:

- `TicketBookingService` acts solely as an orchestrator.
- Testing this service requires zero I/O or internet access, as you can instantly pass in mock `paymentProcessors` that always return `true`.

---

## 7. Best Practices and Common Anti-Patterns

### ✅ Do This

1. **Depend on Abstractions**: If passing a dependency as a parameter, prefer passing an interface/abstract class rather than a concrete type.
2. **Use Dependency Injection**: Shift the creation of dependencies out of your core business logic and up into the application configuration layer.
3. **Limit Dependencies**: A method taking 7 different dependencies is a code smell indicating it violates the Single Responsibility Principle.

### ❌ Don't Do This

1. **Don't hardcode `new` dependencies inside methods**: Instantiating complex objects locally completely removes the ability to mock them gracefully.
2. **Don't store temporary parameters as fields**: Turning a transient method dependency into a saved class member accidentally transforms a clean Dependency into an Association, retaining memory needlessly.

---

## 8. Summary & Final Takeaways

> **Dependencies represent the fleeting moments where objects collaborate to achieve a goal, dispersing immediately afterward.**

- They are structurally the loosest relationship in software design.
- The absence of saved `fields` / `properties` is the defining factor separating it from Association.
- Embracing **Dependency Injection** alongside interfaces generates decoupled, highly-testable codebases.

### 📚 References

- [AlgoMaster: Dependency in LLD](https://algomaster.io/learn/lld/dependency)
- [Martin Fowler: Inversion of Control Containers and DI](https://martinfowler.com/articles/injection.html)
- [UML Standard Documentation](https://www.uml.org/)
