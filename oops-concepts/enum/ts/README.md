# Enums in TypeScript

This note explains how enums work in TypeScript, how they differ from
C++ and Java enums, and how to use them **correctly** in both simple
code and object-oriented design.

---

## 1. What Is an Enum in TypeScript?

A TypeScript enum is a **named set of constants** that compiles to
JavaScript.

Important:

> TypeScript enums are not objects like Java enums.
> 

They are:

- Numbers (numeric enums)
- Or strings (string enums)

---

## 2. Numeric Enums (Default)

```tsx
enum Color {
    Red,
    Green,
    Blue
}
```

### Characteristics

- Auto-incremented numeric values
- Implicit conversion to `number`
- Reverse mapping exists at runtime

```tsx
Color.Red === 0// true
```

### Problems

- Accidental arithmetic allowed
- Weak domain modeling
- Runtime artifacts most users don’t want

---

## 3. Numeric Enums with Explicit Values

```tsx
enum CoinEnum {
    Penny =1,
    Nickel =5,
    Dime =10,
    Quarter =25
}
```

### When This Is Good

- Values are meaningful numbers
- Simple calculations
- No extra metadata or behavior needed

```tsx
CoinEnum.Dime + CoinEnum.Quarter// 35
```

This covers **most real-world use cases**.

---

## 4. `const enum` — Compile-Time Enums

```tsx
const enum CoinConstEnum {
    Dime =10,
    Quarter =25
}
```

### Key Properties

- Completely erased at runtime
- Values are inlined
- No reverse mapping
- Zero runtime cost

### Caveats

- Not supported by all build pipelines
- Avoid in public libraries

---

## 5. Adding Behavior to Enums

Enums cannot have methods.

Behavior is added via **functions**:

```tsx
function coinValue(coin:CoinEnum):number {
    return coin;
}
```

This keeps:

- Enums simple
- Logic explicit
- Code testable

---

## 6. Enums Used in Object-Oriented Code

Enums work well as **value types** inside classes.

```tsx
class Wallet {
    addCoin(coin:CoinEnum) {}
}
```

Key idea:

- Enums define allowed values
- Classes define behavior

This is clean and idiomatic TypeScript.

---

## 7. Java-Style Singleton Enum Emulation

```tsx
class CoinObject {
    static readonly Dime = new CoinObject(10);
}
```

### Why This Idea Exists

- Java enums are objects
- TypeScript enums are limited
- Developers want methods per enum value

### Downsides

- More boilerplate
- Object identity where none is needed
- Harder to serialize
- Over-engineering for simple values

---

## 8. The Preferred Modern TypeScript Pattern (`as const`)

```tsx
const Coin = {
    Dime: {value:10 }
} as const;
```

### Advantages

- No enum runtime quirks
- Strong type inference
- Easy to extend with metadata
- Fully JavaScript-native

This is often the **best replacement** for Java-style enums in TS.

---

## 9. When to Use Each Pattern

| Requirement | Best Choice |
| --- | --- |
| Simple numeric values | `enum` |
| Zero runtime cost | `const enum` |
| Values + metadata | `as const` object |
| Complex behavior | classes |
| Functional modeling | unions |

---

## 10. Final Takeaways

- TypeScript enums are **values, not objects**
- Prefer native enums for simple cases
- Avoid Java-style enum emulation by default
- Use `as const` for rich domain modeling

### Core Insight

> In TypeScript, model data as data first — not as classes.
> 

Good enum usage leads to:

- Cleaner APIs
- Better type safety
- Fewer runtime surprises