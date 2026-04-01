//
// =======================================================
// 1. WHAT IS ENCAPSULATION?
// =======================================================
//
// Encapsulation is one of the four foundational principles of OOP.
// It bundles data (properties) and methods that operate on that data
// into a single unit (class), while restricting direct access to
// internal details.
//
// Key formula:
// Encapsulation = Data Hiding + Controlled Access
//
// Benefits:
// - Protects data integrity through validation
// - Hides implementation details
// - Provides a clean, stable interface
// - Makes code easier to maintain and refactor
//
// Implementation in TypeScript:
// 1. Access modifiers (private, protected, public)
// 2. Getters and setters (native syntax!)
// 3. Private fields with # (ES2022+)
// 4. readonly modifier for immutability

//
// =======================================================
// 2. BASIC ENCAPSULATION: BANK ACCOUNT
// =======================================================
//
// Without encapsulation:
//   account.balance = -1000000;  // ❌ Dangerous!
//
// With encapsulation: control access and enforce rules

class BankAccount {
  // Private property - hidden from outside
  private balance: number = 0;
  private accountNumber: string;

  constructor(accountNumber: string) {
    this.accountNumber = accountNumber;
    console.log(`💳 Account ${accountNumber} created`);
  }

  // Public methods - controlled interface
  deposit(amount: number): void {
    if (amount <= 0) {
      console.log("❌ Deposit amount must be positive");
      return;
    }
    this.balance += amount;
    console.log(`✅ Deposited $${amount}`);
  }

  withdraw(amount: number): void {
    if (amount <= 0) {
      console.log("❌ Withdrawal amount must be positive");
      return;
    }
    if (amount > this.balance) {
      console.log("❌ Insufficient funds!");
      return;
    }
    this.balance -= amount;
    console.log(`✅ Withdrawn $${amount}`);
  }

  // Getter - read-only access
  getBalance(): number {
    return this.balance;
  }

  getAccountNumber(): string {
    return this.accountNumber;
  }

  displayAccount(): void {
    console.log(`Account ${this.accountNumber} | Balance: $${this.balance}`);
  }
}

//
// =======================================================
// 3. INTERMEDIATE: PRIVATE HELPER METHODS
// =======================================================
//
// Hide implementation details with private methods

class PaymentProcessor {
  private cardNumber: string;
  private amount: number;

  constructor(cardNumber: string, amount: number) {
    this.cardNumber = this.maskCardNumber(cardNumber);
    this.amount = amount;
    console.log("💳 Payment processor initialized");
  }

  // Private helper - implementation detail hidden
  private maskCardNumber(raw: string): string {
    if (raw.length < 4) {
      return "****";
    }
    return "****-****-****-" + raw.substring(raw.length - 4);
  }

  private validateAmount(amt: number): boolean {
    return amt > 0 && amt <= 10000; // Max transaction limit
  }

  // Public interface
  processPayment(): void {
    if (!this.validateAmount(this.amount)) {
      console.log("❌ Invalid payment amount");
      return;
    }
    console.log(
      `✅ Processing payment of $${this.amount} for card ${this.cardNumber}`,
    );
  }

  getAmount(): number {
    return this.amount;
  }

  getMaskedCard(): string {
    return this.cardNumber;
  }
}

//
// =======================================================
// 4. ADVANCED: NATIVE GETTERS AND SETTERS
// =======================================================
//
// TypeScript supports get/set accessors (property-like syntax!)

class Employee {
  private name: string;
  private _age: number;
  private _salary: number;
  private department: string;

  constructor(name: string, age: number, salary: number, department: string) {
    this.name = name;
    this._age = age;
    this._salary = salary;
    this.department = department;
  }

  // Getter - access like a property
  get age(): number {
    return this._age;
  }

  // Setter with validation
  set age(newAge: number) {
    if (newAge < 18 || newAge > 100) {
      console.log("❌ Invalid age");
      return;
    }
    this._age = newAge;
    console.log(`✅ Age updated to ${newAge}`);
  }

  get salary(): number {
    return this._salary;
  }

  set salary(newSalary: number) {
    if (newSalary < 0) {
      console.log("❌ Salary cannot be negative");
      return;
    }
    if (newSalary < this._salary) {
      console.log("⚠️  Warning: Salary decreased");
    }
    this._salary = newSalary;
    console.log(`✅ Salary updated to $${newSalary}`);
  }

  // Computed property (no setter!)
  get fullInfo(): string {
    return `${this.name} | Age: ${this._age} | Salary: $${this._salary} | Dept: ${this.department}`;
  }

  setDepartment(newDept: string): void {
    this.department = newDept;
    console.log(`✅ Department updated to ${newDept}`);
  }

  displayInfo(): void {
    console.log(this.fullInfo);
  }
}

//
// =======================================================
// 5. TYPESCRIPT ACCESS MODIFIERS
// =======================================================
//
// TypeScript offers: public (default), private, protected, readonly

class Shape {
  // Protected - accessible in derived classes
  protected area: number = 0;
  protected name: string;

  // Private - only in this class
  private shapeId: number;
  private static nextId: number = 0;

  // Readonly - cannot change after initialization
  readonly createdAt: Date;

  constructor(shapeName: string) {
    this.name = shapeName;
    this.shapeId = ++Shape.nextId;
    this.createdAt = new Date();
    console.log(`🔷 Shape '${shapeName}' created with ID: ${this.shapeId}`);
  }

  // Public method
  getArea(): number {
    return this.area;
  }

  getId(): number {
    return this.shapeId;
  }

  displayInfo(): void {
    console.log(
      `Shape: ${this.name} | ID: ${this.shapeId} | Area: ${this.area}`,
    );
  }
}

class Rectangle extends Shape {
  private width: number;
  private height: number;

  constructor(width: number, height: number) {
    super("Rectangle");
    this.width = width;
    this.height = height;
    this.calculateArea();
  }

  private calculateArea(): void {
    // Can access protected 'area' from base class
    this.area = this.width * this.height;
  }

  setDimensions(w: number, h: number): void {
    if (w <= 0 || h <= 0) {
      console.log("❌ Dimensions must be positive");
      return;
    }
    this.width = w;
    this.height = h;
    this.calculateArea();
    console.log("✅ Dimensions updated");
  }
}

//
// =======================================================
// 6. PRIVATE FIELDS WITH # (ES2022+)
// =======================================================
//
// JavaScript now supports true private fields with #
// Different from TypeScript's 'private' (compile-time only)

class SecureAccount {
  // True private field (runtime enforced)
  #balance: number;
  // TypeScript private (compile-time only)
  private accountId: string;
  // Public
  public accountHolder: string;

  constructor(accountId: string, holder: string, initialBalance: number) {
    this.accountId = accountId;
    this.accountHolder = holder;
    this.#balance = initialBalance;
  }

  deposit(amount: number): void {
    if (amount > 0) {
      this.#balance += amount;
      console.log(`✅ Deposited $${amount}`);
    }
  }

  // Private method with #
  #calculateInterest(): number {
    return this.#balance * 0.05;
  }

  applyInterest(): void {
    const interest = this.#calculateInterest();
    this.#balance += interest;
    console.log(`✅ Applied interest: $${interest.toFixed(2)}`);
  }

  getBalance(): number {
    return this.#balance;
  }

  display(): void {
    console.log(
      `Account ${this.accountId} (${this.accountHolder}): $${this.#balance.toFixed(2)}`,
    );
  }
}

//
// =======================================================
// 7. REAL-WORLD EXAMPLE: USER AUTHENTICATION
// =======================================================
//

class User {
  private username: string;
  private passwordHash: string;
  private isActive: boolean = true;
  private loginAttempts: number = 0;

  constructor(username: string, password: string) {
    this.username = username;
    if (!this.isStrongPassword(password)) {
      console.log("⚠️  Weak password detected");
    }
    this.passwordHash = this.hashPassword(password);
    console.log(`👤 User '${username}' created`);
  }

  // Private helper: hash password (simplified)
  private hashPassword(password: string): string {
    // In real world, use bcrypt, argon2, etc.
    return "hash_" + password;
  }

  // Private helper: validate password strength
  private isStrongPassword(password: string): boolean {
    return password.length >= 8;
  }

  // Public login method
  login(password: string): boolean {
    if (!this.isActive) {
      console.log("❌ Account is locked");
      return false;
    }

    if (this.hashPassword(password) === this.passwordHash) {
      console.log("✅ Login successful!");
      this.loginAttempts = 0;
      return true;
    } else {
      this.loginAttempts++;
      console.log(`❌ Invalid password (Attempt ${this.loginAttempts})`);

      if (this.loginAttempts >= 3) {
        this.isActive = false;
        console.log("🔒 Account locked due to multiple failed attempts");
      }
      return false;
    }
  }

  resetPassword(oldPassword: string, newPassword: string): void {
    if (this.hashPassword(oldPassword) !== this.passwordHash) {
      console.log("❌ Old password incorrect");
      return;
    }
    if (!this.isStrongPassword(newPassword)) {
      console.log("❌ New password is too weak");
      return;
    }
    this.passwordHash = this.hashPassword(newPassword);
    console.log("✅ Password updated successfully");
  }

  getUsername(): string {
    return this.username;
  }

  getActiveStatus(): boolean {
    return this.isActive;
  }

  // Never expose password or hash!
}

//
// =======================================================
// 8. READONLY MODIFIER
// =======================================================
//
// TypeScript-specific: readonly prevents modification after initialization

class Product {
  readonly id: string;
  readonly createdAt: Date;
  private name: string;
  private price: number;

  constructor(id: string, name: string, price: number) {
    this.id = id; // Can set in constructor
    this.createdAt = new Date();
    this.name = name;
    this.price = price;
  }

  updatePrice(newPrice: number): void {
    if (newPrice > 0) {
      this.price = newPrice;
      console.log(`✅ Price updated to $${newPrice}`);
    }
    // this.id = "new-id";  // ❌ Error: readonly property
  }

  get productInfo(): string {
    return `Product ${this.id}: ${this.name} - $${this.price}`;
  }
}

//
// =======================================================
// 9. COMPLEX ENCAPSULATION: SHOPPING CART
// =======================================================
//

class ShoppingCart {
  private items: string[] = [];
  private prices: number[] = [];
  private discount: number = 0;

  // Private helper
  private calculateSubtotal(): number {
    return this.prices.reduce((sum, price) => sum + price, 0);
  }

  addItem(item: string, price: number): void {
    if (price <= 0) {
      console.log("❌ Invalid price");
      return;
    }
    this.items.push(item);
    this.prices.push(price);
    console.log(`✅ Added '${item}' ($${price})`);
  }

  removeItem(item: string): void {
    const index = this.items.indexOf(item);
    if (index === -1) {
      console.log("❌ Item not found");
      return;
    }
    this.items.splice(index, 1);
    this.prices.splice(index, 1);
    console.log(`✅ Removed '${item}'`);
  }

  applyDiscount(percent: number): void {
    if (percent < 0 || percent > 100) {
      console.log("❌ Invalid discount percentage");
      return;
    }
    this.discount = percent;
    console.log(`✅ Applied ${percent}% discount`);
  }

  // Getter for total
  get total(): number {
    const subtotal = this.calculateSubtotal();
    return subtotal * (1 - this.discount / 100);
  }

  displayCart(): void {
    console.log("\n🛒 Shopping Cart:");
    this.items.forEach((item, i) => {
      console.log(`  - ${item}: $${this.prices[i]}`);
    });
    console.log(`Subtotal: $${this.calculateSubtotal().toFixed(2)}`);
    console.log(`Discount: ${this.discount}%`);
    console.log(`Total: $${this.total.toFixed(2)}`);
  }
}

//
// =======================================================
// 10. PARAMETER PROPERTIES WITH ENCAPSULATION
// =======================================================
//
// TypeScript allows declaring properties in constructor parameters

class Book {
  constructor(
    private isbn: string,
    private title: string,
    private author: string,
    private price: number,
  ) {
    console.log(`📚 Book created: ${title} by ${author}`);
  }

  // Getters
  get bookInfo(): string {
    return `${this.title} by ${this.author} (ISBN: ${this.isbn})`;
  }

  get bookPrice(): number {
    return this.price;
  }

  // Setter with validation
  set bookPrice(newPrice: number) {
    if (newPrice < 0) {
      throw new Error("Price cannot be negative");
    }
    this.price = newPrice;
  }

  displayInfo(): void {
    console.log(`${this.bookInfo} - $${this.price}`);
  }
}

//
// =======================================================
// 11. DEMONSTRATION
// =======================================================
//

function main(): void {
  console.log("=== Encapsulation in TypeScript Demo ===\n");

  // ---- Basic Encapsulation ----
  console.log("1. Basic Encapsulation (BankAccount):");
  const account = new BankAccount("ACC-12345");
  account.deposit(1000);
  account.withdraw(250);
  account.displayAccount();
  // account.balance = 999999; // ❌ Error: private property

  console.log("\n2. Private Helper Methods (PaymentProcessor):");
  const payment = new PaymentProcessor("1234567812345678", 500);
  payment.processPayment();

  console.log("\n3. Getters and Setters (Employee):");
  const emp = new Employee("Alice", 30, 75000, "Engineering");
  emp.displayInfo();
  emp.salary = 80000; // Using setter
  emp.age = 31; // Using setter
  emp.age = 200; // Invalid - rejected by setter

  console.log("\n4. Access Levels (Shape & Rectangle):");
  const rect = new Rectangle(10, 20);
  rect.displayInfo();
  rect.setDimensions(15, 25);
  rect.displayInfo();

  console.log("\n5. Private Fields with # (SecureAccount):");
  const secure = new SecureAccount("SEC-001", "John Doe", 5000);
  secure.deposit(1000);
  secure.applyInterest();
  secure.display();
  // console.log(secure.#balance); // ❌ Error: truly private!

  console.log("\n6. Real-World Example (User Authentication):");
  const user = new User("john_doe", "SecurePass123");
  user.login("WrongPassword");
  user.login("WrongPassword");
  user.login("SecurePass123");

  console.log("\n7. Readonly Modifier (Product):");
  const product = new Product("PROD-001", "Laptop", 999.99);
  console.log(product.productInfo);
  product.updatePrice(899.99);
  // product.id = "new-id"; // ❌ Error: readonly

  console.log("\n8. Complex Encapsulation (ShoppingCart):");
  const cart = new ShoppingCart();
  cart.addItem("Laptop", 999.99);
  cart.addItem("Mouse", 25.5);
  cart.addItem("Keyboard", 75.0);
  cart.applyDiscount(10);
  cart.displayCart();
  cart.removeItem("Mouse");
  cart.displayCart();

  console.log("\n9. Parameter Properties (Book):");
  const book = new Book(
    "978-0134685991",
    "Effective TypeScript",
    "Dan Vanderkam",
    39.99,
  );
  book.displayInfo();
  book.bookPrice = 34.99; // Using setter
  book.displayInfo();

  console.log("\n=== Demo Complete ===");
}

// Execute
main();

/*
📘 Key Insights:

1. **TypeScript Access Modifiers** (Compile-time only):
   - private: Checked by TypeScript compiler
   - protected: For inheritance
   - public: Default

2. **JavaScript # Private Fields** (Runtime):
   - Truly private at runtime
   - Cannot access outside class, even in JavaScript

3. **Native Getters/Setters**:
   - Use 'get'/'set' keywords
   - Access like properties: obj.prop = value
   - Add validation in setters

4. **readonly Modifier**:
   - TypeScript-specific
   - Prevents changes after initialization
   - Perfect for IDs, timestamps

5. **Parameter Properties**:
   - Declare and initialize in constructor
   - Less boilerplate
   - Can combine with access modifiers

Best Practices:
- Default to private for all properties
- Use getters for computed properties
- Use setters with validation
- Use # for true runtime privacy
- Use readonly for immutable data
- Leverage TypeScript's native get/set syntax
*/
