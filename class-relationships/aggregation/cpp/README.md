# Aggregation in C++ — A Complete Practical Guide

This guide breaks down **Aggregation** in C++ from fundamental concepts to advanced patterns, structured specifically for system design and OOP concepts.

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

In C++ code:

```cpp
class Department {
    // Stores pointers (or shared_ptrs) to signify weak ownership
    std::vector<Professor*> professors;
};
```

---

## 2. Why Aggregation Matters

### Benefits

1. **Independent Lifecycles**: The whole class does not construct or cleanly destroy the parts; parts are created externally and injected.
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

With aggregation, the parts are created outside the whole object. The whole simply holds references (pointers in C++) to the parts.

```cpp
class Professor {
    std::string name;
public:
    Professor(const std::string& n) : name(n) {}
    std::string getName() const { return name; }
};

class Department {
    std::string name;
    // Uses pointers! The department does not store the objects directly natively.
    std::vector<Professor*> professors;

public:
    Department(const std::string& n) : name(n) {}

    // Assembling: adding a reference to an independent object
    void addProfessor(Professor* p) { professors.push_back(p); }

    // Notice: NO destructor to `delete` the professors!
};

// Usage:
Professor smith("Dr. Smith");
Department cs("Computer Science");
cs.addProfessor(&smith);

// If cs goes out of scope, smith continues to exist.
```

---

## 5. Sharing Parts: Teams and Players

One of the defining characteristics of aggregation is **sharing**. Since there's no strict ownership, one item can be aggregated into multiple containers simultaneously!

```cpp
class Player {
    std::string name;
public:
    Player(const std::string& n) : name(n) {}
};

class Team {
    std::vector<Player*> roster;
public:
    void addPlayer(Player* p) { roster.push_back(p); }
};

// Main Execution
Player messi("Lionel Messi");

Team nationalTeam("Argentina");
Team clubTeam("Inter Miami");

// The exact same player exists in two separate wholes!
nationalTeam.addPlayer(&messi);
clubTeam.addPlayer(&messi);
```

---

## 6. Dependency Injection as Aggregation

When you pass dependencies to a class through its constructor or setter, you are generally utilizing aggregation. The injected class exists outside and is borrowed.

```cpp
class Logger {
public:
    void log(const std::string& msg) { std::cout << msg << "\n"; }
};

class WebServer {
private:
    Logger* logger; // WebServer uses it, but doesn't own it

public:
    WebServer(Logger* l) : logger(l) {}

    void processRequest() {
        logger->log("Request processed.");
    }
};

// Main Execution
Logger sharedLogger;
WebServer serverA(&sharedLogger);
WebServer serverB(&sharedLogger);
```

_This provides massive benefits for testing. You can easily pass a dummy/mock `Logger`._

---

## 7. Real-World Example: Music Library System

Let's look at a multi-layered aggregation example: Albums, Artists, and Playlists.

```cpp
class Artist { /* name */ };

class Song {
    std::string title;
    Artist* artist; // Aggregation (Song doesn't own Artist)
public:
    Song(std::string t, Artist* a) : title(t), artist(a) {}
};

class Playlist {
    std::string name;
    std::vector<Song*> songs; // Aggregation (Playlist doesn't own Songs)
public:
    void addSong(Song* s) { songs.push_back(s); }
};

// Main Execution
Artist coldplay("Coldplay");
Song clocks("Clocks", &coldplay);

Playlist workout("Workout Mix");
Playlist chill("Chill Vibes");

// Sharing parts
workout.addSong(&clocks);
chill.addSong(&clocks);

// Deleting a playlist doesn't delete the songs!
// delete &workout; // Song "Clocks" still lives on.
```

---

## 8. Best Practices and Common Anti-Patterns

### ✅ Do This

1. **Use Pointers or References**: To model weak ownership in C++, use raw pointers, reference wrappers, or `std::shared_ptr`.
2. **Setup from the Outside**: Build the parts first, then assemble the whole (e.g., via constructors or setters).
3. **Understand your Lifecycles**: Ensure the scope that creates the objects handles their deletion (if you use raw pointers and `new`).

### ❌ Don't Do This

1. **Delete Parts in the Destructor**:
   ```cpp
   ~Department() {
       for(auto p : professors) delete p; // ❌ BAD! This is Composition, not Aggregation.
   }
   ```
2. **Instantiate Inside the Container**:
   ```cpp
   Department() {
       professors.push_back(new Professor("Smith")); // ❌ BAD! Creates tight lifecycle coupling.
   }
   ```
3. **Use `std::unique_ptr` for simple Aggregation**: `unique_ptr` enforces strict singular ownership, which is precisely what composition is, not aggregation!

---

## 9. C++ Memory Management in Aggregation

Since you aren't doing automatic deletion inside destructors safely modeling this in modern C++ looks like:

- `std::vector<T*>` : Assuming something higher up owns the heap memory.
- `std::vector<std::shared_ptr<T>>` : Cleanest approach if ownership is genuinely shared and nothing specifically "owns" it strictly.
- `std::reference_wrapper<T>` : Great for non-null aggregated members that exist on the stack.

---

## 10. Summary Table & Key Comparison

| Feature                | Aggregation                         | Composition                          |
| ---------------------- | ----------------------------------- | ------------------------------------ |
| **Symbol (UML)**       | Hollow Diamond `<>---`              | Solid Diamond `♦---`                 |
| **Relationship**       | "has-a"                             | "part-of"                            |
| **Ownership**          | Weak / None                         | Strict / Exclusive                   |
| **Lifecycle**          | Independent                         | Parent controls child                |
| **Sharing**            | Parts can belong to multiple wholes | Parts belong to 1 whole              |
| **C++ Implementation** | `T*` or `std::shared_ptr<T>`        | `T` directly or `std::unique_ptr<T>` |

---

## 11. Final Takeaways

> **Aggregation models logical containment where the containers don't command the lifetime of the objects.**

- It enables high flexibility and modularity.
- It is synonymous with **Dependency Injection**.
- If deleting the container shouldn't delete the object inside it—you want Aggregation!

### 📚 References

- [AlgoMaster: Aggregation in LLD](https://algomaster.io/learn/lld/aggregation)
- [Visual Paradigm: Aggregation vs Composition](https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/)
- [cppreference: C++ Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
