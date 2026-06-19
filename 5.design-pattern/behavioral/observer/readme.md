# Observer Design Pattern

## Resources

- [Refactoring Guru: Observer](https://refactoring.guru/design-patterns/observer)
- [Algomaster: Observer](https://algomaster.io/learn/lld/observer)

## What is the Observer Design Pattern?

The Observer design pattern is a **behavioral** design pattern that establishes a **one-to-many dependency** between objects. When the state of one object (the subject) changes, all its dependents (the observers) are notified and updated automatically.

It is the fundamental pattern behind the **Publish-Subscribe** (Pub/Sub) architecture and event-driven systems.

## Real World Analogy:

Think about a newspaper subscription. You subscribe to a newspaper publisher. Every morning, the publisher prints the paper and delivers a copy to every subscriber on its list. The publisher does not know whether you read the sports section, clip coupons, or just check the headlines. It does not care. It delivers the paper, and each subscriber decides what to do with it.

When you cancel your subscription, the deliveries stop. When a new neighbor subscribes, they start getting the paper. The publisher's printing logic never changes.

The Observer pattern works the same way: the subject (publisher) broadcasts updates, and observers (subscribers) react however they choose.

## What problem does it solve?

**The Problem:**

Imagine you are building a Fitness Tracker App. The wearable device streams real-time data to a central `FitnessData` object. Several modules need to react to this data: a `LiveDisplay`, a `ProgressLogger`, and a `GoalNotifier`.

If you use a naive approach, `FitnessData` directly instantiates and calls these modules:

```text
    ┌─────────────────────────┐
    │       FitnessData       │
    └────┬────────┬────────┬──┘
         │        │        │      (Hardcoded method calls)
         ▼        ▼        ▼
  LiveDisplay  Logger   Notifier
```

This creates **tight coupling**. Every time you add a new feature (like a `SocialShare` module), you must modify the core `FitnessData` class. You can't dynamically add or remove listeners at runtime (e.g., turning off notifications). The core class becomes bloated with unrelated UI and logging responsibilities, violating the Single Responsibility and Open/Closed Principles.

**The Solution:**

Instead of direct method calls, use the Observer Pattern. Have `FitnessData` act as a **Subject**. The dependent modules act as **Observers** that register themselves with the Subject. 

When the state changes, the Subject simply iterates through its list of generic Observers and calls their `update()` method. The Subject doesn't know *who* the observers are or *what* they do.

## When should we use the Observer Pattern?

- When a change to one object requires changing others, and you don't know how many objects need to be changed dynamically.
- When you want to **decouple the publisher** of data from the **subscribers** who react to it.
- When you need a dynamic, event-driven communication model where listeners can subscribe or unsubscribe at runtime without hardcoding who is listening to whom.

## How to implement the Observer Pattern?

### Implementation Steps

1. **Observer Interface:** Declare the `update()` method that the subject will call. All subscriber modules implement this interface.
2. **Subject Interface:** Declare methods for managing observers: registering (`attach`/`register`), removing (`detach`/`remove`), and notifying (`notify`).
3. **Concrete Subject:** Implement the Subject interface. Maintain a list of registered observers and call `notifyObservers()` whenever the internal state changes.
4. **Concrete Observers:** Implement the Observer interface. When `update()` is called, they pull the necessary data from the Subject and execute their own logic.

### Architecture

```text
       ┌────────────────────────┐
       │     <<Interface>>      │
       │        Subject         │
       ├────────────────────────┤
       │ + attach(Observer)     │
       │ + detach(Observer)     │
       │ + notify()             │
       └────────────┬───────────┘
                    △
                    │ implements
       ┌────────────┴───────────┐         notify() calls update() on all
       │    ConcreteSubject     │───────────────────────┐
       ├────────────────────────┤                       │
       │ - observers: List      │                       ▼
       │ - state                │            ┌────────────────────────┐
       ├────────────────────────┤            │     <<Interface>>      │
       │ + getState()           │            │        Observer        │
       │ + setState()           │            ├────────────────────────┤
       └────────────────────────┘            │ + update()             │
                    ▲                        └────────────┬───────────┘
                    │                                     △
                    │                                     │ implements
                    │                        ┌────────────┴───────────┐
                    │                        │    ConcreteObserver    │
                    └────────────────────────┤────────────────────────┤
                         polls state         │ - subject              │
                                             ├────────────────────────┤
                                             │ + update()             │
                                             └────────────────────────┘
```
