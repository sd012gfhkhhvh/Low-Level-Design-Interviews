//
// =======================================================
// 1. WHAT IS POLYMORPHISM?
// =======================================================
//
// Polymorphism = "Many forms"
// It allows the same interface to be used for different underlying forms (data types).
//
// Key formula:
// Polymorphism = Same Interface + Different Behaviors
//
// Benefits:
// - Code reusability (write generic code)
// - Flexibility (add new types without changing existing code)
// - Loose coupling (depend on abstractions, not concrete types)
// -Extensibility (easily add new implementations)
//
// Types in TypeScript:
// 1. Compile-time Polymorphism (Static)
//    - Function Overloading (limited in TS)
//    - Method Overloading via union types
//
// 2. Runtime Polymorphism (Dynamic)
//    - Method Overriding
//    - Interface implementation

//
// =======================================================
// 2. COMPILE-TIME POLYMORPHISM: FUNCTION OVERLOADING
// =======================================================
//
// TypeScript supports function overloading through overload signatures
// However, implementation is single with type checking

class Calculator {
  // Overload signatures
  add(a: number, b: number): number;
  add(a: string, b: string): string;
  add(a: number, b: number, c: number): number;

  // Implementation signature (must handle all overloads)
  add(a: number | string, b: number | string, c?: number): number | string {
    if (typeof a === "number" && typeof b === "number") {
      if (c !== undefined) {
        console.log("📊 add(number, number, number)");
        return a + b + c;
      }
      console.log("📊 add(number, number)");
      return a + b;
    }
    console.log("📊 add(string, string)");
    return String(a) + String(b);
  }
}

//
// =======================================================
// 3. RUNTIME POLYMORPHISM: METHOD OVERRIDING
// =======================================================
//
// TypeScript implements runtime polymorphism through inheritance
// Child classes can override parent methods

abstract class Shape {
  protected color: string;

  constructor(color: string) {
    this.color = color;
  }

  // Abstract method - must be implemented by subclasses
  abstract getArea(): number;

  // Concrete method - can be overridden
  draw(): void {
    console.log(`Drawing a shape with color: ${this.color}`);
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

  // Override parent method
  draw(): void {
    console.log(
      `⭕ Drawing Circle | Color: ${this.color} | Area: ${this.getArea().toFixed(2)}`,
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

  draw(): void {
    console.log(
      `▭ Drawing Rectangle | Color: ${this.color} | Area: ${this.getArea()}`,
    );
  }
}

//
// =======================================================
// 4. INTERFACE-BASED POLYMORPHISM
// =======================================================
//
// TypeScript's interfaces enable structural polymorphism
// Any object matching the interface shape can be used

interface Printable {
  print(): void;
}

interface Saveable {
  save(filename: string): void;
}

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
  }
}

class Image implements Printable {
  constructor(
    private name: string,
    private format: string,
  ) {}

  print(): void {
    console.log(`🖼️  Printing image: ${this.name}.${this.format}`);
  }
}

//
// =======================================================
// 5. ABSTRACT CLASSES
// =======================================================
//
// Abstract classes provide both interface and partial implementation
// Cannot be instantiated directly

abstract class Animal {
  protected name: string;

  constructor(name: string) {
    this.name = name;
  }

  // Abstract method - must implement
  abstract makeSound(): void;

  // Concrete method - shared implementation
  sleep(): void {
    console.log(`${this.name} is sleeping 💤`);
  }

  eat(): void {
    console.log(`${this.name} is eating 🍖`);
  }
}

class Dog extends Animal {
  constructor(name: string) {
    super(name);
  }

  makeSound(): void {
    console.log(`${this.name} says: Woof! Woof! 🐕`);
  }
}

class Cat extends Animal {
  constructor(name: string) {
    super(name);
  }

  makeSound(): void {
    console.log(`${this.name} says: Meow! Meow! 🐱`);
  }
}

//
// =======================================================
// 6. REAL-WORLD EXAMPLE: PAYMENT SYSTEM
// =======================================================
//

interface PaymentMethod {
  processPayment(amount: number): boolean;
  getType(): string;
}

class CreditCardPayment implements PaymentMethod {
  constructor(
    private accountId: string,
    private cardNumber: string,
  ) {}

  processPayment(amount: number): boolean {
    console.log(`💳 Processing $${amount} via Credit Card`);
    console.log(`   Card: ${this.cardNumber.substring(0, 4)}****`);
    return true;
  }

  getType(): string {
    return "Credit Card";
  }
}

class PayPalPayment implements PaymentMethod {
  constructor(
    private accountId: string,
    private email: string,
  ) {}

  processPayment(amount: number): boolean {
    console.log(`💰 Processing $${amount} via PayPal`);
    console.log(`   Email: ${this.email}`);
    return true;
  }

  getType(): string {
    return "PayPal";
  }
}

class CryptoPayment implements PaymentMethod {
  constructor(
    private accountId: string,
    private walletAddress: string,
  ) {}

  processPayment(amount: number): boolean {
    console.log(`₿ Processing $${amount} via Cryptocurrency`);
    console.log(`   Wallet: ${this.walletAddress.substring(0, 8)}...`);
    return true;
  }

  getType(): string {
    return "Cryptocurrency";
  }
}

//
// =======================================================
// 7. POLYMORPHIC BEHAVIOR
// =======================================================
//

function processTransaction(method: PaymentMethod, amount: number): void {
  console.log("\n--- Processing Transaction ---");
  console.log(`Payment Type: ${method.getType()}`);
  method.processPayment(amount);
  console.log("✅ Transaction complete!");
}

//
// =======================================================
// 8. GENERIC POLYMORPHISM
// =======================================================
//
// TypeScript generics enable compile-time polymorphism

class Stack<T> {
  private items: T[] = [];

  push(item: T): void {
    this.items.push(item);
  }

  pop(): T | undefined {
    return this.items.pop();
  }

  peek(): T | undefined {
    return this.items[this.items.length - 1];
  }

  isEmpty(): boolean {
    return this.items.length === 0;
  }

  size(): number {
    return this.items.length;
  }
}

//
// =======================================================
// 9. DUCK TYPING (STRUCTURAL POLYMORPHISM)
// =======================================================
//
// TypeScript uses structural typing: if it looks like a duck, it's a duck

interface Flyable {
  fly(): void;
}

class Bird {
  fly(): void {
    console.log("Bird is flying! 🐦");
  }
}

class Airplane {
  fly(): void {
    console.log("Airplane is flying! ✈️");
  }
}

function makeFly(thing: Flyable): void {
  thing.fly(); // Works with ANY object that has fly() method!
}

//
// =======================================================
// 10. UNION TYPES FOR POLYMORPHISM
// =======================================================
//
// Union types allow variables to have multiple types

type Result = Success | Error;

class Success {
  constructor(public value: any) {}

  isSuccess(): boolean {
    return true;
  }
}

class Error {
  constructor(public message: string) {}

  isSuccess(): boolean {
    return false;
  }
}

function handleResult(result: Result): void {
  if (result.isSuccess()) {
    console.log(`✅ Success: ${(result as Success).value}`);
  } else {
    console.log(`❌ Error: ${(result as Error).message}`);
  }
}

//
// =======================================================
// 11. METHOD OVERRIDING WITH SUPER
// =======================================================
//

class Employee {
  constructor(
    protected name: string,
    protected salary: number,
  ) {}

  work(): void {
    console.log(`${this.name} is working...`);
  }

  getInfo(): string {
    return `${this.name} earns $${this.salary}`;
  }
}

class Manager extends Employee {
  constructor(
    name: string,
    salary: number,
    private teamSize: number,
  ) {
    super(name, salary);
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
// 12. REAL-WORLD EXAMPLE: NOTIFICATION SYSTEM
// =======================================================
//

abstract class Notification {
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

  abstract send(): void;
}

class EmailNotification extends Notification {
  constructor(
    recipient: string,
    message: string,
    private subject: string,
  ) {
    super(recipient, message);
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
  private static readonly MAX_LENGTH = 160;

  constructor(
    recipient: string,
    message: string,
    private phoneNumber: string,
  ) {
    super(recipient, message);
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
  constructor(
    recipient: string,
    message: string,
    private deviceToken: string,
    private priority: string,
  ) {
    super(recipient, message);
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
// 13. DEMONSTRATION
// =======================================================
//

function main(): void {
  console.log("=== Polymorphism in TypeScript Demo ===\n");

  // ---- Function Overloading ----
  console.log("1. Function Overloading (Compile-time):");
  const calc = new Calculator();
  console.log(`Result: ${calc.add(5, 3)}`);
  console.log(`Result: ${calc.add(5, 3, 2)}`);
  console.log(`Result: ${calc.add("Hello", " World")}`);

  // ---- Method Overriding ----
  console.log("\n2. Method Overriding (Runtime Polymorphism):");
  const circle = new Circle("Blue", 5);
  const rectangle = new Rectangle("Green", 10, 20);

  const shapes: Shape[] = [circle, rectangle];
  shapes.forEach((shape) => shape.draw());

  // ---- Interface-based Polymorphism ----
  console.log("\n3. Interface-based Polymorphism:");
  const doc = new Document("My Document", "This is important content.");
  const img = new Image("photo", "jpg");

  const printables: Printable[] = [doc, img];
  printables.forEach((p) => p.print());

  // ---- Abstract Classes ----
  console.log("\n4. Abstract Classes:");
  const dog = new Dog("Buddy");
  const cat = new Cat("Whiskers");

  const animals: Animal[] = [dog, cat];
  animals.forEach((animal) => {
    animal.makeSound();
    animal.sleep();
  });

  // ---- Payment System ----
  console.log("\n5. Real-World Example (Payment System):");
  const creditCard = new CreditCardPayment("ACC001", "1234567890123456");
  const paypal = new PayPalPayment("ACC002", "user@example.com");
  const crypto = new CryptoPayment(
    "ACC003",
    "0x742d35Cc6634C0532925a3b844Bc9e7595f0bEb",
  );

  const payments: PaymentMethod[] = [creditCard, paypal, crypto];
  payments.forEach((payment) => processTransaction(payment, 100.0));

  // ---- Generic Polymorphism ----
  console.log("\n6. Generic Polymorphism (Stack):");
  const numberStack = new Stack<number>();
  numberStack.push(1);
  numberStack.push(2);
  numberStack.push(3);
  console.log(`Stack size: ${numberStack.size()}`);
  console.log(`Popped: ${numberStack.pop()}`);

  const stringStack = new Stack<string>();
  stringStack.push("Hello");
  stringStack.push("World");
  console.log(`Stack size: ${stringStack.size()}`);
  console.log(`Popped: ${stringStack.pop()}`);

  // ---- Duck Typing ----
  console.log("\n7. Duck Typing (Structural Polymorphism):");
  const bird = new Bird();
  const plane = new Airplane();

  makeFly(bird); // Works with Bird
  makeFly(plane); // Works with Airplane (structural typing!)

  // ---- Union Types ----
  console.log("\n8. Union Types:");
  const success = new Success(42);
  const error = new Error("Something went wrong");

  handleResult(success);
  handleResult(error);

  // ---- Method Overriding with Super ----
  console.log("\n9. Method Overriding with Super:");
  const manager = new Manager("Alice", 120000, 5);
  manager.work();
  console.log(manager.getInfo());

  // ---- Notification System ----
  console.log("\n10. Real-World Example (Notification System):");
  const email = new EmailNotification(
    "alice@example.com",
    "Your order has been shipped!",
    "Order Update",
  );
  const sms = new SMSNotification(
    "Bob",
    "Your verification code is 482910.",
    "+1-555-0123",
  );
  const push = new PushNotification(
    "Charlie",
    "New message from Alice",
    "d8a3f4b2c1e5a9b7",
    "high",
  );

  const notifications: Notification[] = [email, sms, push];
  notifications.forEach((notification) => notification.send());

  console.log("\n=== Demo Complete ===");
}

// Execute
main();

export {};

/*
📘 Key Insights:

1. **TypeScript Polymorphism**:
   - Compile-time: Function overloading (limited), generics
   - Runtime: Method overriding, interface implementation
   - Structural typing: Duck typing (if it quacks like a duck...)

2. **Function Overloading**:
   - Overload signatures + single implementation
   - Less flexible than C++ but type-safe
   - Runtime checks needed in implementation

3. **Method Overriding**:
   - Child classes override parent methods
   - No special keyword needed (unlike C++'s virtual)
   - Runtime polymorphic behavior

4. **Interfaces**:
   - Define contracts
   - Structural typing (shape-based)
   - Multiple interface implementation
   - No runtime representation

5. **Abstract Classes**:
   - Cannot instantiate
   - abstract methods must be implemented
   - Can have concrete methods
   - Use when sharing implementation

6. **Generics**:
   - Compile-time polymorphism
   - Type-safe reusable code
   - Works with any type

7. **Duck Typing**:
   - Structural typing unique to TypeScript
   - If it has the right shape, it works
   - More flexible than nominal typing

Best Practices:
- Use interfaces for contracts
- Use abstract classes for shared behavior
- Leverage generics for type-safe code
- Use method overriding for runtime polymorphism
- Prefer composition over inheritance when possible

When to Use:
✅ Need different behaviors for related types
✅ Want to add new types without changing existing code
✅ Building extensible systems
✅ Type-safe generic code needed

TypeScript Advantages:
- Type safety at compile time
- Structural typing flexibility
- Generics for reusable code
- Clean syntax (no virtual keyword needed)
*/
