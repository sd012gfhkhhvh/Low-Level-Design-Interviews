# Design Patterns in C++

Design patterns are typical solutions to commonly occurring problems in software design. They are like pre-made blueprints that you can customize to solve a recurring design problem in your code.

This repository covers practical C++ implementations of the Gang of Four (GoF) design patterns, organized into three categories:

## 1. Creational Patterns
Creational patterns focus on object creation mechanisms, abstracting the instantiation process to make systems independent of how objects are created, composed, and represented. They handle flexibility in *what*, *who*, and *how* objects are instantiated.

- **Builder:** Construct complex objects step-by-step.
- **Factory:** Delegate object creation to dedicated classes or subclasses.
- **Prototype:** Clone existing objects without coupling to their specific classes.
- **Singleton:** Ensure a class has only one single instance and provide global access.

## 2. Structural Patterns
Structural patterns deal with the composition of classes and objects to form larger, more robust structures while preserving flexibility and efficiency. They ensure that changes in one part of a system do not necessitate changes throughout.

- **Adapter:** Bridge incompatible interfaces.
- **Decorator:** Attach new behaviors to objects dynamically without inheritance.
- **Facade:** Provide a simplified interface to a complex subsystem.

## 3. Behavioral Patterns
Behavioral patterns are concerned with communication between objects and the assignment of responsibilities. They outline interaction patterns and streamline how disparate objects collaborate, often encapsulating algorithms or managing complex control flow.

- **Strategy:** Define a family of algorithms, encapsulate each one, and make them interchangeable.
- *(More behavioral patterns coming soon...)*
