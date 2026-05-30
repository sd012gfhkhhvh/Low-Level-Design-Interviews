
/*
 srp: says one class should have one responsibility to change.

 How to know that srp is not followd for a class ?
 -> Always ask a question to yourself that what could be the reasons that the
 class need to be modified. If the ans is multiple distinct reason then it is
 good to split the responsibilities to sepetate dedicated classes.
*/

#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

/*
===================================
 Exercise 1: OrderService God Class
===================================
Refactor OrderService God Class

Problem: You have an OrderService class that handles order processing, inventory
management, and sending notifications. This class has three distinct
responsibilities tangled together. Your task is to refactor it into three
focused classes: OrderProcessor, InventoryManager, and NotificationService.

Requirements:

OrderProcessor should handle order validation and total calculation
InventoryManager should handle stock checks and stock updates
NotificationService should handle sending order confirmation messages
Each class should accept dependencies through its constructor (or method
parameters) rather than doing everything internally
*/

// Before: One class doing three unrelated jobs
class OrderService {
private:
  map<string, int> inventory;
  vector<string> orders;

public:
  OrderService() {
    inventory["LAPTOP"] = 10;
    inventory["PHONE"] = 25;
    inventory["TABLET"] = 15;
  }

  void placeOrder(const string &productId, int quantity,
                  const string &customerEmail) {
    // Responsibility 1: Inventory check
    auto it = inventory.find(productId);
    if (it == inventory.end() || it->second < quantity) {
      cout << "Insufficient stock for " << productId << endl;
      return;
    }

    // Responsibility 2: Order processing
    double pricePerUnit = 100.0;
    double total = pricePerUnit * quantity;
    string orderId = "ORD-" + to_string(orders.size() + 1);
    orders.push_back(orderId);

    // Responsibility 3: Update inventory
    it->second -= quantity;

    // Responsibility 4: Send notification
    cout << "Email to " << customerEmail << ": Order " << orderId
         << " confirmed. Total: $" << fixed << setprecision(1) << total << endl;
  }
};

// After: Refactor into OrderProcessor, InventoryManager, and
// NotificationService.
class InventoryManager {
private:
  map<string, int> inventory;

public:
  InventoryManager() {
    inventory["LAPTOP"] = 10;
    inventory["PHONE"] = 25;
    inventory["TABLET"] = 15;
  }

  bool isProductAvailable(const string &productId, const int quantity) {
    auto it = inventory.find(productId);
    return it != inventory.end() && it->second >= quantity;
  }

  void reduceStock(const string &productId, const int quantity) {
    inventory[productId] -= quantity;
  }
};

class NotificationService {
public:
  NotificationService() {}

  void sendOrderConfirmation(const string &customerEmail, const string &orderId,
                             const double total) {
    cout << fixed << setprecision(1);
    cout << "Email to " << customerEmail << ": Order " << orderId
         << " confirmed. Total: $" << total << endl;
  }
};

class OrderProcessor {
private:
  // dependency injection pattern  - store by reference
  // OrderProcessor does not own an InventoryManager or NotificationService.
  // It merely uses existing ones.
  InventoryManager &inventoryManager;
  NotificationService &notificationService;
  vector<string> orders;

public:
  OrderProcessor(InventoryManager &inv, NotificationService &noti)
      : inventoryManager(inv), notificationService(noti) {}

  void placeOrder(string productId, int quantity, string email) {
    // check availability
    bool available = inventoryManager.isProductAvailable(productId, quantity);

    if (!available) {
      cout << "Insufficient stock for " << productId << endl;
      return;
    }

    // calculate the price & process the order
    double pricePerUnit = 100.0;
    double total = pricePerUnit * quantity;
    string orderId = "ORD-" + to_string(orders.size() + 1);
    orders.push_back(orderId);

    // update the inventory
    inventoryManager.reduceStock(productId, quantity);

    // send email
    notificationService.sendOrderConfirmation(email, orderId, total);
  }
};

int main() {
  // Before:
  //   OrderService order;
  //   order.placeOrder("LAPTOP", 2, "example@gmail.com");
  //   order.placeOrder("LAPTOP", 8, "example2@gmail.com");
  //   order.placeOrder("LAPTOP", 2, "example3@gmail.com");

  // After refactoring, usage should look like:
  InventoryManager inventory;
  NotificationService notifications;
  OrderProcessor processor(inventory, notifications);
  processor.placeOrder("LAPTOP", 2, "alice@example.com");
  processor.placeOrder("LAPTOP", 8, "alice2@example.com");

  OrderProcessor processor2(inventory, notifications);
  processor2.placeOrder("LAPTOP", 3, "alice3@example.com");
  return 0;
}