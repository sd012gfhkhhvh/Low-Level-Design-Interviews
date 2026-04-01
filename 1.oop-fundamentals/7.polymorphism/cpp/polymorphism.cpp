#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Ref - https://algomaster.io/learn/lld/polymorphism
// Ref - https://www.geeksforgeeks.org/polymorphism-in-c/
// Ref - https://en.cppreference.com/w/cpp/language/virtual

//
// =======================================================
// 1. WHAT IS POLYMORPHISM?
// =======================================================
//
// Polymorphism = "Many forms"
// It allows the same interface to be used ford different underlying forms (data
// types).
//
// Key formula:
// Polymorphism = Same Interface + Different Behaviors
//
// Benefits:
// - Code reusability (write generic code)
// - Flexibility (add new types without changing existing code)
// - Loose coupling (depend on abstractions, not concrete types)
// - Extensibility (easily add new implementations)
//
// Types in C++:
// 1. Compile-time Polymorphism (Static/Early Binding)
//    - Function Overloading
//    - Operator Overloading
//    - Templates
//
// 2. Runtime Polymorphism (Dynamic/Late Binding)
//    - Virtual Functions
//    - Function Overriding

//
// =======================================================
// 2. COMPILE-TIME POLYMORPHISM: FUNCTION OVERLOADING
// =======================================================
//
// Multiple functions with same name but different parameters
// Compiler decides which function to call based on arguments

class Calculator {
public:
  // Overload 1: Two integers
  int add(int a, int b) {
    std::cout << "📊 add(int, int)\n";
    return a + b;
  }

  // Overload 2: Three integers
  int add(int a, int b, int c) {
    std::cout << "📊 add(int, int, int)\n";
    return a + b + c;
  }

  // Overload 3: Two doubles
  double add(double a, double b) {
    std::cout << "📊 add(double, double)\n";
    return a + b;
  }

  // Overload 4: String concatenation
  std::string add(const std::string &a, const std::string &b) {
    std::cout << "📊 add(string, string)\n";
    return a + b;
  }
};

//
// =======================================================
// 3. COMPILE-TIME POLYMORPHISM: OPERATOR OVERLOADING
// =======================================================
//
// Redefine how operators work for custom types

class Point {
private:
  double x, y;

public:
  Point(double x = 0, double y = 0) : x(x), y(y) {}

  // Overload + operator
  Point operator+(const Point &other) const {
    return Point(x + other.x, y + other.y);
  }

  // Overload == operator
  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }

  // Overload << for output
  friend std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << "(" << p.x << ", " << p.y << ")";
    return os;
  }

  double getX() const { return x; }
  double getY() const { return y; }
};

//
// =======================================================
// 4. RUNTIME POLYMORPHISM: VIRTUAL FUNCTIONS
// =======================================================
//
// Virtual functions enable dynamic dispatch
// The function called is determined at RUNTIME based on actual object type

class Shape {
protected:
  std::string color;

public:
  Shape(const std::string &c) : color(c) {}

  // Virtual function - can be overridden
  virtual double getArea() const { return 0.0; }

  virtual void draw() const {
    std::cout << "Drawing a shape with color: " << color << "\n";
  }

  // Virtual destructor - ALWAYS use for base classes!
  virtual ~Shape() { std::cout << "~Shape() destructor\n"; }
};

class Circle : public Shape {
private:
  double radius;

public:
  Circle(const std::string &c, double r) : Shape(c), radius(r) {}

  // Override base class method
  double getArea() const override { return 3.14159 * radius * radius; }

  void draw() const override {
    std::cout << "⭕ Drawing Circle | Color: " << color
              << " | Area: " << getArea() << "\n";
  }

  ~Circle() { std::cout << "~Circle() destructor\n"; }
};

class Rectangle : public Shape {
private:
  double width, height;

public:
  Rectangle(const std::string &c, double w, double h)
      : Shape(c), width(w), height(h) {}

  double getArea() const override { return width * height; }

  void draw() const override {
    std::cout << "▭ Drawing Rectangle | Color: " << color
              << " | Area: " << getArea() << "\n";
  }

  ~Rectangle() { std::cout << "~Rectangle() destructor\n"; }
};

//
// =======================================================
// 5. HOW RUNTIME POLYMORPHISM WORKS: vtable & vptr
// =======================================================
//
// When a class has virtual functions, the compiler creates:
// - vtable (virtual table): Array of function pointers for that class
// - vptr (virtual pointer): Pointer in each object to its class's vtable
//
// Runtime dispatch:
// 1. basePtr->virtualFunc()
// 2. Follow basePtr's vptr to vtable
// 3. Look up virtualFunc in vtable
// 4. Call the correct overridden version!
//
// This happens at RUNTIME, not compile time.

//
// =======================================================
// 6. PURE VIRTUAL FUNCTIONS & ABSTRACT CLASSES
// =======================================================
//
// Pure virtual function: = 0
// Abstract class: Has at least one pure virtual function
// Cannot instantiate abstract classes!

class Animal {
protected:
  std::string name;

public:
  Animal(const std::string &n) : name(n) {}

  // Pure virtual function
  virtual void makeSound() const = 0;

  // Regular virtual function
  virtual void sleep() const { std::cout << name << " is sleeping 💤\n"; }

  virtual ~Animal() = default;
};

class Dog : public Animal {
public:
  Dog(const std::string &n) : Animal(n) {}

  void makeSound() const override {
    std::cout << name << " says: Woof! Woof! 🐕\n";
  }
};

class Cat : public Animal {
public:
  Cat(const std::string &n) : Animal(n) {}

  void makeSound() const override {
    std::cout << name << " says: Meow! Meow! 🐱\n";
  }
};

//
// =======================================================
// 7. REAL-WORLD EXAMPLE: PAYMENT SYSTEM
// =======================================================
//

class PaymentMethod {
protected:
  std::string accountId;

public:
  PaymentMethod(const std::string &id) : accountId(id) {}

  virtual bool processPayment(double amount) = 0;
  virtual std::string getType() const = 0;
  virtual ~PaymentMethod() = default;
};

class CreditCard : public PaymentMethod {
private:
  std::string cardNumber;

public:
  CreditCard(const std::string &id, const std::string &card)
      : PaymentMethod(id), cardNumber(card) {}

  bool processPayment(double amount) override {
    std::cout << "💳 Processing $" << amount << " via Credit Card\n";
    std::cout << "   Card: " << cardNumber.substr(0, 4) << "****\n";
    return true;
  }

  std::string getType() const override { return "Credit Card"; }
};

class PayPal : public PaymentMethod {
private:
  std::string email;

public:
  PayPal(const std::string &id, const std::string &mail)
      : PaymentMethod(id), email(mail) {}

  bool processPayment(double amount) override {
    std::cout << "💰 Processing $" << amount << " via PayPal\n";
    std::cout << "   Email: " << email << "\n";
    return true;
  }

  std::string getType() const override { return "PayPal"; }
};

class Cryptocurrency : public PaymentMethod {
private:
  std::string walletAddress;

public:
  Cryptocurrency(const std::string &id, const std::string &wallet)
      : PaymentMethod(id), walletAddress(wallet) {}

  bool processPayment(double amount) override {
    std::cout << "₿ Processing $" << amount << " via Cryptocurrency\n";
    std::cout << "   Wallet: " << walletAddress.substr(0, 8) << "...\n";
    return true;
  }

  std::string getType() const override { return "Cryptocurrency"; }
};

//
// =======================================================
// 8. POLYMORPHIC BEHAVIOR IN ACTION
// =======================================================
//

void processTransaction(PaymentMethod *method, double amount) {
  std::cout << "\n--- Processing Transaction ---\n";
  std::cout << "Payment Type: " << method->getType() << "\n";
  method->processPayment(amount);
  std::cout << "✅ Transaction complete!\n";
}

//
// =======================================================
// 9. FUNCTION OVERRIDING VS OVERLOADING
// =======================================================
//
// OVERLOADING (Compile-time):
// - Same function name, different parameters
// - In same class
// - Resolved at compile time
//
// OVERRIDING (Runtime):
// - Same function signature in base and derived
// - Requires 'virtual' in base, 'override' in derived
// - Resolved at runtime

class Demo {
public:
  // Overloading - same name, different parameters
  void display(int x) { std::cout << "Integer: " << x << "\n"; }

  void display(double x) { std::cout << "Double: " << x << "\n"; }

  void display(const std::string &x) { std::cout << "String: " << x << "\n"; }
};

//
// =======================================================
// 10. SLICING PROBLEM & WHY USE POINTERS/REFERENCES
// =======================================================
//
// Object slicing occurs when you assign a derived object to a base object
// The derived-specific data is "sliced off"

void demonstrateSlicing() {
  std::cout << "\n--- Demonstrating Object Slicing ---\n";

  Circle circle("Red", 5);

  // ❌ Slicing: Derived object assigned to base object
  Shape shape = circle; // Circle-specific data lost!
  shape.draw();         // Calls Shape::draw(), not Circle::draw()

  std::cout << "\n--- Using Polymorphism (Pointer/Reference) ---\n";

  // ✅ Polymorphism: Base pointer to derived object
  Shape *shapePtr = &circle;
  shapePtr->draw(); // Calls Circle::draw() (polymorphic!)

  // ✅ Polymorphism: Base reference to derived object
  Shape &shapeRef = circle;
  shapeRef.draw(); // Calls Circle::draw() (polymorphic!)
}

//
// =======================================================
// 11. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Polymorphism in C++ Demo ===\n\n";

  // ---- Compile-time Polymorphism: Function Overloading ----
  std::cout << "1. Function Overloading (Compile-time):\n";
  Calculator calc;
  std::cout << "Result: " << calc.add(5, 3) << "\n";
  std::cout << "Result: " << calc.add(5, 3, 2) << "\n";
  std::cout << "Result: " << calc.add(5.5, 3.2) << "\n";
  std::cout << "Result: "
            << calc.add(std::string("Hello"), std::string(" World")) << "\n";

  // ---- Compile-time Polymorphism: Operator Overloading ----
  std::cout << "\n2. Operator Overloading (Compile-time):\n";
  Point p1(3, 4);
  Point p2(1, 2);
  Point p3 = p1 + p2;
  std::cout << "p1 = " << p1 << "\n";
  std::cout << "p2 = " << p2 << "\n";
  std::cout << "p3 = p1 + p2 = " << p3 << "\n";
  std::cout << "p1 == p2? " << (p1 == p2 ? "Yes" : "No") << "\n";

  // ---- Runtime Polymorphism: Virtual Functions ----
  std::cout << "\n3. Runtime Polymorphism (Virtual Functions):\n";
  {
    Circle circle("Blue", 5);
    Rectangle rect("Green", 10, 20);

    // Polymorphic behavior through base class pointers
    std::vector<Shape *> shapes = {&circle, &rect};

    for (Shape *shape : shapes) {
      shape->draw(); // Calls correct derived version!
    }
  }

  // ---- Pure Virtual Functions & Abstract Classes ----
  std::cout << "\n4. Abstract Classes (Pure Virtual Functions):\n";
  Dog dog("Buddy");
  Cat cat("Whiskers");

  // Animal* animal = new Animal("Generic");  // ❌ Error: cannot instantiate

  std::vector<Animal *> animals = {&dog, &cat};
  for (Animal *animal : animals) {
    animal->makeSound();
    animal->sleep();
  }

  // ---- Real-World Example: Payment System ----
  std::cout << "\n5. Real-World Example (Payment System):\n";
  CreditCard creditCard("ACC001", "1234567890123456");
  PayPal paypal("ACC002", "user@example.com");
  Cryptocurrency crypto("ACC003", "0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb");

  std::vector<PaymentMethod *> payments = {&creditCard, &paypal, &crypto};

  for (PaymentMethod *payment : payments) {
    processTransaction(payment, 100.00);
  }

  // ---- Function Overloading vs Overriding ----
  std::cout << "\n6. Function Overloading (Compile-time):\n";
  Demo demo;
  demo.display(42);
  demo.display(3.14);
  demo.display("Hello");

  // ---- Object Slicing Problem ----
  demonstrateSlicing();

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Two Types of Polymorphism**:
   - Compile-time: Function/Operator overloading, Templates (static dispatch)
   - Runtime: Virtual functions, Method overriding (dynamic dispatch)

2. **Virtual Functions**:
   - Use 'virtual' keyword in base class
   - Use 'override' keyword in derived class
   - Enables runtime polymorphism
   - Implemented through vtable mechanism

3. **Pure Virtual Functions**:
   - Declared with = 0
   - Makes class abstract (cannot instantiate)
   - Must be implemented by derived classes

4. **Virtual Destructor**:
   - ALWAYS make destructor virtual in base classes
   - Prevents memory leaks when deleting through base pointer
   - Critical for proper cleanup

5. **Object Slicing**:
   - Occurs when assigning derived object to base object
   - Use pointers/references to avoid slicing
   - Pointers/references enable polymorphism

6. **vtable & vptr**:
   - vtable: Array of function pointers per class
   - vptr: Pointer in each object to its vtable
   - Runtime overhead minimal, benefits huge

Best Practices:
- Always virtual destructor in base classes
- Use 'override' keyword for clarity
- Prefer pointers/references for polymorphic behavior
- Use pure virtual for interface-like classes
- Program to abstractions, not implementations

When to Use:
✅ Need different behaviors for related types
✅ Want to add new types without changing existing code
✅ Building extensible systems
✅ Implementing design patterns (Strategy, Factory, etc.)

Compile-time vs Runtime:
- Compile-time: Faster, decided at compile time
- Runtime: Flexible, decided at runtime, slight overhead
*/