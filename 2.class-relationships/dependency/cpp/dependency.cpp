#include <iostream>
#include <string>

// Ref - https://algomaster.io/learn/lld/dependency

//
// =======================================================
// 1. WHAT IS DEPENDENCY?
// =======================================================
//
// A Dependency exists when one class relies on another to fulfill a
// responsibility, but does so WITHOUT retaining a permanent reference to it.
//
// Key Characteristics of Dependency:
// - Short-lived: The relationship exists only during method execution.
// - No ownership: The dependent class does not store the other as a field.
// - "Uses-a" relationship: The class uses another to accomplish a task
// temporarily.
//
// Mental Model:
// Imagine a Chef preparing a meal.
// - The chef picks up a Knife to chop vegetables.
// - Once chopping is done, the knife is put away.
// - The chef doesn't permanently store the knife in their pocket.
// This is a Dependency. It is the lightest form of class relationship.
//
// UML Notation:
// Class1 ..> Class2 (Dashed arrow pointing to the utilized class)

//
// =======================================================
// 2. RECOGNIZING DEPENDENCIES (THE 4 FORMS)
// =======================================================

// ---- 2.1 As Method Parameters ----
// The most common form. The dependent class receives another class as a
// parameter, uses it, and lets it go.

class Document {
public:
  std::string content;
  explicit Document(std::string c) : content(std::move(c)) {}
};

class Printer {
public:
  // Printer depends on Document, but doesn't store it as a class member
  void print(const Document &doc) const {
    std::cout << "🖨️ Printing Document: " << doc.content << "\n";
  }
};

// ---- 2.2 As Local Variables ----
// A class creates another class inside a method, uses it, and discards it.

class JsonFormatter {
public:
  std::string format(const std::string &data) const {
    return "{ \"data\": \"" + data + "\" }";
  }
};

class OrderProcessor {
public:
  void process(const std::string &orderData) const {
    // Formatter is instantiated and destroyed within this method scope
    JsonFormatter formatter;
    std::string json = formatter.format(orderData);
    std::cout << "⚙️ Processing JSON: " << json << "\n";
  }
};

// ---- 2.3 As Return Types ----
// A method returns an object of another class.

class User {
public:
  std::string username;
  explicit User(std::string name) : username(std::move(name)) {}
};

class UserFactory {
public:
  // Factory depends on User because it produces it, yet stores no User fields.
  User createUser(const std::string &name) const {
    std::cout << "🏭 Factory producing user: " << name << "\n";
    return User(name);
  }
};

// ---- 2.4 As Static Method Calls ----
// A class depends on another class by calling its static methods. No object
// instance is stored.

class HashUtils {
public:
  static std::string hash(const std::string &password) {
    return "hashed_" + password;
  }
};

class PasswordService {
public:
  void securePassword(const std::string &rawPassword) const {
    // Depends on HashUtils class entirely through static context
    std::string secure = HashUtils::hash(rawPassword);
    std::cout << "🔐 Securing password -> " << secure << "\n";
  }
};

//
// =======================================================
// 3. DEPENDENCY INJECTION (DI)
// =======================================================
//
// Letting a class instantiate its own dependencies internally (e.g.
// EmailSender) leads to tight coupling and poor testability. The solution is
// Dependency Injection (DI): providing dependencies from the outside via
// interfaces.
//
// Technically, structural DI is often Aggregation, but injecting a behavior for
// a single method call is pure Dependency Injection. Let's look at injecting
// dependencies via method parameters.

class ISender {
public:
  virtual ~ISender() = default;
  virtual void send(const std::string &msg) const = 0;
};

class EmailSender : public ISender {
public:
  void send(const std::string &msg) const override {
    std::cout << "📧 Email sent: " << msg << "\n";
  }
};

class SmsSender : public ISender {
public:
  void send(const std::string &msg) const override {
    std::cout << "📱 SMS sent: " << msg << "\n";
  }
};

class NotificationService {
public:
  // Dependency Injection via method parameter mapping directly to an interface.
  // Highly testable and loosely coupled!
  void notifyUser(const std::string &message, const ISender &sender) const {
    sender.send(message);
  }
};

//
// =======================================================
// 4. PRACTICAL EXAMPLE: EVENT TICKETING SYSTEM
// =======================================================
//
// A TicketBookingService handles booking an event ticket. It validates seats,
// processes payments, generates a QR code, and sends a confirmation email.
// It uses all these services ONLY during the `bookTicket` method.

class SeatValidator {
public:
  bool validate(int seatNumber) const {
    std::cout << "✅ Seat " << seatNumber << " is available.\n";
    return true;
  }
};

class PaymentProcessor {
public:
  bool processPayment(double amount) const {
    std::cout << "💳 Processed payment of $" << amount << ".\n";
    return true;
  }
};

class QRCodeGenerator {
public:
  std::string generate(int seatNumber) const {
    std::cout << "🔳 Generated QR Code for seat " << seatNumber << ".\n";
    return "QR_DATA_" + std::to_string(seatNumber);
  }
};

class TicketBookingService {
public:
  // The service has ZERO fields. It relies purely on Temporary Dependencies.
  void bookTicket(int seatNumber, double price, const SeatValidator &validator,
                  const PaymentProcessor &paymentProcessor,
                  const QRCodeGenerator &qrGenerator,
                  const ISender &emailService) const {

    std::cout << "🎟️ Initiating ticket booking for seat " << seatNumber
              << "...\n";

    if (!validator.validate(seatNumber))
      return;
    if (!paymentProcessor.processPayment(price))
      return;

    std::string qrCode = qrGenerator.generate(seatNumber);

    std::string confirmationMsg =
        "Your booking is confirmed! Seat: " + std::to_string(seatNumber) +
        ". Code: " + qrCode;
    emailService.send(confirmationMsg);

    std::cout << "🎉 Booking pipeline complete for seat " << seatNumber
              << "!\n";
  }
};

//
// =======================================================
// 5. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Dependency in C++ Demo ===\n\n";

  // ---- 1. Recognize Forms of Dependency ----
  std::cout << "--- The 4 Forms of Dependency ---\n";
  Printer printer;
  Document doc("Financial Report Q3");
  printer.print(doc); // Passed as parameter

  OrderProcessor processor;
  processor.process("ItemX: 200"); // Uses local variable

  UserFactory factory;
  User u = factory.createUser("Alice"); // Returned as type

  PasswordService passService;
  passService.securePassword("mySecret123"); // Static method call
  std::cout << "\n";

  // ---- 2. Dependency Injection ----
  std::cout << "--- Dependency Injection ---\n";
  NotificationService notifier;
  EmailSender emailSender;
  SmsSender smsSender;

  // Injecting implementations dynamically into the method
  notifier.notifyUser("Server downtime at 3AM.", emailSender);
  notifier.notifyUser("Your table is ready.", smsSender);
  std::cout << "\n";

  // ---- 3. Event Ticketing System ----
  std::cout << "--- Event Ticketing System (Pure Dependencies) ---\n";
  TicketBookingService bookingService;
  SeatValidator validator;
  PaymentProcessor paymentProcessor;
  QRCodeGenerator qrGenerator;

  // Booking a ticket by dynamically injecting all the dependencies required for
  // the operation
  bookingService.bookTicket(42, 199.99, validator, paymentProcessor,
                            qrGenerator, emailSender);

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Lightweight Structural Impact**:
   - Dependencies don't bloat the class with member variables.
   - The memory and structural coupling dies the moment the method returns.

2. **The Four Forms**:
   - Method Parameter (most common)
   - Local Variable instantiation
   - Return Type
   - Static Method utilization

3. **Dependency Injection (DI)**:
   - DI aims to flip internal coupling inside out by injecting objects via
parameters (or constructors).
   - Injecting interfaces (e.g. `ISender`) instead of concrete classes makes the
code easily mockable in unit tests.

4. **Association vs. Dependency**:
   - Association (`uses-a`): "I have a phone number saved in my contacts." ->
Stores a field/pointer.
   - Dependency (`uses-a`): "I borrowed a pen for 5 seconds to sign my name." ->
Just a method parameter.
*/