//
// =======================================================
// 1. WHAT IS DEPENDENCY?
// =======================================================
//
// A Dependency exists when one class relies on another to fulfill a responsibility,
// but does so WITHOUT retaining a permanent reference to it.
//
// Key Characteristics of Dependency:
// - Short-lived: The relationship exists only during method execution.
// - No ownership: The dependent class does not store the other as a class property.
// - "Uses-a" relationship: The class uses another to accomplish a task temporarily.
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
// The most common form. The dependent class receives another class as a parameter,
// uses it, and lets it go.

class Document {
  constructor(public content: string) {}
}

class Printer {
  // Printer depends on Document, but doesn't store it as a class property
  print(doc: Document): void {
    console.log(`🖨️ Printing Document: ${doc.content}`);
  }
}

// ---- 2.2 As Local Variables ----
// A class creates another class inside a method, uses it, and discards it.

class JsonFormatter {
  format(data: string): string {
    return `{ "data": "${data}" }`;
  }
}

class OrderProcessor {
  process(orderData: string): void {
    // Formatter is instantiated and destroyed within this method scope
    const formatter = new JsonFormatter();
    const json = formatter.format(orderData);
    console.log(`⚙️ Processing JSON: ${json}`);
  }
}

// ---- 2.3 As Return Types ----
// A method returns an object of another class.

class User {
  constructor(public username: string) {}
}

class UserFactory {
  // Factory depends on User because it produces it, yet stores no User properties.
  createUser(name: string): User {
    console.log(`🏭 Factory producing user: ${name}`);
    return new User(name);
  }
}

// ---- 2.4 As Static Method Calls ----
// A class depends on another class by calling its static methods. No object instance is stored.

class HashUtils {
  static hash(password: string): string {
    return `hashed_${password}`;
  }
}

class PasswordService {
  securePassword(rawPassword: string): void {
    // Depends on HashUtils class entirely through static context
    const secure = HashUtils.hash(rawPassword);
    console.log(`🔐 Securing password -> ${secure}`);
  }
}

//
// =======================================================
// 3. DEPENDENCY INJECTION (DI)
// =======================================================
//
// Letting a class instantiate its own dependencies internally (e.g. EmailSender inside a method)
// leads to tight coupling and poor testability.
// The solution is Dependency Injection (DI): providing dependencies from the outside via interfaces.
//
// Technically, constructor DI is often Aggregation, but injecting a behavior for a single
// method call (Method Injection) perfectly highlights pure Dependency Isolation.

interface ISender {
  send(msg: string): void;
}

class EmailSender implements ISender {
  send(msg: string): void {
    console.log(`📧 Email sent: ${msg}`);
  }
}

class SmsSender implements ISender {
  send(msg: string): void {
    console.log(`📱 SMS sent: ${msg}`);
  }
}

class NotificationService {
  // Dependency Injection via method parameter mapping directly to an interface.
  // Highly testable and loosely coupled!
  notifyUser(message: string, sender: ISender): void {
    sender.send(message);
  }
}

//
// =======================================================
// 4. PRACTICAL EXAMPLE: EVENT TICKETING SYSTEM
// =======================================================
//
// A TicketBookingService handles booking an event ticket. It validates seats,
// processes payments, generates a QR code, and sends a confirmation email.
// It uses all these services ONLY during the `bookTicket` method.

class SeatValidator {
  validate(seatNumber: number): boolean {
    console.log(`✅ Seat ${seatNumber} is available.`);
    return true;
  }
}

class PaymentProcessor {
  processPayment(amount: number): boolean {
    console.log(`💳 Processed payment of $${amount}.`);
    return true;
  }
}

class QRCodeGenerator {
  generate(seatNumber: number): string {
    console.log(`🔳 Generated QR Code for seat ${seatNumber}.`);
    return `QR_DATA_${seatNumber}`;
  }
}

class TicketBookingService {
  // The service has ZERO fields. It relies purely on Temporary Dependencies.
  bookTicket(
    seatNumber: number,
    price: number,
    validator: SeatValidator,
    paymentProcessor: PaymentProcessor,
    qrGenerator: QRCodeGenerator,
    emailService: ISender,
  ): void {
    console.log(`🎟️ Initiating ticket booking for seat ${seatNumber}...`);

    if (!validator.validate(seatNumber)) return;
    if (!paymentProcessor.processPayment(price)) return;

    const qrCode = qrGenerator.generate(seatNumber);
    const confirmationMsg = `Your booking is confirmed! Seat: ${seatNumber}. Code: ${qrCode}`;

    emailService.send(confirmationMsg);

    console.log(`🎉 Booking pipeline complete for seat ${seatNumber}!\n`);
  }
}

//
// =======================================================
// 5. DEMONSTRATION
// =======================================================
//

function main(): void {
  console.log("=== Dependency in TypeScript Demo ===\n");

  // ---- 1. Recognize Forms of Dependency ----
  console.log("--- The 4 Forms of Dependency ---");
  const printer = new Printer();
  const doc = new Document("Financial Report Q3");
  printer.print(doc); // Form 1: Passed as parameter

  const processor = new OrderProcessor();
  processor.process("ItemX: 200"); // Form 2: Uses local variable

  const factory = new UserFactory();
  const u = factory.createUser("Alice"); // Form 3: Returned as type

  const passService = new PasswordService();
  passService.securePassword("mySecret123"); // Form 4: Static method call
  console.log("");

  // ---- 2. Dependency Injection ----
  console.log("--- Dependency Injection ---");
  const notifier = new NotificationService();
  const emailSender = new EmailSender();
  const smsSender = new SmsSender();

  // Injecting implementations dynamically into the method
  notifier.notifyUser("Server downtime at 3AM.", emailSender);
  notifier.notifyUser("Your table is ready.", smsSender);
  console.log("");

  // ---- 3. Event Ticketing System ----
  console.log("--- Event Ticketing System (Pure Dependencies) ---");
  const bookingService = new TicketBookingService();
  const validator = new SeatValidator();
  const paymentProcessor = new PaymentProcessor();
  const qrGenerator = new QRCodeGenerator();

  // Booking a ticket by dynamically injecting all the dependencies required for the operation
  bookingService.bookTicket(
    42,
    199.99,
    validator,
    paymentProcessor,
    qrGenerator,
    emailSender,
  );

  console.log("=== Demo Complete ===");
}

main();

export {};

/*
📘 Key Insights:

1. **Lightweight Structural Impact**:
   - Dependencies don't bloat the class with member variables (properties).
   - The memory and structural coupling dies the moment the method returns.

2. **The Four Forms**:
   - Method Parameter (most common)
   - Local Variable instantiation
   - Return Type
   - Static Method utilization

3. **Dependency Injection (DI)**:
   - DI aims to flip internal coupling inside out by injecting objects via parameters (or constructors).
   - Injecting interfaces (e.g. `ISender` rather than `EmailSender`) makes the code easily mockable in unit tests.
   - Frameworks like NestJS build their entirely architecture around Constructor DI.

4. **Association vs. Dependency**:
   - Association (`uses-a`): "I have a phone number saved in my contacts." -> Stores a property `this.contact`.
   - Dependency (`uses-a`): "I borrowed a pen for 5 seconds to sign my name." -> Passed via function parameters.
*/
