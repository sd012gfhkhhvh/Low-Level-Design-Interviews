---
description: Refactor and document a specific OOP topic with comprehensive study guide
---

# Refactor and Document OOP Topic Workflow

This workflow helps you transform raw code examples into a complete, well-documented study guide for your OOP learning journey.

## Prerequisites

Before running this workflow, ensure you have:

-   Written the code example(s) for the topic in the target language(s)
-   Placed the code in the correct directory structure: `oop-fundamentals/[topic]/[language]/`
-   Named your code file appropriately (e.g., `topic.cpp`, `topic.ts`, `topic.java`)

## Architecture Guidelines

### **Core Principle**

> One topic = One comprehensive code file + One README per language

### **When to Use ONE File (Most Cases)**

Default to a single comprehensive file for most topics:

-   Encapsulation, Interface, Abstract Classes, Enum
-   Operator Overloading, Composition, Aggregation
-   Constructor/Destructor, Static Members, etc.

Your `enum` examples demonstrate this perfectly!

### **When to Split into Subtopics (Rare Exceptions)**

Only split when:

1. File would exceed ~300 lines
2. Natural conceptual divisions exist (e.g., compile-time vs runtime polymorphism)
3. Topic is genuinely massive (Polymorphism, Inheritance)

**Example:**

```
polymorphism/cpp/
├── compile-time-polymorphism.cpp
├── runtime-polymorphism.cpp
└── README.md  # ONE README covering both
```

### **README Strategy**

-   **Always ONE README per topic per language**
-   Even with multiple code files, maintain a single unified README
-   README should reference and explain all code files in that directory

### **Important Notes**

-   ❌ Don't mix unrelated concepts (e.g., operator-overloading in interface/)
-   ✅ Each distinct OOP concept gets its own topic directory
-   ✅ Keep files focused and comprehensive

## Workflow Steps

### 1. Identify the Topic and Language

Determine which topic you want to refactor and document, and which language(s) you've written code for.

**Verify topic is correctly structured:**

-   Is this a distinct OOP concept? → Own directory
-   Does it belong to another topic? → Move it first
-   Should it be split into subtopics? → Rare, check guidelines above

Example:

-   Topic: `interface`
-   Languages: `cpp`, `ts`, `java`
-   Files: `interface.cpp` (single file, comprehensive)

### 2. Invoke the AI with the Documentation Prompt

Use the comprehensive documentation prompt (see below) to refactor and document your code.

You should provide:

-   The topic name
-   The language(s) you've written code for
-   The file path(s) to your code

### 3. Review the Generated Documentation

The AI will:

-   Refactor your code with:
    -   Clear section headers
    -   Descriptive comments
    -   Multiple practical examples
    -   Best practices and anti-patterns
-   Generate a comprehensive README.md with:
    -   Theoretical explanation
    -   Code examples with explanations
    -   Comparison tables
    -   Mental models and takeaways
    -   References to authoritative sources

### 4. Iterate if Needed

If you need adjustments:

-   Ask for more examples
-   Request clarification on specific concepts
-   Add comparisons with other languages
-   Include more edge cases or practical scenarios

## The Documentation Prompt

Use the following prompt template when requesting refactoring and documentation:

---

**TOPIC REFACTORING AND DOCUMENTATION REQUEST**

I need you to refactor and document my code for the topic: **[TOPIC_NAME]**

**Language**: [cpp/ts/java/etc.]

**Code Location**: `[PATH_TO_CODE_FILE]`

Please analyze my existing code and:

1. **Refactor the code** following the style and structure demonstrated in `oop-fundamentals/enum/`:

    - Add clear section markers with visual separators (=======)
    - Include numbered sections (1., 2., 3., etc.)
    - Add descriptive comments explaining each concept
    - Show multiple practical examples (basic, intermediate, advanced)
    - Include comparisons with other language styles when relevant
    - Add a complete demonstration/main function showing all concepts in action

2. **Create a comprehensive README.md** in the same directory following this structure:

    **Title**: [Topic] in [Language] — A Complete Practical Guide

    **Sections to include**:

    - Brief introduction explaining what the topic is
    - When and why to use it
    - Multiple numbered sections covering:
        - Basic concepts
        - Common patterns
        - Advanced usage
        - Comparisons with other languages (Java/C++/TypeScript)
        - Best practices
        - Common pitfalls and anti-patterns
        - Real-world use cases
    - Summary table comparing different approaches
    - Final takeaways with core insights
    - Reference links to authoritative sources (prefer: GeeksforGeeks, official docs, MDN, cppreference, TypeScript handbook)

3. **Formatting Guidelines**:

    - Use markdown formatting with code blocks
    - Include emoji indicators: ✅ (good), ❌ (bad), ⚠️ (warning)
    - Use blockquotes (>) for key insights and mental models
    - Create comparison tables when showing different approaches
    - Keep explanations concise but comprehensive
    - Use practical, relatable examples (like coins, wallets, real-world objects)

4. **Quality Standards**:
    - All code must compile and run without errors
    - Comments should explain "why" not just "what"
    - Cover edge cases and common mistakes
    - Show the evolution from basic to advanced usage
    - Compare with equivalent patterns in other OOP languages
    - Include theoretical context where it aids understanding

**Important**: Follow the exact style, depth, and comprehensiveness demonstrated in:

-   `oop-fundamentals/enum/cpp/enum.cpp`
-   `oop-fundamentals/enum/cpp/README.md`
-   `oop-fundamentals/enum/ts/enum.ts`
-   `oop-fundamentals/enum/ts/README.md`

---

## Expected Output

After running this workflow, you should have:

1. **Refactored code file** with:

    - Clear sectioning and structure
    - Comprehensive comments
    - Multiple examples
    - Runnable demonstration code

2. **README.md** with:
    - 200-250 lines of comprehensive documentation
    - Theoretical explanations
    - Practical examples
    - Comparison tables
    - Key takeaways
    - References

## Example Usage

```bash
# After writing your code in interface/cpp/interface.cpp, say:

"Please refactor and document the interface topic in C++.
Code location: /mnt/data/learning/lld/oop-fundamentals/interface/cpp/interface.cpp

Follow the documentation prompt from the workflow."
```

## Tips

-   **Write code first, document second**: Focus on implementing your understanding first
-   **Be specific**: The more context you provide about what you want to learn, the better the documentation
-   **Iterate**: Don't hesitate to ask for more examples, clarifications, or comparisons
-   **Cross-reference**: When documenting a new language, reference existing documented languages for consistency
-   **Quality over quantity**: It's better to have fewer topics deeply documented than many topics superficially covered
