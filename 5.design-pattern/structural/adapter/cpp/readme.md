# Adapter Design Pattern

## Resources

- [Algomaster: Adapter Pattern](https://algomaster.io/learn/lld/adapter)
- [Refactoring Guru: Adapter](https://refactoring.guru/design-patterns/adapter)

## What is the Adapter Design Pattern?

The Adapter Design Pattern is a structural pattern that allows incompatible interfaces to work together by converting the interface of one class into another that the client expects. It acts as a bridge (translator) between two incompatible interfaces.

## What problem does it solve?

**The Problem:**

Your new application communicates in JSON, but the legacy payment system only accepts XML. Without an adapter, you'd have to rewrite your application to speak XML — time-consuming and error-prone.

**The Solution:**

Create a wrapper class (the adapter) that translates your JSON requests into the XML format the old system expects. Both systems work together seamlessly without modifying their original code.

## When should we use the Adapter Pattern?

- Integrating with a **legacy system** or **third-party library** that doesn't match your current interface.
- You want to **reuse existing functionality** without modifying its source code.
- Bridging the gap between **new and old code**, or between systems with different interface designs.

## Types of Adapter

| Type | Mechanism | Pros | Cons |
|---|---|---|---|
| **Object Adapter** (Composition) | Adapter *holds an instance* of the adaptee | No multiple inheritance needed; more flexible | Slightly more indirection |
| **Class Adapter** (Inheritance) | Adapter *inherits from* both target and adaptee | Direct access to adaptee internals | Requires multiple inheritance (not supported in Java); tighter coupling |

> **Recommendation:** Prefer the Object Adapter (composition) in most cases.

## How to implement the Adapter Pattern?

### Four Components

```
    Client  ──uses──▶  Target (Interface)
                            ▲
                            │ implements
                       ┌────┴─────┐
                       │  Adapter  │──holds──▶  Adaptee
                       └──────────┘         (legacy/third-party)
```

1. **Target:** The interface the client expects.
2. **Adaptee:** The existing class with an incompatible interface that needs adapting.
3. **Adapter:** Implements the Target interface, holds an instance of the Adaptee, and translates calls from the Target interface to the Adaptee's interface.
4. **Client:** Works with the Target interface, unaware of the Adapter or Adaptee behind it.

### Implementation Steps

1. Identify the **Target** interface the client expects.
2. Identify the **Adaptee** class that needs to be adapted.
3. Create an **Adapter** class that implements the Target and holds an instance of the Adaptee.
4. In each Adapter method, **translate** the call to the Adaptee's corresponding method.
