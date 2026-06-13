# Decorator Design Pattern

## Resources

- [Refactoring Guru: Decorator](https://refactoring.guru/design-patterns/decorator)

## What is the Decorator Design Pattern?

The Decorator Design Pattern is a structural pattern that lets you dynamically add new behavior or responsibilities to objects without modifying their underlying code. It achieves this by wrapping the original object inside a new "decorator" object that implements the same interface.

## What problem does it solve?

**The Problem:**

Imagine you are building a rich text rendering system. At the core is a `TextView` component that renders plain text. As requirements grow, you need to support **bold**, _italic_, and <u>underlined</u> text.

Your first instinct might be to use inheritance. But to support combinations (e.g., bold + italic, bold + underline), you end up with a massive class explosion. With just 3 features, you need 7 subclasses. With 5 features, you need 31 subclasses (the formula is `2^n - 1`).

```text
                  TextView
                     │
    ┌────────────────┼────────────────┐
    ▼                ▼                ▼
BoldTextView   ItalicTextView   UnderlineTextView
    │                │                │
    ▼                ▼                ▼
BoldItalic...  ItalicUnderline... BoldUnderline... (Class Explosion!)
```

**The Solution:**

Instead of extending a class through inheritance, use the Decorator Pattern. Wrap the original object inside a "decorator" object that shares the same interface. This allows you to stack multiple behaviors dynamically at runtime.

```text
                    TextView (Interface)
                          ▲
            ┌─────────────┴──────────────┐
            │                            │
      PlainTextView                TextDecorator ───────┐
    (Base Component)              (Base Decorator)      │
                                         ◇             │ wraps
                                         ├──────────┐   │
                                         ▼          ▼   ▼
                                   BoldDecorator   ItalicDecorator...
```

## When should we use the Decorator Pattern?

- When you want to **extend the functionality** of a class dynamically without subclassing it.
- When you need to **compose behaviors at runtime** in various combinations.
- When you want to **avoid bloated classes** filled with complex `if-else` logic for optional features.
- When it's not possible to extend a class's behavior using inheritance (e.g., the class is marked `final` or `sealed`).

## How to implement the Decorator Pattern?

### Implementation Steps

1. **Component Interface:** Declare the common interface (`TextView`) that both the core object and all decorators will implement.
2. **Concrete Component:** Create the base object (`PlainTextView`). It provides the default behavior.
3. **Base Decorator:** Create an abstract decorator class (`TextDecorator`) that implements the Component interface and holds a reference to another Component (the "wrappee"). It simply forwards calls to the wrapped object.
4. **Concrete Decorators:** Extend the base decorator (`BoldDecorator`, `ItalicDecorator`) to add new functionality before or after calling the wrapped component’s method.

### Example Walkthrough: Stacking Decorators

1. **Create the base:** `PlainTextView("Hello")`
2. **Wrap in Bold:** `BoldDecorator( PlainTextView )`
3. **Wrap in Italic:** `ItalicDecorator( BoldDecorator )`
4. **Wrap in Underline:** `UnderlineDecorator( ItalicDecorator )`
5. **Call `render()`:** The outermost decorator (`Underline`) prints `<u>`, calls its wrappee (`Italic`), which prints `<i>`, which calls `Bold` (`<b>`), which calls `PlainTextView` (`Hello`).
6. **Result propagates outward:** The final output is `<u><i><b>Hello</b></i></u>`.

### Architecture

```text
    ┌─────────────────────────┐
    │         Client          │
    └────────────┬────────────┘
                 │
                 ▼
    ┌─────────────────────────┐
    │      <<Interface>>      │
    │        Component        │◁──────────────────────┐
    ├─────────────────────────┤                       │
    │ + execute()             │                       │
    └────────────┬────────────┘                       │
                 │                                    │
        ┌────────┴────────┐                           │
        ▼                 ▼                           │
 ┌─────────────┐   ┌─────────────┐                    │
 │ Concrete    │   │  Decorator  │◇───────────────────┘
 │ Component   │   ├─────────────┤
 ├─────────────┤   │ - wrappee   │
 │ + execute() │   ├─────────────┤
 └─────────────┘   │ + execute() │
                   └──────┬──────┘
                          │
              ┌───────────┴───────────┐
              ▼                       ▼
       ┌─────────────┐         ┌─────────────┐
       │ Concrete    │         │ Concrete    │
       │ Decorator A │         │ Decorator B │
       ├─────────────┤         ├─────────────┤
       │ + execute() │         │ + execute() │
       └─────────────┘         └─────────────┘
```
