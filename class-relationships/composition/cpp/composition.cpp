#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Ref - https://algomaster.io/learn/lld/composition
// Ref -
// https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/

//
// =======================================================
// 1. WHAT IS COMPOSITION?
// =======================================================
//
// Composition is a specialized form of Association that models a "whole-part"
// relationship with "strict ownership" and "full lifecycle control".
//
// Key Formula:
// Composition = "part-of" relationship + strict ownership + dependent
// lifecycles
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
// - Dependent lifecycles (parts are created by the whole and die with the
// whole)
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
// The rooms are created when the house is created and destroyed
// when the house is destroyed.

class Room {
private:
  std::string name;

public:
  Room(const std::string &n) : name(n) {
    std::cout << "    [+] Room created: " << name << "\n";
  }

  ~Room() { std::cout << "    [-] Room destroyed: " << name << "\n"; }

  std::string getName() const { return name; }
};

class House {
private:
  std::string address;
  // Composition using direct objects in a vector
  // Memory is managed automatically by the vector and House lifecycle
  std::vector<Room> rooms;

public:
  House(const std::string &addr) : address(addr) {
    std::cout << "🏠 House created at: " << address << "\n";
  }

  ~House() {
    std::cout << "🏚️ House destroyed at: " << address << "\n";
    // Rooms are automatically destroyed here
  }

  void addRoom(const std::string &roomName) {
    // House controls the creation of the room
    rooms.emplace_back(roomName);
  }

  void showDetails() const {
    std::cout << "House at " << address << " has " << rooms.size()
              << " rooms:\n";
    for (const auto &room : rooms) {
      std::cout << "  - " << room.getName() << "\n";
    }
  }
};

//
// =======================================================
// 3. COMPOSITION VIA SMART POINTERS: CAR AND ENGINE
// =======================================================
//
// Sometime objects are large or polymorphic, so we need pointers.
// In C++, strictly owned parts are best represented with std::unique_ptr.

class Engine {
private:
  std::string type;
  int horsepower;

public:
  Engine(const std::string &t, int hp) : type(t), horsepower(hp) {
    std::cout << "    [+] Engine built: " << type << " (" << horsepower
              << " HP)\n";
  }

  ~Engine() { std::cout << "    [-] Engine scrapped: " << type << "\n"; }

  void start() const {
    std::cout << "    Vroom! " << type << " engine roaring to life.\n";
  }
};

class Car {
private:
  std::string model;
  // Composition using std::unique_ptr provides explicit exclusive ownership
  // The Engine's lifecycle is bound to the Car's lifecycle.
  std::unique_ptr<Engine> engine;

public:
  Car(const std::string &m, const std::string &engType, int hp) : model(m) {
    std::cout << "🚗 Building Car: " << model << "\n";
    // Car controls creation of the engine
    engine = std::make_unique<Engine>(engType, hp);
  }

  ~Car() {
    std::cout << "💥 Scraping Car: " << model << "\n";
    // unique_ptr automatically deletes the engine
  }

  void drive() const {
    std::cout << model << " is starting...\n";
    engine->start();
    std::cout << model << " is now moving!\n";
  }
};

//
// =======================================================
// 4. REAL-WORLD EXAMPLE: DOCUMENT AND PAGES
// =======================================================
//
// A Document is composed of multiple Pages.
// If you delete the document, the pages are gone.

class Page {
private:
  int pageNumber;
  std::string content;

public:
  Page(int num, const std::string &text) : pageNumber(num), content(text) {
    std::cout << "    [+] Page " << pageNumber << " created.\n";
  }

  ~Page() { std::cout << "    [-] Page " << pageNumber << " destroyed.\n"; }

  void render() const {
    std::cout << "--- Page " << pageNumber << " ---\n";
    std::cout << content << "\n";
    std::cout << "--------------\n";
  }
};

class Document {
private:
  std::string title;
  // Composition using direct objects
  std::vector<Page> pages;

public:
  Document(const std::string &t) : title(t) {
    std::cout << "📄 Document '" << title << "' created.\n";
  }

  ~Document() { std::cout << "🗑️ Document '" << title << "' destroyed.\n"; }

  void appendPage(const std::string &text) {
    // Document instantiates the part
    pages.emplace_back(pages.size() + 1, text);
  }

  void print() const {
    std::cout << "\nPrinting Document: " << title << "\n";
    for (const auto &page : pages) {
      page.render();
    }
  }
};

//
// =======================================================
// 5. ANOTHER REAL-WORLD EXAMPLE: ORDER AND LINE ITEMS
// =======================================================
//
// An E-commerce Order is composed of LineItems.
// If the Order is cancelled/destroyed, the line items disappear with it.

class LineItem {
private:
  std::string productName;
  int quantity;
  double unitPrice;

public:
  LineItem(const std::string &productName, int quantity, double unitPrice)
      : productName(productName), quantity(quantity), unitPrice(unitPrice) {}

  double getSubtotal() const { return quantity * unitPrice; }

  std::string getProductName() const { return productName; }

  void describe() const {
    std::cout << "    " << productName << " x" << quantity << " @ $"
              << unitPrice << " = $" << getSubtotal() << "\n";
  }
};

class Order {
private:
  std::string orderId;
  std::vector<LineItem> lineItems;

public:
  Order(const std::string &orderId) : orderId(orderId) {
    std::cout << "📦 Order '" << orderId << "' created.\n";
  }

  ~Order() { std::cout << "🗑️ Order '" << orderId << "' destroyed.\n"; }

  void addItem(const std::string &product, int quantity, double unitPrice) {
    lineItems.emplace_back(product, quantity, unitPrice);
    std::cout << "    [+] Added item: " << product << "\n";
  }

  void removeItem(const std::string &product) {
    lineItems.erase(std::remove_if(lineItems.begin(), lineItems.end(),
                                   [&](const LineItem &item) {
                                     return item.getProductName() == product;
                                   }),
                    lineItems.end());
  }

  double getTotal() const {
    double total = 0;
    for (const auto &item : lineItems) {
      total += item.getSubtotal();
    }
    return total;
  }

  void printReceipt() const {
    std::cout << "\nReceipt for Order: " << orderId << "\n";
    for (const auto &item : lineItems) {
      item.describe();
    }
    std::cout << "Total: $" << getTotal() << "\n";
  }
};

//
// =======================================================
// 6. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Composition in C++ Demo ===\n\n";

  // ---- 1. Basic Composition: House and Rooms ----
  std::cout << "--- 1. Basic Composition: House & Rooms ---\n";
  {
    // Scope block to demonstrate destruction
    House myHouse("123 Main St");
    // We do NOT pass in pre-made rooms. The house creates them.
    myHouse.addRoom("Living Room");
    myHouse.addRoom("Kitchen");
    myHouse.addRoom("Master Bedroom");

    myHouse.showDetails();
    std::cout << "Exiting scope... House will be destroyed.\n";
  } // myHouse goes out of scope, destroying the house and all its rooms
  std::cout << "\n";

  // ---- 2. Unique Pointers: Car and Engine ----
  std::cout << "--- 2. Smart Pointers: Car & Engine ---\n";
  {
    Car myCar("Mustang GT", "V8", 450);
    myCar.drive();
    std::cout << "Exiting scope... Car will be destroyed.\n";
  } // myCar and its engine are destroyed automatically and safely
  std::cout << "\n";

  // ---- 3. Real-World: Document and Pages ----
  std::cout << "--- 3. Real-World: Document & Pages ---\n";
  {
    Document doc("OOP Guide");
    doc.appendPage("Chapter 1: Principles of OOP...");
    doc.appendPage("Chapter 2: Inheritance vs Composition...");

    doc.print();
    std::cout << "Exiting scope... Document will be destroyed.\n";
  } // Document and its pages perish
  std::cout << "\n";

  // ---- 4. Another Real-World: Order and Line Items ----
  std::cout << "--- 4. Real-World: Order & Line Items ---\n";
  {
    Order order("ORD-1001");
    order.addItem("Wireless Mouse", 2, 29.99);
    order.addItem("USB-C Cable", 3, 9.99);
    order.addItem("Laptop Stand", 1, 49.99);

    order.printReceipt();
    std::cout << "Exiting scope... Order will be destroyed.\n";
  } // When order goes out of scope, all LineItems are destroyed
  std::cout << "\n";

  std::cout << "=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Strict Ownership**:
   - The "whole" class completely owns the "part".
   - The part is hidden from the outside world.
   - Example: Car owns its Engine strictly.

2. **Dependent Lifecycles (Lifecycle Control)**:
   - The Whole controls creation and destruction of its Parts.
   - If the Whole dies, the Part dies. It cannot survive its parent.
   - There's a "coincidence of lifetimes."

3. **Implementation Strategy in C++**:
   - Best implemented via **Direct membership** (values): `std::vector<Room>` or
`Engine engine;`
   - If pointers are needed (for polymorphism, incomplete types, or size),
strictly use **`std::unique_ptr<T>`**.
   - Raw pointers with `new` and `delete` in the destructor are okay, but
`unique_ptr` prevents memory leaks perfectly.

4. **Composition over Inheritance**:
   - A widely used design principle. Rather than inheriting behavior (`is-a`),
you include an object that gives you the behavior (`has-a`).
   - e.g., instead of `WoodenDoor` inheriting `Door`, `Door` is composed of a
`Material` object.

5. **Aggregation vs Composition**:
   - Aggregation: Parts are created externally and injected ->
`Department(Professor* p)`. Parts survive the whole.
   - Composition: Parts are created internally -> `House() {
rooms.push_back(Room(...)) }`. Parts die with the whole.
*/