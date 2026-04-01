#include <iostream>
#include <string>
#include <vector>

// Ref -
// https://www.geeksforgeeks.org/cpp/cpp-program-to-create-an-interface/ Ref
// - https://www.scaler.com/topics/cpp/abstract-class-in-cpp/

//
// =======================================================
// 1. WHAT IS AN INTERFACE IN C++?
// =======================================================
//
// C++ doesn't have a dedicated "interface" keyword like Java.
// Instead, we use **pure abstract classes** to achieve the same goal.
//
// A pure abstract class is a class that:
//   - Contains ONLY pure virtual functions (= 0)
//   - Has NO data members (convention, not enforced)
//   - Has a virtual destructor
//   - Cannot be instantiated directly
//
// Purpose: Define a CONTRACT that derived classes must implement

//
// =======================================================
// 2. BASIC INTERFACE EXAMPLE
// =======================================================
//

// Interface: Defines what operations are possible
class Drawable {
public:
  virtual ~Drawable() {}         // Always include virtual destructor
  virtual void draw() const = 0; // Pure virtual function
};

// Concrete implementation 1
class Circle : public Drawable {
public:
  void draw() const override { std::cout << "Drawing a Circle ⭕️\n"; }
};

// Concrete implementation 2
class Rectangle : public Drawable {
public:
  void draw() const override { std::cout << "Drawing a Rectangle ▭\n"; }
};

//
// =======================================================
// 3. MULTIPLE INTERFACES (C++ Supports Multiple Inheritance)
// =======================================================
//
// Unlike Java (before Java 8), C++ allows a class to inherit
// from multiple interfaces without issues (when done right)

class Movable {
public:
  virtual ~Movable() {}
  virtual void move(int x, int y) = 0;
};

class Resizable {
public:
  virtual ~Resizable() {}
  virtual void resize(double factor) = 0;
};

// A class implementing multiple interfaces
class Shape : public Drawable, public Movable, public Resizable {
private:
  std::string name_;
  int x_ = 0, y_ = 0;
  double size_ = 1.0;

public:
  explicit Shape(const std::string &name) : name_(name) {}

  void draw() const override {
    std::cout << "Drawing " << name_ << " at (" << x_ << "," << y_ << ")"
              << " size=" << size_ << "\n";
  }

  void move(int x, int y) override {
    x_ = x;
    y_ = y;
    std::cout << name_ << " moved to (" << x << "," << y << ")\n";
  }

  void resize(double factor) override {
    size_ *= factor;
    std::cout << name_ << " resized by factor " << factor << "\n";
  }
};

//
// =======================================================
// 4. INTERFACE SEGREGATION PRINCIPLE (ISP)
// =======================================================
//
// "Clients should not be forced to depend on methods they don't use"
//
// ❌ BAD: Fat interface forcing unnecessary implementations
class BadAnimal {
public:
  virtual ~BadAnimal() {}
  virtual void walk() = 0;
  virtual void fly() = 0;
  virtual void swim() = 0;
};
// Problem: A dog must implement fly(), a bird must implement swim()

// ✅ GOOD: Segregated interfaces
class Walkable {
public:
  virtual ~Walkable() {}
  virtual void walk() = 0;
};

class Flyable {
public:
  virtual ~Flyable() {}
  virtual void fly() = 0;
};

class Swimmable {
public:
  virtual ~Swimmable() {}
  virtual void swim() = 0;
};

// Now we can compose exactly what we need
class Dog : public Walkable, public Swimmable {
public:
  void walk() override { std::cout << "🐕 Dog is walking\n"; }
  void swim() override { std::cout << "🐕 Dog is swimming\n"; }
};

class Duck : public Walkable, public Flyable, public Swimmable {
public:
  void walk() override { std::cout << "🦆 Duck is walking\n"; }
  void fly() override { std::cout << "🦆 Duck is flying\n"; }
  void swim() override { std::cout << "🦆 Duck is swimming\n"; }
};

//
// =======================================================
// 5. POLYMORPHISM WITH INTERFACES
// =======================================================
//
// The power of interfaces: program to the interface, not implementation

void renderShapes(const std::vector<Drawable *> &shapes) {
  std::cout << "\n--- Rendering all shapes ---\n";
  for (const auto *shape : shapes) {
    shape->draw(); // Polymorphic call
  }
}

//
// =======================================================
// 6. REAL-WORLD EXAMPLE: PAYMENT GATEWAY
// =======================================================
//
// Strategy Pattern using interfaces

class PaymentGateway {
public:
  virtual ~PaymentGateway() {}
  virtual void initiatePayment(double amount) = 0;
  virtual std::string getProviderName() const = 0;
};

class StripePayment : public PaymentGateway {
public:
  void initiatePayment(double amount) override {
    std::cout << "💳 Processing payment via Stripe: $" << amount << "\n";
  }

  std::string getProviderName() const override { return "Stripe"; }
};

class RazorpayPayment : public PaymentGateway {
public:
  void initiatePayment(double amount) override {
    std::cout << "💳 Processing payment via Razorpay: ₹" << amount << "\n";
  }

  std::string getProviderName() const override { return "Razorpay"; }
};

class PayPalPayment : public PaymentGateway {
public:
  void initiatePayment(double amount) override {
    std::cout << "💳 Processing payment via PayPal: $" << amount << "\n";
  }

  std::string getProviderName() const override { return "PayPal"; }
};

// Service using the interface (Dependency Inversion)
class CheckoutService {
private:
  PaymentGateway *gateway_;

public:
  explicit CheckoutService(PaymentGateway *gateway) : gateway_(gateway) {}

  void setPaymentGateway(PaymentGateway *gateway) { gateway_ = gateway; }

  void processCheckout(double amount) {
    if (gateway_ != nullptr) {
      std::cout << "Using " << gateway_->getProviderName() << "...\n";
      gateway_->initiatePayment(amount);
    } else {
      std::cout << "⚠️  No payment gateway configured!\n";
    }
  }
};

//
// =======================================================
// 7. DIAMOND PROBLEM SOLUTION
// =======================================================
//
// When using multiple interfaces, C++ avoids the diamond problem
// because interfaces have no data (by convention)

class Printable {
public:
  virtual ~Printable() {}
  virtual void print() const = 0;
};

class Serializable {
public:
  virtual ~Serializable() {}
  virtual std::string serialize() const = 0;
};

// No diamond problem because both are pure interfaces
class Document : public Printable, public Serializable {
private:
  std::string content_;

public:
  explicit Document(const std::string &content) : content_(content) {}

  void print() const override {
    std::cout << "📄 Document: " << content_ << "\n";
  }

  std::string serialize() const override {
    return "{\"content\":\"" + content_ + "\"}";
  }
};

//
// =======================================================
// 8. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Interface in C++ Demo ===\n\n";

  // ---- Basic interface usage ----
  std::cout << "1. Basic Interface:\n";
  Circle circle;
  Rectangle rect;
  circle.draw();
  rect.draw();

  // ---- Multiple interfaces ----
  std::cout << "\n2. Multiple Interfaces:\n";
  Shape triangle("Triangle");
  triangle.draw();
  triangle.move(10, 20);
  triangle.resize(1.5);

  // ---- Interface segregation ----
  std::cout << "\n3. Interface Segregation:\n";
  Dog dog;
  dog.walk();
  dog.swim();

  Duck duck;
  duck.walk();
  duck.fly();
  duck.swim();

  // ---- Polymorphism ----
  std::cout << "\n4. Polymorphism:\n";
  std::vector<Drawable *> shapes = {&circle, &rect, &triangle};
  renderShapes(shapes);

  // ---- Real-world payment gateway ----
  std::cout << "\n5. Payment Gateway (Strategy Pattern):\n";
  StripePayment stripe;
  RazorpayPayment razorpay;
  PayPalPayment paypal;

  CheckoutService checkout(&stripe);
  checkout.processCheckout(99.99);

  checkout.setPaymentGateway(&razorpay);
  checkout.processCheckout(1500.0);

  checkout.setPaymentGateway(&paypal);
  checkout.processCheckout(49.99);

  // ---- Document example ----
  std::cout << "\n6. Multiple Interfaces (Document):\n";
  Document doc("Hello, World!");
  doc.print();
  std::cout << "Serialized: " << doc.serialize() << "\n";

  return 0;
}