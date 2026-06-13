# Prototype Design Pattern

## Resources

- [Refactoring Guru: Prototype](https://refactoring.guru/design-patterns/prototype)

## What is the Prototype Design Pattern?

The Prototype Design Pattern is a creational pattern that allows cloning objects, even complex ones, without coupling to their specific classes. Instead of creating objects from scratch (which might be costly or impossible from outside the class), you copy an existing object.

## Real World Analogy:

Think of dividing a single cell in biology. Instead of creating a new cell from scratch by gathering raw proteins and DNA, a cell duplicates itself completely, creating an exact clone that can then function independently.

The Prototype Pattern works the same way: instead of building a new object from scratch and copying all its fields manually, you just ask an existing object to clone itself.

## What problem does it solve?

**The Problem:**

Imagine you have a complex object, and you want an exact copy of it. If you try to create a new object and copy all the fields manually, you will run into several issues:
- Some fields might be private and inaccessible from the outside.
- You must know the object's concrete class, tying your code to that implementation.
- Setting up the object from scratch might require expensive database calls or heavy configuration.

**The Solution:**

Delegate the cloning process to the actual objects that are being cloned. The pattern declares a common interface for all objects that support cloning (usually containing a single `clone()` method). This lets you copy objects without knowing their concrete types, and the object itself can safely copy its own private fields.

## When should we use the Prototype Pattern?

- When your code shouldn't depend on the concrete classes of objects that you need to copy.
- When creating a new object from scratch is more expensive than copying an existing one (e.g., loading data from a database or network).
- When you have many subclasses that differ only in how they are initialized. Instead of creating a subclass for each configuration, you can use a few pre-configured prototypes and clone them.

## How to implement the Prototype Pattern?

### Implementation Steps

1. **Create the Prototype Interface:** Declare a `clone()` method in an interface or base class.
2. **Implement the Clone Method:** In concrete classes, implement the `clone()` method to return a new instance with the same state. Be mindful of **deep vs. shallow copying** if your object contains pointers or references.
3. **(Optional) Create a Registry:** Create a central registry (a map or factory class) to store frequently used prototypes for easy access by name or ID.

### Architecture

```text
    ┌─────────────────────────┐
    │         Client          │
    └────────────┬────────────┘
                 │ calls clone()
                 ▼
    ┌─────────────────────────┐
    │      <<Interface>>      │
    │        Prototype        │
    ├─────────────────────────┤
    │ + clone(): Prototype    │
    └────────────┬────────────┘
                 │
        ┌────────┴────────┐
        ▼                 ▼
 ┌─────────────┐   ┌─────────────┐
 │ Concrete    │   │ Concrete    │
 │ Prototype A │   │ Prototype B │
 ├─────────────┤   ├─────────────┤
 │ + clone()   │   │ + clone()   │
 └─────────────┘   └─────────────┘
```
