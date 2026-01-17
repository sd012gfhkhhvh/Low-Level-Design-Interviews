# OOP Study Guide Documentation Prompt

Use this prompt template whenever you want to refactor and document a topic in your OOP learning journey.

---

## 📝 PROMPT TEMPLATE

```
I need you to refactor and document my code for the topic: **[TOPIC_NAME]**

Language: [cpp/ts/java]
Code Location: [PATH_TO_CODE_FILE]

Please analyze my existing code and:

1. **Refactor the code** following the style demonstrated in `oop-fundamentals/enum/`:
   - Add clear section markers with visual separators (=======)
   - Include numbered sections (1., 2., 3., etc.)
   - Add descriptive comments explaining each concept
   - Show multiple practical examples (basic, intermediate, advanced)
   - Include comparisons with other language styles when relevant
   - Add a complete demonstration/main function

2. **Create a comprehensive README.md** with this structure:
   - Title: [Topic] in [Language] — A Complete Practical Guide
   - Brief introduction and when to use it
   - Multiple numbered sections covering:
     * Basic concepts
     * Common patterns
     * Advanced usage
     * Comparisons with other languages
     * Best practices
     * Common pitfalls
     * Real-world use cases
   - Summary table comparing approaches
   - Final takeaways with core insights
   - Reference links to authoritative sources

3. **Follow these formatting guidelines**:
   - Use markdown with code blocks
   - Include emoji indicators: ✅ (good), ❌ (bad), ⚠️ (warning)
   - Use blockquotes (>) for key insights
   - Create comparison tables when appropriate
   - Keep explanations concise but comprehensive
   - Use practical, relatable examples

4. **Quality standards**:
   - All code must compile and run
   - Comments explain "why" not just "what"
   - Cover edge cases and common mistakes
   - Show evolution from basic to advanced
   - Compare with equivalent patterns in other OOP languages
   - Include theoretical context

Important: Match the style, depth, and comprehensiveness of:
- `oop-fundamentals/enum/cpp/enum.cpp`
- `oop-fundamentals/enum/cpp/README.md`
- `oop-fundamentals/enum/ts/enum.ts`
- `oop-fundamentals/enum/ts/README.md`
```

---

## 🏗️ ARCHITECTURE GUIDELINES

Before using the prompt, understand the structure:

### **Core Principle**

> One topic = One comprehensive code file + One README per language

### **When to Use ONE File (Default)**

Most OOP concepts fit in a single comprehensive file (~100-200 lines):

-   ✅ Encapsulation
-   ✅ Interface
-   ✅ Abstract Classes
-   ✅ Enum (your perfect example!)
-   ✅ Operator Overloading
-   ✅ Composition
-   ✅ Constructor/Destructor
-   ✅ Static Members

### **When to Split into Subtopics (RARE)**

Only split when:

1. **File would be > 300 lines** and unwieldy
2. **Natural conceptual divisions** exist (compile-time vs runtime)
3. **Genuinely massive topic** like Polymorphism or Inheritance

**Examples that warrant splitting:**

```
polymorphism/
├── cpp/
│   ├── compile-time-polymorphism.cpp    # Function/operator overloading, templates
│   ├── runtime-polymorphism.cpp          # Virtual functions, inheritance
│   └── README.md                         # ONE README covering both

inheritance/
├── cpp/
│   ├── single-inheritance.cpp
│   ├── multiple-inheritance.cpp
│   ├── diamond-problem.cpp
│   └── README.md                         # ONE README covering all
```

### **README Strategy**

-   **Always ONE README per topic per language**
-   Even with multiple code files, write ONE unified README
-   README should reference all code files in that directory
-   Structure README with sections for each code file

### **Decision Matrix**

| Question                 | Action                  |
| ------------------------ | ----------------------- |
| Different OOP concept?   | ✅ New topic directory  |
| Main file > 300 lines?   | ✅ Split into subtopics |
| Natural divisions exist? | ✅ Split into subtopics |
| Just another example?    | ❌ Add to main file     |
| Helper/utility code?     | ❌ Include in main file |

---

## 🎯 EXAMPLE USAGE

### For a new topic (Interface in C++)

```
I need you to refactor and document my code for the topic: **Interface**

Language: cpp
Code Location: /mnt/data/learning/lld/oop-fundamentals/interface/cpp/interface.cpp

[Include the full prompt above]
```

### For adding a new language to existing topic

```
I need you to refactor and document my code for the topic: **Enum**

Language: java
Code Location: /mnt/data/learning/lld/oop-fundamentals/enum/java/enum.java

Note: I already have C++ and TypeScript documented. Please maintain consistency
with those implementations while highlighting Java-specific features.

[Include the full prompt above]
```

---

## 🔄 WORKFLOW INTEGRATION

This prompt is integrated into the workflow: `/refactor-and-document`

To use the workflow:

1. **Write your code** in the appropriate directory:

    - Format: `oop-fundamentals/[topic]/[language]/[topic].[ext]`
    - Example: `oop-fundamentals/polymorphism/cpp/polymorphism.cpp`

2. **Run the workflow**:

    ```
    /refactor-and-document
    ```

    Then provide the topic name and language when prompted.

3. **Review and iterate**:
    - Check the refactored code
    - Review the README.md
    - Request adjustments if needed

---

## 📋 QUICK REFERENCE

### Directory Structure

```
oop-fundamentals/
├── [topic]/
│   ├── cpp/
│   │   ├── [topic].cpp
│   │   └── README.md
│   ├── ts/
│   │   ├── [topic].ts
│   │   └── README.md
│   └── java/
│       ├── [topic].java
│       └── README.md
```

### Expected README Length

-   **200-250 lines** of comprehensive documentation
-   Theoretical + Practical balance
-   Multiple examples and comparisons

### Code File Structure

-   **100-150 lines** of well-commented code
-   5-7 major sections
-   Complete runnable examples
-   Demonstration/main function

---

## 💡 TIPS

1. **Before documenting**:

    - Ensure your code compiles
    - Run it to verify it works
    - Think about what concepts you want to highlight

2. **During documentation**:

    - Reference existing documented topics for consistency
    - Ask for specific examples if needed
    - Request comparisons with other languages

3. **After documentation**:
    - Read through the README
    - Run the refactored code
    - Check if all concepts are clear
    - Update if you discover new insights

---

## 🎓 TOPICS TO COVER

### **Phase 1: Fundamentals** (Start Here)

1. ✅ Enum (Complete)
2. 🚧 Class and Object (In Progress)
3. 🚧 Encapsulation (Access modifiers, getters/setters)
4. 🚧 Constructor & Destructor
5. 🚧 Static Members
6. 🚧 This Pointer (C++)

### **Phase 2: Advanced OOP Concepts**

7. 🚧 Operator Overloading (separate from interface!)
8. 🚧 Interface
9. 🚧 Abstract Classes
10. 🚧 Inheritance (may split: single, multiple, diamond)
11. 🚧 Polymorphism (split: compile-time, runtime)
12. 🚧 Composition
13. 🚧 Aggregation
14. 🚧 Association
15. 🚧 Friend Functions (C++)

### **Phase 3: SOLID Principles**

16. Single Responsibility Principle
17. Open/Closed Principle
18. Liskov Substitution Principle
19. Interface Segregation Principle
20. Dependency Inversion Principle

### **Phase 4: Design Patterns** (Future)

21. Creational Patterns
22. Structural Patterns
23. Behavioral Patterns

---

**Note**: Save this file for easy reference. You can copy the prompt template whenever you need to document a new topic!
