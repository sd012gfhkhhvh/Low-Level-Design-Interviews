# Inheritance in TypeScript — A Complete Practical Guide

This guide explains inheritance in TypeScript from fundamentals to advanced patterns, including:

- What is inheritance and why it matters
- Types of inheritance (Single, Multi-level, Hierarchical)
- Interfaces for multiple inheritance behavior
- The `super` keyword and constructor chains
- Access modifiers in inheritance
- Method overriding and polymorphism
- Abstract classes
- Real-world examples and best practices

---

## 1. What Is Inheritance?

**Inheritance** allows one class (child/derived) to inherit properties and behaviors from another class (parent/base). It's one of the four pillars of OOP.

### The Formula

> **Inheritance = Code Reuse + "is-a" Relationship + Hierarchy**

### Mental Model

Think of inheritance like **biological inheritance**:

- Children inherit genes from parents (eye color, height)
- Children can have unique traits too (personality, talents)
- Multiple generations can inherit traits
- Each generation builds upon the previous

In TypeScript:

```typescript
class Animal {} // Parent/Base
class Dog extends Animal {} // Child/Derived (Dog IS-A Animal)
```

---

## 2. Why Inheritance Matters

### Benefits

1. **Code Reusability**: Write shared logic once
2. **Logical Hierarchy**: Models real-world "is-a" relationships
3. **Maintainability**: Change once, affects all children
4. **Polymorphism**: Runtime behavior based on actual type

### Example: Without vs With Inheritance

**Without Inheritance** ❌ (Code Duplication):

```typescript
class Dog {
  eat() {
    /* eating logic */
  }
  sleep() {
    /* sleeping logic */
  }
  bark() {
    /* barking logic */
  }
}

class Cat {
  eat() {
    /* same eating logic! */
  }
  sleep() {
    /* same sleeping logic! */
  }
  meow() {
    /* meowing logic */
  }
}
```

**With Inheritance** ✅ (Code Reuse):

```typescript
class Animal {
  eat() {
    /* eating logic */
  }
  sleep() {
    /* sleeping logic */
  }
}

class Dog extends Animal {
  bark() {
    /* barking logic */
  }
}

class Cat extends Animal {
  meow() {
    /* meowing logic */
  }
}
```

---

## 3. Basic Syntax

TypeScript uses the `extends` keyword for inheritance:

```typescript
class Base {
  protected value: number = 0;

  display(): void {
    console.log(this.value);
  }
}

class Derived extends Base {
  private extra: number = 10;

  show(): void {
    this.display(); // Can call parent method
    this.value = 20; // Can access protected member
  }
}

const obj = new Derived();
obj.show(); // Works!
```

---

## 4. Types of Inheritance

### 1. Single Inheritance

**One child, one parent** (most common):

```typescript
class Animal {
  eat(): void {
    console.log("Eating");
  }
}

class Dog extends Animal {
  bark(): void {
    console.log("Barking");
  }
}

const dog = new Dog();
dog.eat(); // From Animal
dog.bark(); // From Dog
```

### 2. Multi-level Inheritance

**Chain of inheritance** (grandparent → parent → child):

```typescript
class Vehicle {
  start(): void {}
}

class Car extends Vehicle {
  honk(): void {}
}

class ElectricCar extends Car {
  charge(): void {}
}

const tesla = new ElectricCar();
tesla.start(); // From Vehicle
tesla.honk(); // From Car
tesla.charge(); // From ElectricCar
```

### 3. Hierarchical Inheritance

**Multiple children, one parent**:

```typescript
abstract class Shape {
  abstract getArea(): number;
}

class Circle extends Shape {
  constructor(private radius: number) {
    super();
  }

  getArea(): number {
    return Math.PI * this.radius ** 2;
  }
}

class Rectangle extends Shape {
  constructor(
    private width: number,
    private height: number,
  ) {
    super();
  }

  getArea(): number {
    return this.width * this.height;
  }
}
```

### 4. Multiple Inheritance (via Interfaces)

TypeScript **doesn't support multiple class inheritance**, but you can **implement multiple interfaces**:

```typescript
interface Flyable {
  fly(): void;
}

interface Swimmable {
  swim(): void;
}

class Duck extends Animal implements Flyable, Swimmable {
  fly(): void {
    console.log("Flying!");
  }

  swim(): void {
    console.log("Swimming!");
  }
}

const duck = new Duck();
duck.eat(); // From Animal
duck.fly(); // From Flyable interface
duck.swim(); // From Swimmable interface
```

---

## 5. The `super` Keyword

The `super` keyword is used to:

1. **Call parent constructor** (must be first in child constructor)
2. **Call parent methods**

### Calling Parent Constructor

```typescript
class Employee {
  constructor(
    protected name: string,
    protected salary: number,
  ) {}
}

class Manager extends Employee {
  constructor(
    name: string,
    salary: number,
    private teamSize: number,
  ) {
    super(name, salary); // ✅ Must call super() first!
    // Now can use 'this'
  }
}
```

### Calling Parent Methods

```typescript
class Employee {
  work(): void {
    console.log(`${this.name} is working`);
  }
}

class Manager extends Employee {
  work(): void {
    super.work(); // Call parent version
    console.log(`and managing ${this.teamSize} people`);
  }
}

const mgr = new Manager("Alice", 120000, 5);
mgr.work();
// Output:
// Alice is working
// and managing 5 people
```

---

## 6. Access Modifiers in Inheritance

| Modifier    | Accessible In           | Use Case                |
| ----------- | ----------------------- | ----------------------- |
| `public`    | Everywhere              | Public API (default)    |
| `protected` | Class + subclasses      | For inheritance         |
| `private`   | Only in class           | Internal implementation |
| `#private`  | Only in class (runtime) | True privacy (ES2022+)  |
| `readonly`  | Cannot modify           | Immutable after init    |

### Example

```typescript
class BankAccount {
  public accountNumber: string; // Accessible everywhere
  protected balance: number; // Accessible in subclasses
  private pin: string; // Only in BankAccount
  readonly createdAt: Date; // Cannot modify

  constructor(accountNumber: string, pin: string) {
    this.accountNumber = accountNumber;
    this.balance = 0;
    this.pin = pin;
    this.createdAt = new Date();
  }

  protected validatePin(inputPin: string): boolean {
    return this.pin === inputPin;
  }
}

class SavingsAccount extends BankAccount {
  addInterest(): void {
    this.balance += 100; // ✅ OK: protected
    // this.pin = "1234";       // ❌ Error: private
    // this.createdAt = new Date(); // ❌ Error: readonly
  }

  withdraw(amount: number, pin: string): void {
    if (!this.validatePin(pin)) {
      // ✅ OK: protected method
      console.log("Invalid PIN");
    }
  }
}
```

---

## 7. Method Overriding

Children can **override** parent methods to provide specialized behavior:

```typescript
class Animal {
  makeSound(): void {
    console.log("Some sound");
  }
}

class Dog extends Animal {
  makeSound(): void {
    // Override (no keyword needed)
    console.log("Woof!");
  }
}

class Cat extends Animal {
  makeSound(): void {
    // Override
    console.log("Meow!");
  }
}

const animals: Animal[] = [new Dog(), new Cat()];
animals.forEach((animal) => animal.makeSound());
// Output:
// Woof!
// Meow!
```

### Calling Parent Method in Override

```typescript
class Vehicle {
  start(): void {
    console.log("Vehicle starting...");
  }
}

class Car extends Vehicle {
  start(): void {
    super.start(); // Call parent version
    console.log("Car started!");
  }
}

const car = new Car();
car.start();
// Output:
// Vehicle starting...
// Car started!
```

---

## 8. Abstract Classes

**Abstract classes** can't be instantiated directly. They serve as base classes with:

- Abstract methods (must implement in children)
- Concrete methods (shared implementation)

```typescript
abstract class DatabaseConnection {
  protected connectionString: string;

  constructor(connectionString: string) {
    this.connectionString = connectionString;
  }

  // Abstract methods - must implement
  abstract connect(): void;
  abstract disconnect(): void;
  abstract executeQuery(query: string): void;

  // Concrete method - can use as-is
  getStatus(): string {
    return "Connected";
  }
}

class MySQLConnection extends DatabaseConnection {
  connect(): void {
    console.log(`Connecting to MySQL at ${this.connectionString}`);
  }

  disconnect(): void {
    console.log("Disconnecting from MySQL");
  }

  executeQuery(query: string): void {
    console.log(`Executing: ${query}`);
  }
}

// const db = new DatabaseConnection("..."); // ❌ Error: cannot instantiate abstract class
const mysql = new MySQLConnection("localhost:3306"); // ✅ OK
```

---

## 9. Real-World Example: Notification System

```typescript
class Notification {
  protected recipient: string;
  protected message: string;
  protected timestamp: Date;

  constructor(recipient: string, message: string) {
    this.recipient = recipient;
    this.message = message;
    this.timestamp = new Date();
  }

  formatHeader(): string {
    return `[${this.timestamp.toISOString()}] To: ${this.recipient}`;
  }

  send(): void {
    console.log(this.formatHeader());
    console.log(`Message: ${this.message}`);
  }
}

class EmailNotification extends Notification {
  constructor(
    recipient: string,
    message: string,
    private subject: string,
  ) {
    super(recipient, message);
  }

  send(): void {
    console.log("📧 EMAIL");
    console.log(this.formatHeader());
    console.log(`Subject: ${this.subject}`);
    console.log(`Body: ${this.message}`);
  }
}

class SMSNotification extends Notification {
  constructor(
    recipient: string,
    message: string,
    private phoneNumber: string,
  ) {
    super(recipient, message);
  }

  send(): void {
    console.log("📱 SMS");
    console.log(`To: ${this.phoneNumber}`);
    console.log(this.message);
  }
}

// Polymorphic usage
function sendNotification(notification: Notification): void {
  notification.send(); // Different behavior based on actual type!
}

const email = new EmailNotification("user@example.com", "Hello", "Greeting");
const sms = new SMSNotification("Bob", "Hi", "+1-555-0123");

sendNotification(email); // Sends email
sendNotification(sms); // Sends SMS
```

---

## 10. Best Practices

### ✅ Do This

1. **Always call super() first in constructor**

   ```typescript
   class Derived extends Base {
     constructor() {
       super(); // ✅ First statement!
       // Now can use 'this'
     }
   }
   ```

2. **Use protected for inherited members**

   ```typescript
   class Base {
     protected value: number; // ✅ Accessible to children
   }
   ```

3. **Follow "is-a" relationship**

   ```typescript
   class Dog extends Animal {} // ✅ Dog IS-A Animal
   ```

4. **Keep hierarchies shallow**

   ```typescript
   // ✅ Good: 2-3 levels
   Animal → Dog

   // ❌ Bad: Too deep
   A → B → C → D → E
   ```

5. **Use interfaces for multiple inheritance**
   ```typescript
   class Duck extends Animal implements Flyable, Swimmable {
     // ✅ One class, multiple interfaces
   }
   ```

### ❌ Don't Do This

1. **Forget to call super()**

   ```typescript
   class Derived extends Base {
     constructor() {
       // ❌ Error: must call super()
       this.value = 10;
     }
   }
   ```

2. **Use inheritance for "has-a"**

   ```typescript
   class Car extends Engine {} // ❌ Car HAS-A Engine, not IS-A!
   ```

3. **Deep hierarchies**
   ```typescript
   // ❌ Too many levels
   A → B → C → D → E → F
   ```

---

## 11. Inheritance vs Composition

| Aspect           | Inheritance       | Composition         |
| ---------------- | ----------------- | ------------------- |
| **Relationship** | "is-a"            | "has-a"             |
| **Coupling**     | Tight             | Loose               |
| **Flexibility**  | Compile-time      | Runtime             |
| **Reusability**  | Vertical          | Horizontal          |
| **Multiple**     | Single class only | Multiple components |

### Example

**Inheritance** (is-a):

```typescript
class Dog extends Animal {
  // Dog IS-A Animal
}
```

**Composition** (has-a):

```typescript
class Car {
  private engine: Engine; // Car HAS-A Engine
  private wheels: Wheels; // Car HAS-A Wheels

  constructor() {
    this.engine = new Engine();
    this.wheels = new Wheels();
  }
}
```

**Rule:** Prefer composition over inheritance unless there's a clear "is-a" relationship.

---

## 12. When to Use Inheritance

### Use Inheritance When ✅

- Clear **"is-a"** relationship (Dog is an Animal)
- Shared **behavior/data** across classes
- Want **code reuse** within same family
- Need **polymorphism**
- Hierarchy is **shallow** (2-3 levels)

### Avoid Inheritance When ❌

- Relationship is **"has-a"** (Car has an Engine)
- Need **multiple inheritance** of classes
- Want **runtime flexibility**
- Hierarchy would be **deep** (>3 levels)
- **Tight coupling** is a concern

---

## 13. Common Patterns

### 1. Template Method Pattern

```typescript
abstract class DataProcessor {
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
    console.log("Transforming CSV...");
  }
}
```

---

## 14. Comparison with Other Languages

| Feature                  | TypeScript              | C++                 | Java                    |
| ------------------------ | ----------------------- | ------------------- | ----------------------- |
| **Single inheritance**   | ✅ Yes                  | ✅ Yes              | ✅ Yes                  |
| **Multiple inheritance** | ❌ No (interfaces only) | ✅ Yes              | ❌ No (interfaces only) |
| **Super keyword**        | `super`                 | Base class name     | `super`                 |
| **Override indicator**   | Not needed              | `override` (C++11+) | `@Override`             |
| **Abstract classes**     | `abstract` keyword      | Pure virtual        | `abstract` keyword      |
| **Access modifiers**     | Compile-time only       | Runtime             | Runtime                 |

---

## 15. Summary Table

| Concept                 | Syntax                                | Purpose                 |
| ----------------------- | ------------------------------------- | ----------------------- |
| **Single inheritance**  | `class D extends B`                   | One parent              |
| **Multi-level**         | `A → B → C`                           | Chain                   |
| **Hierarchical**        | `B, C extends A`                      | Multiple children       |
| **Multiple interfaces** | `class D extends B implements I1, I2` | Multiple behaviors      |
| **Super constructor**   | `super(args)`                         | Call parent constructor |
| **Super method**        | `super.method()`                      | Call parent method      |
| **Abstract class**      | `abstract class A`                    | Can't instantiate       |
| **Override**            | Just redefine                         | Override parent method  |

---

## 16. Final Takeaways

> **Inheritance models "is-a" relationships and enables code reuse and polymorphism.**

### Core Principles

1. **Use for "is-a"** - Dog is an Animal
2. **Call super() first** - In constructor
3. **Prefer composition** - When in doubt
4. **Keep shallow** - 2-3 levels max
5. **Use interfaces** - For multiple inheritance

### Key Insight

> "TypeScript's single inheritance with multiple interface implementation provides a good balance: the simplicity of single inheritance with the flexibility of multiple behaviors."

**Benefits:**

- **Code Reuse**: Write once, use many
- **Polymorphism**: Runtime behavior
- **Hierarchy**: Logical organization
- **Type Safety**: Compile-time checking

**Drawbacks:**

- **Tight Coupling**: Child depends on parent
- **Single Inheritance**: Only one parent class
- **Fragile Base**: Changes ripple down
- **Inflexibility**: Fixed at compile time

---

## 17. References

- [TypeScript Handbook: Classes](https://www.typescriptlang.org/docs/handbook/2/classes.html)
- [TypeScript Deep Dive: Inheritance](https://basarat.gitbook.io/typescript/future-javascript/classes)
- [MDN: Inheritance in JavaScript](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Inheritance_and_the_prototype_chain)
- [AlgoMaster: Inheritance in LLD](https://algomaster.io/learn/lld/inheritance)
