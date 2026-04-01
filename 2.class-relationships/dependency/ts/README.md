# Dependency in TypeScript — A Complete Practical Guide

This guide breaks down **Dependency** in TypeScript from fundamental concepts to Dependency Injection patterns, showing how it serves as the foundation for modern TS frameworks (like NestJS and Angular).

- What is dependency and how it differs from association
- The 4 forms of dependencies in code
- Dependency Injection (DI) and its structural benefits
- Real-world example: Event Ticketing System
- Best practices and common anti-patterns

---

## 1. What Is Dependency?

A **Dependency** exists when one class relies on another class to fulfill a specific responsibility, but does so **without retaining a permanent reference** to it.

### The Formula

> **Dependency = "uses-a" temporarily + No Properties Stored + Short-lived Scope**

### Mental Model

Imagine a **Chef preparing a meal**:

- The chef picks up a **Knife** to chop vegetables.
- Once the chopping is done, the knife is put away.
- The chef doesn't permanently store the knife in their pocket (no class properties).
  This represents a dependency. The chef depends on the knife _only_ during the cooking process.

In TypeScript code:

```typescript
class Printer {
  // Printer uses Document temporarily. It has no properties.
  print(doc: Document): void {
    console.log(doc.text);
  }
}
```

---

## 2. Why Dependency Matters

### Benefits

1. **Extreme Loose Coupling**: Classes aren't structurally bound to one another.
2. **High Testability**: Since behaviors aren't hardcoded into instance variables, mocking dependencies injected into methods is effortless in testing frameworks like Jest.
3. **Single Responsibility Enabler**: By relying on other focused classes transiently, a service class can coordinate heavy logic orchestrations elegantly.

### The Relationship Hierarchy

| Relationship    | Concept            | Ownership | Lifetime    | Property Stored? |
| --------------- | ------------------ | --------- | ----------- | ---------------- |
| **Dependency**  | "uses temporarily" | None      | Independent | ❌ **No**        |
| **Association** | "uses-a"           | None      | Independent | ✅ Yes           |
| **Aggregation** | "has-a"            | Weak      | Independent | ✅ Yes           |
| **Composition** | "part-of"          | Strict    | Dependent   | ✅ Yes           |

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

The class receives another instance explicitly as an argument.

```typescript
class ReportGenerator {
  generate(source: DataSource): void {
    /* ... */
  }
}
```

### 2. As Local Variables

The class constructs another class inside a method, uses it, and lets it get garbage collected immediately after.

```typescript
class OrderProcessor {
  process(): void {
    const formatter = new JsonFormatter(); // Local scope
    formatter.format();
  }
}
```

### 3. As Return Types

The class acts as a factory, building and returning a type it doesn't store itself.

```typescript
class UserFactory {
  createUser(name: string): User {
    return new User(name);
  }
}
```

### 4. As Static Method Calls

The class relies on logic from another component without instantiating it.

```typescript
class PasswordService {
  secure(pass: string): void {
    const hash = HashUtils.hash(pass);
  }
}
```

---

## 5. Dependency Injection (DI)

When a class instantiates its own dependencies internally (e.g. creating a concrete `EmailSender` inside its functions), it binds the code rigidly. Attempting to test this, or attempting to swap the `EmailSender` for an `SmsSender`, becomes practically impossible without monkey-patching.

**Dependency Injection** solves this: Provide dependencies from the outside relying on `interfaces`.

### The Solution: Inject from Outside

```typescript
// Interface
interface ISender {
  send(msg: string): void;
}

class EmailSender implements ISender {
  send(msg: string): void {
    /*...*/
  }
}
class SmsSender implements ISender {
  send(msg: string): void {
    /*...*/
  }
}

class NotificationService {
  // Any sender can be passed here! Highly modular!
  notify(msg: string, sender: ISender): void {
    sender.send(msg);
  }
}
```

This structural injection ensures:

1. **Mockability** (Pass a MockSender into functions during Jest tests)
2. **Dynamic implementations** (Swap Email for SMS dynamically based on user config)

_(Note: Frameworks like NestJS automate Dependency Injection via constructor properties, elevating this into the Architectural layer—which aligns closer to Aggregation, but relies heavily on the DI paradigm)_

---

## 6. Real-World Example: Event Ticketing System

Let's model an event booking architecture. The `TicketBookingService` coordinates validations, payments, notifications, and QR generation, but owns absolutely none of them.

```typescript
class TicketBookingService {
  // Pure Coordinator! Zero internal properties stored!
  bookTicket(
    seat: number,
    validator: SeatValidator,
    paymentProcessor: PaymentProcessor,
    qrGenerator: QRCodeGenerator,
    emailService: ISender,
  ): void {
    if (!validator.validate(seat)) return;
    if (!paymentProcessor.processPayment(199.0)) return;

    const qr = qrGenerator.generate(seat);
    emailService.send(`Ticket confirmed! Code: ${qr}`);
  }
}
```

**Why this is brilliant**:

- `TicketBookingService` acts solely as an orchestrator.
- Testing this service requires zero database/API connections, as you can instantly pass in mock `paymentProcessors` that always return `true`.

---

## 7. Best Practices and Common Anti-Patterns

### ✅ Do This

1. **Depend on Abstractions**: Utilize TypeScript `interfaces` and `types` everywhere when passing method parameters.
2. **Reverse the Flow**: Shift the creation of dependencies out of your core business logic and up into the bootstrap layer.
3. **Control Method Arguments**: If a method is taking 7 dependencies, it is violating the Single Responsibility Principle. Refactor immediately.

### ❌ Don't Do This

1. **Don't hardcode `new` dependencies inside methods blindly**: Making heavy I/O instances locally inside a function completely removes the ability to mock them safely.
2. **Don't store transient payloads**: Don't save a method parameter (like a `Request` object) onto `this.request` if it's only meant to be tracked during that one isolated method execution. That upgrades a Dependency into a permanent Association leak.

---

## 8. Summary & Final Takeaways

> **Dependencies represent the fleeting moments where objects collaborate to achieve a goal, dispersing immediately afterward.**

- They are structurally the loosest relationship in software design.
- The absence of saved `properties` (`this.field`) is the defining factor separating it from Association.
- Embracing **Dependency Injection** alongside interfaces is the absolute pinnacle of creating decoupled, enterprise-grade generic TS code.

### 📚 References

- [AlgoMaster: Dependency in LLD](https://algomaster.io/learn/lld/dependency)
- [Martin Fowler: Inversion of Control Containers and DI](https://martinfowler.com/articles/injection.html)
- [NestJS Architecture Fundamentals](https://docs.nestjs.com/providers)
