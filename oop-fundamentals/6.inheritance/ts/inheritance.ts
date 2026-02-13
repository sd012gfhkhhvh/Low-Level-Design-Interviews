//
// =======================================================
// 1. WHAT IS INHERITANCE?
// =======================================================
//
// Inheritance allows one class (child/subclass) to inherit properties
// and behaviors from another class (parent/superclass).
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
// 2. BASIC SYNTAX
// =======================================================
//
// TypeScript uses 'extends' keyword for inheritance
// Single inheritance only (no multiple inheritance like C++)

class Animal {
  protected name: string;
  protected age: number;

  constructor(name: string, age: number) {
    this.name = name;
    this.age = age;
    console.log(`🐾 Animal constructor: ${name}`);
  }

  makeSound(): void {
    console.log(`${this.name} makes a sound`);
  }

  sleep(): void {
    console.log(`${this.name} is sleeping 💤`);
  }

  displayInfo(): void {
    console.log(`Name: ${this.name} | Age: ${this.age}`);
  }
}

class Dog extends Animal {
  private breed: string;

  constructor(name: string, age: number, breed: string) {
    super(name, age); // Must call super() first!
    this.breed = breed;
    console.log(`🐕 Dog constructor: ${name}`);
  }

  // Override parent method
  makeSound(): void {
    console.log(`${this.name} barks: Woof! Woof! 🐕`);
  }

  // Dog-specific method
  fetch(): void {
    console.log(`${this.name} is fetching the ball! 🎾`);
  }

  displayBreed(): void {
    console.log(`${this.name} is a ${this.breed}`);
  }
}

//
// =======================================================
// 3. TYPES OF INHERITANCE
// =======================================================
//
// TypeScript supports:
// 1. Single Inheritance (one parent only)
// 2. Multi-level Inheritance (chain)
// 3. Hierarchical Inheritance (multiple children)
//
// Does NOT support:
// - Multiple Inheritance (use interfaces instead)

// ----- Single Inheritance (shown above) -----

// ----- Multi-level Inheritance -----
class Vehicle {
  protected make: string;
  protected year: number;

  constructor(make: string, year: number) {
    this.make = make;
    this.year = year;
  }

  start(): void {
    console.log("🔑 Vehicle starting...");
  }

  displayInfo(): void {
    console.log(`${this.year} ${this.make}`);
  }
}

class Car extends Vehicle {
  protected numDoors: number;

  constructor(make: string, year: number, numDoors: number) {
    super(make, year);
    this.numDoors = numDoors;
  }

  start(): void {
    console.log("🚗 Car engine starting...");
  }

  honk(): void {
    console.log("🎺 Beep beep!");
  }
}

// Multi-level: ElectricCar extends Car, which extends Vehicle
class ElectricCar extends Car {
  private batteryCapacity: number;

  constructor(make: string, year: number, numDoors: number, battery: number) {
    super(make, year, numDoors);
    this.batteryCapacity = battery;
  }

  start(): void {
    console.log("⚡ Electric car starting silently...");
  }

  charge(): void {
    console.log(`🔌 Charging ${this.batteryCapacity} kWh battery`);
  }
}

// ----- Hierarchical Inheritance -----
abstract class Shape {
  protected color: string;

  constructor(color: string) {
    this.color = color;
  }

  abstract getArea(): number;

  display(): void {
    console.log(`Color: ${this.color}`);
  }
}

class Circle extends Shape {
  private radius: number;

  constructor(color: string, radius: number) {
    super(color);
    this.radius = radius;
  }

  getArea(): number {
    return Math.PI * this.radius ** 2;
  }

  display(): void {
    console.log(
      `⭕ Circle | Color: ${this.color} | Area: ${this.getArea().toFixed(2)}`,
    );
  }
}

class Rectangle extends Shape {
  private width: number;
  private height: number;

  constructor(color: string, width: number, height: number) {
    super(color);
    this.width = width;
    this.height = height;
  }

  getArea(): number {
    return this.width * this.height;
  }

  display(): void {
    console.log(`▭ Rectangle | Color: ${this.color} | Area: ${this.getArea()}`);
  }
}

//
// =======================================================
// 4. MULTIPLE INHERITANCE (via INTERFACES)
// =======================================================
//
// TypeScript doesn't support multiple class inheritance
// But you CAN implement multiple interfaces!

interface Flyable {
  fly(): void;
}

interface Swimmable {
  swim(): void;
}

class Duck extends Animal implements Flyable, Swimmable {
  constructor(name: string, age: number) {
    super(name, age);
  }

  makeSound(): void {
    console.log(`${this.name} quacks: Quack! Quack! 🦆`);
  }

  fly(): void {
    console.log(`${this.name} is flying! ✈️`);
  }

  swim(): void {
    console.log(`${this.name} is swimming! 🏊`);
  }
}

//
// =======================================================
// 5. SUPER KEYWORD
// =======================================================
//
// 'super' is used to:
// 1. Call parent constructor (must be first in child constructor)
// 2. Call parent methods

class Employee {
  protected name: string;
  protected salary: number;

  constructor(name: string, salary: number) {
    this.name = name;
    this.salary = salary;
  }

  work(): void {
    console.log(`${this.name} is working...`);
  }

  getInfo(): string {
    return `${this.name} earns $${this.salary}`;
  }
}

class Manager extends Employee {
  private teamSize: number;

  constructor(name: string, salary: number, teamSize: number) {
    super(name, salary); // Call parent constructor
    this.teamSize = teamSize;
  }

  work(): void {
    super.work(); // Call parent method
    console.log(`${this.name} is managing ${this.teamSize} people`);
  }

  getInfo(): string {
    return super.getInfo() + ` and manages ${this.teamSize} people`;
  }
}

//
// =======================================================
// 6. ACCESS MODIFIERS IN INHERITANCE
// =======================================================
//
// - public: Accessible everywhere
// - protected: Accessible in class and subclasses
// - private: Only in the class itself
// - readonly: Can't modify after initialization

class BankAccount {
  public accountNumber: string;
  protected balance: number;
  private pin: string;
  readonly createdAt: Date;

  constructor(accountNumber: string, pin: string) {
    this.accountNumber = accountNumber;
    this.balance = 0;
    this.pin = pin;
    this.createdAt = new Date();
  }

  protected validatePin(inputPin: string): boolean {
    return this.pin === inputPin;
  }

  getBalance(): number {
    return this.balance;
  }
}

class SavingsAccount extends BankAccount {
  private interestRate: number;

  constructor(accountNumber: string, pin: string, rate: number) {
    super(accountNumber, pin);
    this.interestRate = rate;
  }

  addInterest(): void {
    // Can access protected 'balance'
    const interest = this.balance * this.interestRate;
    this.balance += interest;
    console.log(`✅ Added interest: $${interest.toFixed(2)}`);
  }

  // Can access protected method
  withdraw(amount: number, pin: string): void {
    if (!this.validatePin(pin)) {
      console.log("❌ Invalid PIN");
      return;
    }
    if (amount > this.balance) {
      console.log("❌ Insufficient funds");
      return;
    }
    this.balance -= amount;
    console.log(`✅ Withdrew $${amount}`);
  }

  // Cannot access private 'pin' directly!
}

//
// =======================================================
// 7. REAL-WORLD EXAMPLE: NOTIFICATION SYSTEM
// =======================================================
//

class Notification {
  protected recipient: string;
  protected message: string;
  protected timestamp: Date;

  constructor(recipient: string, message: string) {
    this.recipient = recipient;
    this.message = message;
    this.timestamp = new Date();
  }

  formatHeader(): string {
    return `[${this.timestamp.toISOString()}] To: ${this.recipient}`;
  }

  send(): void {
    console.log(this.formatHeader());
    console.log(`Message: ${this.message}`);
  }
}

class EmailNotification extends Notification {
  private subject: string;

  constructor(recipient: string, message: string, subject: string) {
    super(recipient, message);
    this.subject = subject;
  }

  send(): void {
    console.log("\n📧 EMAIL NOTIFICATION");
    console.log(this.formatHeader());
    console.log(`Subject: ${this.subject}`);
    console.log(`Body: ${this.message}`);
    console.log("✅ Email delivered");
  }
}

class SMSNotification extends Notification {
  private phoneNumber: string;
  private static readonly MAX_LENGTH = 160;

  constructor(recipient: string, message: string, phoneNumber: string) {
    super(recipient, message);
    this.phoneNumber = phoneNumber;
  }

  send(): void {
    console.log("\n📱 SMS NOTIFICATION");
    console.log(this.formatHeader());
    console.log(`Phone: ${this.phoneNumber}`);

    const smsBody =
      this.message.length > SMSNotification.MAX_LENGTH
        ? this.message.substring(0, SMSNotification.MAX_LENGTH - 3) + "..."
        : this.message;

    console.log(`SMS: ${smsBody}`);
    console.log(
      `✅ SMS sent (${smsBody.length}/${SMSNotification.MAX_LENGTH} chars)`,
    );
  }
}

class PushNotification extends Notification {
  private deviceToken: string;
  private priority: string;

  constructor(
    recipient: string,
    message: string,
    deviceToken: string,
    priority: string,
  ) {
    super(recipient, message);
    this.deviceToken = deviceToken;
    this.priority = priority;
  }

  send(): void {
    console.log("\n🔔 PUSH NOTIFICATION");
    console.log(this.formatHeader());
    console.log(`Device: ${this.deviceToken.substring(0, 8)}...`);
    console.log(`Priority: ${this.priority}`);
    console.log(`Alert: ${this.message}`);
    console.log("✅ Push delivered");
  }
}

//
// =======================================================
// 8. POLYMORPHISM THROUGH INHERITANCE
// =======================================================
//

function sendNotification(notification: Notification): void {
  notification.send(); // Polymorphic call!
}

//
// =======================================================
// 9. METHOD OVERRIDING
// =======================================================
//

class Payment {
  protected amount: number;

  constructor(amount: number) {
    this.amount = amount;
  }

  process(): void {
    console.log(`Processing payment of $${this.amount}`);
  }

  getType(): string {
    return "Generic Payment";
  }
}

class CreditCardPayment extends Payment {
  private cardNumber: string;

  constructor(amount: number, cardNumber: string) {
    super(amount);
    this.cardNumber = cardNumber;
  }

  // Override parent method
  process(): void {
    console.log(`💳 Processing credit card payment of $${this.amount}`);
    console.log(`Card: ${this.cardNumber.substring(0, 4)}****`);
  }

  getType(): string {
    return "Credit Card";
  }
}

class PayPalPayment extends Payment {
  private email: string;

  constructor(amount: number, email: string) {
    super(amount);
    this.email = email;
  }

  process(): void {
    console.log(`💰 Processing PayPal payment of $${this.amount}`);
    console.log(`Account: ${this.email}`);
  }

  getType(): string {
    return "PayPal";
  }
}

//
// =======================================================
// 10. ABSTRACT CLASSES
// =======================================================
//
// Abstract classes can't be instantiated
// They serve as base classes for other classes

abstract class DatabaseConnection {
  protected connectionString: string;
  protected isConnected: boolean = false;

  constructor(connectionString: string) {
    this.connectionString = connectionString;
  }

  // Abstract method - must be implemented by subclasses
  abstract connect(): void;
  abstract disconnect(): void;
  abstract executeQuery(query: string): void;

  // Concrete method - can be used by all subclasses
  getStatus(): string {
    return this.isConnected ? "Connected" : "Disconnected";
  }
}

class MySQLConnection extends DatabaseConnection {
  connect(): void {
    console.log(`🔗 Connecting to MySQL at ${this.connectionString}`);
    this.isConnected = true;
  }

  disconnect(): void {
    console.log("🔌 Disconnecting from MySQL");
    this.isConnected = false;
  }

  executeQuery(query: string): void {
    if (!this.isConnected) {
      console.log("❌ Not connected");
      return;
    }
    console.log(`📊 Executing MySQL query: ${query}`);
  }
}

class MongoDBConnection extends DatabaseConnection {
  connect(): void {
    console.log(`🔗 Connecting to MongoDB at ${this.connectionString}`);
    this.isConnected = true;
  }

  disconnect(): void {
    console.log("🔌 Disconnecting from MongoDB");
    this.isConnected = false;
  }

  executeQuery(query: string): void {
    if (!this.isConnected) {
      console.log("❌ Not connected");
      return;
    }
    console.log(`📊 Executing MongoDB query: ${query}`);
  }
}

//
// =======================================================
// 11. DEMONSTRATION
// =======================================================
//

function main(): void {
  console.log("=== Inheritance in TypeScript Demo ===\n");

  // ---- Basic Inheritance ----
  console.log("1. Basic Inheritance (Animal → Dog):");
  const dog = new Dog("Buddy", 3, "Golden Retriever");
  dog.makeSound();
  dog.sleep();
  dog.fetch();
  dog.displayBreed();

  // ---- Multi-level Inheritance ----
  console.log("\n2. Multi-level Inheritance (Vehicle → Car → ElectricCar):");
  const tesla = new ElectricCar("Tesla", 2024, 4, 100);
  tesla.start();
  tesla.charge();
  tesla.honk();

  // ---- Hierarchical Inheritance ----
  console.log("\n3. Hierarchical Inheritance (Shape → Circle, Rectangle):");
  const circle = new Circle("Red", 5);
  const rectangle = new Rectangle("Blue", 10, 20);

  circle.display();
  rectangle.display();

  // ---- Multiple Interfaces ----
  console.log("\n4. Multiple Interfaces (Duck: Animal + Flyable + Swimmable):");
  const duck = new Duck("Donald", 2);
  duck.makeSound();
  duck.fly();
  duck.swim();

  // ---- Super Keyword ----
  console.log("\n5. Super Keyword (Employee → Manager):");
  const manager = new Manager("Alice", 120000, 5);
  manager.work();
  console.log(manager.getInfo());

  // ---- Access Modifiers ----
  console.log("\n6. Access Modifiers (BankAccount → SavingsAccount):");
  const savings = new SavingsAccount("SA-001", "1234", 0.05);
  savings["balance"] = 1000; // Set balance (normally via deposit method)
  savings.addInterest();
  savings.withdraw(100, "1234");

  // ---- Notification System ----
  console.log("\n7. Real-World Example (Notification System):");
  const email = new EmailNotification(
    "alice@example.com",
    "Your order has been shipped!",
    "Order Update",
  );
  email.send();

  const sms = new SMSNotification(
    "Bob",
    "Your verification code is 482910.",
    "+1-555-0123",
  );
  sms.send();

  const push = new PushNotification(
    "Charlie",
    "New message from Alice",
    "d8a3f4b2c1e5a9b7",
    "high",
  );
  push.send();

  // ---- Polymorphism ----
  console.log("\n8. Polymorphism Through Inheritance:");
  const notifications: Notification[] = [email, sms, push];

  notifications.forEach((notification) => {
    sendNotification(notification);
  });

  // ---- Method Overriding ----
  console.log("\n9. Method Overriding (Payment System):");
  const creditCard = new CreditCardPayment(100, "1234567890123456");
  const paypal = new PayPalPayment(50, "user@example.com");

  const payments: Payment[] = [creditCard, paypal];
  payments.forEach((payment) => {
    console.log(`\nPayment Type: ${payment.getType()}`);
    payment.process();
  });

  // ---- Abstract Classes ----
  console.log("\n10. Abstract Classes (Database Connections):");
  const mysql: DatabaseConnection = new MySQLConnection("localhost:3306");
  mysql.connect();
  mysql.executeQuery("SELECT * FROM users");
  console.log(`Status: ${mysql.getStatus()}`);
  mysql.disconnect();

  console.log();

  const mongo: DatabaseConnection = new MongoDBConnection("localhost:27017");
  mongo.connect();
  mongo.executeQuery("db.users.find()");
  mongo.disconnect();

  console.log("\n=== Demo Complete ===");
}

// Execute
main();

export {};

/*
📘 Key Insights:

1. **Inheritance in TypeScript**:
   - Single inheritance only (extends one class)
   - Can implement multiple interfaces
   - No multiple class inheritance (unlike C++)

2. **Super Keyword**:
   - Must call super() in constructor (first statement!)
   - Use super.method() to call parent methods
   - Enables method extension, not just replacement

3. **Access Modifiers**:
   - public: Accessible everywhere (default)
   - protected: Accessible in class and subclasses
   - private: Only in the class itself
   - # private: True runtime privacy (ES2022+)

4. **Method Overriding**:
   - Override parent methods without special keyword
   - Use super to call parent version
   - Enables polymorphic behavior

5. **Abstract Classes**:
   - Can't instantiate directly
   - abstract methods must be implemented
   - Can have concrete methods
   - Good for defining contracts with shared logic

Best Practices:
- Always call super() first in constructor
- Use protected for inherited members
- Prefer composition over inheritance when possible
- Keep inheritance hierarchies shallow
- Use interfaces for multiple inheritance of behavior
- Use abstract classes for shared behavior

When to Use Inheritance:
✅ Clear "is-a" relationship
✅ Shared behavior across classes
✅ Need polymorphism
✅ Logical hierarchy exists

When to Avoid:
❌ "has-a" relationship (use composition)
❌ Need multiple inheritance (use interfaces)
❌ Deep hierarchies (>3 levels)
❌ Runtime flexibility needed
*/
