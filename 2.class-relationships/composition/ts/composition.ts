//
// =======================================================
// 1. WHAT IS COMPOSITION?
// =======================================================
//
// Composition is a specialized form of Association that models a "whole-part"
// relationship with "strict ownership" and "full lifecycle control".
//
// Key Formula:
// Composition = "part-of" relationship + strict ownership + dependent lifecycles
//
// Mental Model:
// Think of a House and its Rooms:
// - A house (the whole) is composed of rooms (the parts)
// - If the house is destroyed, the rooms are destroyed with it.
// - A room cannot exist independently without a house.
// - A room belongs to exactly one house at a time.
//
// Key Characteristics of Composition:
// - Whole-part relationship
// - Strict ownership (the "whole" owns the "part")
// - Dependent lifecycles (parts are created by the whole and die with the whole)
// - Parts cannot be shared among multiple wholes
//
// UML Notation:
// Class1 <#>------- Class2  (filled diamond on the "whole" side)
// Whole <#>---- Part

//
// =======================================================
// 2. BASIC COMPOSITION: HOUSE AND ROOMS
// =======================================================
//
// A classical example of composition. The House contains Rooms.
// The rooms are created natively internally when the house is created.

class Room {
  private name: string;

  constructor(name: string) {
    this.name = name;
    console.log(`    [+] Room created: ${this.name}`);
  }

  destroy(): void {
    console.log(`    [-] Room destroyed: ${this.name}`);
  }

  getName(): string {
    return this.name;
  }
}

class House {
  private address: string;
  // Strict ownership via encapsulation
  private rooms: Room[] = [];

  constructor(address: string) {
    this.address = address;
    console.log(`🏠 House created at: ${this.address}`);
  }

  // The House controls the creation lifecycle of its parts
  addRoom(name: string): void {
    const newRoom = new Room(name);
    this.rooms.push(newRoom);
  }

  // The House controls the destruction lifecycle of its parts
  // (In TS this is mostly semantic/logic since GC handles memory)
  destroyHouse(): void {
    console.log(`🏚️ House destroyed at: ${this.address}`);
    for (const room of this.rooms) {
      room.destroy();
    }
    this.rooms = []; // Eliminate references
  }

  showDetails(): void {
    console.log(`House at ${this.address} has ${this.rooms.length} rooms:`);
    for (const room of this.rooms) {
      console.log(`  - ${room.getName()}`);
    }
  }
}

//
// =======================================================
// 3. COMPOSITION IN CONSTRUCTORS: CAR AND ENGINE
// =======================================================
//
// In composition, parts are often instantiated directly inside the whole's
// constructor. The whole absolutely guards the child object.

class Engine {
  private type: string;
  private horsepower: number;

  constructor(type: string, hp: number) {
    this.type = type;
    this.horsepower = hp;
    console.log(`    [+] Engine built: ${type} (${hp} HP)`);
  }

  start(): void {
    console.log(`    Vroom! ${this.type} engine roaring to life.`);
  }

  destroy(): void {
    console.log(`    [-] Engine scrapped: ${this.type}`);
  }
}

class Car {
  private model: string;
  // Composition: Strict singular ownership
  private engine: Engine;

  constructor(model: string, engineType: string, hp: number) {
    this.model = model;
    console.log(`🚗 Building Car: ${this.model}`);

    // The car is solely responsible for creating its engine component internally
    this.engine = new Engine(engineType, hp);
  }

  drive(): void {
    console.log(`${this.model} is starting...`);
    this.engine.start();
    console.log(`${this.model} is now moving!`);
  }

  scrapCar(): void {
    console.log(`💥 Scraping Car: ${this.model}`);
    this.engine.destroy();
    // In TS, abandoning references ensures Garbage Collection cleans them both up.
  }
}

//
// =======================================================
// 4. REAL-WORLD EXAMPLE: DOCUMENT AND PAGES
// =======================================================
//
// A Document is composed of multiple Pages.
// If you delete the document, the pages are effectively gone.

class Page {
  private pageNumber: number;
  private content: string;

  constructor(pageNumber: number, content: string) {
    this.pageNumber = pageNumber;
    this.content = content;
    console.log(`    [+] Page ${this.pageNumber} created.`);
  }

  destroy(): void {
    console.log(`    [-] Page ${this.pageNumber} destroyed.`);
  }

  render(): void {
    console.log(`--- Page ${this.pageNumber} ---`);
    console.log(this.content);
    console.log(`--------------`);
  }
}

class Document {
  private title: string;
  // Strict ownership via private array
  private pages: Page[] = [];

  constructor(title: string) {
    this.title = title;
    console.log(`📄 Document '${this.title}' created.`);
  }

  // Document acts as a factory constructing its own pages
  appendPage(content: string): void {
    const newPageNum = this.pages.length + 1;
    this.pages.push(new Page(newPageNum, content));
  }

  print(): void {
    console.log(`\nPrinting Document: ${this.title}`);
    for (const page of this.pages) {
      page.render();
    }
  }

  deleteDocument(): void {
    console.log(`🗑️ Document '${this.title}' destroyed.`);
    for (const page of this.pages) {
      page.destroy();
    }
    this.pages = []; // sever the connection for GC
  }
}

//
// =======================================================
// 5. ANOTHER REAL-WORLD EXAMPLE: ORDER AND LINE ITEMS
// =======================================================
//
// An E-commerce Order is composed of LineItems.
// If the Order is cancelled/destroyed, the line items disappear with it.

class LineItem {
  private productName: string;
  private quantity: number;
  private unitPrice: number;

  constructor(productName: string, quantity: number, unitPrice: number) {
    this.productName = productName;
    this.quantity = quantity;
    this.unitPrice = unitPrice;
  }

  getSubtotal(): number {
    return this.quantity * this.unitPrice;
  }

  getProductName(): string {
    return this.productName;
  }

  describe(): void {
    console.log(
      `    ${this.productName} x${this.quantity} @ $${this.unitPrice} = $${this.getSubtotal()}`,
    );
  }
}

class Order {
  private orderId: string;
  private lineItems: LineItem[] = [];

  constructor(orderId: string) {
    this.orderId = orderId;
    console.log(`📦 Order '${this.orderId}' created.`);
  }

  addItem(product: string, quantity: number, unitPrice: number): void {
    const newItem = new LineItem(product, quantity, unitPrice);
    this.lineItems.push(newItem);
    console.log(`    [+] Added item: ${product}`);
  }

  removeItem(product: string): void {
    this.lineItems = this.lineItems.filter(
      (item) => item.getProductName() !== product,
    );
  }

  getTotal(): number {
    return this.lineItems.reduce(
      (total, item) => total + item.getSubtotal(),
      0,
    );
  }

  printReceipt(): void {
    console.log(`\nReceipt for Order: ${this.orderId}`);
    for (const item of this.lineItems) {
      item.describe();
    }
    console.log(`Total: $${this.getTotal()}`);
  }

  cancelOrder(): void {
    console.log(`🗑️ Order '${this.orderId}' destroyed.`);
    this.lineItems = []; // sever the connection for GC
  }
}

//
// =======================================================
// 6. DEMONSTRATION
// =======================================================
//

function main(): void {
  console.log("=== Composition in TypeScript Demo ===\n");

  // ---- 1. Basic Composition: House and Rooms ----
  console.log("--- 1. Basic Composition: House & Rooms ---");
  let myHouse: House | null = new House("123 Main St");

  // We do NOT pass in pre-made rooms objects. We pass data for the house to make them.
  myHouse.addRoom("Living Room");
  myHouse.addRoom("Kitchen");
  myHouse.addRoom("Master Bedroom");

  myHouse.showDetails();

  // Explicitly simulating destruction
  console.log("Destroying house scope...");
  myHouse.destroyHouse();
  myHouse = null; // GC takes over both house and rooms simultaneously
  console.log("");

  // ---- 2. Constructor instantiation: Car and Engine ----
  console.log("--- 2. Direct Initialization: Car & Engine ---");
  let myCar: Car | null = new Car("Mustang GT", "V8", 450);

  myCar.drive();

  console.log("Destroying car scope...");
  myCar.scrapCar();
  myCar = null;
  console.log("");

  // ---- 3. Real-World: Document and Pages ----
  console.log("--- 3. Real-World: Document & Pages ---");
  let doc: Document | null = new Document("OOP Guide");

  doc.appendPage("Chapter 1: Principles of OOP...");
  doc.appendPage("Chapter 2: Inheritance vs Composition...");

  doc.print();

  console.log("Destroying document scope...");
  doc.deleteDocument();
  doc = null;
  console.log("");

  // ---- 4. Another Real-World: Order and Line Items ----
  console.log("--- 4. Real-World: Order & Line Items ---");
  let order: Order | null = new Order("ORD-1001");

  order.addItem("Wireless Mouse", 2, 29.99);
  order.addItem("USB-C Cable", 3, 9.99);
  order.addItem("Laptop Stand", 1, 49.99);

  order.printReceipt();

  console.log("Destroying order scope...");
  order.cancelOrder();
  order = null; // GC takes over the items
  console.log("");

  console.log("=== Demo Complete ===");
}

main();

export {};

/*
📘 Key Insights:

1. **Strict Ownership**:
   - The "whole" class completely owns the "part".
   - The part is hidden from the outside world.
   - You cannot safely extract the part and give it to another whole.

2. **Dependent Lifecycles (Lifecycle Control)**:
   - The Whole controls creation and destruction of its Parts.
   - If the Whole dies, the Part dies. It cannot survive its parent.
   - In TypeScript, since GC handles memory, composition is predominantly a 
     logic/semantics concept. The container ensures references to children are dropped.

3. **Implementation Strategy in TypeScript**:
   - Do not use Constructor Dependency Injection `constructor(engine: Engine)`. 
     That is Aggregation.
   - Instantiate children tightly inside constructors or factory methods:
     `this.engine = new Engine(...)`

4. **Encapsulation**:
   - Children parts should be `private`.
   - Never expose setters or getters that let the outside world replace or 
     hijack the parts.
*/
