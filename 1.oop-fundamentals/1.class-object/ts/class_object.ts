//
// =======================================================
// 1. WHAT ARE CLASSES AND OBJECTS?
// =======================================================
//
// A CLASS is a blueprint that defines the structure and behavior of objects.
// It encapsulates data (properties) and functions (methods).
//
// An OBJECT is an instance of a class - a concrete entity created from
// the class blueprint.
//
// In TypeScript, classes are a syntactic sugar over JavaScript's
// prototype-based inheritance, with added type safety.

//
// =======================================================
// 2. BASIC CLASS DEFINITION
// =======================================================
//

class BasicCar {
    // Properties (attributes) - TypeScript requires type annotations
    private brand: string;
    private model: string;
    private speed: number;

    // Constructor
    constructor(brand: string, model: string) {
        this.brand = brand;
        this.model = model;
        this.speed = 0;
    }

    // Methods
    setBrand(b: string): void {
        this.brand = b;
    }

    setSpeed(s: number): void {
        this.speed = s;
    }

    displayInfo(): void {
        console.log(`${this.brand} ${this.model} at ${this.speed} km/h`);
    }
}

//
// =======================================================
// 3. CONSTRUCTORS AND INITIALIZATION
// =======================================================
//

class Car {
    private brand: string;
    private model: string;
    private speed: number = 0; // Default value

    // Standard constructor
    constructor(brand: string, model: string) {
        console.log(`🚗 Creating ${brand} ${model}`);
        this.brand = brand;
        this.model = model;
    }

    accelerate(increment: number): void {
        this.speed += increment;
    }

    displayStatus(): void {
        console.log(`${this.brand} ${this.model} | Speed: ${this.speed} km/h`);
    }
}

// TYPESCRIPT MAGIC: Parameter Properties! ✨
class CompactCar {
    // Constructor with parameter properties (declares and initializes in one line!)
    constructor(
        private brand: string,
        private model: string,
        private speed: number = 0,
    ) {
        console.log(`🚗 Compact car created: ${brand} ${model}`);
    }

    accelerate(increment: number): void {
        this.speed += increment;
    }

    displayStatus(): void {
        console.log(`${this.brand} ${this.model} | Speed: ${this.speed} km/h`);
    }
}

//
// =======================================================
// 4. THE 'this' KEYWORD
// =======================================================
//
// In TypeScript/JavaScript, 'this' refers to the current object
// BUT its value depends on HOW the function is called

class Rectangle {
    constructor(
        private width: number,
        private height: number,
    ) {}

    // Regular method - 'this' binds to the object
    area(): number {
        return this.width * this.height;
    }

    // Arrow function - 'this' is lexically bound (captured from surrounding context)
    getAreaArrow = (): number => {
        return this.width * this.height;
    };

    // Method chaining - return 'this' for fluent interface
    setWidth(w: number): Rectangle {
        this.width = w;
        return this; // Return current object
    }

    setHeight(h: number): Rectangle {
        this.height = h;
        return this;
    }

    display(): void {
        console.log(
            `Rectangle [${this.width}x${this.height}] Area: ${this.area()}`,
        );
    }
}

//
// =======================================================
// 5. ACCESS MODIFIERS
// =======================================================
//
// public:    Accessible from anywhere (default in TypeScript)
// private:   Accessible only within the class
// protected: Accessible within class and derived classes
// readonly:  Can't be modified after initialization (TypeScript-specific)

class BankAccount {
    private accountNumber: string;
    private balance: number;
    protected accountType: string;
    readonly createdDate: Date; // readonly - cannot change after init

    constructor(accNum: string, initialBalance: number) {
        this.accountNumber = accNum;
        this.balance = initialBalance;
        this.accountType = "Savings";
        this.createdDate = new Date();
    }

    // Getter methods
    getAccountNumber(): string {
        return this.accountNumber;
    }

    getBalance(): number {
        return this.balance;
    }

    // Methods with validation
    deposit(amount: number): void {
        if (amount > 0) {
            this.balance += amount;
            console.log(`💰 Deposited $${amount}`);
        } else {
            console.log(`❌ Invalid deposit amount`);
        }
    }

    withdraw(amount: number): void {
        if (amount > 0 && amount <= this.balance) {
            this.balance -= amount;
            console.log(`💸 Withdrawn $${amount}`);
        } else {
            console.log(`❌ Insufficient funds or invalid amount`);
        }
    }

    displayAccount(): void {
        console.log(
            `Account: ${this.accountNumber} | Balance: $${this.balance}`,
        );
    }
}

//
// =======================================================
// 6. STATIC MEMBERS
// =======================================================
//
// Static members belong to the CLASS, not individual objects
// Shared across all instances

class Counter {
    private static objectCount: number = 0; // Static property
    private id: number;

    constructor() {
        Counter.objectCount++;
        this.id = Counter.objectCount;
        console.log(`🔢 Counter object #${this.id} created`);
    }

    // Static method - can only access static members
    static getCount(): number {
        return Counter.objectCount;
    }

    displayId(): void {
        console.log(`My ID: ${this.id}`);
    }
}

//
// =======================================================
// 7. GETTERS AND SETTERS
// =======================================================
//
// TypeScript supports get/set accessors for computed properties

class Person {
    constructor(
        private firstName: string,
        private lastName: string,
        private _age: number = 0,
    ) {}

    // Getter - access like a property
    get fullName(): string {
        return `${this.firstName} ${this.lastName}`;
    }

    // Setter - set like a property
    set fullName(value: string) {
        const parts = value.split(" ");
        this.firstName = parts[0];
        this.lastName = parts[1] || "";
    }

    get age(): number {
        return this._age;
    }

    // Setter with validation
    set age(value: number) {
        if (value >= 0 && value <= 150) {
            this._age = value;
        } else {
            console.log("❌ Invalid age");
        }
    }

    display(): void {
        console.log(`${this.fullName}, Age: ${this.age}`);
    }
}

//
// =======================================================
// 8. PRIVATE FIELDS (ES2022+)
// =======================================================
//
// JavaScript now has true private fields with # syntax
// Different from TypeScript's 'private' (which is compile-time only)

class ModernAccount {
    #balance: number; // True private field (runtime enforced)
    private accountId: string; // TypeScript private (compile-time only)

    constructor(accountId: string, initialBalance: number) {
        this.accountId = accountId;
        this.#balance = initialBalance;
    }

    deposit(amount: number): void {
        this.#balance += amount;
    }

    getBalance(): number {
        return this.#balance;
    }

    display(): void {
        console.log(`Account ${this.accountId}: $${this.#balance}`);
    }
}

//
// =======================================================
// 9. COMPARISON: TypeScript private vs # private
// =======================================================
//
// TypeScript 'private':
//   - Compile-time check only
//   - Erased in JavaScript output
//   - Can be accessed via JavaScript tricks
//
// JavaScript '#' private:
//   - Runtime enforcement
//   - Truly private (even in JavaScript)
//   - Cannot be accessed outside class

//
// =======================================================
// 10. OBJECT LIFECYCLE
// =======================================================
//

class Resource {
    private data: number;
    private name: string;

    constructor(name: string, value: number) {
        this.name = name;
        this.data = value;
        console.log(`📦 Resource '${name}' created`);
    }

    // TypeScript/JavaScript has no destructors (garbage collected)
    // But you can provide cleanup methods
    dispose(): void {
        console.log(`🗑️  Resource '${this.name}' disposed`);
        // Cleanup code here
    }

    display(): void {
        console.log(`${this.name} -> ${this.data}`);
    }

    setValue(val: number): void {
        this.data = val;
    }
}

//
// =======================================================
// 11. REAL-WORLD EXAMPLE: ENHANCED VEHICLE CLASS
// =======================================================
//

class Vehicle {
    private static totalVehicles: number = 0;
    private speed: number = 0;
    private fuel: number = 100;

    constructor(
        private brand: string,
        private model: string,
        private year: number = 2024,
    ) {
        Vehicle.totalVehicles++;
    }

    // Method chaining for fluent interface
    accelerate(increment: number): Vehicle {
        if (this.fuel > 0) {
            this.speed += increment;
            this.fuel -= increment * 0.1;
        } else {
            console.log("⚠️  Out of fuel!");
        }
        return this;
    }

    brake(decrement: number): Vehicle {
        this.speed = Math.max(0, this.speed - decrement);
        return this;
    }

    refuel(amount: number): Vehicle {
        this.fuel = Math.min(100, this.fuel + amount);
        console.log(`⛽ Refueled +${amount}%`);
        return this;
    }

    displayStatus(): void {
        console.log(`🚙 ${this.year} ${this.brand} ${this.model}`);
        console.log(
            `   Speed: ${this.speed} km/h | Fuel: ${this.fuel.toFixed(1)}%`,
        );
    }

    static getTotalVehicles(): number {
        return Vehicle.totalVehicles;
    }
}

//
// =======================================================
// 12. TYPESCRIPT-SPECIFIC FEATURES SUMMARY
// =======================================================
//
// 1. Parameter properties:
//    constructor(private x: number) {}
//
// 2. readonly modifier:
//    readonly createdAt: Date;
//
// 3. Type annotations:
//    private name: string;
//
// 4. Optional properties:
//    middleName?: string;
//
// 5. Getters/setters:
//    get fullName(): string { ... }
//
// 6. Access modifiers compile-time checked:
//    private, protected, public
//
// 7. JavaScript # private fields (runtime):
//    #balance: number;

//
// =======================================================
// 13. DEMONSTRATION
// =======================================================
//

function main(): void {
    console.log("=== Classes and Objects in TypeScript Demo ===\n");

    // ---- Basic class ----
    console.log("1. Basic Class:");
    const basic = new BasicCar("Honda", "Civic");
    basic.setSpeed(80);
    basic.displayInfo();

    // ---- Standard vs Parameter Properties ----
    console.log("\n2. Standard Constructor vs Parameter Properties:");
    const car1 = new Car("Toyota", "Camry");
    const car2 = new CompactCar("Mazda", "3");
    car1.accelerate(60);
    car2.accelerate(70);
    car1.displayStatus();
    car2.displayStatus();

    // ---- this and Method Chaining ----
    console.log("\n3. 'this' Keyword and Method Chaining:");
    const rect = new Rectangle(10, 20);
    rect.setWidth(15).setHeight(25).display(); // Chaining!

    // ---- Access Modifiers ----
    console.log("\n4. Access Modifiers and Encapsulation:");
    const account = new BankAccount("ACC123456", 1000);
    account.deposit(500);
    account.withdraw(200);
    account.displayAccount();
    // account.balance = 999999;  // ❌ Error: private property

    // ---- Static Members ----
    console.log("\n5. Static Members:");
    console.log(`Initial count: ${Counter.getCount()}`);
    const c1 = new Counter();
    const c2 = new Counter();
    const c3 = new Counter();
    console.log(`Count after creation: ${Counter.getCount()}`);

    // ---- Getters and Setters ----
    console.log("\n6. Getters and Setters:");
    const person = new Person("John", "Doe", 30);
    console.log(`Full name: ${person.fullName}`); // Getter
    person.fullName = "Jane Smith"; // Setter
    person.age = 25;
    person.display();

    // ---- Modern Private Fields ----
    console.log("\n7. Modern Private Fields (#):");
    const modern = new ModernAccount("MOD001", 5000);
    modern.deposit(1000);
    modern.display();
    // console.log(modern.#balance);  // ❌ Error: truly private!

    // ---- Object Lifecycle ----
    console.log("\n8. Object Lifecycle:");
    const res1 = new Resource("Original", 42);
    const res2 = new Resource("Copy", 99);
    res1.display();
    res2.display();
    res1.dispose();
    res2.dispose();

    // ---- Real-World Example (Method Chaining) ----
    console.log("\n9. Real-World Example (Vehicle with Chaining):");
    const tesla = new Vehicle("Tesla", "Model 3", 2024);
    tesla.accelerate(50).accelerate(30).brake(20).displayStatus();

    const toyota = new Vehicle("Toyota", "Prius");
    toyota.accelerate(40).refuel(20).displayStatus();

    console.log(`\nTotal vehicles: ${Vehicle.getTotalVehicles()}`);

    console.log("\n=== Demo Complete ===");
}

// Execute the main function
main();

// Make this file a module to avoid global scope conflicts
export {};
