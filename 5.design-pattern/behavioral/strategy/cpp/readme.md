# Strategy Design Pattern

## Resources

- [Refactoring Guru: Strategy](https://refactoring.guru/design-patterns/strategy)

## What is the Strategy Design Pattern?

The Strategy Design Pattern is a behavioral pattern that defines a family of algorithms, encapsulates each one, and makes them interchangeable. Strategy lets the algorithm vary independently from the clients that use it.

## Real World Analogy:

Think about how you might travel from your home to the airport. You have several options:
- Drive yourself: Flexible timing, but you pay for parking.
- Taxi/Uber: Door-to-door service, variable pricing.
- Public transit: Cheapest option, but takes longer.

Each of these is a "travel strategy." You (the traveler) decide which strategy to use based on factors like cost, time, and convenience. The important point is that you do not change how you "travel" as a concept. You just swap out the method dynamically based on the context.

The Strategy Pattern works the exact same way in code.

## What problem does it solve?

**The Problem:**

Imagine you are building a payment system for an e-commerce application. Users should be able to pay using different methods: Credit Card, UPI, PayPal, etc.

At first, you might write a `PaymentProcessor` with a giant `if-else` block:

```cpp
if (paymentMethod == "Credit Card") {
    // Process Credit Card
} else if (paymentMethod == "UPI") {
    // Process UPI
} else if (paymentMethod == "PayPal") {
    // Process PayPal
}
```

As the application grows (adding Crypto, Buy-Now-Pay-Later, etc.), you have to modify this core class for every new payment method. This violates the **Open/Closed Principle** and the **Single Responsibility Principle**. The `if-else` chain becomes massive, unreadable, and hard to test.

**The Solution:**

Extract all these different payment algorithms into their own separate classes (Strategies) that all implement a common interface. The original `PaymentProcessor` class (the Context) stores a reference to a Strategy object and delegates the payment logic to it.

## When should we use the Strategy Pattern?

- When you have **multiple ways to perform the same operation**, and the choice might change at runtime.
- When you want to **avoid bloated conditional statements** (`if-else` or `switch`) that select between different behaviors.
- When you need to isolate algorithm-specific data and logic from the code that uses it.
- When different clients might need different algorithms for the exact same task.

## How to implement the Strategy Pattern?

### Implementation Steps

1. **Strategy Interface:** Declare the interface common to all supported algorithms. The Context uses this interface to call the algorithm defined by a ConcreteStrategy.
2. **Concrete Strategies:** Implement the specific algorithms using the Strategy interface. Each concrete strategy encapsulates one specific variation of the algorithm.
3. **Context Class:** Create the main class that uses a strategy. It holds a reference to a Strategy object and delegates the execution to it. The context doesn’t know or care which specific strategy is being used.
4. **Client Setup:** In your main client code, instantiate the specific Concrete Strategy you want and pass it into the Context.

### Architecture

```text
    ┌─────────────────────────┐
    │         Client          │
    └────────────┬────────────┘
                 │ configures Context with a Strategy
                 ▼
    ┌─────────────────────────┐         ┌─────────────────────────┐
    │        Context          │         │      <<Interface>>      │
    ├─────────────────────────┤         │        Strategy         │
    │ - strategy: Strategy    │◇───────▶├─────────────────────────┤
    ├─────────────────────────┤         │ + execute(data)         │
    │ + setStrategy(strategy) │         └────────────┬────────────┘
    │ + doSomething(data)     │                      │
    └─────────────────────────┘                      │
                                           ┌─────────┴─────────┐
                                           ▼                   ▼
                                  ┌─────────────┐     ┌─────────────┐
                                  │  Concrete   │     │  Concrete   │
                                  │ Strategy A  │     │ Strategy B  │
                                  ├─────────────┤     ├─────────────┤
                                  │ + execute() │     │ + execute() │
                                  └─────────────┘     └─────────────┘
```
