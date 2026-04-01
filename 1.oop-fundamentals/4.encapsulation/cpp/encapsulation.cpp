#include <iostream>
#include <string>
#include <vector>

// Ref - https://www.geeksforgeeks.org/encapsulation-in-c/
// Ref - https://en.cppreference.com/w/cpp/language/access

//
// =======================================================
// 1. WHAT IS ENCAPSULATION?
// =======================================================
//
// Encapsulation is one of the four foundational principles of OOP.
// It is the practice of bundling data (attributes) and methods that
// operate on that data into a single unit (class), while restricting
// direct access to internal details.
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
// Implementation in C++:
// 1. Access modifiers (private, protected, public)
// 2. Getters and setters for controlled access
// 3. Private helper methods for internal logic

//
// =======================================================
// 2. BASIC ENCAPSULATION: BANK ACCOUNT
// =======================================================
//
// Without encapsulation, anyone could modify the balance directly:
//   account.balance = -1000000;  // ❌ Dangerous!
//
// With encapsulation, we control access and enforce rules

class BankAccount {
private:
  // Private data - hidden from outside
  double balance;
  std::string accountNumber;

public:
  // Constructor
  BankAccount(const std::string &accNum) : balance(0.0), accountNumber(accNum) {
    std::cout << "💳 Account " << accountNumber << " created\n";
  }

  // Public interface - controlled access
  void deposit(double amount) {
    if (amount <= 0.0) {
      std::cout << "❌ Deposit amount must be positive\n";
      return;
    }
    balance += amount;
    std::cout << "✅ Deposited $" << amount << "\n";
  }

  void withdraw(double amount) {
    if (amount <= 0.0) {
      std::cout << "❌ Withdrawal amount must be positive\n";
      return;
    }
    if (amount > balance) {
      std::cout << "❌ Insufficient funds!\n";
      return;
    }
    balance -= amount;
    std::cout << "✅ Withdrawn $" << amount << "\n";
  }

  // Getter - read-only access
  double getBalance() const { return balance; }

  std::string getAccountNumber() const { return accountNumber; }

  void displayAccount() const {
    std::cout << "Account " << accountNumber << " | Balance: $" << balance
              << "\n";
  }
};

//
// =======================================================
// 3. INTERMEDIATE: PRIVATE HELPER METHODS
// =======================================================
//
// Encapsulation also means hiding implementation details through
// private helper methods

class PaymentProcessor {
private:
  std::string cardNumber;
  double amount;

  // Private helper method - implementation detail hidden
  std::string maskCardNumber(const std::string &raw) {
    if (raw.length() < 4) {
      return "****";
    }
    return "****-****-****-" + raw.substr(raw.length() - 4);
  }

  bool validateAmount(double amt) const {
    return amt > 0 && amt <= 10000; // Max transaction limit
  }

public:
  PaymentProcessor(const std::string &card, double amt) : amount(amt) {
    cardNumber = maskCardNumber(card); // Mask on construction
    std::cout << "💳 Payment processor initialized\n";
  }

  void processPayment() {
    if (!validateAmount(amount)) {
      std::cout << "❌ Invalid payment amount\n";
      return;
    }
    std::cout << "✅ Processing payment of $" << amount << " for card "
              << cardNumber << "\n";
  }

  // Only expose what's necessary
  double getAmount() const { return amount; }

  // Never expose raw card number!
  std::string getMaskedCard() const { return cardNumber; }
};

//
// =======================================================
// 4. ADVANCED: ENCAPSULATION WITH SETTERS
// =======================================================
//
// Setters allow controlled modification with validation

class Employee {
private:
  std::string name;
  int age;
  double salary;
  std::string department;

public:
  Employee(const std::string &n, int a, double s, const std::string &dept)
      : name(n), age(a), salary(s), department(dept) {}

  // Getters - read access
  std::string getName() const { return name; }
  int getAge() const { return age; }
  double getSalary() const { return salary; }
  std::string getDepartment() const { return department; }

  // Setters with validation
  void setAge(int newAge) {
    if (newAge < 18 || newAge > 100) {
      std::cout << "❌ Invalid age\n";
      return;
    }
    age = newAge;
    std::cout << "✅ Age updated to " << age << "\n";
  }

  void setSalary(double newSalary) {
    if (newSalary < 0) {
      std::cout << "❌ Salary cannot be negative\n";
      return;
    }
    if (newSalary < salary) {
      std::cout << "⚠️  Warning: Salary decreased\n";
    }
    salary = newSalary;
    std::cout << "✅ Salary updated to $" << salary << "\n";
  }

  void setDepartment(const std::string &newDept) {
    department = newDept;
    std::cout << "✅ Department updated to " << department << "\n";
  }

  void displayInfo() const {
    std::cout << "Employee: " << name << " | Age: " << age << " | Salary: $"
              << salary << " | Dept: " << department << "\n";
  }
};

//
// =======================================================
// 5. ENCAPSULATION LEVELS
// =======================================================
//
// C++ offers three access levels:
// - private: Only accessible within the class
// - protected: Accessible within class and derived classes
// - public: Accessible everywhere

class Shape {
protected:
  // Protected - accessible in derived classes
  double area;
  std::string name;

private:
  // Private - only accessible in this class
  int shapeId;
  static int nextId;

  void assignId() { shapeId = ++nextId; }

public:
  Shape(const std::string &shapeName) : name(shapeName), area(0.0) {
    assignId();
    std::cout << "🔷 Shape '" << name << "' created with ID: " << shapeId
              << "\n";
  }

  virtual ~Shape() = default;

  // Public interface
  virtual void calculateArea() = 0; // Pure virtual

  double getArea() const { return area; }

  int getId() const { return shapeId; }

  void displayInfo() const {
    std::cout << "Shape: " << name << " | ID: " << shapeId
              << " | Area: " << area << "\n";
  }
};

int Shape::nextId = 0;

class Rectangle : public Shape {
private:
  double width;
  double height;

public:
  Rectangle(double w, double h) : Shape("Rectangle"), width(w), height(h) {
    calculateArea();
  }

  void calculateArea() override {
    // Can access protected 'area' from base class
    area = width * height;
  }

  // Setters with validation
  void setDimensions(double w, double h) {
    if (w <= 0 || h <= 0) {
      std::cout << "❌ Dimensions must be positive\n";
      return;
    }
    width = w;
    height = h;
    calculateArea();
    std::cout << "✅ Dimensions updated\n";
  }
};

//
// =======================================================
// 6. REAL-WORLD EXAMPLE: USER AUTHENTICATION
// =======================================================
//

class User {
private:
  std::string username;
  std::string passwordHash; // Never store password in plain text!
  bool isActive;
  int loginAttempts;

  // Private helper: hash password (simplified)
  std::string hashPassword(const std::string &password) const {
    // In real world, use proper hashing (bcrypt, argon2)
    return "hash_" + password; // Simplified for demo
  }

  // Private helper: validate password strength
  bool isStrongPassword(const std::string &password) const {
    return password.length() >= 8; // Simplified
  }

public:
  User(const std::string &user, const std::string &pass)
      : username(user), isActive(true), loginAttempts(0) {
    if (!isStrongPassword(pass)) {
      std::cout << "⚠️  Weak password detected\n";
    }
    passwordHash = hashPassword(pass);
    std::cout << "👤 User '" << username << "' created\n";
  }

  // Public login method
  bool login(const std::string &password) {
    if (!isActive) {
      std::cout << "❌ Account is locked\n";
      return false;
    }

    if (hashPassword(password) == passwordHash) {
      std::cout << "✅ Login successful!\n";
      loginAttempts = 0;
      return true;
    } else {
      loginAttempts++;
      std::cout << "❌ Invalid password (Attempt " << loginAttempts << ")\n";

      if (loginAttempts >= 3) {
        isActive = false;
        std::cout << "🔒 Account locked due to multiple failed attempts\n";
      }
      return false;
    }
  }

  void resetPassword(const std::string &oldPass, const std::string &newPass) {
    if (hashPassword(oldPass) != passwordHash) {
      std::cout << "❌ Old password incorrect\n";
      return;
    }
    if (!isStrongPassword(newPass)) {
      std::cout << "❌ New password is too weak\n";
      return;
    }
    passwordHash = hashPassword(newPass);
    std::cout << "✅ Password updated successfully\n";
  }

  std::string getUsername() const { return username; }

  bool getActiveStatus() const { return isActive; }

  // Never expose password or hash!
};

//
// =======================================================
// 7. ENCAPSULATION BENEFITS DEMONSTRATION
// =======================================================
//

class ShoppingCart {
private:
  std::vector<std::string> items;
  std::vector<double> prices;
  double discount;

  // Private calculation
  double calculateSubtotal() const {
    double total = 0;
    for (double price : prices) {
      total += price;
    }
    return total;
  }

public:
  ShoppingCart() : discount(0.0) {}

  void addItem(const std::string &item, double price) {
    if (price <= 0) {
      std::cout << "❌ Invalid price\n";
      return;
    }
    items.push_back(item);
    prices.push_back(price);
    std::cout << "✅ Added '" << item << "' ($" << price << ")\n";
  }

  void removeItem(const std::string &item) {
    for (size_t i = 0; i < items.size(); i++) {
      if (items[i] == item) {
        std::cout << "✅ Removed '" << item << "'\n";
        items.erase(items.begin() + i);
        prices.erase(prices.begin() + i);
        return;
      }
    }
    std::cout << "❌ Item not found\n";
  }

  void applyDiscount(double percent) {
    if (percent < 0 || percent > 100) {
      std::cout << "❌ Invalid discount percentage\n";
      return;
    }
    discount = percent;
    std::cout << "✅ Applied " << discount << "% discount\n";
  }

  double getTotal() const {
    double subtotal = calculateSubtotal();
    return subtotal * (1.0 - discount / 100.0);
  }

  void displayCart() const {
    std::cout << "\n🛒 Shopping Cart:\n";
    for (size_t i = 0; i < items.size(); i++) {
      std::cout << "  - " << items[i] << ": $" << prices[i] << "\n";
    }
    std::cout << "Subtotal: $" << calculateSubtotal() << "\n";
    std::cout << "Discount: " << discount << "%\n";
    std::cout << "Total: $" << getTotal() << "\n";
  }
};

//
// =======================================================
// 8. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Encapsulation in C++ Demo ===\n\n";

  // ---- Basic Encapsulation ----
  std::cout << "1. Basic Encapsulation (BankAccount):\n";
  BankAccount account("ACC-12345");
  account.deposit(1000);
  account.withdraw(250);
  account.displayAccount();
  // account.balance = 999999; // ❌ Error: private member

  std::cout << "\n2. Private Helper Methods (PaymentProcessor):\n";
  PaymentProcessor payment("1234567812345678", 500);
  payment.processPayment();

  std::cout << "\n3. Setters with Validation (Employee):\n";
  Employee emp("Alice", 30, 75000, "Engineering");
  emp.displayInfo();
  emp.setSalary(80000);
  emp.setAge(31);
  emp.setAge(200); // Invalid

  std::cout << "\n4. Access Levels (Shape & Rectangle):\n";
  Rectangle rect(10, 20);
  rect.displayInfo();
  rect.setDimensions(15, 25);
  rect.displayInfo();

  std::cout << "\n5. Real-World Example (User Authentication):\n";
  User user("john_doe", "SecurePass123");
  user.login("WrongPassword");
  user.login("WrongPassword");
  user.login("SecurePass123");

  std::cout << "\n6. Complex Encapsulation (ShoppingCart):\n";
  ShoppingCart cart;
  cart.addItem("Laptop", 999.99);
  cart.addItem("Mouse", 25.50);
  cart.addItem("Keyboard", 75.00);
  cart.applyDiscount(10);
  cart.displayCart();
  cart.removeItem("Mouse");
  cart.displayCart();

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Data Hiding**: Keep data members private
   - Prevents unauthorized access
   - Protects data integrity

2. **Controlled Access**: Use getters/setters
   - Add validation logic
   - Log access if needed
   - Change internal implementation without affecting users

3. **Private Helpers**: Hide implementation details
   - Internal logic invisible to users
   - Can refactor without breaking code

4. **Access Levels**: Use appropriately
   - private: Default for data members
   - protected: For inheritance hierarchies
   - public: For interface methods only

5. **Benefits**:
   - Security (password hashing example)
   - Validation (age, salary checks)
   - Maintainability (change internals freely)
   - Debugging (control points for logging)

Rule of Thumb:
- Make everything private by default
- Only expose what's necessary through public methods
- Add validation in setters
- Never expose internal data structures directly
*/