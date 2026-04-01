# Polymorphism in TypeScript — A Complete Practical Guide

This guide explains polymorphism in TypeScript from fundamentals to advanced patterns, including:

- What is polymorphism and why it matters
- Compile-time polymorphism (Function Overloading, Generics)
- Runtime polymorphism (Method Overriding, Interfaces)
- Abstract classes vs interfaces
- Duck typing (Structural polymorphism)
- Real-world examples and best practices

---

## 1. What Is Polymorphism?

**Polymorphism** (from Greek: "many forms") allows the same interface to work with different underlying forms (data types).

### The Formula

> **Polymorphism = Same Interface + Different Behaviors**

### Mental Model

Think of a **universal charger**:

- One charging port (same interface)
- Works with phone, tablet, laptop (different devices)
- Same action, different implementations

In TypeScript:

```typescript
const shape: Shape = new Circle(); // Circle treated as Shape
shape.draw(); // Calls Circle's draw() at runtime!
```

---

## 2. Why Polymorphism Matters

### Benefits

1. **Code Reusability**: Write generic code once, works with many types
2. **Flexibility**: Add new types without changing existing code
3. **Loose Coupling**: Depend on abstractions, not concrete types
4. **Extensibility**: Easily plug in new implementations

### Example: Without vs With Polymorphism

**Without Polymorphism** ❌:

```typescript
function drawCircle(c: Circle) {
  c.draw();
}
function drawRectangle(r: Rectangle) {
  r.draw();
}
function drawTriangle(t: Triangle) {
  t.draw();
}

// Need new function for every shape!
```

**With Polymorphism** ✅:

```typescript
function draw(shape: Shape) {
  shape.draw(); // Works with ANY shape!
}

// One function handles all current and future shapes
```

---

## 3. Types of Polymorphism in TypeScript

| Type             | Resolved At  | Mechanism                | Example                    |
| ---------------- | ------------ | ------------------------ | -------------------------- |
| **Compile-time** | Compile time | Function overloading     | Overload signatures        |
| **Compile-time** | Compile time | Generics                 | `<T>` type parameters      |
| **Runtime**      | Runtime      | Method overriding        | Child overrides parent     |
| **Runtime**      | Runtime      | Interface implementation | Class implements interface |
| **Structural**   | Compile time | Duck typing              | Shape-based matching       |

---

## 4. Compile-Time Polymorphism: Function Overloading

TypeScript supports function overloading through **overload signatures**, but requires a single implementation.

```typescript
class Calculator {
  // Overload signatures (compile-time)
  add(a: number, b: number): number;
  add(a: string, b: string): string;
  add(a: number, b: number, c: number): number;

  // Implementation (must handle all overloads)
  add(a: number | string, b: number | string, c?: number): number | string {
    if (typeof a === "number" && typeof b === "number") {
      if (c !== undefined) {
        return a + b + c;
      }
      return a + b;
    }
    return String(a) + String(b);
  }
}

const calc = new Calculator();
calc.add(2, 3); // Calls number overload
calc.add(2, 3, 4); // Calls three-number overload
calc.add("Hello", " World"); // Calls string overload
```

### Limitations

⚠️ Unlike C++, TypeScript has:

- Only **one implementation** (not multiple distinct functions)
- **Runtime type checking** needed in implementation
- **Less flexibility** than true overloading

---

## 5. Compile-Time Polymorphism: Generics

**Generics** enable writing type-safe, reusable code that works with any type.

```typescript
class Stack<T> {
  private items: T[] = [];

  push(item: T): void {
    this.items.push(item);
  }

  pop(): T | undefined {
    return this.items.pop();
  }

  peek(): T | undefined {
    return this.items[this.items.length - 1];
  }
}

const numberStack = new Stack<number>();
numberStack.push(1);
numberStack.push(2);
// numberStack.push("hello"); // ❌ Error: Type 'string' not assignable to 'number'

const stringStack = new Stack<string>();
stringStack.push("hello");
stringStack.push("world");
```

---

## 6. Runtime Polymorphism: Method Overriding

Child classes can **override** parent methods to provide specialized behavior.

```typescript
abstract class Shape {
  protected color: string;

  constructor(color: string) {
    this.color = color;
  }

  abstract getArea(): number; // Must implement

  draw(): void {
    // Can override
    console.log(`Drawing shape: ${this.color}`);
  }
}

class Circle extends Shape {
  constructor(
    color: string,
    private radius: number,
  ) {
    super(color);
  }

  getArea(): number {
    return Math.PI * this.radius ** 2;
  }

  // Override parent method
  draw(): void {
    console.log(`⭕ Drawing Circle | Area: ${this.getArea().toFixed(2)}`);
  }
}

class Rectangle extends Shape {
  constructor(
    color: string,
    private width: number,
    private height: number,
  ) {
    super(color);
  }

  getArea(): number {
    return this.width * this.height;
  }

  draw(): void {
    console.log(`▭ Drawing Rectangle | Area: ${this.getArea()}`);
  }
}

// Polymorphic behavior
const shapes: Shape[] = [new Circle("Red", 5), new Rectangle("Blue", 10, 20)];

shapes.forEach((shape) => shape.draw()); // Calls correct version!
// Output:
// ⭕ Drawing Circle | Area: 78.54
// ▭ Drawing Rectangle | Area: 200
```

### Key Points

- No `virtual` keyword needed (unlike C++)
- Child methods automatically override parent with same signature
- Use `super` to call parent version
- Type system ensures correctness

---

## 7. Interface-Based Polymorphism

**Interfaces** define contracts that classes must fulfill.

```typescript
interface Printable {
  print(): void;
}

interface Saveable {
  save(filename: string): void;
}

class Document implements Printable, Saveable {
  constructor(
    private title: string,
    private content: string,
  ) {}

  print(): void {
    console.log(`=== ${this.title} ===`);
    console.log(this.content);
  }

  save(filename: string): void {
    console.log(`💾 Saving to ${filename}`);
  }
}

class Image implements Printable {
  constructor(private name: string) {}

  print(): void {
    console.log(`🖼️  Printing image: ${this.name}`);
  }
}

// Polymorphic usage
const printables: Printable[] = [
  new Document("Report", "Content"),
  new Image("photo.jpg"),
];

printables.forEach((p) => p.print());
```

---

## 8. Abstract Classes

**Abstract classes** provide both interface and partial implementation.

```typescript
abstract class Animal {
  protected name: string;

  constructor(name: string) {
    this.name = name;
  }

  // Abstract method - must implement
  abstract makeSound(): void;

  // Concrete method - shared implementation
  sleep(): void {
    console.log(`${this.name} is sleeping 💤`);
  }
}

class Dog extends Animal {
  makeSound(): void {
    console.log(`${this.name} says: Woof! 🐕`);
  }
}

class Cat extends Animal {
  makeSound(): void {
    console.log(`${this.name} says: Meow! 🐱`);
  }
}

// const animal = new Animal("Generic");  // ❌ Error: cannot instantiate
const dog = new Dog("Buddy"); // ✅ OK
const animals: Animal[] = [dog, new Cat("Whiskers")];

animals.forEach((animal) => {
  animal.makeSound(); // Polymorphic!
  animal.sleep(); // Shared implementation
});
```

---

## 9. Abstract Class vs Interface

| Aspect                   | Abstract Class            | Interface         |
| ------------------------ | ------------------------- | ----------------- |
| **Implementation**       | Can have concrete methods | No implementation |
| **Properties**           | Can have properties       | Only declarations |
| **Constructor**          | Can have constructor      | Cannot            |
| **Multiple inheritance** | ❌ Single only            | ✅ Multiple       |
| **Access modifiers**     | ✅ Can use                | ❌ Public only    |
| **Use case**             | Shared behavior           | Pure contract     |

### When to Use Each

**Use Abstract Class:**

- Share **state** (properties)
- Share **implementation** (methods)
- Want **default behavior**

**Use Interface:**

- Pure **contract**
- **Multiple** implementations
- **Unrelated** classes share behavior

---

## 10. Duck Typing (Structural Polymorphism)

TypeScript uses **structural typing**: if ithas the right shape, it works!

```typescript
interface Flyable {
  fly(): void;
}

class Bird {
  fly(): void {
    console.log("Bird flying! 🐦");
  }
}

class Airplane {
  fly(): void {
    console.log("Airplane flying! ✈️");
  }
}

// No explicit "implements Flyable" needed!
function makeFly(thing: Flyable): void {
  thing.fly();
}

const bird = new Bird();
const plane = new Airplane();

makeFly(bird); // ✅ Works! Bird has fly()
makeFly(plane); // ✅ Works! Airplane has fly()

// Even plain objects work!
makeFly({ fly: () => console.log("Object flying!") }); // ✅ Works!
```

**This is unique to TypeScript/JavaScript!** C++ and Java use nominal typing.

---

## 11. Real-World Example: Payment System

```typescript
interface PaymentMethod {
  processPayment(amount: number): boolean;
  getType(): string;
}

class CreditCardPayment implements PaymentMethod {
  constructor(
    private accountId: string,
    private cardNumber: string,
  ) {}

  processPayment(amount: number): boolean {
    console.log(`💳 Processing $${amount} via Credit Card`);
    console.log(`   Card: ${this.cardNumber.substring(0, 4)}****`);
    return true;
  }

  getType(): string {
    return "Credit Card";
  }
}

class PayPalPayment implements PaymentMethod {
  constructor(
    private accountId: string,
    private email: string,
  ) {}

  processPayment(amount: number): boolean {
    console.log(`💰 Processing $${amount} via PayPal`);
    console.log(`   Email: ${this.email}`);
    return true;
  }

  getType(): string {
    return "PayPal";
  }
}

// Polymorphic function
function processTransaction(method: PaymentMethod, amount: number): void {
  console.log(`Payment Type: ${method.getType()}`);
  method.processPayment(amount);
}

const creditCard = new CreditCardPayment("ACC001", "1234567890123456");
const paypal = new PayPalPayment("ACC002", "user@example.com");

processTransaction(creditCard, 100); // Works with CreditCard
processTransaction(paypal, 50); // Works with PayPal
```

---

## 12. Super Keyword & Method Overriding

Use `super` to call parent methods when overriding:

```typescript
class Employee {
  constructor(
    protected name: string,
    protected salary: number,
  ) {}

  work(): void {
    console.log(`${this.name} is working...`);
  }

  getInfo(): string {
    return `${this.name} earns $${this.salary}`;
  }
}

class Manager extends Employee {
  constructor(
    name: string,
    salary: number,
    private teamSize: number,
  ) {
    super(name, salary);
  }

  work(): void {
    super.work(); // Call parent method
    console.log(`Managing ${this.teamSize} people`);
  }

  getInfo(): string {
    return super.getInfo() + ` and manages ${this.teamSize} people`;
  }
}

const mgr = new Manager("Alice", 120000, 5);
mgr.work();
// Output:
// Alice is working...
// Managing 5 people

console.log(mgr.getInfo());
// Output: Alice earns $120000 and manages 5 people
```

---

## 13. Best Practices

### ✅ Do This

1. **Use interfaces for contracts**

   ```typescript
   interface PaymentMethod {
     processPayment(amount: number): boolean;
   }
   ```

2. **Use abstract classes for shared behavior**

   ```typescript
   abstract class Animal {
     abstract makeSound(): void;
     sleep(): void {
       /* shared */
     }
   }
   ```

3. **Leverage duck typing**

   ```typescript
   function log(logger: { log(msg: string): void }): void {
     logger.log("message"); // Works with ANY object with log()
   }
   ```

4. **Use generics for type safety**

   ```typescript
   function identity<T>(value: T): T {
     return value;
   }
   ```

5. **Program to abstractions**
   ```typescript
   function process(shape: Shape): void {
     // Not Circle or Rectangle!
     shape.draw();
   }
   ```

### ❌ Don't Do This

1. **Overuse `any` type**

   ```typescript
   function process(item: any) {
     // ❌ Loses type safety!
     item.doSomething();
   }
   ```

2. **Ignore interface contracts**

   ```typescript
   class Bad implements Printable {
     // ❌ Error: must implement print()
   }
   ```

3. **Create deep hierarchies**
   ```typescript
   // ❌ Too deep
   A → B → C → D → E
   ```

---

## 14. Comparison with Other Languages

| Feature                  | TypeScript            | C++                   | Java               |
| ------------------------ | --------------------- | --------------------- | ------------------ |
| **Function overloading** | Overload signatures   | True overloading      | True overloading   |
| **Operator overloading** | ❌ No                 | ✅ Yes                | ❌ No              |
| **Method overriding**    | ✅ Yes (automatic)    | `virtual`, `override` | `@Override`        |
| **Abstract classes**     | ✅ `abstract` keyword | Pure virtual          | `abstract` keyword |
| **Interfaces**           | ✅ Native             | Pure abstract class   | ✅ Native          |
| **Duck typing**          | ✅ Yes (structural)   | ❌ No                 | ❌ No              |
| **Generics**             | ✅ Yes                | Templates             | ✅ Yes             |
| **Runtime overhead**     | None (compiles to JS) | vtable                | vtable             |

---

## 15. TypeScript-Specific Features

### 1. Structural Typing

```typescript
interface Point {
  x: number;
  y: number;
}

class Position {
  constructor(
    public x: number,
    public y: number,
  ) {}
}

function printPoint(p: Point): void {
  console.log(`(${p.x}, ${p.y})`);
}

printPoint(new Position(3, 4)); // ✅ Works! Same shape
printPoint({ x: 1, y: 2 }); // ✅ Works! Plain object
```

### 2. Union Types

```typescript
type Result = Success | Error;

class Success {
  constructor(public value: any) {}
}

class Error {
  constructor(public message: string) {}
}

function handleResult(result: Result): void {
  if (result instanceof Success) {
    console.log(`Success: ${result.value}`);
  } else {
    console.log(`Error: ${result.message}`);
  }
}
```

### 3. Type Guards

```typescript
function isCircle(shape: Shape): shape is Circle {
  return shape instanceof Circle;
}

const shape: Shape = new Circle("Red", 5);
if (isCircle(shape)) {
  // TypeScript knows shape is Circle here
  console.log(shape.getArea());
}
```

---

## 16. Summary Table

| Concept               | Syntax              | Purpose                     |
| --------------------- | ------------------- | --------------------------- |
| **Function overload** | Multiple signatures | Same name, different params |
| **Generics**          | `class Stack<T>`    | Type-safe reusable code     |
| **Method override**   | Just redefine       | Specialized behavior        |
| **Abstract method**   | `abstract method()` | Must implement              |
| **Interface**         | `interface Name`    | Define contract             |
| **Duck typing**       | Shape-based         | If it fits, it works        |

---

## 17. Final Takeaways

> **Polymorphism enables writing flexible code that works with many types while maintaining type safety.**

### Core Principles

1. **Function overloading** - Limited but useful
2. **Generics** - Type-safe polymorphism
3. **Method overriding** - Runtime behavior
4. **Interfaces** - Define contracts
5. **Duck typing** - Structural flexibility

### Key Insight

> "TypeScript combines the best of both worlds: compile-time type safety with runtime flexibility through duck typing."

**Benefits:**

- **Type Safety**: Catch errors at compile time
- **Flexibility**: Duck typing allows structural polymorphism
- **Reusability**: Generics enable code reuse
- **Maintainability**: Clear contracts through interfaces

**Trade-offs:**

- **No operator overloading**: Unlike C++
- **Limited overloading**: Single implementation
- **Compile-time only**: No runtime type information (erased)

---

## 18. References

- [TypeScript Handbook: Classes](https://www.typescriptlang.org/docs/handbook/2/classes.html)
- [TypeScript Handbook: Generics](https://www.typescriptlang.org/docs/handbook/2/generics.html)
- [TypeScript Deep Dive: Polymorphism](https://basarat.gitbook.io/typescript/)
- [AlgoMaster: Polymorphism in LLD](https://algomaster.io/learn/lld/polymorphism)
