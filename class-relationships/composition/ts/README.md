# Composition in TypeScript — A Complete Practical Guide

This guide breaks down **Composition** in TypeScript from fundamental concepts to implementation patterns, highlighting how it logically structures strictly-owned hierarchies.

- What is composition and how it fundamentally enforces strict ownership
- The concept of dependent lifecycles and lifecycle control in a GC language
- Implementing composition correctly without breaking encapsulation
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
- The Rooms cannot logically exist outside the House.
- A Room belongs to exactly one House.
- If a wrecking ball destroys the House, the Rooms vanish with it.

In TypeScript code:

```typescript
class House {
  private rooms: Room[] = [];

  constructor() {
    // The house creates its own parts internally!
    this.rooms.push(new Room("Kitchen"));
  }
}
```

---

## 2. Why Composition Matters

### Benefits

1. **Information Hiding**: The parent completely encapsulates the children. The outside code only talks to the parent, unaware of the complex child mechanisms.
2. **Simplified Memory/State**: Because lifecycles are joined, you don't worry about rogue objects floating around independently. When the parent is garbage-collected, children are immediately collected.
3. **Flexibility**: Composition is heavily favored in design ("Prefer Composition over Inheritance") because it allows swapping out behavior dynamically.

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

## 4. Implementation: Lifecycle Binding

In languages like C++, Composition relies heavily on explicit destructors. In TypeScript, object destruction is handled by the **Garbage Collector (GC)**.

Therefore, in TypeScript, **Composition is enforced by how objects are instantiated and referenced.** You enforce it by making sure the Parent exclusively instantiates the Child, and that no external code gets a reference to the Child.

```typescript
class Engine {
  start(): void {
    console.log("Engine started.");
  }
}

class Car {
  // Composition! Exclusively owned.
  private engine: Engine;

  constructor() {
    // 1. Created internally!
    this.engine = new Engine();
  }

  drive(): void {
    // 2. Used internally!
    this.engine.start();
  }
}

// When `myCar` goes out of scope and is Garbage Collected,
// its unique `Engine` reference is lost,
// causing the Engine to be Garbage Collected simultaneously!
```

---

## 5. Composition via Factory Methods & Real-World Examples

### Example A: Document and Pages

Often you don't instantiate everything directly in the constructor. Instead, you use methods in the "whole" that instantiate the "parts" dynamically.

```typescript
class Page {
  constructor(public text: string) {}
}

class Document {
  private title: string;
  private pages: Page[] = []; // Strict ownership

  constructor(title: string) {
    this.title = title;
  }

  // The document acts as a gatekeeper/factory for parts
  addPage(text: string): void {
    const page = new Page(text); // Internally instantiated
    this.pages.push(page);
  }
}

const doc = new Document("My Essay");
// We pass data, not instances!
doc.addPage("Introduction...");
```

### Example B: Order and Line Items

An e-commerce order strictly owns its line items. They cannot exist independently of the transaction.

```typescript
class LineItem {
  constructor(
    public product: string,
    public price: number,
  ) {}
}

class Order {
  // Strict ownership by reference trapping
  private items: LineItem[] = [];

  constructor(public orderId: string) {}

  addItem(product: string, price: number): void {
    // The Container instantiates the item internally
    this.items.push(new LineItem(product, price));
  }
}
```

---

## 6. Composition Over Inheritance

A major OOP design principle is **"Prefer Composition over Inheritance"**.

Inheritance creates a very rigid `is-a` coupling. Subclasses inherit everything, even properties they don't need, which leads to fragile base classes.
Composition (`has-a / uses-a-component`) is incredibly flexible.

**Bad (Inheritance):**

```typescript
class Bird {
  fly(): void {}
}
class Ostrich extends Bird {
  // Wait, Ostriches don't fly! We have to override and throw exceptions to fix this.
}
```

**Good (Composition):**

```typescript
interface FlyBehavior {
  execute(): void;
}
class NoFly implements FlyBehavior {
  execute(): void {}
}
class WingsFly implements FlyBehavior {
  execute(): void {
    /* flap */
  }
}

class Bird {
  // We compose the Bird containing a behavior module
  constructor(private flyBehavior: FlyBehavior) {}

  performFly(): void {
    this.flyBehavior.execute();
  }
}

// Highly flexible!
const ostrich = new Bird(new NoFly());
const eagle = new Bird(new WingsFly());
```

_(Note, injecting behaviors like this blurs the line slightly towards Aggregation, but when structural logic binds them permanently to the overarching instance, it represents Composition of Behaviors)._

---

## 7. Best Practices and Common Anti-Patterns

### ✅ Do This

1. **Instantiate Parts Internally:** The "Whole" class should be responsible for calling `new Part()` internally. This asserts tight lifecycle control.
2. **Use `private` variables:** Protect your components `private engine: Engine;`. Exposing them to the outside breaches exclusive ownership concepts.
3. **Pass Raw Data, Not Objects:** For methods that add parts (e.g., `addRoom(name: string)`), pass primitive parameters so the Container can create the object itself rather than taking an object pointer.

### ❌ Don't Do This

1. **Do Not Inject Built Parts for Structural Composition**:
   ```typescript
   // ❌ BAD! This is Dependency Injection / Aggregation.
   // Car doesn't control the lifecycle if you pass the pointer from outside!
   constructor(externalEngine: Engine) {
     this.engine = externalEngine;
   }
   ```
2. **Avoid Returning Internal References**:
   ```typescript
   // ❌ BAD! Leaking references destroys exclusive ownership.
   // An external variable can grab this and keep it alive after the parent dies!
   getEngine(): Engine { return this.engine; }
   ```

---

## 8. Summary Table & Key Comparison

| Feature           | Aggregation                         | Composition                       |
| ----------------- | ----------------------------------- | --------------------------------- |
| **Symbol (UML)**  | Hollow Diamond `<>---`              | Solid Diamond `♦---`              |
| **Relationship**  | "has-a"                             | "part-of"                         |
| **Ownership**     | Weak / Shared                       | Strict / Exclusive                |
| **Lifecycle**     | Independent                         | Parent controls child             |
| **Sharing**       | Parts can belong to multiple wholes | Parts belong to 1 whole           |
| **Instantiation** | Passed in from outside (Injection)  | Created internally (`new` inside) |

---

## 9. Final Takeaways

> **Composition is the safest, tightest relationship in OOP. When you compose objects, they live, operate, and perish as one unified entity.**

- Lean heavily on Composition. It simplifies state management and guarantees components aren't accidentally manipulated globally.
- In TS, simply using `private child = new Child()` perfectly establishes the compositional relationship, and naturally integrates with Garbage Collection.

### 📚 References

- [AlgoMaster: Composition in LLD](https://algomaster.io/learn/lld/composition)
- [Visual Paradigm: Aggregation vs Composition](https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/)
- [TypeScript Handbook: Classes](https://www.typescriptlang.org/docs/handbook/2/classes.html)
