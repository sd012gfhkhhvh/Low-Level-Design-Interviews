//
// =======================================================
// 1. WHAT IS AN INTERFACE IN TYPESCRIPT?
// =======================================================
//
// TypeScript has a dedicated `interface` keyword for defining contracts.
// Interfaces define the SHAPE of an object - what properties and methods
// it must have.
//
// Key characteristics:
//   - Define contracts for objects and classes
//   - Compile-time only (erased at runtime)
//   - Support multiple inheritance naturally
//   - No implementation details
//
// Purpose: Type safety + developer experience

//
// =======================================================
// 2. BASIC INTERFACE EXAMPLE
// =======================================================
//

// Interface: Defines what a drawable object must have
interface Drawable {
    draw(): void;
}

// Concrete implementation 1
class Circle implements Drawable {
    draw(): void {
        console.log("Drawing a Circle ⭕️");
    }
}

// Concrete implementation 2
class Rectangle implements Drawable {
    draw(): void {
        console.log("Drawing a Rectangle ▭");
    }
}

//
// =======================================================
// 3. MULTIPLE INTERFACES (TypeScript Native Feature)
// =======================================================
//
// TypeScript fully supports implementing multiple interfaces
// This is cleaner than C++ multiple inheritance

interface Movable {
    move(x: number, y: number): void;
}

interface Resizable {
    resize(factor: number): void;
}

// A class implementing multiple interfaces
class Shape implements Drawable, Movable, Resizable {
    private name: string;
    private x = 0;
    private y = 0;
    private size = 1.0;

    constructor(name: string) {
        this.name = name;
    }

    draw(): void {
        console.log(
            `Drawing ${this.name} at (${this.x},${this.y}) size=${this.size}`
        );
    }

    move(x: number, y: number): void {
        this.x = x;
        this.y = y;
        console.log(`${this.name} moved to (${x},${y})`);
    }

    resize(factor: number): void {
        this.size *= factor;
        console.log(`${this.name} resized by factor ${factor}`);
    }
}

//
// =======================================================
// 4. INTERFACE SEGREGATION PRINCIPLE (ISP)
// =======================================================
//
// "Clients should not be forced to depend on methods they don't use"

// ❌ BAD: Fat interface
interface BadAnimal {
    walk(): void;
    fly(): void;
    swim(): void;
}
// Problem: Dog must implement fly(), Bird must implement swim()

// ✅ GOOD: Segregated interfaces
interface Walkable {
    walk(): void;
}

interface Flyable {
    fly(): void;
}

interface Swimmable {
    swim(): void;
}

// Compose exactly what you need
class Dog implements Walkable, Swimmable {
    walk(): void {
        console.log("🐕 Dog is walking");
    }

    swim(): void {
        console.log("🐕 Dog is swimming");
    }
}

class Duck implements Walkable, Flyable, Swimmable {
    walk(): void {
        console.log("🦆 Duck is walking");
    }

    fly(): void {
        console.log("🦆 Duck is flying");
    }

    swim(): void {
        console.log("🦆 Duck is swimming");
    }
}

//
// =======================================================
// 5. POLYMORPHISM WITH INTERFACES
// =======================================================
//
// Interfaces enable polymorphism: one interface, many implementations

function renderShapes(shapes: Drawable[]): void {
    console.log("\n--- Rendering all shapes ---");
    for (const shape of shapes) {
        shape.draw(); // Polymorphic call
    }
}

//
// =======================================================
// 6. REAL-WORLD EXAMPLE: PAYMENT GATEWAY
// =======================================================
//
// Strategy Pattern using interfaces

interface PaymentGateway {
    initiatePayment(amount: number): void;
    getProviderName(): string;
}

class StripePayment implements PaymentGateway {
    initiatePayment(amount: number): void {
        console.log(`💳 Processing payment via Stripe: $${amount}`);
    }

    getProviderName(): string {
        return "Stripe";
    }
}

class RazorpayPayment implements PaymentGateway {
    initiatePayment(amount: number): void {
        console.log(`💳 Processing payment via Razorpay: ₹${amount}`);
    }

    getProviderName(): string {
        return "Razorpay";
    }
}

class PayPalPayment implements PaymentGateway {
    initiatePayment(amount: number): void {
        console.log(`💳 Processing payment via PayPal: $${amount}`);
    }

    getProviderName(): string {
        return "PayPal";
    }
}

// Service using dependency injection
class CheckoutService {
    private gateway: PaymentGateway;

    constructor(gateway: PaymentGateway) {
        this.gateway = gateway;
    }

    setPaymentGateway(gateway: PaymentGateway): void {
        this.gateway = gateway;
    }

    processCheckout(amount: number): void {
        console.log(`Using ${this.gateway.getProviderName()}...`);
        this.gateway.initiatePayment(amount);
    }
}

//
// =======================================================
// 7. INTERFACE EXTENSION (TypeScript Specific)
// =======================================================
//
// TypeScript interfaces can extend other interfaces (like inheritance)

interface Printable {
    print(): void;
}

interface Serializable {
    serialize(): string;
}

// Interface extending multiple interfaces
interface Exportable extends Printable, Serializable {
    export(): void;
}

class Document implements Exportable {
    private content: string;

    constructor(content: string) {
        this.content = content;
    }

    print(): void {
        console.log(`📄 Document: ${this.content}`);
    }

    serialize(): string {
        return JSON.stringify({ content: this.content });
    }

    export(): void {
        console.log(`Exporting: ${this.content}`);
    }
}

//
// =======================================================
// 8. INTERFACE VS TYPE ALIAS
// =======================================================
//
// TypeScript has both `interface` and `type` - when to use which?

// Interface (preferred for objects/classes)
interface IUser {
    name: string;
    age: number;
}

// Type alias (more flexible)
type TUser = {
    name: string;
    age: number;
};

// Key differences:
// 1. Interfaces can be extended, types use intersections
// 2. Interfaces can be merged (declaration merging)
// 3. Types can represent primitives, unions, tuples
// 4. Interfaces are better for OOP patterns

//
// =======================================================
// 9. OPTIONAL PROPERTIES AND READONLY
// =======================================================
//

interface Config {
    readonly apiKey: string; // Cannot be modified after creation
    timeout?: number; // Optional property
    retries: number;
}

class ApiClient implements Config {
    readonly apiKey: string;
    timeout?: number;
    retries: number;

    constructor(apiKey: string, retries: number, timeout?: number) {
        this.apiKey = apiKey;
        this.retries = retries;
        this.timeout = timeout;
    }

    connect(): void {
        console.log(`Connecting with API key: ${this.apiKey.slice(0, 8)}...`);
        console.log(
            `Timeout: ${this.timeout ?? "default"}, Retries: ${this.retries}`
        );
    }
}

//
// =======================================================
// 10. FUNCTION INTERFACES
// =======================================================
//
// Interfaces can also define function signatures

interface Validator {
    (value: string): boolean;
}

const emailValidator: Validator = (value: string): boolean => {
    return value.includes("@");
};

const phoneValidator: Validator = (value: string): boolean => {
    return /^\d{10}$/.test(value);
};

//
// =======================================================
// 11. DEMONSTRATION
// =======================================================
//

function main(): void {
    console.log("=== Interface in TypeScript Demo ===\n");

    // ---- Basic interface usage ----
    console.log("1. Basic Interface:");
    const circle = new Circle();
    const rect = new Rectangle();
    circle.draw();
    rect.draw();

    // ---- Multiple interfaces ----
    console.log("\n2. Multiple Interfaces:");
    const triangle = new Shape("Triangle");
    triangle.draw();
    triangle.move(10, 20);
    triangle.resize(1.5);

    // ---- Interface segregation ----
    console.log("\n3. Interface Segregation:");
    const dog = new Dog();
    dog.walk();
    dog.swim();

    const duck = new Duck();
    duck.walk();
    duck.fly();
    duck.swim();

    // ---- Polymorphism ----
    console.log("\n4. Polymorphism:");
    renderShapes([circle, rect, triangle]);

    // ---- Real-world payment gateway ----
    console.log("\n5. Payment Gateway (Strategy Pattern):");
    const stripe = new StripePayment();
    const razorpay = new RazorpayPayment();
    const paypal = new PayPalPayment();

    const checkout = new CheckoutService(stripe);
    checkout.processCheckout(99.99);

    checkout.setPaymentGateway(razorpay);
    checkout.processCheckout(1500.0);

    checkout.setPaymentGateway(paypal);
    checkout.processCheckout(49.99);

    // ---- Interface extension ----
    console.log("\n6. Interface Extension:");
    const doc = new Document("Hello, TypeScript!");
    doc.print();
    console.log(`Serialized: ${doc.serialize()}`);
    doc.export();

    // ---- Config example ----
    console.log("\n7. Optional & Readonly:");
    const api = new ApiClient("sk_test_secret123", 3, 5000);
    api.connect();

    // ---- Function interfaces ----
    console.log("\n8. Function Interfaces:");
    console.log(`Email valid: ${emailValidator("test@example.com")}`);
    console.log(`Phone valid: ${phoneValidator("1234567890")}`);
}

main();
