#include <iostream>
#include <string>

// Ref - https://www.geeksforgeeks.org/c-classes-and-objects/
// Ref - https://en.cppreference.com/w/cpp/language/classes

//
// =======================================================
// 1. WHAT ARE CLASSES AND OBJECTS?
// =======================================================
//
// A CLASS is a blueprint or template that defines the structure and behavior
// of objects. It encapsulates data (attributes) and functions (methods).
//
// An OBJECT is an instance of a class - a concrete entity created from
// the class blueprint.
//
// Key relationship: Class = Blueprint, Object = House built from blueprint

//
// =======================================================
// 2. BASIC CLASS DEFINITION
// =======================================================
//

class BasicCar {
private:
  // Data members (attributes) - typically private for encapsulation
  std::string brand;
  std::string model;
  int speed;

public:
  // Member functions (methods) - typically public for interface
  void setBrand(const std::string &b) { brand = b; }
  void setSpeed(int s) { speed = s; }

  void displayInfo() const {
    std::cout << brand << " " << model << " at " << speed << " km/h\n";
  }
};

//
// =======================================================
// 3. CONSTRUCTORS AND DESTRUCTORS
// =======================================================
//
// Constructor: Special method called when object is created
// Destructor: Special method called when object is destroyed

class Car {
private:
  std::string brand;
  std::string model;
  int speed;
  int *mileage; // Pointer to demonstrate destructor

public:
  // Default constructor
  Car() : brand("Unknown"), model("Unknown"), speed(0), mileage(new int(0)) {
    std::cout << "🚗 Default constructor called\n";
  }

  // Parameterized constructor with member initializer list (preferred)
  Car(const std::string &b, const std::string &m)
      : brand(b), model(m), speed(0), mileage(new int(0)) {
    std::cout << "🚗 Parameterized constructor for " << brand << " " << model
              << "\n";
  }

  // Destructor - cleanup resources
  ~Car() {
    std::cout << "🔧 Destructor called for " << brand << " " << model << "\n";
    delete mileage; // Free dynamically allocated memory
  }

  void accelerate(int increment) { speed += increment; }

  void addMileage(int km) { *mileage += km; }

  int getMileage() const { return *mileage; }

  void displayStatus() const {
    std::cout << brand << " " << model << " | Speed: " << speed
              << " km/h | Mileage: " << *mileage << " km\n";
  }
};

//
// =======================================================
// 4. THE 'this' POINTER
// =======================================================
//
// 'this' is a pointer to the current object
// Used to:
//   1. Resolve naming conflicts
//   2. Return the object itself (for method chaining)
//   3. Pass the object to other functions

class Rectangle {
private:
  int width;
  int height;

public:
  // Using 'this' to resolve naming conflict
  Rectangle(int width, int height) {
    this->width = width;   // 'this->width' refers to member variable
    this->height = height; // 'width' alone would refer to parameter
  }

  // Method chaining: return *this
  Rectangle &setWidth(int w) {
    this->width = w;
    return *this; // Return reference to current object
  }

  Rectangle &setHeight(int h) {
    this->height = h;
    return *this;
  }

  int area() const { return this->width * this->height; }

  void display() const {
    std::cout << "Rectangle [" << width << "x" << height << "] Area: " << area()
              << "\n";
  }
};

//
// =======================================================
// 5. ACCESS SPECIFIERS
// =======================================================
//
// public:    Accessible from anywhere
// private:   Accessible only within the class (default for class)
// protected: Accessible within class and derived classes

class BankAccount {
private:
  std::string accountNumber; // Private - cannot access directly
  double balance;            // Private - encapsulated

protected:
  std::string accountType; // Protected - for inheritance

public:
  BankAccount(const std::string &accNum, double initialBalance)
      : accountNumber(accNum), balance(initialBalance), accountType("Savings") {
  }

  // Getter methods (accessors)
  std::string getAccountNumber() const { return accountNumber; }
  double getBalance() const { return balance; }

  // Setter methods (mutators) with validation
  void deposit(double amount) {
    if (amount > 0) {
      balance += amount;
      std::cout << "💰 Deposited $" << amount << "\n";
    }
  }

  void withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
      balance -= amount;
      std::cout << "💸 Withdrawn $" << amount << "\n";
    } else {
      std::cout << "❌ Insufficient funds!\n";
    }
  }

  void displayAccount() const {
    std::cout << "Account: " << accountNumber << " | Balance: $" << balance
              << "\n";
  }
};

//
// =======================================================
// 6. STATIC MEMBERS
// =======================================================
//
// Static members belong to the CLASS, not to individual objects
// Shared across all instances

class Counter {
private:
  int id;
  static int objectCount; // Static data member (shared)

public:
  Counter() : id(++objectCount) {
    std::cout << "🔢 Counter object #" << id << " created\n";
  }

  ~Counter() {
    --objectCount;
    std::cout << "🔢 Counter object #" << id << " destroyed\n";
  }

  // Static member function (can only access static members)
  static int getCount() { return objectCount; }

  void displayId() const { std::cout << "My ID: " << id << "\n"; }
};

// Static member definition (required outside class)
int Counter::objectCount = 0;

//
// =======================================================
// 7. CONST CORRECTNESS
// =======================================================
//
// Best practices for const usage in classes

class ConstDemo {
private:
  int value;
  mutable int accessCount; // Can be modified even in const functions

public:
  ConstDemo(int v) : value(v), accessCount(0) {}

  // Const member function - promises not to modify object state
  int getValue() const {
    ++accessCount; // OK: mutable member can be modified
    return value;
  }

  // Non-const member function - can modify object
  void setValue(int v) { value = v; }

  // Const parameter - prevents modification
  void process(const std::string &data) {
    // data cannot be modified here
    std::cout << "Processing: " << data << "\n";
  }

  int getAccessCount() const { return accessCount; }
};

/*
📘 Const Correctness Best Practices:

1. Use const for parameters passed by reference when not modifying them
   Example: const std::string& brand
   Benefits: Prevents accidental changes, avoids costly copies

2. Mark member functions as const if they don't change object state
   Example: displayStatus() const
   Benefits: Enforces read-only behavior, allows calls on const objects

3. DO NOT mark functions const if they modify data members
   Example: accelerate() should NOT be const (modifies speed)

4. Pass small fundamental types by value, not by const reference
   Example: int increment (not const int& increment)
   Reason: Simpler, clearer, more efficient for small types

Rule of Thumb:
- Pass small types (int, char, bool) by value
- Pass large types (string, vector, objects) by const reference
- Use const wherever modification is not intended
*/

//
// =======================================================
// 8. OBJECT LIFECYCLE
// =======================================================
//

class Resource {
private:
  int *data;
  std::string name;

public:
  // Constructor
  Resource(const std::string &n, int value) : name(n), data(new int(value)) {
    std::cout << "📦 Resource '" << name << "' created\n";
  }

  // Copy constructor (deep copy)
  Resource(const Resource &other)
      : name(other.name + "_copy"), data(new int(*other.data)) {
    std::cout << "📋 Copy constructor called for " << name << "\n";
  }

  // Copy assignment operator
  Resource &operator=(const Resource &other) {
    if (this != &other) {
      std::cout << "📝 Copy assignment for " << name << "\n";
      delete data;
      name = other.name + "_assigned";
      data = new int(*other.data);
    }
    return *this;
  }

  // Destructor
  ~Resource() {
    std::cout << "🗑️  Resource '" << name << "' destroyed\n";
    delete data;
  }

  void display() const { std::cout << name << " -> " << *data << "\n"; }

  void setValue(int val) { *data = val; }
};

//
// =======================================================
// 9. REAL-WORLD EXAMPLE: ENHANCED CAR CLASS
// =======================================================
//

class Vehicle {
private:
  std::string brand;
  std::string model;
  int year;
  double speed;
  double fuel;
  static int totalVehicles;

public:
  // Constructor with default parameters
  Vehicle(const std::string &b, const std::string &m, int y = 2024)
      : brand(b), model(m), year(y), speed(0.0), fuel(100.0) {
    ++totalVehicles;
  }

  ~Vehicle() { --totalVehicles; }

  // Method chaining for fluent interface
  Vehicle &accelerate(double increment) {
    if (fuel > 0) {
      speed += increment;
      fuel -= increment * 0.1;
    }
    return *this;
  }

  Vehicle &brake(double decrement) {
    speed = std::max(0.0, speed - decrement);
    return *this;
  }

  Vehicle &refuel(double amount) {
    fuel = std::min(100.0, fuel + amount);
    return *this;
  }

  void displayStatus() const {
    std::cout << "🚙 " << year << " " << brand << " " << model << "\n"
              << "   Speed: " << speed << " km/h | Fuel: " << fuel << "%\n";
  }

  static int getTotalVehicles() { return totalVehicles; }
};

int Vehicle::totalVehicles = 0;

//
// =======================================================
// 10. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Classes and Objects in C++ Demo ===\n\n";

  // ---- Basic class usage ----
  std::cout << "1. Basic Class:\n";
  BasicCar basic;
  basic.setBrand("Honda");
  basic.setSpeed(80);
  basic.displayInfo();

  std::cout << "\n2. Constructors and Destructors:\n";
  {
    Car car1("Toyota", "Camry");
    car1.accelerate(60);
    car1.addMileage(150);
    car1.displayStatus();
  } // car1 destroyed here - destructor called

  std::cout << "\n3. This Pointer and Method Chaining:\n";
  Rectangle rect(10, 20);
  rect.setWidth(15).setHeight(25).display(); // Chaining!

  std::cout << "\n4. Access Specifiers and Encapsulation:\n";
  BankAccount account("ACC123456", 1000.0);
  account.deposit(500);
  account.withdraw(200);
  account.displayAccount();
  // account.balance = 999999; // ❌ Error: private member

  std::cout << "\n5. Static Members:\n";
  std::cout << "Initial count: " << Counter::getCount() << "\n";
  {
    Counter c1, c2, c3;
    std::cout << "Count after creation: " << Counter::getCount() << "\n";
  } // c1, c2, c3 destroyed
  std::cout << "Count after destruction: " << Counter::getCount() << "\n";

  std::cout << "\n6. Const Correctness:\n";
  ConstDemo demo(42);
  std::cout << "Value: " << demo.getValue() << "\n";
  std::cout << "Access count: " << demo.getAccessCount() << "\n";
  demo.setValue(100);
  demo.process("Important Data");

  std::cout << "\n7. Object Lifecycle (Copy):\n";
  Resource res1("Original", 42);
  Resource res2 = res1; // Copy constructor
  res2.setValue(99);
  res1.display();
  res2.display();

  std::cout << "\n8. Real-World Example (Method Chaining):\n";
  Vehicle tesla("Tesla", "Model 3", 2024);
  tesla.accelerate(50).accelerate(30).brake(20).displayStatus();

  Vehicle toyota("Toyota", "Prius");
  toyota.accelerate(40).refuel(20).displayStatus();

  std::cout << "\nTotal vehicles: " << Vehicle::getTotalVehicles() << "\n";

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}