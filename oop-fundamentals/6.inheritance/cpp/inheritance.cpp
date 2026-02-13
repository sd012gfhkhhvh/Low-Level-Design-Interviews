#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Ref - https://algomaster.io/learn/lld/inheritance
// Ref - https://www.geeksforgeeks.org/inheritance-in-c/
// Ref - https://en.cppreference.com/w/cpp/language/derived_class

//
// =======================================================
// 1. WHAT IS INHERITANCE?
// =======================================================
//
// Inheritance allows one class (subclass/child) to inherit properties
// and behaviors of another class (superclass/parent).
//
// Key formula:
// Inheritance = Code Reuse + "is-a" Relationship + Hierarchy
//
// Benefits:
// - Code reusability (write once, use many)
// - Logical hierarchy (models real-world relationships)
// - Ease of maintenance (change once, affects all)
// - Enables polymorphism (runtime behavior)
//
// Mental Model:
// Think of a family tree: children inherit traits from parents,
// but can also have unique characteristics.

//
// =======================================================
// 2. BASIC SYNTAX & ACCESS SPECIFIERS
// =======================================================
//
// C++ supports three inheritance modes: public, protected, private
//
// class Derived : <access_specifier> Base { };
//
// Public inheritance (most common):
// - public members → public in derived
// - protected members → protected in derived
// - private members → not accessible
//
// Protected inheritance (rare):
// - public members → protected in derived
// - protected members → protected in derived
//
// Private inheritance (very rare):
// - public members → private in derived
// - protected members → private in derived

class Animal {
protected:
  std::string name;
  int age;

public:
  Animal(const std::string &n, int a) : name(n), age(a) {
    std::cout << "🐾 Animal constructor: " << name << "\n";
  }

  virtual ~Animal() { std::cout << "🐾 Animal destructor: " << name << "\n"; }

  virtual void makeSound() const { std::cout << name << " makes a sound\n"; }

  void sleep() const { std::cout << name << " is sleeping 💤\n"; }

  void displayInfo() const {
    std::cout << "Name: " << name << " | Age: " << age << "\n";
  }
};

class Dog : public Animal {
private:
  std::string breed;

public:
  Dog(const std::string &n, int a, const std::string &b)
      : Animal(n, a), breed(b) {
    std::cout << "🐕 Dog constructor: " << name << "\n";
  }

  ~Dog() { std::cout << "🐕 Dog destructor: " << name << "\n"; }

  // Override parent method
  void makeSound() const override {
    std::cout << name << " barks: Woof! Woof! 🐕\n";
  }

  // Dog-specific method
  void fetch() const { std::cout << name << " is fetching the ball! 🎾\n"; }

  void displayBreed() const { std::cout << name << " is a " << breed << "\n"; }
};

//
// =======================================================
// 3. TYPES OF INHERITANCE
// =======================================================
//
// 1. Single Inheritance: One child, one parent
// 2. Multi-level Inheritance: Chain of inheritance
// 3. Hierarchical Inheritance: Multiple children, one parent
// 4. Multiple Inheritance: One child, multiple parents (C++ only)

// ----- Single Inheritance (already shown above) -----

// ----- Multi-level Inheritance -----
class Vehicle {
protected:
  std::string make;
  int year;

public:
  Vehicle(const std::string &m, int y) : make(m), year(y) {}

  virtual void start() { std::cout << "🔑 Vehicle starting...\n"; }

  void displayInfo() const { std::cout << year << " " << make << "\n"; }
};

class Car : public Vehicle {
protected:
  int numDoors;

public:
  Car(const std::string &m, int y, int doors)
      : Vehicle(m, y), numDoors(doors) {}

  void start() override { std::cout << "🚗 Car engine starting...\n"; }

  void honk() const { std::cout << "🎺 Beep beep!\n"; }
};

// Multi-level: ElectricCar inherits from Car, which inherits from Vehicle
class ElectricCar : public Car {
private:
  int batteryCapacity;

public:
  ElectricCar(const std::string &m, int y, int doors, int battery)
      : Car(m, y, doors), batteryCapacity(battery) {}

  void start() override {
    std::cout << "⚡ Electric car starting silently...\n";
  }

  void charge() const {
    std::cout << "🔌 Charging " << batteryCapacity << " kWh battery\n";
  }
};

// ----- Hierarchical Inheritance -----
// Multiple classes inherit from same base

class Shape {
protected:
  std::string color;

public:
  Shape(const std::string &c) : color(c) {}

  virtual double getArea() const = 0; // Pure virtual

  virtual void display() const { std::cout << "Color: " << color << "\n"; }

  virtual ~Shape() = default;
};

class Circle : public Shape {
private:
  double radius;

public:
  Circle(const std::string &c, double r) : Shape(c), radius(r) {}

  double getArea() const override { return 3.14159 * radius * radius; }

  void display() const override {
    std::cout << "⭕ Circle | ";
    Shape::display();
    std::cout << "Area: " << getArea() << "\n";
  }
};

class Rectangle : public Shape {
private:
  double width, height;

public:
  Rectangle(const std::string &c, double w, double h)
      : Shape(c), width(w), height(h) {}

  double getArea() const override { return width * height; }

  void display() const override {
    std::cout << "▭ Rectangle | ";
    Shape::display();
    std::cout << "Area: " << getArea() << "\n";
  }
};

//
// =======================================================
// 4. MULTIPLE INHERITANCE
// =======================================================
//
// C++ allows a class to inherit from multiple base classes
// This can lead to the "Diamond Problem"

class Flyable {
public:
  virtual void fly() const { std::cout << "✈️  Flying in the air!\n"; }
  virtual ~Flyable() = default;
};

class Swimmable {
public:
  virtual void swim() const { std::cout << "🏊 Swimming in water!\n"; }
  virtual ~Swimmable() = default;
};

// Multiple inheritance: Duck can both fly and swim
class Duck : public Animal, public Flyable, public Swimmable {
public:
  Duck(const std::string &name, int age) : Animal(name, age) {}

  void makeSound() const override {
    std::cout << name << " quacks: Quack! Quack! 🦆\n";
  }
};

//
// =======================================================
// 5. THE DIAMOND PROBLEM & VIRTUAL INHERITANCE
// =======================================================
//
// Diamond Problem occurs when:
// - Class D inherits from B and C
// - Both B and C inherit from A
// - D gets two copies of A's members!
//
// Solution: Virtual inheritance

class PowerSource {
protected:
  int powerLevel;

public:
  PowerSource(int level = 100) : powerLevel(level) {
    std::cout << "⚡ PowerSource created (Level: " << powerLevel << ")\n";
  }

  void displayPower() const {
    std::cout << "Power Level: " << powerLevel << "%\n";
  }
};

// Virtual inheritance prevents duplicate PowerSource
class ElectricComponent : virtual public PowerSource {
public:
  ElectricComponent(int level = 100) : PowerSource(level) {
    std::cout << "🔌 ElectricComponent initialized\n";
  }
};

class BatteryComponent : virtual public PowerSource {
public:
  BatteryComponent(int level = 100) : PowerSource(level) {
    std::cout << "🔋 BatteryComponent initialized\n";
  }
};

// Without virtual: would have 2 PowerSource instances
// With virtual: only 1 PowerSource instance
class HybridDevice : public ElectricComponent, public BatteryComponent {
public:
  HybridDevice(int level = 100)
      : PowerSource(level), // Must initialize virtual base explicitly
        ElectricComponent(level), BatteryComponent(level) {
    std::cout << "🤖 HybridDevice ready\n";
  }
};

//
// =======================================================
// 6. CONSTRUCTOR & DESTRUCTOR CHAIN
// =======================================================
//
// Construction order: Base → Derived
// Destruction order: Derived → Base (reverse!)

class Base {
public:
  Base() { std::cout << "1️⃣  Base constructor\n"; }

  virtual ~Base() { std::cout << "3️⃣  Base destructor\n"; }
};

class Middle : public Base {
public:
  Middle() { std::cout << "2️⃣  Middle constructor\n"; }

  ~Middle() { std::cout << "2️⃣  Middle destructor\n"; }
};

class Derived : public Middle {
public:
  Derived() { std::cout << "3️⃣  Derived constructor\n"; }

  ~Derived() { std::cout << "1️⃣  Derived destructor\n"; }
};

//
// =======================================================
// 7. REAL-WORLD EXAMPLE: NOTIFICATION SYSTEM
// =======================================================
//

class Notification {
protected:
  std::string recipient;
  std::string message;
  std::string timestamp;

  std::string getCurrentTimestamp() const {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return std::string(buf);
  }

public:
  Notification(const std::string &r, const std::string &m)
      : recipient(r), message(m), timestamp(getCurrentTimestamp()) {}

  virtual ~Notification() = default;

  std::string formatHeader() const {
    return "[" + timestamp + "] To: " + recipient;
  }

  virtual void send() const {
    std::cout << formatHeader() << "\n";
    std::cout << "Message: " << message << "\n";
  }
};

class EmailNotification : public Notification {
private:
  std::string subject;

public:
  EmailNotification(const std::string &r, const std::string &m,
                    const std::string &s)
      : Notification(r, m), subject(s) {}

  void send() const override {
    std::cout << "\n📧 EMAIL NOTIFICATION\n";
    std::cout << formatHeader() << "\n";
    std::cout << "Subject: " << subject << "\n";
    std::cout << "Body: " << message << "\n";
    std::cout << "✅ Email delivered\n";
  }
};

class SMSNotification : public Notification {
private:
  std::string phoneNumber;
  static const int MAX_LENGTH = 160;

public:
  SMSNotification(const std::string &r, const std::string &m,
                  const std::string &phone)
      : Notification(r, m), phoneNumber(phone) {}

  void send() const override {
    std::cout << "\n📱 SMS NOTIFICATION\n";
    std::cout << formatHeader() << "\n";
    std::cout << "Phone: " << phoneNumber << "\n";

    std::string smsBody = message.length() > MAX_LENGTH
                              ? message.substr(0, MAX_LENGTH - 3) + "..."
                              : message;

    std::cout << "SMS: " << smsBody << "\n";
    std::cout << "✅ SMS sent (" << smsBody.length() << "/" << MAX_LENGTH
              << " chars)\n";
  }
};

class PushNotification : public Notification {
private:
  std::string deviceToken;
  std::string priority;

public:
  PushNotification(const std::string &r, const std::string &m,
                   const std::string &token, const std::string &prio)
      : Notification(r, m), deviceToken(token), priority(prio) {}

  void send() const override {
    std::cout << "\n🔔 PUSH NOTIFICATION\n";
    std::cout << formatHeader() << "\n";
    std::cout << "Device: " << deviceToken.substr(0, 8) << "...\n";
    std::cout << "Priority: " << priority << "\n";
    std::cout << "Alert: " << message << "\n";
    std::cout << "✅ Push delivered\n";
  }
};

//
// =======================================================
// 8. POLYMORPHISM THROUGH INHERITANCE
// =======================================================
//

void sendNotification(const Notification *notify) {
  notify->send(); // Polymorphic call!
}

//
// =======================================================
// 9. BEST PRACTICES
// =======================================================
//
// 1. Always use virtual destructors in base classes
// 2. Use 'override' keyword for clarity
// 3. Prefer composition over inheritance when possible
// 4. Keep inheritance hierarchies shallow (2-3 levels max)
// 5. Follow the "is-a" relationship test

//
// =======================================================
// 10. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Inheritance in C++ Demo ===\n\n";

  // ---- Basic Inheritance ----
  std::cout << "1. Basic Inheritance (Animal → Dog):\n";
  {
    Dog dog("Buddy", 3, "Golden Retriever");
    dog.makeSound();
    dog.sleep();
    dog.fetch();
    dog.displayBreed();
  }

  std::cout << "\n2. Multi-level Inheritance (Vehicle → Car → ElectricCar):\n";
  ElectricCar tesla("Tesla", 2024, 4, 100);
  tesla.start();
  tesla.charge();
  tesla.honk();

  // ---- Hierarchical Inheritance ----
  std::cout << "\n3. Hierarchical Inheritance (Shape → Circle, Rectangle):\n";
  Circle circle("Red", 5);
  Rectangle rect("Blue", 10, 20);

  circle.display();
  rect.display();

  // ---- Multiple Inheritance ----
  std::cout
      << "\n4. Multiple Inheritance (Duck: Animal + Flyable + Swimmable):\n";
  Duck duck("Donald", 2);
  duck.makeSound();
  duck.fly();
  duck.swim();

  // ---- Virtual Inheritance ----
  std::cout << "\n5. Virtual Inheritance (Diamond Problem Solution):\n";
  HybridDevice device(80);
  device.displayPower(); // Only ONE PowerSource instance!

  // ---- Constructor/Destructor Chain ----
  std::cout << "\n6. Constructor/Destructor Chain:\n";
  std::cout << "Creating Derived object:\n";
  {
    Derived d;
    std::cout << "Derived object exists here\n";
  }
  std::cout << "Derived object destroyed\n";

  // ---- Notification System ----
  std::cout << "\n7. Real-World Example (Notification System):\n";
  EmailNotification email("alice@example.com", "Your order has been shipped!",
                          "Order Update");
  email.send();

  SMSNotification sms("Bob", "Your verification code is 482910", "+1-555-0123");
  sms.send();

  PushNotification push("Charlie", "New message from Alice", "d8a3f4b2c1e5a9b7",
                        "high");
  push.send();

  // ---- Polymorphism ----
  std::cout << "\n8. Polymorphism Through Inheritance:\n";
  std::vector<Notification *> notifications = {&email, &sms, &push};

  for (Notification *notify : notifications) {
    sendNotification(notify);
  }

  // ---- Upcasting ----
  std::cout << "\n9. Upcasting (Derived → Base):\n";
  Dog myDog("Max", 5, "Labrador");
  Animal *animalPtr = &myDog; // Upcasting
  animalPtr->makeSound();     // Calls Dog's version (polymorphism!)

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Inheritance Types**:
   - Single: One parent
   - Multi-level: Chain of parents
   - Hierarchical: Multiple children
   - Multiple: Multiple parents (C++ only)

2. **Access Specifiers**:
   - Public inheritance: "is-a" relationship (most common)
   - Protected inheritance: Rare, "implemented-in-terms-of"
   - Private inheritance: Very rare, "implemented-using"

3. **Virtual Functions**:
   - Enable runtime polymorphism
   - Must use 'virtual' in base class
   - Use 'override' in derived class
   - Always virtual destructors!

4. **Diamond Problem**:
   - Occurs with multiple inheritance
   - Solution: Virtual inheritance
   - Prevents duplicate base class instances

5. **Constructor/Destructor Order**:
   - Construction: Base → Derived
   - Destruction: Derived → Base (reverse)

Best Practices:
- Use inheritance for "is-a" relationships
- Prefer composition for "has-a" relationships
- Keep hierarchies shallow (2-3 levels)
- Always virtual destructors in base classes
- Use 'override' keyword
- Follow Liskov Substitution Principle

When to Use Inheritance:
✅ Clear "is-a" relationship
✅ Shared behavior/data
✅ Code reuse within same family
✅ Polymorphism needed

When to Avoid:
❌ "has-a" relationship (use composition)
❌ Need runtime flexibility (use composition)
❌ Deep hierarchies
❌ Tight coupling concerns
*/