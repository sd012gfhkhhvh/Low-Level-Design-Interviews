# Abstraction in TypeScript — A Complete Practical Guide

This guide explains abstraction in TypeScript from fundamentals to advanced patterns, including:

- What is abstraction and why it matters
- Abstraction vs Encapsulation
- Abstract classes with `abstract` keyword
- Interfaces (native TypeScript feature)
- Public API design
- Polymorphism through abstraction
- Design patterns (Strategy, Template Method)
- TypeScript-specific features

---

## 1. What Is Abstraction?

**Abstraction** is the process of hiding complex implementation details and exposing only the relevant, high-level functionality. It allows developers to focus on **WHAT** an object does, rather than **HOW** it does it.

### The Formula

> **Abstraction = Hiding Complexity + Showing Essentials**

### Mental Model

Think of abstraction like a **coffee machine**:

- You press **simple buttons** (brew, strength, size)
- You don't see **complex internals** (pumps, heating, grinding)
- You focus on **what it does** (makes coffee)
- You don't care about **how it works** (temperature control, pressure)

---

## 2. Abstraction vs Encapsulation

These are **distinct but complementary** concepts:

| Aspect          | Abstraction            | Encapsulation             |
| --------------- | ---------------------- | ------------------------- |
| **Focus**       | Hiding **complexity**  | Hiding **data**           |
| **Question**    | "What can I do?"       | "How is data protected?"  |
| **Perspective** | External (user view)   | Internal (implementation) |
| **Example**     | Remote control buttons | Remote control internals  |
| **Goal**        | Simplify usage         | Protect integrity         |

### Code Example

```typescript
// Abstraction: Simple interface, complex behind scenes
class EmailService {
  sendEmail(to: string, subject: string, body: string): void {
    // User sees ONE simple method
    // Doesn't see: SMTP, auth, formatting, retries
  }
}

// Encapsulation: Hide data, control access
class BankAccount {
  private balance: number = 0; // Hidden data

  deposit(amount: number): void {
    // Controlled access
    if (amount > 0) this.balance += amount;
  }
}
```

> **Key:** Abstraction = "What?", Encapsulation = "How protected?"

---

## 3. Abstract Classes in TypeScript

Abstract classes contain the `abstract` keyword and can have both abstract and concrete methods.

### Basic Syntax

```typescript
abstract class Shape {
  // Abstract methods - MUST be implemented
  abstract calculateArea(): number;
  abstract calculatePerimeter(): number;

  // Concrete method - shared implementation
  displayInfo(): void {
    console.log("I am a shape");
  }
}
```

### Key Rules

1. **Cannot instantiate** abstract classes directly
2. **Subclasses must implement** all abstract methods
3. **Can have** both abstract and concrete methods
4. **Can have** constructors and properties
5. Use `abstract` keyword before method

### Example

```typescript
abstract class Shape {
  constructor(protected name: string) {}

  abstract calculateArea(): number; // Must implement

  displayName(): void {
    console.log(`Shape: ${this.name}`);
  }
}

class Circle extends Shape {
  constructor(private radius: number) {
    super("Circle");
  }

  calculateArea(): number {
    // Implementation required!
    return Math.PI * this.radius ** 2;
  }
}

// const s = new Shape("test");  // ❌ Error: cannot instantiate
const c = new Circle(5); // ✅ OK
const s: Shape = c; // ✅ OK: polymorphism
```

---

## 4. Interfaces (Native TypeScript)

**Interfaces** define contracts that classes must fulfill. TypeScript has **native interface support**.

```typescript
interface Printable {
  print(): void;
}

interface Saveable {
  save(filename: string): void;
}

// Implementing multiple interfaces
class Document implements Printable, Saveable {
  constructor(private content: string) {}

  print(): void {
    console.log(this.content);
  }

  save(filename: string): void {
    // Save logic
  }
}
```

### Interface vs Type Alias

| Feature                 | `interface`        | `type`     |
| ----------------------- | ------------------ | ---------- |
| **Extensible**          | ✅ Yes (`extends`) | ⚠️ Limited |
| **Implements**          | ✅ Yes             | ✅ Yes     |
| **Declaration merging** | ✅ Yes             | ❌ No      |
| **Computed properties** | ❌ No              | ✅ Yes     |
| **Union types**         | ❌ No              | ✅ Yes     |

**Recommendation:** Use `interface` for object contracts, `type` for unions/primitives.

---

## 5. Abstract Class vs Interface

| Feature                  | Abstract Class            | Interface         |
| ------------------------ | ------------------------- | ----------------- |
| **Implementation**       | Can have concrete methods | No implementation |
| **Properties**           | Can have properties       | Only declarations |
| **Constructor**          | Can have constructor      | Cannot have       |
| **Multiple inheritance** | ❌ Single only            | ✅ Multiple       |
| **Access modifiers**     | ✅ Can use                | ❌ Cannot use     |
| **Use case**             | Shared behavior           | Pure contract     |

### When to Use Each

**Use Abstract Class:**

- Shared **state** (properties)
- Shared **behavior** (methods)
- Want **default implementation**

**Use Interface:**

- Pure **contract**
- **Multiple** implementations needed
- **Unrelated** classes share behavior

---

## 6. Abstraction Through Public APIs

Even without abstractions, good API design hides complexity:

```typescript
class EmailService {
  constructor(
    private smtpServer: string,
    private port: number,
  ) {}

  // Private: Hidden complexity
  private connectToServer(): void {
    /* ... */
  }
  private authenticate(): void {
    /* ... */
  }
  private formatEmail(): void {
    /* ... */
  }

  // Public: Simple interface
  sendEmail(to: string, subject: string, body: string): void {
    this.connectToServer();
    this.authenticate();
    this.formatEmail();
    // User sees 1 method, not 10 steps!
  }
}
```

**Benefits:**

- ✅ Simple to use
- ✅ Hides complexity
- ✅ Can change implementation
- ✅ Reduces cognitive load

---

## 7. Real-World Example: Payment Processing

```typescript
interface PaymentMethod {
  processPayment(amount: number): boolean;
  getPaymentType(): string;
}

class CreditCardPayment implements PaymentMethod {
  constructor(private cardNumber: string) {}

  private validateCard(): boolean {
    // Complex validation logic
    return this.cardNumber.length === 16;
  }

  processPayment(amount: number): boolean {
    if (!this.validateCard()) return false;
    console.log(`Processing $${amount} via Credit Card`);
    return true;
  }

  getPaymentType(): string {
    return "Credit Card";
  }
}

class PayPalPayment implements PaymentMethod {
  constructor(private email: string) {}

  private authenticateUser(): boolean {
    // OAuth logic
    return this.email.includes("@");
  }

  processPayment(amount: number): boolean {
    if (!this.authenticateUser()) return false;
    console.log(`Processing $${amount} via PayPal`);
    return true;
  }

  getPaymentType(): string {
    return "PayPal";
  }
}

// Usage: Polymorphic
function checkout(payment: PaymentMethod, amount: number): void {
  payment.processPayment(amount);
}
```

**Abstraction benefits:**

- User sees simple `processPayment()` interface
- Different implementations hidden
- Can add new payment types
- Implementation details (validation, auth) hidden

---

## 8. Polymorphism Through Abstraction

```typescript
abstract class Vehicle {
  constructor(protected brand: string) {}

  abstract start(): void;
  abstract stop(): void;

  displayInfo(): void {
    console.log(`Vehicle: ${this.brand}`);
  }
}

class Car extends Vehicle {
  start(): void {
    console.log("Car engine starting...");
  }

  stop(): void {
    console.log("Car engine stopping...");
  }
}

class Motorcycle extends Vehicle {
  start(): void {
    console.log("Motorcycle revving...");
  }

  stop(): void {
    console.log("Motorcycle off...");
  }
}

// Polymorphic behavior
function operateVehicle(v: Vehicle): void {
  v.start(); // Different behavior based on actual type!
  v.stop();
}

const car = new Car("Toyota");
const bike = new Motorcycle("Harley");
operateVehicle(car); // Car-specific
operateVehicle(bike); // Motorcycle-specific
```

---

## 9. Design Patterns Using Abstraction

### Strategy Pattern

```typescript
interface SortStrategy {
  sort(data: number[]): number[];
}

class BubbleSort implements SortStrategy {
  sort(data: number[]): number[] {
    // Bubble sort implementation
    return data;
  }
}

class QuickSort implements SortStrategy {
  sort(data: number[]): number[] {
    // Quick sort implementation
    return data;
  }
}

class Sorter {
  constructor(private strategy: SortStrategy) {}

  setStrategy(strategy: SortStrategy): void {
    this.strategy = strategy;
  }

  sortData(data: number[]): number[] {
    return this.strategy.sort(data);
  }
}

// Usage
const sorter = new Sorter(new BubbleSort());
sorter.sortData([5, 2, 8, 1]);

sorter.setStrategy(new QuickSort());
sorter.sortData([5, 2, 8, 1]);
```

### Template Method Pattern

```typescript
abstract class DataProcessor {
  // Template method
  process(): void {
    this.loadData();
    this.validateData();
    this.transformData();
    this.saveData();
  }

  protected abstract loadData(): void;
  protected abstract transformData(): void;

  protected validateData(): void {
    console.log("Validating...");
  }

  protected saveData(): void {
    console.log("Saving...");
  }
}

class CSVProcessor extends DataProcessor {
  protected loadData(): void {
    console.log("Loading CSV...");
  }

  protected transformData(): void {
    console.log("Converting CSV...");
  }
}
```

---

## 10. TypeScript-Specific Features

### Structural Typing (Duck Typing)

TypeScript uses **structural typing** - if it looks like a duck, it's a duck:

```typescript
interface Flyable {
  fly(): void;
}

class Bird {
  fly(): void {
    console.log("Bird flying");
  }
}

class Airplane {
  fly(): void {
    console.log("Airplane flying");
  }
}

function makeFly(thing: Flyable): void {
  thing.fly();
}

makeFly(new Bird()); // ✅ Works!
makeFly(new Airplane()); // ✅ Also works! (structural typing)
```

### Optional Methods

```typescript
interface Logger {
  log(message: string): void;
  debug?(message: string): void; // Optional
}

class SimpleLogger implements Logger {
  log(message: string): void {
    console.log(message);
  }
  // debug is optional - no need to implement
}
```

### Interface Extension

```typescript
interface Printable {
  print(): void;
}

interface ColorPrintable extends Printable {
  printInColor(color: string): void;
}

class Printer implements ColorPrintable {
  print(): void {
    console.log("Printing...");
  }

  printInColor(color: string): void {
    console.log(`Printing in ${color}...`);
  }
}
```

---

## 11. Best Practices

### ✅ Do This

1. **Use interfaces for contracts**

   ```typescript
   interface Serializable {
     serialize(): string;
   }
   ```

2. **Use abstract classes for shared behavior**

   ```typescript
   abstract class Animal {
     abstract makeSound(): void;
     sleep(): void {
       // Shared implementation
       console.log("Sleeping...");
     }
   }
   ```

3. **Program to abstractions**

   ```typescript
   function process(db: Database) {
     // ✅ Accept interface
     db.query("SELECT * FROM users");
   }
   ```

4. **Hide implementation details**

   ```typescript
   class Service {
     private complexLogic(): void {
       /* hidden */
     }

     public simpleMethod(): void {
       this.complexLogic();
     }
   }
   ```

5. **Use readonly for abstraction safety**
   ```typescript
   interface Config {
     readonly apiUrl: string;
   }
   ```

### ❌ Don't Do This

1. **Exposing implementation details**

   ```typescript
   class Bad {
     public internalHelper(): void {} // ❌ Should be private!
   }
   ```

2. **Programming to concrete types**

   ```typescript
   function process(db: MySQLDatabase) {
     // ❌ Too specific!
     // Should accept Database interface
   }
   ```

3. **Empty interfaces**
   ```typescript
   interface Empty {} // ❌ No value!
   ```

---

## 12. When to Use Abstraction

### Use Abstract Classes When:

- ✅ Shared **behavior** across classes
- ✅ Need **default implementation**
- ✅ Need **shared state** (properties)
- ✅ Building **class hierarchy**

### Use Interfaces When:

- ✅ Defining **pure contract**
- ✅ Multiple **unrelated** classes
- ✅ Need **multiple inheritance**
- ✅ Want **structural typing**

### Use Public API Abstraction When:

- ✅ Simplifying **complex operations**
- ✅ Hiding **implementation details**
- ✅ Building **libraries**
- ✅ Creating **user-friendly** interfaces

---

## 13. Comparison with Other Languages

| Feature                  | TypeScript              | C++                 | Java                    |
| ------------------------ | ----------------------- | ------------------- | ----------------------- |
| **Abstract classes**     | ✅ `abstract` keyword   | ✅ `virtual = 0`    | ✅ `abstract`           |
| **Interfaces**           | ✅ Native `interface`   | Pure abstract class | ✅ `interface`          |
| **Multiple inheritance** | ❌ No (interfaces only) | ✅ Yes              | ❌ No (interfaces only) |
| **Structural typing**    | ✅ Yes (duck typing)    | ❌ No               | ❌ No                   |
| **Runtime enforcement**  | ⚠️ Compile-time only    | ✅ Runtime          | ✅ Runtime              |
| **Optional methods**     | ✅ Yes (`?`)            | ❌ No               | ❌ No                   |

### TypeScript Unique Features

- **Structural typing** (duck typing)
- **Optional methods** in interfaces
- **Interface extension** (`extends`)
- **Type aliases** as alternative
- **Compile-time only** (erased at runtime)

---

## 14. Summary Table

| Concept             | Purpose          | TypeScript Syntax          |
| ------------------- | ---------------- | -------------------------- |
| **Abstract class**  | Shared behavior  | `abstract class Name`      |
| **Abstract method** | Must override    | `abstract method(): type;` |
| **Interface**       | Define contract  | `interface Name { }`       |
| **Implements**      | Fulfill contract | `class C implements I`     |
| **Extends**         | Inherit abstract | `class C extends A`        |
| **Optional**        | Not required     | `method?(): type;`         |

---

## 15. Final Takeaways

> **Abstraction shows WHAT, not HOW.**

### Core Principles

1. **Hide complexity** - Simple interfaces
2. **Program to abstractions** - Not concrete types
3. **Use interfaces** - For pure contracts
4. **Use abstract classes** - For shared behavior
5. **Leverage TypeScript** - Structural typing

### Key Insight

> "TypeScript's structural typing makes abstraction more flexible than Java or C++. If an object has the right shape, it satisfies the contract—no explicit declaration needed."

**Benefits:**

- **Simplicity**: Clean, simple APIs
- **Flexibility**: Change implementation freely
- **Reusability**: Common abstractions
- **Type Safety**: Compile-time checking

### The Abstraction Ladder

```
High:  interface PaymentMethod
         ↓
Mid:   abstract class Payment
         ↓
Low:   class CreditCardPayment
         ↓
Detail: API calls, validation, etc.
```

Users interact at the top, details hidden below.

---

## 16. References

- [TypeScript Handbook: Classes](https://www.typescriptlang.org/docs/handbook/2/classes.html)
- [TypeScript Handbook: Interfaces](https://www.typescriptlang.org/docs/handbook/2/objects.html)
- [TypeScript Deep Dive: Interfaces](https://basarat.gitbook.io/typescript/type-system/interfaces)
- [AlgoMaster: Abstraction in LLD](https://algomaster.io/learn/lld/abstraction)
