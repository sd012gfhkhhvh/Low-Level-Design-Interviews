#include <iostream>
#include <string>
#include <vector>

// Ref - https://algomaster.io/learn/lld/abstraction
// Ref - https://www.geeksforgeeks.org/abstraction-in-c/

//
// =======================================================
// 1. WHAT IS ABSTRACTION?
// =======================================================
//
// Abstraction is the process of hiding complex implementation details
// and exposing only the relevant, high-level functionality to the
// outside world. It allows developers to focus on WHAT an object does,
// rather than HOW it does it.
//
// Key formula:
// Abstraction = Hiding Complexity + Showing Essentials
//
// Benefits:
// - Simplifies complex systems
// - Reduces coupling between components
// - Makes code easier to understand and use
// - Allows changing implementation without affecting users
//
// Implementation in C++:
// 1. Abstract classes (with pure virtual functions)
// 2. Interfaces (pure abstract classes)
// 3. Public APIs (well-designed public methods)

//
// =======================================================
// 2. ABSTRACTION vs ENCAPSULATION
// =======================================================
//
// Although often discussed together, they are DISTINCT concepts:
//
// ABSTRACTION:
// - Focuses on hiding COMPLEXITY
// - Shows WHAT an object does
// - External view (user perspective)
// - Example: Car accelerator pedal (simple interface)
//
// ENCAPSULATION:
// - Focuses on hiding DATA
// - Shows HOW to protect data
// - Internal view (implementation perspective)
// - Example: Car engine (self-contained unit with private parts)
//
// Mental Model:
// > Abstraction = "What can I do with this?"
// > Encapsulation = "How is this internal state protected?"

//
// =======================================================
// 3. ABSTRACT CLASSES: BASIC EXAMPLE
// =======================================================
//
// Abstract classes define a common blueprint for a family of classes.
// They contain at least one pure virtual function (= 0)

class Shape {
protected:
  std::string name;
  std::string color;

public:
  Shape(const std::string &n, const std::string &c) : name(n), color(c) {}

  // Pure virtual function - MUST be implemented by subclasses
  virtual double calculateArea() = 0;
  virtual double calculatePerimeter() = 0;

  // Concrete method - shared implementation
  void displayInfo() const {
    std::cout << "Shape: " << name << " | Color: " << color << "\n";
  }

  std::string getName() const { return name; }

  // Virtual destructor (important!)
  virtual ~Shape() = default;
};

class Circle : public Shape {
private:
  double radius;

public:
  Circle(const std::string &color, double r)
      : Shape("Circle", color), radius(r) {}

  // Implement abstract methods
  double calculateArea() override { return 3.14159 * radius * radius; }

  double calculatePerimeter() override { return 2 * 3.14159 * radius; }
};

class Rectangle : public Shape {
private:
  double width;
  double height;

public:
  Rectangle(const std::string &color, double w, double h)
      : Shape("Rectangle", color), width(w), height(h) {}

  double calculateArea() override { return width * height; }

  double calculatePerimeter() override { return 2 * (width + height); }
};

//
// =======================================================
// 4. INTERFACES (PURE ABSTRACT CLASSES)
// =======================================================
//
// An interface is 100% abstract - no implementation at all.
// It defines a CONTRACT that implementing classes must fulfill.

// Interface for objects that can be printed
class Printable {
public:
  virtual void print() const = 0;
  virtual ~Printable() = default;
};

// Interface for objects that can be saved
class Saveable {
public:
  virtual void save(const std::string &filename) const = 0;
  virtual ~Saveable() = default;
};

// Document class implementing multiple interfaces
class Document : public Printable, public Saveable {
private:
  std::string content;
  std::string title;

public:
  Document(const std::string &t, const std::string &c) : title(t), content(c) {}

  // Implement Printable interface
  void print() const override {
    std::cout << "=== " << title << " ===\n";
    std::cout << content << "\n";
  }

  // Implement Saveable interface
  void save(const std::string &filename) const override {
    std::cout << "💾 Saving '" << title << "' to " << filename << "\n";
    // File saving logic would go here
  }

  std::string getContent() const { return content; }
  std::string getTitle() const { return title; }
};

//
// =======================================================
// 5. REAL-WORLD EXAMPLE: PAYMENT PROCESSING
// =======================================================
//
// Abstract interface hides different payment implementations

class PaymentMethod {
public:
  virtual bool processPayment(double amount) = 0;
  virtual std::string getPaymentType() const = 0;
  virtual ~PaymentMethod() = default;
};

class CreditCardPayment : public PaymentMethod {
private:
  std::string cardNumber;

  // Private helper - user doesn't see this complexity
  bool validateCard() const { return cardNumber.length() == 16; }

public:
  CreditCardPayment(const std::string &card) : cardNumber(card) {}

  bool processPayment(double amount) override {
    if (!validateCard()) {
      std::cout << "❌ Invalid card number\n";
      return false;
    }
    std::cout << "💳 Processing $" << amount << " via Credit Card\n";
    return true;
  }

  std::string getPaymentType() const override { return "Credit Card"; }
};

class PayPalPayment : public PaymentMethod {
private:
  std::string email;

  bool authenticateUser() const { return email.find("@") != std::string::npos; }

public:
  PayPalPayment(const std::string &e) : email(e) {}

  bool processPayment(double amount) override {
    if (!authenticateUser()) {
      std::cout << "❌ Invalid PayPal email\n";
      return false;
    }
    std::cout << "💰 Processing $" << amount << " via PayPal (" << email
              << ")\n";
    return true;
  }

  std::string getPaymentType() const override { return "PayPal"; }
};

class CryptoPayment : public PaymentMethod {
private:
  std::string walletAddress;

public:
  CryptoPayment(const std::string &wallet) : walletAddress(wallet) {}

  bool processPayment(double amount) override {
    std::cout << "₿ Processing $" << amount << " via Cryptocurrency\n";
    return true;
  }

  std::string getPaymentType() const override { return "Cryptocurrency"; }
};

//
// =======================================================
// 6. ABSTRACTION THROUGH PUBLIC APIs
// =======================================================
//
// Even without abstract classes, abstraction is achieved through
// well-designed public interfaces that hide complexity

class EmailService {
private:
  std::string smtpServer;
  int port;
  bool isConnected;

  // Private helpers - hidden complexity
  void connectToServer() {
    std::cout << "🔌 Connecting to SMTP server: " << smtpServer << ":" << port
              << "\n";
    isConnected = true;
  }

  void authenticate() { std::cout << "🔐 Authenticating...\n"; }

  void formatEmail(const std::string &to, const std::string &subject,
                   const std::string &body) {
    std::cout << "📝 Formatting email...\n";
  }

  void disconnect() {
    std::cout << "🔌 Disconnecting from server\n";
    isConnected = false;
  }

public:
  EmailService(const std::string &server, int p)
      : smtpServer(server), port(p), isConnected(false) {}

  // Simple public API - user doesn't see complexity!
  void sendEmail(const std::string &to, const std::string &subject,
                 const std::string &body) {
    connectToServer();
    authenticate();
    formatEmail(to, subject, body);

    std::cout << "✅ Email sent to: " << to << "\n";
    std::cout << "   Subject: " << subject << "\n";

    disconnect();
  }
};

//
// =======================================================
// 7. ABSTRACTION LEVELS
// =======================================================
//
// Demonstration of abstraction at different levels

// High-level abstraction
class Database {
public:
  virtual void connect(const std::string &connectionString) = 0;
  virtual void executeQuery(const std::string &query) = 0;
  virtual void disconnect() = 0;
  virtual ~Database() = default;
};

// Mid-level abstraction
class MySQLDatabase : public Database {
private:
  std::string host;
  bool connected;

  void establishConnection() {
    std::cout << "🔗 Establishing MySQL connection to " << host << "\n";
    connected = true;
  }

public:
  MySQLDatabase() : connected(false) {}

  void connect(const std::string &connectionString) override {
    host = connectionString;
    establishConnection();
  }

  void executeQuery(const std::string &query) override {
    if (!connected) {
      std::cout << "❌ Not connected to database\n";
      return;
    }
    std::cout << "📊 Executing MySQL query: " << query << "\n";
  }

  void disconnect() override {
    std::cout << "🔌 Disconnecting from MySQL\n";
    connected = false;
  }
};

//
// =======================================================
// 8. POLYMORPHISM THROUGH ABSTRACTION
// =======================================================
//
// Abstraction enables polymorphic behavior

class Vehicle {
protected:
  std::string brand;
  int year;

public:
  Vehicle(const std::string &b, int y) : brand(b), year(y) {}

  // Abstract methods
  virtual void start() = 0;
  virtual void stop() = 0;
  virtual std::string getType() const = 0;

  // Concrete method
  void displayInfo() const {
    std::cout << year << " " << brand << " " << getType() << "\n";
  }

  virtual ~Vehicle() = default;
};

class Car : public Vehicle {
public:
  Car(const std::string &brand, int year) : Vehicle(brand, year) {}

  void start() override { std::cout << "🚗 Car engine starting...\n"; }

  void stop() override { std::cout << "🚗 Car engine stopping...\n"; }

  std::string getType() const override { return "Car"; }
};

class Motorcycle : public Vehicle {
public:
  Motorcycle(const std::string &brand, int year) : Vehicle(brand, year) {}

  void start() override { std::cout << "🏍️  Motorcycle engine revving...\n"; }

  void stop() override { std::cout << "🏍️  Motorcycle engine off...\n"; }

  std::string getType() const override { return "Motorcycle"; }
};

//
// =======================================================
// 9. ADVANCED: TEMPLATE METHOD PATTERN
// =======================================================
//
// Abstract class defines algorithm structure, subclasses fill in details

class DataProcessor {
public:
  // Template method - defines the algorithm
  void process() {
    loadData();
    validateData();
    transformData();
    saveData();
    std::cout << "✅ Processing complete!\n\n";
  }

  virtual ~DataProcessor() = default;

protected:
  // Abstract steps - subclasses provide implementation
  virtual void loadData() = 0;
  virtual void transformData() = 0;

  // Concrete steps - shared implementation
  void validateData() { std::cout << "✓ Validating data...\n"; }

  void saveData() { std::cout << "💾 Saving processed data...\n"; }
};

class CSVProcessor : public DataProcessor {
protected:
  void loadData() override {
    std::cout << "📂 Loading data from CSV file...\n";
  }

  void transformData() override {
    std::cout << "🔄 Converting CSV to structured format...\n";
  }
};

class JSONProcessor : public DataProcessor {
protected:
  void loadData() override {
    std::cout << "📂 Loading data from JSON file...\n";
  }

  void transformData() override { std::cout << "🔄 Parsing JSON data...\n"; }
};

//
// =======================================================
// 10. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Abstraction in C++ Demo ===\n\n";

  // ---- Abstract Classes ----
  std::cout << "1. Abstract Classes (Shape):\n";
  Circle circle("Red", 5);
  Rectangle rect("Blue", 10, 20);

  circle.displayInfo();
  std::cout << "   Area: " << circle.calculateArea() << "\n";

  rect.displayInfo();
  std::cout << "   Area: " << rect.calculateArea() << "\n";

  // ---- Polymorphism with pointers ----
  std::cout << "\n2. Polymorphism via Abstraction:\n";
  std::vector<Shape *> shapes = {&circle, &rect};
  for (Shape *shape : shapes) {
    std::cout << "   " << shape->getName()
              << " area: " << shape->calculateArea() << "\n";
  }

  // ---- Interfaces ----
  std::cout << "\n3. Interfaces (Printable & Saveable):\n";
  Document doc("My Document", "This is important content.");
  doc.print();
  doc.save("document.txt");

  // ---- Payment Processing ----
  std::cout << "\n4. Real-World Example (Payment Processing):\n";
  CreditCardPayment creditCard("1234567812345678");
  PayPalPayment paypal("user@example.com");
  CryptoPayment crypto("0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb");

  std::vector<PaymentMethod *> payments = {&creditCard, &paypal, &crypto};
  for (PaymentMethod *payment : payments) {
    std::cout << "Using " << payment->getPaymentType() << ":\n";
    payment->processPayment(100.00);
    std::cout << "\n";
  }

  // ---- Public API Abstraction ----
  std::cout << "5. Abstraction via Public API (EmailService):\n";
  EmailService emailService("smtp.example.com", 587);
  emailService.sendEmail("user@example.com", "Hello", "This is a test email");

  // ---- Database Abstraction ----
  std::cout << "\n6. Abstraction Levels (Database):\n";
  MySQLDatabase db;
  db.connect("localhost:3306");
  db.executeQuery("SELECT * FROM users");
  db.disconnect();

  // ---- Polymorphic Vehicles ----
  std::cout << "\n7. Polymorphism Through Abstraction (Vehicles):\n";
  Car car("Toyota", 2024);
  Motorcycle bike("Harley", 2023);

  std::vector<Vehicle *> vehicles = {&car, &bike};
  for (Vehicle *v : vehicles) {
    v->displayInfo();
    v->start();
    v->stop();
    std::cout << "\n";
  }

  // ---- Template Method Pattern ----
  std::cout << "8. Template Method Pattern (Data Processing):\n";
  CSVProcessor csvProc;
  std::cout << "Processing CSV:\n";
  csvProc.process();

  JSONProcessor jsonProc;
  std::cout << "Processing JSON:\n";
  jsonProc.process();

  std::cout << "=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Abstraction vs Encapsulation**:
   - Abstraction: Hiding complexity (WHAT)
   - Encapsulation: Hiding data (HOW)
   - Both work together for clean design

2. **Pure Virtual Functions**:
   - Use = 0 to make function abstract
   - Class becomes abstract (cannot instantiate)
   - Subclasses MUST implement

3. **Interfaces in C++**:
   - Pure abstract classes (all methods = 0)
   - Define contracts
   - Enable multiple implementation "inheritance"

4. **Benefits of Abstraction**:
   - Simplifies complex systems
   - Enables polymorphism
   - Reduces coupling
   - Makes code flexible and maintainable

5. **Design Patterns**:
   - Strategy Pattern (PaymentMethod)
   - Template Method (DataProcessor)
   - Factory Method (through interfaces)

Rule of Thumb:
- Use abstract classes when you have shared state/behavior
- Use interfaces for pure contracts
- Design public APIs that hide complexity
- Program to interfaces, not implementations
*/