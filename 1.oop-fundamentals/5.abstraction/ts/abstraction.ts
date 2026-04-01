//
// =======================================================
// 1. WHAT IS ABSTRACTION?
// =======================================================
//
// Abstraction is the process of hiding complex implementation details
// and exposing only the relevant, high-level functionality. It allows
// developers to focus on WHAT an object does, rather than HOW it does it.
//
// Key formula:
// Abstraction = Hiding Complexity + Showing Essentials
//
// Benefits:
// - Simplifies complex systems
// - Reduces coupling
// - Makes code easier to understand
// - Allows changing implementation without affecting users
//
// Implementation in TypeScript:
// 1. Abstract classes (with abstract keyword)
// 2. Interfaces (native TypeScript feature)
// 3. Public APIs (well-designed methods)

//
// =======================================================
// 2. ABSTRACTION vs ENCAPSULATION
// =======================================================
//
// ABSTRACTION:
// - Focuses on hiding COMPLEXITY
// - Shows WHAT an object does
// - External view (user perspective)
// - Example: TV remote buttons (simple interface)
//
// ENCAPSULATION:
// - Focuses on hiding DATA
// - Shows HOW to protect data
// - Internal view (implementation perspective)
// - Example: TV internal circuits (protected components)
//
// Mental Model:
// Abstraction = "What can I do with this?"
// Encapsulation = "How is this data protected?"

//
// =======================================================
// 3. ABSTRACT CLASSES: BASIC EXAMPLE
// =======================================================
//
// Abstract classes define common blueprint for related classes
// Cannot be instantiated directly

abstract class Shape {
  protected name: string;
  protected color: string;

  constructor(name: string, color: string) {
    this.name = name;
    this.color = color;
  }

  // Abstract methods - MUST be implemented by subclasses
  abstract calculateArea(): number;
  abstract calculatePerimeter(): number;

  // Concrete method - shared implementation
  displayInfo(): void {
    console.log(`Shape: ${this.name} | Color: ${this.color}`);
  }

  getName(): string {
    return this.name;
  }
}

class Circle extends Shape {
  private radius: number;

  constructor(color: string, radius: number) {
    super("Circle", color);
    this.radius = radius;
  }

  calculateArea(): number {
    return Math.PI * this.radius ** 2;
  }

  calculatePerimeter(): number {
    return 2 * Math.PI * this.radius;
  }
}

class Rectangle extends Shape {
  private width: number;
  private height: number;

  constructor(color: string, width: number, height: number) {
    super("Rectangle", color);
    this.width = width;
    this.height = height;
  }

  calculateArea(): number {
    return this.width * this.height;
  }

  calculatePerimeter(): number {
    return 2 * (this.width + this.height);
  }
}

//
// =======================================================
// 4. INTERFACES (NATIVE TYPESCRIPT)
// =======================================================
//
// Interfaces define contracts that classes must fulfill
// TypeScript's structural typing allows flexible implementations

interface Printable {
  print(): void;
}

interface Saveable {
  save(filename: string): void;
}

// Implementing multiple interfaces
class Document implements Printable, Saveable {
  constructor(
    private title: string,
    private content: string,
  ) {}

  print(): void {
    console.log(`=== ${this.title} ===`);
    console.log(this.content);
  }

  save(filename: string): void {
    console.log(`💾 Saving '${this.title}' to ${filename}`);
    // File saving logic would go here
  }

  getContent(): string {
    return this.content;
  }

  getTitle(): string {
    return this.title;
  }
}

//
// =======================================================
// 5. REAL-WORLD EXAMPLE: PAYMENT PROCESSING
// =======================================================
//
// Abstract interface hides different payment implementations

interface PaymentMethod {
  processPayment(amount: number): boolean;
  getPaymentType(): string;
}

class CreditCardPayment implements PaymentMethod {
  constructor(private cardNumber: string) {}

  private validateCard(): boolean {
    return this.cardNumber.length === 16;
  }

  processPayment(amount: number): boolean {
    if (!this.validateCard()) {
      console.log("❌ Invalid card number");
      return false;
    }
    console.log(`💳 Processing $${amount} via Credit Card`);
    return true;
  }

  getPaymentType(): string {
    return "Credit Card";
  }
}

class PayPalPayment implements PaymentMethod {
  constructor(private email: string) {}

  private authenticateUser(): boolean {
    return this.email.includes("@");
  }

  processPayment(amount: number): boolean {
    if (!this.authenticateUser()) {
      console.log("❌ Invalid PayPal email");
      return false;
    }
    console.log(`💰 Processing $${amount} via PayPal (${this.email})`);
    return true;
  }

  getPaymentType(): string {
    return "PayPal";
  }
}

class CryptoPayment implements PaymentMethod {
  constructor(private walletAddress: string) {}

  processPayment(amount: number): boolean {
    console.log(`₿ Processing $${amount} via Cryptocurrency`);
    return true;
  }

  getPaymentType(): string {
    return "Cryptocurrency";
  }
}

//
// =======================================================
// 6. ABSTRACTION THROUGH PUBLIC APIs
// =======================================================
//
// Hide complexity through well-designed public interfaces

class EmailService {
  constructor(
    private smtpServer: string,
    private port: number,
  ) {}

  // Private helpers - hidden complexity
  private connectToServer(): void {
    console.log(
      `🔌 Connecting to SMTP server: ${this.smtpServer}:${this.port}`,
    );
  }

  private authenticate(): void {
    console.log("🔐 Authenticating...");
  }

  private formatEmail(to: string, subject: string, body: string): void {
    console.log("📝 Formatting email...");
  }

  private disconnect(): void {
    console.log("🔌 Disconnecting from server");
  }

  // Simple public API - user doesn't see complexity!
  sendEmail(to: string, subject: string, body: string): void {
    this.connectToServer();
    this.authenticate();
    this.formatEmail(to, subject, body);

    console.log(`✅ Email sent to: ${to}`);
    console.log(`   Subject: ${subject}`);

    this.disconnect();
  }
}

//
// =======================================================
// 7. ABSTRACTION LEVELS
// =======================================================
//
// Demonstration of abstraction at different levels

interface Database {
  connect(connectionString: string): void;
  executeQuery(query: string): void;
  disconnect(): void;
}

class MySQLDatabase implements Database {
  private host: string = "";
  private connected: boolean = false;

  private establishConnection(): void {
    console.log(`🔗 Establishing MySQL connection to ${this.host}`);
    this.connected = true;
  }

  connect(connectionString: string): void {
    this.host = connectionString;
    this.establishConnection();
  }

  executeQuery(query: string): void {
    if (!this.connected) {
      console.log("❌ Not connected to database");
      return;
    }
    console.log(`📊 Executing MySQL query: ${query}`);
  }

  disconnect(): void {
    console.log("🔌 Disconnecting from MySQL");
    this.connected = false;
  }
}

class MongoDatabase implements Database {
  private host: string = "";
  private connected: boolean = false;

  connect(connectionString: string): void {
    this.host = connectionString;
    console.log(`🔗 Connecting to MongoDB at ${this.host}`);
    this.connected = true;
  }

  executeQuery(query: string): void {
    if (!this.connected) {
      console.log("❌ Not connected to database");
      return;
    }
    console.log(`📊 Executing MongoDB query: ${query}`);
  }

  disconnect(): void {
    console.log("🔌 Disconnecting from MongoDB");
    this.connected = false;
  }
}

//
// =======================================================
// 8. POLYMORPHISM THROUGH ABSTRACTION
// =======================================================
//
// Abstraction enables polymorphic behavior

abstract class Vehicle {
  constructor(
    protected brand: string,
    protected year: number,
  ) {}

  abstract start(): void;
  abstract stop(): void;
  abstract getType(): string;

  displayInfo(): void {
    console.log(`${this.year} ${this.brand} ${this.getType()}`);
  }
}

class Car extends Vehicle {
  constructor(brand: string, year: number) {
    super(brand, year);
  }

  start(): void {
    console.log("🚗 Car engine starting...");
  }

  stop(): void {
    console.log("🚗 Car engine stopping...");
  }

  getType(): string {
    return "Car";
  }
}

class Motorcycle extends Vehicle {
  constructor(brand: string, year: number) {
    super(brand, year);
  }

  start(): void {
    console.log("🏍️  Motorcycle engine revving...");
  }

  stop(): void {
    console.log("🏍️  Motorcycle engine off...");
  }

  getType(): string {
    return "Motorcycle";
  }
}

//
// =======================================================
// 9. ADVANCED: TEMPLATE METHOD PATTERN
// =======================================================
//
// Abstract class defines algorithm structure, subclasses fill in details

abstract class DataProcessor {
  // Template method - defines the algorithm
  process(): void {
    this.loadData();
    this.validateData();
    this.transformData();
    this.saveData();
    console.log("✅ Processing complete!\n");
  }

  // Abstract steps - subclasses provide implementation
  protected abstract loadData(): void;
  protected abstract transformData(): void;

  // Concrete steps - shared implementation
  protected validateData(): void {
    console.log("✓ Validating data...");
  }

  protected saveData(): void {
    console.log("💾 Saving processed data...");
  }
}

class CSVProcessor extends DataProcessor {
  protected loadData(): void {
    console.log("📂 Loading data from CSV file...");
  }

  protected transformData(): void {
    console.log("🔄 Converting CSV to structured format...");
  }
}

class JSONProcessor extends DataProcessor {
  protected loadData(): void {
    console.log("📂 Loading data from JSON file...");
  }

  protected transformData(): void {
    console.log("🔄 Parsing JSON data...");
  }
}

//
// =======================================================
// 10. TYPESCRIPT-SPECIFIC: TYPE vs INTERFACE
// =======================================================
//
// Both can define contracts, but interfaces are preferred for abstraction

// Interface - extensible
interface Logger {
  log(message: string): void;
  error(message: string): void;
}

// Can extend
interface FileLogger extends Logger {
  setFilePath(path: string): void;
}

// Type alias - less flexible for this use case
type LoggerType = {
  log(message: string): void;
  error(message: string): void;
};

// Implementing interface
class ConsoleLogger implements Logger {
  log(message: string): void {
    console.log(`[LOG] ${message}`);
  }

  error(message: string): void {
    console.error(`[ERROR] ${message}`);
  }
}

//
// =======================================================
// 11. STRATEGY PATTERN WITH ABSTRACTION
// =======================================================
//

interface SortStrategy {
  sort(data: number[]): number[];
}

class BubbleSort implements SortStrategy {
  sort(data: number[]): number[] {
    console.log("🔄 Using Bubble Sort");
    // Simplified bubble sort
    const arr = [...data];
    for (let i = 0; i < arr.length; i++) {
      for (let j = 0; j < arr.length - i - 1; j++) {
        if (arr[j] > arr[j + 1]) {
          [arr[j], arr[j + 1]] = [arr[j + 1], arr[j]];
        }
      }
    }
    return arr;
  }
}

class QuickSort implements SortStrategy {
  sort(data: number[]): number[] {
    console.log("⚡ Using Quick Sort");
    // Using built-in sort for demo
    return [...data].sort((a, b) => a - b);
  }
}

class Sorter {
  constructor(private strategy: SortStrategy) {}

  setStrategy(strategy: SortStrategy): void {
    this.strategy = strategy;
  }

  sortData(data: number[]): number[] {
    return this.strategy.sort(data);
  }
}

//
// =======================================================
// 12. DEMONSTRATION
// =======================================================
//

function main(): void {
  console.log("=== Abstraction in TypeScript Demo ===\n");

  // ---- Abstract Classes ----
  console.log("1. Abstract Classes (Shape):");
  const circle = new Circle("Red", 5);
  const rect = new Rectangle("Blue", 10, 20);

  circle.displayInfo();
  console.log(`   Area: ${circle.calculateArea().toFixed(2)}`);

  rect.displayInfo();
  console.log(`   Area: ${rect.calculateArea()}`);

  // ---- Polymorphism ----
  console.log("\n2. Polymorphism via Abstraction:");
  const shapes: Shape[] = [circle, rect];
  shapes.forEach((shape) => {
    console.log(
      `   ${shape.getName()} area: ${shape.calculateArea().toFixed(2)}`,
    );
  });

  // ---- Interfaces ----
  console.log("\n3. Interfaces (Printable & Saveable):");
  const doc = new Document("My Document", "This is important content.");
  doc.print();
  doc.save("document.txt");

  // ---- Payment Processing ----
  console.log("\n4. Real-World Example (Payment Processing):");
  const creditCard: PaymentMethod = new CreditCardPayment("1234567812345678");
  const paypal: PaymentMethod = new PayPalPayment("user@example.com");
  const crypto: PaymentMethod = new CryptoPayment(
    "0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb",
  );

  const payments: PaymentMethod[] = [creditCard, paypal, crypto];
  payments.forEach((payment) => {
    console.log(`Using ${payment.getPaymentType()}:`);
    payment.processPayment(100.0);
    console.log();
  });

  // ---- Public API Abstraction ----
  console.log("5. Abstraction via Public API (EmailService):");
  const emailService = new EmailService("smtp.example.com", 587);
  emailService.sendEmail("user@example.com", "Hello", "This is a test email");

  // ---- Database Abstraction ----
  console.log("\n6. Abstraction Levels (Multiple Databases):");
  const mysql: Database = new MySQLDatabase();
  mysql.connect("localhost:3306");
  mysql.executeQuery("SELECT * FROM users");
  mysql.disconnect();

  console.log();

  const mongo: Database = new MongoDatabase();
  mongo.connect("localhost:27017");
  mongo.executeQuery("db.users.find()");
  mongo.disconnect();

  // ---- Polymorphic Vehicles ----
  console.log("\n7. Polymorphism Through Abstraction (Vehicles):");
  const car: Vehicle = new Car("Toyota", 2024);
  const bike: Vehicle = new Motorcycle("Harley", 2023);

  const vehicles: Vehicle[] = [car, bike];
  vehicles.forEach((v) => {
    v.displayInfo();
    v.start();
    v.stop();
    console.log();
  });

  // ---- Template Method Pattern ----
  console.log("8. Template Method Pattern (Data Processing):");
  const csvProc = new CSVProcessor();
  console.log("Processing CSV:");
  csvProc.process();

  const jsonProc = new JSONProcessor();
  console.log("Processing JSON:");
  jsonProc.process();

  // ---- Logger Implementation ----
  console.log("9. Interface Implementation (Logger):");
  const logger: Logger = new ConsoleLogger();
  logger.log("Application started");
  logger.error("An error occurred");

  // ---- Strategy Pattern ----
  console.log("\n10. Strategy Pattern (Sorting):");
  const data = [5, 2, 8, 1, 9];
  const sorter = new Sorter(new BubbleSort());
  console.log(`Original: [${data}]`);
  console.log(`Sorted: [${sorter.sortData(data)}]`);

  sorter.setStrategy(new QuickSort());
  console.log(`Sorted: [${sorter.sortData(data)}]`);

  console.log("\n=== Demo Complete ===");
}

// Execute
main();

export {};

/*
📘 Key Insights:

1. **Abstract Classes in TypeScript**:
   - Use 'abstract' keyword
   - Can have abstract and concrete methods
   - Cannot instantiate directly
   - Ideal for shared behavior

2. **Interfaces in TypeScript**:
   - Define contracts
   - Structural typing (duck typing)
   - Can implement multiple interfaces
   - Preferred for pure abstraction

3. **Abstraction vs Encapsulation**:
   - Abstraction: WHAT (hiding complexity)
   - Encapsulation: HOW (hiding data)
   - Different but complementary

4. **TypeScript Advantages**:
   - Native interface support
   - Abstract keyword
   - Type checking at compile time
   - Structural typing flexibility

5. **Design Patterns**:
   - Strategy Pattern (Payment, Sorting)
   - Template Method (DataProcessor)
   - Works perfectly with abstraction

Best Practices:
- Use interfaces for contracts
- Use abstract classes for shared behavior
- Hide complexity in private methods
- Program to abstractions, not implementations
- Leverage TypeScript's type system
*/
