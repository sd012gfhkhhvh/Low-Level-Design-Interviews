# Factory Pattern in C++ — Notification Service Example

> Resources:
>
> - https://algomaster.io/learn/lld/factory-method

## Overview

The Factory Pattern is a creational design pattern that helps encapsulate object creation logic. Instead of allowing client code to directly instantiate concrete classes, object creation is delegated to a dedicated factory component.

In practice, developers often refer to any centralized object creation mechanism as a "Factory Pattern". However, it is important to distinguish between:

1. **Simple Factory** (commonly used in industry)
2. **Factory Method** (formal Gang of Four pattern)

This document explains both approaches using a Notification Service example.

---

# Problem Statement

Suppose we are building a notification system that supports multiple channels:

- Email
- SMS
- Push Notifications

A naive implementation might look like:

```cpp
if(channel == "EMAIL")
{
    notification = new EmailNotification();
}
else if(channel == "SMS")
{
    notification = new SmsNotification();
}
else if(channel == "PUSH")
{
    notification = new PushNotification();
}
```

### Problems

- Client code becomes tightly coupled to concrete classes.
- Adding a new channel requires modifying multiple places.
- Violates the Open/Closed Principle.
- Object creation logic gets scattered throughout the application.

We need a better approach.

---

# Solution: Factory Pattern

The Factory Pattern moves object creation logic into a dedicated component.

Instead of:

```cpp
auto notification = new EmailNotification();
```

the client does:

```cpp
auto notification =
    NotificationFactory::createChannel("EMAIL");
```

The client no longer needs to know:

- Which class is instantiated
- How it is instantiated
- What dependencies are required

---

# Version 1: Simple Factory (Industry Standard)

## Intent

Centralize object creation inside a single factory class.

### Structure

```text
Client
   |
   v
NotificationFactory
   |
   +---- EmailNotification
   +---- SmsNotification
   +---- PushNotification
```

### Example

```cpp
auto notification =
    NotificationFactory::createChannel("EMAIL");
```

### Factory Implementation

```cpp
class NotificationFactory
{
public:
    static unique_ptr<NotificationStrategy>
    createChannel(const string& type)
    {
        if(type == "EMAIL")
            return make_unique<EmailNotification>();

        if(type == "SMS")
            return make_unique<SmsNotification>();

        if(type == "PUSH")
            return make_unique<PushNotification>();

        return nullptr;
    }
};
```

---

## Advantages

### Encapsulated Creation Logic

All object creation happens in one place.

### Reduced Coupling

Clients depend on abstractions rather than concrete implementations.

### Easier Maintenance

Changes to object creation affect only the factory.

### Production Friendly

Simple, readable, and commonly used in real-world systems.

---

## Drawback

Adding a new notification type requires modifying the factory.

Example:

```cpp
if(type == "WHATSAPP")
{
    return make_unique<WhatsappNotification>();
}
```

This means the factory is not fully closed for modification.

---

# Version 2: Factory Method (GoF)

## Intent

Delegate object creation to subclasses.

Instead of a centralized factory making decisions, each subclass is responsible for creating its own product.

---

## Structure

```text
NotificationCreator
        ^
        |
  -------------------------
  |           |           |
  |           |           |
Email      SMS       Push Creator
Creator   Creator
  |           |           |
  v           v           v
Email       SMS        Push
Notification Notification Notification
```

---

## Creator

```cpp
class NotificationCreator
{
public:
    virtual unique_ptr<Notification>
    createNotification() = 0;
};
```

---

## Concrete Creator

```cpp
class EmailNotificationCreator
    : public NotificationCreator
{
public:
    unique_ptr<Notification>
    createNotification() override
    {
        return make_unique<EmailNotification>();
    }
};
```

---

## Usage

```cpp
unique_ptr<NotificationCreator> creator =
    make_unique<EmailNotificationCreator>();

auto notification =
    creator->createNotification();
```

---

## Advantages

### Follows Formal GoF Pattern

Creation is delegated through inheritance.

### Highly Extensible

New creators can be added without modifying existing creators.

### Strong Separation of Responsibilities

Each creator owns its product creation process.

---

## Drawbacks

### More Classes

Every product requires a corresponding creator.

```text
EmailNotification
EmailNotificationCreator

SmsNotification
SmsNotificationCreator

PushNotification
PushNotificationCreator
```

### Additional Boilerplate

Can become excessive for simple object creation scenarios.

---

# Factory + Strategy Combination

Our Notification Service combines Factory with Strategy.

## Why Strategy?

The sending behavior varies:

```text
Email -> Send Email
SMS   -> Send SMS
Push  -> Send Push Notification
```

Each behavior is encapsulated inside its own strategy implementation.

---

## Architecture

```text
                 Factory
                    |
                    v
        -------------------------
        |           |           |
        v           v           v
     Email        SMS         Push
     Strategy    Strategy    Strategy
        \           |           /
         \          |          /
          \         |         /
           \        |        /
            v       v       v

             NotificationService
```

### Responsibilities

#### Factory

Determines:

```text
What object should be created?
```

#### Strategy

Determines:

```text
How should the notification be sent?
```

---

# Interview Discussion

## If Asked:

### "What pattern is this?"

For:

```cpp
NotificationFactory::createChannel("EMAIL");
```

Answer:

> This is a Simple Factory. It centralizes object creation behind a factory class. Although many developers casually call it a Factory Pattern, it is not the formal Gang of Four Factory Method Pattern.

---

### "What is the Factory Method version?"

Answer:

> In the GoF Factory Method Pattern, creation is delegated to subclasses through a Creator hierarchy rather than a centralized factory class.

---

### "Which version would you use in production?"

Answer:

> For a notification service, I would typically choose Simple Factory + Strategy because the creation logic is straightforward and the design remains easy to maintain. I would use the formal Factory Method Pattern when different creators have distinct creation workflows or when the problem explicitly benefits from inheritance-based extensibility.

---

# Key Takeaways

| Feature                       | Simple Factory | Factory Method (GoF) |
| ----------------------------- | -------------- | -------------------- |
| GoF Pattern                   | ❌ No          | ✅ Yes               |
| Centralized Creation          | ✅ Yes         | ❌ No                |
| Uses Inheritance for Creation | ❌ No          | ✅ Yes               |
| Easy to Understand            | ✅ Yes         | ⚠️ Moderate          |
| Production Usage              | ✅ Very Common | ⚠️ Less Common       |
| Number of Classes             | Low            | Higher               |
| Interview Pattern Knowledge   | Moderate       | High                 |

---

# Final Rule

For most real-world notification systems:

```text
Simple Factory + Strategy
```

For demonstrating the formal design pattern in interviews:

```text
Factory Method (GoF) + Strategy
```

Understanding the difference between these two approaches is often more valuable than merely implementing either one.
