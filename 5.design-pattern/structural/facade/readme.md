# Facade Design Pattern

## Resources

- [Algomaster: Facade Pattern](https://algomaster.io/learn/lld/facade)
- [Refactoring Guru: Facade](https://refactoring.guru/design-patterns/facade)

## What is the Facade Design Pattern?

The Facade Design Pattern is a structural pattern that provides a single, simplified interface to a complex subsystem. Instead of forcing clients to coordinate many moving parts, a facade hides the internal complexity and exposes a clean, easy-to-use entry point.

## Real World Analogy:

Think of calling a customer service hotline to order a new phone. You don't need to speak to the billing department to process your card, the warehouse to find the phone, and the shipping company to send it. You just talk to the customer service operator (the facade), who coordinates all those complex internal departments for you.

The Facade Pattern works the same way: it provides a simple, unified interface to a complex subsystem, hiding all the messy coordination from the client.

## What problem does it solve?

**The Problem:**

In real applications, a "simple" task often requires orchestrating multiple components. Without a facade, each client ends up talking to several subsystems directly and coordinating the sequence on its own. This leads to tightly coupled and brittle code.

```text
     Client
       │
  ┌────┼─────────┬─────────┐
  ▼    ▼         ▼         ▼
Sub1  Sub2      Sub3      Sub4
```

**The Solution:**

Introduce a single entry point (a facade) that wraps the complex interactions behind a clean interface. The client interacts with one object, while the facade coordinates the subsystem calls behind the scenes. This reduces coupling and improves maintainability.

```text
     Client
       │
     Facade
       │
  ┌────┼─────────┬─────────┐
  ▼    ▼         ▼         ▼
Sub1  Sub2      Sub3      Sub4
```

## When should we use the Facade Pattern?

- Your system contains **many interdependent classes** or low-level APIs.
- You want to orchestrate multiple components **without knowing their internals**.
- You want to **reduce coupling** between the client and the subsystem, making the system easier to learn and use.
- You want to layer your subsystems, providing one facade per layer.

## How to implement the Facade Pattern?

### Implementation Steps

1. **Identify Complexities:** Find the operations that require complex coordination among multiple subsystem components.
2. **Create the Facade:** Encapsulate this complex logic inside a single Facade class that provides a simple interface to the client.
3. **Delegate Requests:** The Facade class delegates the client's requests to the appropriate subsystem components in the correct order.
4. **(Optional) Add Logic:** The Facade can also perform additional tasks like validation, logging, caching, or data conversion before calling the subsystems.

### Architecture

```text
    ┌─────────────────────────┐
    │         Client          │
    └────────────┬────────────┘
                 │ calls
                 ▼
    ┌─────────────────────────┐
    │         Facade          │
    │ (Simplified Interface)  │
    └────────────┬────────────┘
                 │ coordinates
        ┌────────┴────────┐
        ▼                 ▼
 ┌─────────────┐   ┌─────────────┐
 │ Subsystem A │   │ Subsystem B │
 └──────┬──────┘   └──────┬──────┘
        │                 │
        ▼                 ▼
 ┌─────────────┐   ┌─────────────┐
 │ Subsystem C │   │ Subsystem D │
 └─────────────┘   └─────────────┘
```
