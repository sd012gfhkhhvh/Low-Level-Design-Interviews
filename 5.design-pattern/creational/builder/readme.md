# Builder Design Pattern

## Resource

- [Algomaster: Builder Pattern](https://algomaster.io/learn/lld/builder)

## What is the Builder Design Pattern?

The Builder Design Pattern is a creational pattern that lets you construct complex objects step-by-step, separating the construction logic from the final representation.

## Real World Analogy:

Think of ordering a custom pizza. You don't just ask for "a pizza" and get a random combination. Instead, you go step-by-step: choosing the crust, then the sauce, then adding cheese, pepperoni, and olives. 

The Builder Pattern works the same way: it allows you to construct a complex object step-by-step, producing different types and representations using the same construction process.

## What problem does it solve?

**The Problem:**

- **Telescoping Constructors:** Complex classes often have many attributes, including optional ones. This can lead to the creation of multiple constructors with different sets of parameters to fulfill various needs (the telescoping constructor anti-pattern).

For example, a `User` object might include fields like name, email, phone, address, and preferences. As the number of fields grows, the constructor API becomes harder to use correctly, easier to misuse, and more difficult to maintain.

**The Solution:**

The Builder Pattern addresses this by introducing a dedicated builder class that owns the creation logic. Clients configure the builder step-by-step and then build the final object, which can remain immutable, validated, and consistently constructed.

## When should we use the Builder Design Pattern?

**Use the Builder pattern when:**

- You have a complex object with lots of attributes, and most callers only need to set a subset of them for any given use case.
- You want to avoid telescoping constructors or long parameter lists.
- The complex object must be assembled through multiple steps, possibly in a specific order.
- You need to create an **immutable** final object (i.e., a class with no setters and private attributes).
  - *Example:* Composing an email by adding CCs, BCCs, and attachments step-by-step. When you click send, it creates an immutable final email object.

## How to implement the Builder Design Pattern?

### Implementation Details:

1. **Dedicated Builder Class:** Introduce a dedicated builder class that owns the creation logic.
2. **Step-by-Step Configuration:** Clients configure the builder step-by-step, typically through setter methods that return `this` (or `*this` in C++) for method chaining (fluent interface).
3. **Build Method:** Provide a `build()` method in the builder that instantiates and returns the final object, optionally validating the configuration beforehand. The final object can be immutable since its state is fully determined at creation.
