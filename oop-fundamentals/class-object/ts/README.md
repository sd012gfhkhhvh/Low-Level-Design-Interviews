# Classes and Objects in TypeScript — A Complete Practical Guide

This guide explains classes and objects in TypeScript from fundamentals to advanced patterns, including:

- Class definition and object instantiation
- Constructors and parameter properties
- The `this` keyword and binding
- Access modifiers (including `readonly`)
- Static members
- Getters and setters
- Private fields (`#` syntax)
- TypeScript-specific features
- Real-world design patterns

---

## 1. What Are Classes and Objects?

A **class** is a blueprint that defines the structure and behavior of objects. It encapsulates data (properties) and functions (methods).

An **object** is an instance of a class—a concrete entity created from the class blueprint.

```typescript
class Car {
    brand: string;
    speed: number;
}

const myCar = new Car(); // Object: Instance of Car
```

### TypeScript Context

TypeScript classes are syntactic sugar over JavaScript's prototype-based inheritance, with added **type safety** at compile time.

```typescript
// TypeScript adds type annotations
class Car {
    private brand: string; // Type-safe!
    private speed: number = 0;
}
```

---

## 2. Basic Class Definition

```typescript
class BankAccount {
    // Properties with type annotations
    private accountNumber: string;
    private balance: number;

    // Constructor
    constructor(accNum: string, initialBalance: number) {
        this.accountNumber = accNum;
        this.balance = initialBalance;
    }

    // Method
    deposit(amount: number): void {
        this.balance += amount;
    }

    getBalance(): number {
        return this.balance;
    }
}
```

### Access Modifiers

TypeScript provides three access modifiers:

- **`public`**: Accessible from anywhere (default)
- **`private`**: Accessible only within the class
- **`protected`**: Accessible within class and subclasses

---

## 3. Constructors and Parameter Properties

### Standard Constructor

```typescript
class Car {
    private brand: string;
    private model: string;
    private speed: number;

    constructor(brand: string, model: string) {
        this.brand = brand;
        this.model = model;
        this.speed = 0;
    }
}
```

### Parameter Properties (TypeScript Magic! ✨)

TypeScript allows you to **declare and initialize** properties directly in the constructor:

```typescript
class Car {
    // Declares properties AND assigns them automatically!
    constructor(
        private brand: string,
        private model: string,
        private speed: number = 0, // Default value
    ) {
        // No need for this.brand = brand!
    }
}
```

**Benefits:**

- ✅ Less boilerplate
- ✅ More concise
- ✅ Cleaner code

**When to use:**

- Simple classes with straightforward initialization
- When you don't need constructor logic beyond assignment

---

## 4. The `this` Keyword

In TypeScript/JavaScript, `this` refers to the current object, but its value depends on **how the function is called**.

### Regular Methods

```typescript
class Rectangle {
    constructor(
        private width: number,
        private height: number,
    ) {}

    area(): number {
        return this.width * this.height; // 'this' = current object
    }
}
```

### Arrow Functions (Lexical `this`)

Arrow functions capture `this` from the surrounding context:

```typescript
class Button {
    label: string = "Click me";

    // Regular function - 'this' can change
    handleClick() {
        console.log(this.label);
    }

    // Arrow function - 'this' is always the Button instance
    handleClickArrow = () => {
        console.log(this.label);
    };
}
```

### Method Chaining

Return `this` for fluent interfaces:

```typescript
class QueryBuilder {
    private query: string = "";

    select(cols: string): QueryBuilder {
        this.query += `SELECT ${cols} `;
        return this; // Return current object
    }

    from(table: string): QueryBuilder {
        this.query += `FROM ${table} `;
        return this;
    }

    build(): string {
        return this.query;
    }
}

// Usage: method chaining!
const sql = new QueryBuilder().select("*").from("users").build();
```

---

## 5. Access Modifiers and `readonly`

### Access Modifiers

```typescript
class Account {
    public name: string; // Accessible anywhere
    private balance: number; // Only within class
    protected type: string; // Within class and subclasses

    constructor(name: string, balance: number) {
        this.name = name;
        this.balance = balance;
        this.type = "Savings";
    }
}
```

### `readonly` Modifier (TypeScript-Specific)

```typescript
class User {
    readonly id: string; // Cannot be modified after init
    readonly createdAt: Date;

    constructor(id: string) {
        this.id = id;
        this.createdAt = new Date();
    }

    updateId(newId: string): void {
        // this.id = newId;  // ❌ Error: cannot assign to readonly
    }
}
```

**Use cases for `readonly`:**

- IDs that shouldn't change
- Timestamps
- Configuration values

---

## 6. Static Members

Static members belong to the **class itself**, not individual objects.

```typescript
class Counter {
    private static count: number = 0; // Shared across all instances
    private id: number;

    constructor() {
        Counter.count++;
        this.id = Counter.count;
    }

    static getCount(): number {
        return Counter.count;
    }

    displayId(): void {
        console.log(`ID: ${this.id}`);
    }
}

// Usage
const c1 = new Counter();
const c2 = new Counter();
console.log(Counter.getCount()); // 2 (shared count)
```

### Static vs Instance

| Type         | Accessed via | Example              |
| ------------ | ------------ | -------------------- |
| **Static**   | Class name   | `Counter.getCount()` |
| **Instance** | Object       | `c1.displayId()`     |

---

## 7. Getters and Setters

TypeScript supports `get`/`set` accessors for computed or validated properties:

```typescript
class Person {
    constructor(
        private firstName: string,
        private lastName: string,
        private _age: number = 0,
    ) {}

    // Getter - accessed like a property
    get fullName(): string {
        return `${this.firstName} ${this.lastName}`;
    }

    // Setter - set like a property
    set fullName(value: string) {
        const [first, last] = value.split(" ");
        this.firstName = first;
        this.lastName = last;
    }

    get age(): number {
        return this._age;
    }

    // Setter with validation
    set age(value: number) {
        if (value >= 0 && value <= 150) {
            this._age = value;
        }
    }
}

// Usage
const person = new Person("John", "Doe");
console.log(person.fullName); // "John Doe" (getter)
person.fullName = "Jane Smith"; // (setter)
person.age = 25;
```

**Benefits:**

- ✅ Property-like syntax
- ✅ Validation logic
- ✅ Computed properties
- ✅ Backward compatibility

---

## 8. Private Fields (`#` Syntax) - ES2022+

JavaScript now has **true private fields** with `#` syntax:

```typescript
class Account {
    #balance: number; // True private (runtime enforced)

    constructor(initial: number) {
        this.#balance = initial;
    }

    deposit(amount: number): void {
        this.#balance += amount;
    }

    getBalance(): number {
        return this.#balance;
    }
}

const acc = new Account(1000);
// console.log(acc.#balance);  // ❌ Error: truly private!
```

### TypeScript `private` vs JavaScript `#`

| Feature               | TypeScript `private` | JavaScript `#` |
| --------------------- | -------------------- | -------------- |
| **Enforcement**       | Compile-time only    | Runtime        |
| **JavaScript output** | Becomes public       | Stays private  |
| **Truly hidden**      | ❌ No                | ✅ Yes         |
| **Syntax**            | `private x: number`  | `#x: number`   |

**Recommendation:**

- Use TypeScript `private` for most cases (type safety)
- Use `#` when you need true runtime privacy

---

## 9. Object Lifecycle

### Object Creation

```typescript
class Car {
    constructor(private brand: string) {
        console.log(`Car created: ${brand}`);
    }
}

const car = new Car("Toyota"); // Constructor called
```

### No Destructors

TypeScript/JavaScript uses **garbage collection**—no explicit destructors like C++.

However, you can provide cleanup methods:

```typescript
class Database {
    private connection: any;

    connect(): void {
        this.connection = /* ... */;
    }

    // Cleanup method (call manually)
    dispose(): void {
        if (this.connection) {
            this.connection.close();
            console.log("Connection closed");
        }
    }
}

// Usage
const db = new Database();
db.connect();
// ... use database ...
db.dispose();  // Manual cleanup
```

---

## 10. Best Practices and Anti-Patterns

### ✅ Best Practices

1. **Use parameter properties for simple classes**

    ```typescript
    constructor(private name: string, private age: number) {}  // ✅
    ```

2. **Make properties private/protected by default**

    ```typescript
    class Good {
        private value: number; // ✅ Encapsulated
    }
    ```

3. **Use `readonly` for immutable data**

    ```typescript
    readonly id: string;  // ✅ Cannot change
    ```

4. **Use getters/setters for validation**

    ```typescript
    set age(value: number) {
        if (value >= 0) this._age = value;  // ✅ Validated
    }
    ```

5. **Use arrow functions for callbacks**
    ```typescript
    handleClick = () => {
        /* 'this' is bound */
    }; // ✅
    ```

### ❌ Common Pitfalls

1. **Forgetting type annotations**

    ```typescript
    class Bad {
        value; // ❌ Type is 'any'
    }
    ```

2. **Public properties without validation**

    ```typescript
    class Bad {
        balance: number; // ❌ No encapsulation
    }
    ```

3. **Not using parameter properties**

    ```typescript
    class Verbose {
        private x: number;
        constructor(x: number) {
            this.x = x; // ❌ Verbose
        }
    }
    ```

4. **Misunderstanding `this` in callbacks**
    ```typescript
    setTimeout(this.method, 1000); // ❌ 'this' is undefined
    setTimeout(() => this.method(), 1000); // ✅
    ```

---

## 11. TypeScript-Specific Features Summary

| Feature                  | Syntax                           | Benefit             |
| ------------------------ | -------------------------------- | ------------------- |
| **Parameter properties** | `constructor(private x: number)` | Less boilerplate    |
| **Type annotations**     | `name: string`                   | Type safety         |
| **`readonly`**           | `readonly id: string`            | Immutability        |
| **Optional properties**  | `middleName?: string`            | Flexible APIs       |
| **Access modifiers**     | `private`, `protected`, `public` | Encapsulation       |
| **Getters/setters**      | `get name()`                     | Computed properties |
| **`#` private fields**   | `#balance: number`               | Runtime privacy     |

---

## 12. Real-World Examples

### Example 1: Fluent API with Method Chaining

```typescript
class HttpRequest {
    private url: string = "";
    private headers: Record<string, string> = {};
    private body: any;

    setUrl(url: string): HttpRequest {
        this.url = url;
        return this;
    }

    setHeader(key: string, value: string): HttpRequest {
        this.headers[key] = value;
        return this;
    }

    setBody(data: any): HttpRequest {
        this.body = data;
        return this;
    }

    async send(): Promise<Response> {
        return fetch(this.url, {
            headers: this.headers,
            body: JSON.stringify(this.body),
        });
    }
}

// Usage
const response = await new HttpRequest()
    .setUrl("/api/users")
    .setHeader("Content-Type", "application/json")
    .setBody({ name: "John" })
    .send();
```

### Example 2: Validated Properties with Getters/Setters

```typescript
class Product {
    private _price: number = 0;
    private _quantity: number = 0;

    get price(): number {
        return this._price;
    }

    set price(value: number) {
        if (value < 0) {
            throw new Error("Price cannot be negative");
        }
        this._price = value;
    }

    get total(): number {
        return this._price * this._quantity;
    }

    set quantity(value: number) {
        if (value < 0) {
            throw new Error("Quantity cannot be negative");
        }
        this._quantity = value;
    }
}
```

---

## 13. Comparison with Other Languages

| Feature                  | TypeScript              | C++                | Java               |
| ------------------------ | ----------------------- | ------------------ | ------------------ |
| **Syntax**               | `class Car { }`         | `class Car { };`   | `class Car { }`    |
| **Constructor**          | `constructor()`         | Same as class name | Same as class name |
| **Destructor**           | ❌ No (GC)              | ✅ `~ClassName()`  | ❌ No (GC)         |
| **Parameter properties** | ✅ Yes                  | ❌ No              | ❌ No              |
| **`readonly`**           | ✅ Yes                  | ✅ `const`         | ✅ `final`         |
| **Getters/setters**      | ✅ native syntax        | ❌ Manual          | ✅ Manual          |
| **Private fields**       | ✅ `#field` + `private` | ✅ `private:`      | ✅ `private`       |
| **Type erasure**         | ✅ Yes (compile-time)   | ❌ No              | ❌ No              |
| **Garbage collection**   | ✅ Yes                  | ❌ No              | ✅ Yes             |

### TypeScript Unique Strengths

- **Parameter properties** (concise syntax)
- **Type safety** without runtime cost
- **Flexible** (structural typing)
- **JavaScript compatibility**

---

## 14. Summary Table

| Concept                  | Purpose               | Syntax                           |
| ------------------------ | --------------------- | -------------------------------- |
| **Class**                | Blueprint for objects | `class Name { }`                 |
| **Object**               | Instance of class     | `new Name()`                     |
| **Constructor**          | Initialize object     | `constructor() { }`              |
| **Parameter properties** | Declare + init        | `constructor(private x: number)` |
| **`this`**               | Current object        | `this.property`                  |
| **private**              | Restrict access       | `private x: number`              |
| **readonly**             | Immutable             | `readonly id: string`            |
| **static**               | Class-level           | `static count: number`           |
| **get/set**              | Accessors             | `get name(): string`             |

---

## 15. Final Takeaways

> **Classes structure data, TypeScript adds safety**

1. **Use parameter properties** for concise code
2. **Leverage `readonly`** for immutability
3. **Use getters/setters** for validation
4. **Understand `this` binding** (arrow functions!)
5. **Choose right privacy**: `private` (compile-time) vs `#` (runtime)

### Core Insight

> TypeScript classes provide Java-like OOP structure with JavaScript flexibility—giving you type safety at compile time and prototype-based performance at runtime.

When used well, TypeScript classes make your code:

- More type-safe
- Easier to refactor
- Better documented
- More maintainable

---

## 16. References

- [TypeScript Handbook: Classes](https://www.typescriptlang.org/docs/handbook/2/classes.html)
- [MDN: Classes](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes)
- [TypeScript Deep Dive: Classes](https://basarat.gitbook.io/typescript/future-javascript/classes)
- [JavaScript Private Fields](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Private_class_fields)
