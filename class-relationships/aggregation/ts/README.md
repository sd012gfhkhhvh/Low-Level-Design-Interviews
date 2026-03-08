# Aggregation in TypeScript — A Complete Practical Guide

This guide breaks down **Aggregation** in TypeScript from fundamental concepts to advanced patterns, highlighting how it differs from composition and plain association.

- What is aggregation and how it differs from association and composition
- The concepts of weak ownership and independent lifecycles
- Part-sharing (1 part, many wholes)
- Dependency injection as an aggregation pattern
- Real-world example: Music Library System
- Best practices and common anti-patterns

---

## 1. What Is Aggregation?

**Aggregation** is a specialized version of Association. It models a **"whole-part" (or container-contained)** relationship, but unlike composition, it employs **"weak ownership"**.

### The Formula

> **Aggregation = "has-a" relationship + Whole-Part Structure + Independent Lifecycles**

### Mental Model

Think of a **University Department and its Professors**:

- The Department (whole) "has" Professors (parts).
- The Department is a container grouping the professors.
- If the Department gets shut down, the Professors are not terminated. They still exist and can join another department.
- The professors exist independently of the department's lifecycle.

In TypeScript code:

```typescript
class Department {
  // Uses references! A Department has an array grouping the professors.
  private professors: Professor[] = [];
}
```

---

## 2. Why Aggregation Matters

### Benefits

1. **Independent Lifecycles**: The whole class does not exclusively construct or control the destruction of the parts; parts are created externally and injected.
2. **Reusability & Modularity**: The same component can easily be swapped, tested, or moved.
3. **Part Sharing**: Parts can be shared among multiple wholes simultaneously (e.g., a Song can be in multiple Playlists, a Player in multiple Teams).
4. **Natural Reality**: It correctly models most real-world hierarchical relationships minus strict ownership.

### Aggregation vs. Other Relationships

| Relationship    | Concept   | Ownership     | Lifetime    | Example                |
| --------------- | --------- | ------------- | ----------- | ---------------------- |
| **Association** | "uses-a"  | None          | Independent | Doctor ↔ Patient       |
| **Aggregation** | "has-a"   | Weak / Shared | Independent | Department ↔ Professor |
| **Composition** | "part-of" | Exclusive     | Dependent   | House ↔ Room           |

---

## 3. UML Notation

Aggregation is represented in UML by a line with a **hollow (unfilled) diamond** on the side of the "whole".

```text
Whole  <>──────  Part
(Department)    (Professor)
```

---

## 4. Basic Aggregation: Department and Professor

With aggregation, the parts are created outside the whole object. The whole simply holds references to the parts.

```typescript
class Professor {
  constructor(private name: string) {}
  getName(): string {
    return this.name;
  }
}

class Department {
  // The department stores references to Professor instances
  private professors: Professor[] = [];

  constructor(public name: string) {}

  // Assembling: adding a reference to an independent object
  addProfessor(p: Professor): void {
    this.professors.push(p);
  }
}

// Usage:
const smith = new Professor("Dr. Smith");
let cs: Department | null = new Department("Computer Science");
cs.addProfessor(smith);

// If the department is removed, the professor remains.
cs = null; // simulate destruction
console.log(smith.getName()); // Dr. Smith still exists!
```

---

## 5. Sharing Parts: Teams and Players

One of the defining characteristics of aggregation is **sharing**. Since there's no strict ownership, one item can be aggregated into multiple containers simultaneously!

```typescript
class Player {
  constructor(private name: string) {}
}

class Team {
  private roster: Player[] = [];

  addPlayer(p: Player): void {
    this.roster.push(p);
  }
}

// Main Execution
const messi = new Player("Lionel Messi");

const nationalTeam = new Team();
const clubTeam = new Team();

// The exact same player instance exists in two separate wholes!
nationalTeam.addPlayer(messi);
clubTeam.addPlayer(messi);
```

---

## 6. Dependency Injection as Aggregation

When you pass dependencies to a class through its constructor or setter, you are generally utilizing aggregation. The injected class exists outside and is borrowed.

```typescript
class Logger {
  log(msg: string): void {
    console.log(msg);
  }
}

class WebServer {
  // WebServer uses logger, but doesn't own it globally
  constructor(private logger: Logger) {}

  processRequest(): void {
    this.logger.log("Request processed.");
  }
}

// Main Execution
const sharedLogger = new Logger();

// Dependency Injection establishes the aggregation
const serverA = new WebServer(sharedLogger);
const serverB = new WebServer(sharedLogger);
```

_This provides massive benefits for testing. You can easily pass a dummy/mock `Logger`._

---

## 7. Real-World Example: Music Library System

Let's look at a multi-layered aggregation example: Albums, Artists, and Playlists.

```typescript
class Artist {
  constructor(public name: string) {}
}

class Song {
  // Aggregation: Song points to its Artist
  constructor(
    public title: string,
    private artist: Artist,
  ) {}
}

class Playlist {
  // Aggregation: Playlist doesn't own Songs, just holds a collection of references
  private songs: Song[] = [];

  constructor(public name: string) {}

  addSong(s: Song): void {
    this.songs.push(s);
  }
}

// Main Execution
const coldplay = new Artist("Coldplay");
const clocks = new Song("Clocks", coldplay);

let workout: Playlist | null = new Playlist("Workout Mix");
const chill = new Playlist("Chill Vibes");

// Sharing parts
workout.addSong(clocks);
chill.addSong(clocks);

// Deleting a playlist doesn't delete the songs!
workout = null;
// The instance 'clocks' still lives in memory and is used by 'chill'
```

---

## 8. Best Practices and Common Anti-Patterns

### ✅ Do This

1. **Setup from the Outside**: Build the parts first, then assemble the whole (e.g., via constructors or setters). This naturally forms Dependency Injection.
2. **Leverage Interfaces**: Instead of aggregating concrete classes, aggregate interfaces (`private logger: ILogger`).
3. **Respect Shared State**: Remember that modifying an aggregated part (e.g., changing a Song's title) will reflect in all other wholes (Playlists) sharing that part.

### ❌ Don't Do This

1. **Instantiate Inside the Container blindly**:
   ```typescript
   class Department {
     private professors: Professor[] = [];

     constructor() {
       // ❌ BAD! Creates tight lifecycle coupling, forming Composition instead of Aggregation.
       this.professors.push(new Professor("Smith"));
     }
   }
   ```
2. **Assume Deep Copies**: Setting `this.parts = partsArray` does a shallow copy of object references. Be cautious of array mutations.

---

## 9. Memory Management in TypeScript Aggregation

TypeScript (and JavaScript) uses **Garbage Collection**. Meaning, an object is destroyed from memory only when _zero references_ point to it.

Because Aggregation implies multiple things might hold a reference to the same Part (or a master array holds all Parts), naturally setting a Container to `null` will simply wipe out the Container's referencing link. The Parts themselves stay perfectly safe in memory as long as something else (like your main scope, or another Array) still holds them!

---

## 10. Summary Table & Key Comparison

| Feature          | Aggregation                         | Composition                    |
| ---------------- | ----------------------------------- | ------------------------------ |
| **Symbol (UML)** | Hollow Diamond `<>---`              | Solid Diamond `♦---`           |
| **Relationship** | "has-a"                             | "part-of"                      |
| **Ownership**    | Weak / Shared                       | Strict / Exclusive             |
| **Lifecycle**    | Independent                         | Parent controls child          |
| **Sharing**      | Parts can belong to multiple wholes | Parts belong to 1 whole        |
| **Creation**     | Usually via Dependency Injection    | Usually inside the Constructor |

---

## 11. Final Takeaways

> **Aggregation models logical containment where the containers don't command the lifetime of the objects.**

- It enables high flexibility and modularity.
- It is practically synonymous with **Dependency Injection**.
- If deleting the container shouldn't delete the object inside it—you want Aggregation!

### 📚 References

- [AlgoMaster: Aggregation in LLD](https://algomaster.io/learn/lld/aggregation)
- [Visual Paradigm: Aggregation vs Composition](https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/)
- [TypeScript Handbook: Classes](https://www.typescriptlang.org/docs/handbook/2/classes.html)
