# Encapsulation in TypeScript — A Complete Practical Guide

This guide explains encapsulation in TypeScript from fundamentals to advanced patterns, including:

- What is encapsulation and why it matters
- Access modifiers (private, protected, public)
- Native getters and setters
- Private fields with `#` (ES2022+)
- `readonly` modifier
- Parameter properties
- TypeScript vs JavaScript encapsulation
- Real-world design patterns

---

## 1. What Is Encapsulation?

**Encapsulation** is one of the four foundational principles of object-oriented programming (OOP). It bundles data (properties) and methods that operate on that data into a single unit (class), while restricting direct access to internal components.

### The Formula

> **Encapsulation = Data Hiding + Controlled Access**

### Mental Model

Think of encapsulation like a **smartphone**:

- You see the **interface** (touchscreen, buttons, apps)
- You don't see the **implementation** (CPU, memory, circuits)
- You **control** it through a simple interface
- The phone **validates** your input (can't install incompatible apps)

---

## 2. Why Encapsulation Matters in TypeScript

### The Problem ❌

```typescript
class BankAccount {
  balance: number = 0; // ❌ Public by default!
}

const account = new BankAccount();
account.balance = -1000000; // ❌ No validation!
account.balance = 999999999; // ❌ Free money exploit!
```

### The Solution ✅

```typescript
class BankAccount {
  private balance: number = 0; // ✅ Private

  deposit(amount: number): void {
    if (amount > 0) {
      // ✅ Validation!
      this.balance += amount;
    }
  }

  getBalance(): number {
    return this.balance; // ✅ Controlled access
  }
}
```

---

## 3. Access Modifiers in TypeScript

TypeScript provides three access modifiers (compile-time checked):

| Modifier        | Accessible From    | Use Case                     |
| --------------- | ------------------ | ---------------------------- |
| **`public`**    | Anywhere           | Public API methods (default) |
| **`private`**   | Only within class  | Internal data, helpers       |
| **`protected`** | Class + subclasses | Inheritance hierarchies      |

### Example

```typescript
class Example {
  public publicData: number = 1; // Anyone
  private privateData: number = 2; // Only Example
  protected protectedData: number = 3; // Example + subclasses

  publicMethod(): void {
    this.privateData = 10; // ✅ OK
    this.protectedData = 20; // ✅ OK
  }
}

class Derived extends Example {
  method(): void {
    this.publicData = 10; // ✅ OK
    // this.privateData = 20;  // ❌ Error: private
    this.protectedData = 30; // ✅ OK: protected
  }
}
```

### Important: Compile-Time Only!

TypeScript access modifiers are **erased at runtime**:

```typescript
class Test {
  private secret: number = 42;
}

const t = new Test();
console.log(t["secret"]); // ⚠️ Works in JS! TypeScript can't stop this
```

For **runtime privacy**, use `#` private fields (see below).

---

## 4. Native Getters and Setters

TypeScript supports native `get`/`set` accessors (property-like syntax):

### Basic Usage

```typescript
class Person {
  private _age: number = 0;

  // Getter - access like a property
  get age(): number {
    return this._age;
  }

  // Setter - validation!
  set age(value: number) {
    if (value < 0 || value > 150) {
      throw new Error("Invalid age");
    }
    this._age = value;
  }
}

const person = new Person();
person.age = 25; // Setter called
console.log(person.age); // Getter called
```

### Computed Properties

```typescript
class Rectangle {
  constructor(
    private width: number,
    private height: number,
  ) {}

  // Computed property (no setter!)
  get area(): number {
    return this.width * this.height;
  }

  get perimeter(): number {
    return 2 * (this.width + this.height);
  }
}

const rect = new Rectangle(10, 20);
console.log(rect.area); // 200 (computed on access)
```

### Benefits of get/set

- ✅ **Property-like syntax**: Clean, intuitive
- ✅ **Validation**: Enforce rules in setters
- ✅ **Lazy evaluation**: Compute on access
- ✅ **Logging**: Track access/modifications
- ✅ **Backward compatibility**: Can change implementation

---

## 5. Private Fields with `#` (ES2022+)

JavaScript now has **true private fields** with `#` syntax:

```typescript
class SecureAccount {
  #balance: number; // True private (runtime!)

  constructor(initial: number) {
    this.#balance = initial;
  }

  deposit(amount: number): void {
    this.#balance += amount;
  }

  getBalance(): number {
    return this.#balance;
  }

  #internalMethod(): void {
    // Private method
  }
}

const account = new SecureAccount(1000);
console.log(account.getBalance()); // ✅ 1000
// console.log(account.#balance);   // ❌ Error: truly private!
// console.log(account['#balance']); // ❌ Error: can't access via bracket notation
```

### TypeScript `private` vs JavaScript `#`

| Feature               | `private`           | `#`           |
| --------------------- | ------------------- | ------------- |
| **Enforcement**       | Compile-time only   | Runtime       |
| **JavaScript output** | Becomes public      | Stays private |
| **Truly hidden**      | ❌ No               | ✅ Yes        |
| **Syntax**            | `private x: number` | `#x: number`  |
| **Method privacy**    | `private method()`  | `#method()`   |

**When to use:**

- Use `private` for most cases (TypeScript type safety)
- Use `#` when you need **true runtime privacy** (sensitive data)

---

## 6. The `readonly` Modifier

TypeScript-specific: `readonly` prevents modification after initialization.

```typescript
class User {
  readonly id: string;
  readonly createdAt: Date;
  private name: string;

  constructor(id: string, name: string) {
    this.id = id; // ✅ Can set in constructor
    this.createdAt = new Date();
    this.name = name;
  }

  updateName(newName: string): void {
    this.name = newName; // ✅ OK: not readonly
    // this.id = "new-id";  // ❌ Error: readonly
  }
}
```

**Use cases for `readonly`:**

- IDs that shouldn't change
- Creation timestamps
- Configuration values
- Immutable references

---

## 7. Parameter Properties

TypeScript allows declaring properties directly in constructor parameters:

### Standard Way

```typescript
class BookStandard {
  private title: string;
  private author: string;

  constructor(title: string, author: string) {
    this.title = title;
    this.author = author;
  }
}
```

### Parameter Properties (Shorter!)

```typescript
class BookShort {
  constructor(
    private title: string,
    private author: string,
    readonly isbn: string, // Can mix with readonly!
  ) {
    // Properties automatically created and assigned!
  }

  getTitle(): string {
    return this.title;
  }
}
```

**Benefits:**

- ✅ Less boilerplate
- ✅ More concise
- ✅ Same functionality

---

## 8. Real-World Examples

### Example 1: User Authentication

```typescript
class User {
  private username: string;
  private passwordHash: string;
  private isActive: boolean = true;
  private loginAttempts: number = 0;

  constructor(username: string, password: string) {
    this.username = username;
    this.passwordHash = this.hashPassword(password);
  }

  private hashPassword(password: string): string {
    // Use bcrypt, argon2, etc. in production
    return "hash_" + password;
  }

  private isStrongPassword(password: string): boolean {
    return password.length >= 8;
  }

  login(password: string): boolean {
    if (!this.isActive) {
      return false;
    }

    if (this.hashPassword(password) === this.passwordHash) {
      this.loginAttempts = 0;
      return true;
    } else {
      this.loginAttempts++;
      if (this.loginAttempts >= 3) {
        this.isActive = false; // Lock account
      }
      return false;
    }
  }

  resetPassword(oldPass: string, newPass: string): void {
    if (this.hashPassword(oldPass) !== this.passwordHash) {
      throw new Error("Old password incorrect");
    }
    if (!this.isStrongPassword(newPass)) {
      throw new Error("Weak password");
    }
    this.passwordHash = this.hashPassword(newPass);
  }
}
```

**Encapsulation benefits:**

- Password never exposed
- Hashing logic hidden
- Automatic account locking
- Can't manipulate login attempts

### Example 2: Shopping Cart

```typescript
class ShoppingCart {
  private items: string[] = [];
  private prices: number[] = [];
  private discount: number = 0;

  private calculateSubtotal(): number {
    return this.prices.reduce((sum, p) => sum + p, 0);
  }

  addItem(item: string, price: number): void {
    if (price <= 0) {
      throw new Error("Invalid price");
    }
    this.items.push(item);
    this.prices.push(price);
  }

  applyDiscount(percent: number): void {
    if (percent < 0 || percent > 100) {
      throw new Error("Invalid discount");
    }
    this.discount = percent;
  }

  get total(): number {
    const subtotal = this.calculateSubtotal();
    return subtotal * (1 - this.discount / 100);
  }
}

const cart = new ShoppingCart();
cart.addItem("Laptop", 999);
cart.applyDiscount(10);
console.log(cart.total); // Using getter
```

---

## 9. Best Practices

### ✅ Do This

1. **Make properties private by default**

   ```typescript
   class Good {
     private value: number = 0; // ✅ Private
   }
   ```

2. **Use getters for computed properties**

   ```typescript
   get fullName(): string {
       return `${this.firstName} ${this.lastName}`;
   }
   ```

3. **Validate in setters**

   ```typescript
   set age(value: number) {
       if (value < 0 || value > 150) {  // ✅ Validation
           throw new Error("Invalid age");
       }
       this._age = value;
   }
   ```

4. **Use `readonly` for immutable data**

   ```typescript
   readonly id: string;  // ✅ Can't change
   ```

5. **Use `#` for sensitive data**

   ```typescript
   #password: string;  // ✅ True runtime privacy
   ```

6. **Leverage parameter properties**
   ```typescript
   constructor(private name: string) {}  // ✅ Concise
   ```

### ❌ Don't Do This

1. **Public writable properties**

   ```typescript
   class Bad {
     balance: number = 0; // ❌ No encapsulation!
   }
   ```

2. **Getters that return mutable references**

   ```typescript
   get data(): number[] {
       return this._data;  // ❌ Exposes internal array!
   }
   ```

3. **Setters without validation**

   ```typescript
   set age(value: number) {
       this._age = value;  // ❌ No validation!
   }
   ```

4. **Relying on `private` for security**
   ```typescript
   private secret = "password123";  // ❌ Not secure at runtime!
   ```

---

## 10. Common Patterns

### Pattern 1: Read-Only Properties

```typescript
class Product {
  readonly id: string;

  constructor(id: string) {
    this.id = id;
  }

  // No setter - truly read-only!
}
```

### Pattern 2: Lazy Initialization

```typescript
class DataLoader {
  private _data: number[] | null = null;

  get data(): number[] {
    if (!this._data) {
      this._data = this.loadExpensiveData();
    }
    return this._data;
  }

  private loadExpensiveData(): number[] {
    // Load data only when needed
    return [1, 2, 3, 4, 5];
  }
}
```

### Pattern 3: Validation Wrapper

```typescript
class Email {
  constructor(private value: string) {
    if (!this.isValid(value)) {
      throw new Error("Invalid email");
    }
  }

  private isValid(email: string): boolean {
    return email.includes("@");
  }

  toString(): string {
    return this.value;
  }
}
```

---

## 11. TypeScript vs Other Languages

| Feature                  | TypeScript                       | C++               | Java            |
| ------------------------ | -------------------------------- | ----------------- | --------------- |
| **Default access**       | `public`                         | `private` (class) | Package-private |
| **Access modifiers**     | `private`, `protected`, `public` | Same              | Same + package  |
| **Runtime enforcement**  | ❌ (unless `#`)                  | ✅ Yes            | ✅ Yes          |
| **Getters/setters**      | ✅ Native `get`/`set`            | Manual            | Manual          |
| **`readonly`**           | ✅ Yes                           | `const`           | `final`         |
| **Parameter properties** | ✅ Yes                           | ❌ No             | ❌ No           |
| **`#` private fields**   | ✅ Yes (ES2022+)                 | N/A               | N/A             |

### TypeScript Unique Features

- **Native get/set syntax** (cleaner than Java/C++)
- **Parameter properties** (less boilerplate)
- **Compile-time + runtime privacy** (`private` + `#`)
- **`readonly` for immutability**

---

## 12. When to Use Each Privacy Level

| Privacy Level   | When to Use                        | Example                   |
| --------------- | ---------------------------------- | ------------------------- |
| **`private`**   | TypeScript projects, internal data | `private balance: number` |
| **`#` private** | True runtime privacy needed        | `#password: string`       |
| **`protected`** | Data for inheritance               | `protected baseData: any` |
| **`readonly`**  | Immutable after init               | `readonly id: string`     |
| **`public`**    | External API                       | `public login(): void`    |

---

## 13. Summary Table

| Concept                | Purpose          | TypeScript Syntax                |
| ---------------------- | ---------------- | -------------------------------- |
| **Data hiding**        | Protect data     | `private value: number`          |
| **Getter**             | Read access      | `get name(): string`             |
| **Setter**             | Controlled write | `set age(v: number)`             |
| **Private helper**     | Hide logic       | `private hash(): string`         |
| **Runtime privacy**    | True security    | `#password: string`              |
| **Immutability**       | No modifications | `readonly id: string`            |
| **Parameter property** | Less boilerplate | `constructor(private x: number)` |

---

## 14. Final Takeaways

> **Encapsulation is about protecting data integrity and hiding complexity.**

### Core Principles

1. **Default to private** - Only expose what's necessary
2. **Use native get/set** - Property-like syntax with validation
3. **Use `#` for sensitive data** - True runtime privacy
4. **Use `readonly` for immutability** - IDs, timestamps, config
5. **Leverage parameter properties** - Less boilerplate

### Key Insight

> "TypeScript gives you compile-time type safety with `private`, and JavaScript gives you runtime privacy with `#`. Use both strategically for maximum protection."

**Benefits:**

- **Security**: Protect passwords, tokens, sensitive data
- **Integrity**: Enforce business rules through validation
- **Maintainability**: Change internals without breaking code
- **Type Safety**: TypeScript catches errors at compile time

---

## 15. References

- [TypeScript Handbook: Classes](https://www.typescriptlang.org/docs/handbook/2/classes.html)
- [MDN: Private Class Fields](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes/Private_class_fields)
- [TypeScript Deep Dive: Classes](https://basarat.gitbook.io/typescript/future-javascript/classes)
- [ECMAScript Private Fields Proposal](https://github.com/tc39/proposal-class-fields)
