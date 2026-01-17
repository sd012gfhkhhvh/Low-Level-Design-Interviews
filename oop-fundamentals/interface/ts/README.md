# Interface in TypeScript — A Complete Practical Guide

This guide explains interfaces in TypeScript from fundamentals to advanced patterns, including:

- Interface basics and syntax
- Multiple interface implementation
- Interface Segregation Principle
- Polymorphism and dependency injection
- Interface vs Type comparison
- Real-world design patterns
- TypeScript-specific features
- Comparison with C++ and Java

---

## 1. What Is an Interface?

An **interface** defines a contract—the shape that an object or class must conform to.

```typescript
interface Drawable {
    draw(): void;
}
```

In TypeScript, interfaces are:

- **Compile-time only** (erased at runtime)
- **Structural** (duck typing - if it looks like a duck...)
- **Extendable** (can inherit from other interfaces)
- **Free** (zero runtime cost)

### When to Use

- Define contracts for classes
- Ensure type safety across modules
- Enable polymorphism
- Document expected object shapes
- Implement design patterns

---

## 2. Basic Interface Implementation

```typescript
interface Drawable {
    draw(): void;
}

class Circle implements Drawable {
    draw(): void {
        console.log("Drawing a Circle ⭕️");
    }
}

class Rectangle implements Drawable {
    draw(): void {
        console.log("Drawing a Rectangle ▭");
    }
}
```

### Key Points

✅ **`implements` keyword** - Class explicitly declares it follows the contract

✅ **Compile-time checking** - TypeScript verifies all methods are implemented

✅ **No runtime overhead** - Interfaces disappear after compilation

---

## 3. Multiple Interface Implementation

TypeScript natively supports implementing multiple interfaces:

```typescript
interface Drawable {
    draw(): void;
}

interface Movable {
    move(x: number, y: number): void;
}

interface Resizable {
    resize(factor: number): void;
}

class Shape implements Drawable, Movable, Resizable {
    draw(): void {
        /* ... */
    }
    move(x: number, y: number): void {
        /* ... */
    }
    resize(factor: number): void {
        /* ... */
    }
}
```

### Advantages Over C++

- ✅ Cleaner syntax (no virtual destructors needed)
- ✅ No diamond problem concerns
- ✅ Better IDE support and autocomplete
- ✅ Structural typing (more flexible)

### Mental Model

> Interface = capability  
> Multiple interfaces = multiple capabilities  
> Mix and match as needed

---

## 4. Interface Segregation Principle (ISP)

**"Clients should not be forced to depend on methods they don't use"**

### ❌ Bad Design: Fat Interface

```typescript
interface Animal {
    walk(): void;
    fly(): void;
    swim(): void;
}

class Dog implements Animal {
    walk(): void {
        /* OK */
    }
    fly(): void {
        /* ??? Dogs don't fly! */
    }
    swim(): void {
        /* OK */
    }
}
```

**Problems:**

- Forces unnecessary implementations
- Confusing API
- Violates single responsibility
- Harder to test

### ✅ Good Design: Segregated Interfaces

```typescript
interface Walkable {
    walk(): void;
}

interface Flyable {
    fly(): void;
}

interface Swimmable {
    swim(): void;
}

class Dog implements Walkable, Swimmable {
    walk(): void {
        /* ... */
    }
    swim(): void {
        /* ... */
    }
}

class Duck implements Walkable, Flyable, Swimmable {
    walk(): void {
        /* ... */
    }
    fly(): void {
        /* ... */
    }
    swim(): void {
        /* ... */
    }
}
```

**Benefits:**

- Clear, focused contracts
- Compose exactly what's needed
- Easy to extend
- Better maintainability

---

## 5. Polymorphism with Interfaces

Interfaces enable **runtime polymorphism** through dependency inversion:

```typescript
function renderShapes(shapes: Drawable[]): void {
    for (const shape of shapes) {
        shape.draw(); // Polymorphic call
    }
}

const shapes: Drawable[] = [new Circle(), new Rectangle(), new Triangle()];
renderShapes(shapes);
```

### Benefits

✅ **Flexibility**: Add new shapes without changing `renderShapes()`  
✅ **Testability**: Mock implementations for testing  
✅ **Loose coupling**: Code depends on abstractions, not concrete types  
✅ **Open/Closed Principle**: Open for extension, closed for modification

---

## 6. Real-World Pattern: Strategy (Payment Gateway)

The **Strategy Pattern** lets you swap algorithms at runtime:

```typescript
interface PaymentGateway {
    initiatePayment(amount: number): void;
    getProviderName(): string;
}

class StripePayment implements PaymentGateway {
    initiatePayment(amount: number): void {
        console.log(`Processing via Stripe: $${amount}`);
    }
    getProviderName(): string {
        return "Stripe";
    }
}

class RazorpayPayment implements PaymentGateway {
    initiatePayment(amount: number): void {
        console.log(`Processing via Razorpay: ₹${amount}`);
    }
    getProviderName(): string {
        return "Razorpay";
    }
}

class CheckoutService {
    private gateway: PaymentGateway;

    constructor(gateway: PaymentGateway) {
        this.gateway = gateway;
    }

    processCheckout(amount: number): void {
        this.gateway.initiatePayment(amount);
    }

    setPaymentGateway(gateway: PaymentGateway): void {
        this.gateway = gateway;
    }
}
```

### Usage

```typescript
const checkout = new CheckoutService(new StripePayment());
checkout.processCheckout(99.99); // Uses Stripe

checkout.setPaymentGateway(new RazorpayPayment());
checkout.processCheckout(1500); // Uses Razorpay
```

### Benefits

- Swap providers at runtime
- Add new providers without changing `CheckoutService`
- Easy to mock for testing
- Follows SOLID principles

---

## 7. Interface Extension

TypeScript interfaces can **extend** other interfaces:

```typescript
interface Printable {
    print(): void;
}

interface Serializable {
    serialize(): string;
}

// Extend multiple interfaces
interface Exportable extends Printable, Serializable {
    export(): void;
}

class Document implements Exportable {
    print(): void {
        /* ... */
    }
    serialize(): string {
        /* ... */
    }
    export(): void {
        /* ... */
    }
}
```

### Why This Matters

- ✅ Compose interfaces hierarchically
- ✅ Reuse common contracts
- ✅ Build complex types from simple ones
- ✅ Better code organization

---

## 8. Interface vs Type Alias

TypeScript has both `interface` and `type`. When to use which?

### Interface

```typescript
interface User {
    name: string;
    age: number;
}
```

### Type Alias

```typescript
type User = {
    name: string;
    age: number;
};
```

### Comparison Table

| Feature                 | Interface         | Type             |
| ----------------------- | ----------------- | ---------------- |
| **Extend/Compose**      | `extends` keyword | `&` intersection |
| **Declaration merging** | ✅ Yes            | ❌ No            |
| **Primitives/Unions**   | ❌ No             | ✅ Yes           |
| **Performance**         | Slightly better   | Same             |
| **OOP patterns**        | ✅ Preferred      | Possible         |
| **React props**         | ✅ Common choice  | Also common      |

### Recommendation

> Use **`interface`** for object shapes and class contracts.  
> Use **`type`** for unions, primitives, mapped types, and complex transformations.

```typescript
// ✅ Good
interface ApiResponse {
    /* ... */
}

// ✅ Good
type Status = "pending" | "success" | "error";
```

---

## 9. Optional Properties and Readonly

TypeScript interfaces support modifiers:

```typescript
interface Config {
    readonly apiKey: string; // Cannot be modified
    timeout?: number; // Optional
    retries: number; // Required
}

const config: Config = {
    apiKey: "secret",
    retries: 3,
    // timeout is optional
};

// config.apiKey = "new"; // ❌ Error: readonly property
```

### Use Cases

- **`readonly`**: Immutable data, configuration
- **`?` (optional)**: Flexible APIs, partial updates
- **Required**: Critical fields that must exist

---

## 10. Function Interfaces

Interfaces can define function signatures:

```typescript
interface Validator {
    (value: string): boolean;
}

const emailValidator: Validator = (value) => {
    return value.includes("@");
};

const phoneValidator: Validator = (value) => {
    return /^\d{10}$/.test(value);
};
```

### Benefits

- Type-safe function contracts
- Interchangeable implementations
- Clear API documentation

---

## 11. Best Practices and Anti-Patterns

### ✅ Best Practices

1. **Prefer interfaces for OOP**

    ```typescript
    interface Repository {
        /* ... */
    } // ✅
    ```

2. **Use ISP for focused contracts**

    ```typescript
    interface Readable {
        read(): void;
    }
    interface Writable {
        write(): void;
    }
    ```

3. **Program to interfaces**

    ```typescript
    function process(repo: Repository) {
        /* ... */
    } // ✅ Flexible
    ```

4. **Use `readonly` for immutability**

    ```typescript
    interface User {
        readonly id: string;
    }
    ```

5. **Leverage structural typing**
    ```typescript
    // No need to explicitly implement if shape matches!
    const obj = {
        draw() {
            /* ... */
        },
    }; // Drawable!
    ```

### ❌ Common Pitfalls

1. **Fat interfaces**

    ```typescript
    interface GodObject {
        // ❌ Too many methods
        method1(): void;
        // ... 50 more methods
    }
    ```

2. **Mixing data and behavior**

    ```typescript
    interface User {
        name: string;
        save(): void; // ❌ Mix of data and behavior
    }
    ```

3. **Overusing `any`**

    ```typescript
    interface BadApi {
        data: any; // ❌ Loses type safety
    }
    ```

4. **Not using optional/required correctly**
    ```typescript
    interface Config {
        apiKey: string; // Required?
        debug: boolean; // Required?
    }
    // Better: make debug optional with default
    ```

---

## 12. Comparison with Other Languages

| Feature                     | TypeScript     | C++                  | Java                     |
| --------------------------- | -------------- | -------------------- | ------------------------ |
| **Keyword**                 | `interface`    | Pure abstract class  | `interface`              |
| **Runtime presence**        | ❌ No (erased) | ✅ Yes (vtable)      | ✅ Yes                   |
| **Multiple implementation** | ✅ Yes         | ✅ Yes               | ✅ Yes (interfaces only) |
| **Extension**               | `extends`      | Multiple inheritance | `extends`                |
| **Type system**             | Structural     | Nominal              | Nominal                  |
| **Default implementations** | ❌ No          | ❌ No (pure)         | ✅ Yes (Java 8+)         |
| **Optional members**        | ✅ Yes (`?`)   | ❌ No                | ❌ No                    |
| **Readonly**                | ✅ Yes         | ✅ Yes (`const`)     | ✅ Yes (`final`)         |

### TypeScript Unique Strengths

- **Zero runtime cost** (compile-time only)
- **Structural typing** (duck typing)
- **Optional properties** built-in
- **Better type inference**
- **Declaration merging**

---

## 13. Summary Table

| Concept             | Purpose                | Syntax                            |
| ------------------- | ---------------------- | --------------------------------- |
| **Basic interface** | Define contract        | `interface I { method(): void; }` |
| **Implements**      | Class follows contract | `class C implements I`            |
| **Multiple**        | Compose capabilities   | `class C implements I1, I2`       |
| **Extends**         | Interface inheritance  | `interface I2 extends I1`         |
| **Optional**        | Flexible properties    | `property?: type`                 |
| **Readonly**        | Immutable properties   | `readonly property: type`         |
| **Function**        | Function signature     | `interface F { (x: T): R; }`      |

---

## 14. Final Takeaways

> **Interfaces define contracts, not implementations**

1. **Use interfaces** for object shapes and class contracts
2. **Apply ISP** for maintainable, focused APIs
3. **Leverage structural typing** - TypeScript's superpower
4. **Prefer interfaces over types** for OOP patterns
5. **Zero runtime cost** - interfaces are free

### Core Insight

> TypeScript interfaces provide compile-time safety with zero runtime overhead—the best of both worlds for building scalable applications.

When used well, interfaces make your code:

- More type-safe
- Easier to refactor
- Better documented
- Simpler to test
- More maintainable

---

## 15. References

- [TypeScript Handbook: Interfaces](https://www.typescriptlang.org/docs/handbook/interfaces.html)
- [TypeScript Deep Dive: Interfaces](https://basarat.gitbook.io/typescript/type-system/interfaces)
- [MDN: Object-Oriented JavaScript](https://developer.mozilla.org/en-US/docs/Learn/JavaScript/Objects/Object-oriented_programming)
- [Interface Segregation Principle](https://en.wikipedia.org/wiki/Interface_segregation_principle)
