/*

# The Problem:
Every time you modify an existing class to add new functionality, you expose
yourself to several dangers.

# Solution:
# OCP - Software entities (classes, modules, functions, etc.) should be open for
extension, but closed for modification

Let's break that down:

# Open for Extension: The behavior of the entity can be extended. As new
requirements come in (like new payment types), you should be able to add new
behavior without touching existing code.

# Closed for Modification: The existing, working code should not be changed.
Once it is written, tested, and working, you should not need to go back and
alter it to add new features.

Sounds like a paradox, right? How can you add new features without changing
existing code? The answer lies in abstraction.

By programming against interfaces rather than concrete implementations, you can
introduce new behavior simply by creating new classes that implement the
existing interface.
*/

#include <iomanip>
#include <ios>
#include <iostream>
using namespace std;

/*
==================================================
Exercise 1: The Problem: A Growing Payment System
==================================================

Problem: If you want to add another payment options like upi, strip etc
Each time, you need to open the PaymentProcessor class. Each time, you are
adding another else if branch to CheckoutService. And each modification carries
real risk.

Requirements:
Apply OCP.
Create a paymentMethod interface so that each concrete payment type
implements that interface.
The PaymentProcessor depends on the PaymentMethod interface.  It has no idea
whether it is processing a credit card, PayPal, UPI, or Bitcoin transaction. All
the concrete implementations can be swapped in freely.

*/

// Before applying OCP
// ==========================================
// class PaymentProcessor {
// public:
//   void processCreditCardPayment(double amount) {
//     cout << "Processing credit card payment of $" << amount << endl;
//     // Complex logic for credit card processing
//   }

//   void processPayPalPayment(double amount) {
//     cout << "Processing PayPal payment of $" << amount << endl;
//     // Logic for PayPal processing
//   }
// };

// class CheckoutService {
// public:
//   void processPayment(const string &paymentType) {
//     PaymentProcessor processor;

//     if (paymentType == "CreditCard") {
//       processor.processCreditCardPayment(100.00);
//     } else if (paymentType == "PayPal") {
//       processor.processPayPalPayment(100.00);
//     }
//   }
// };
// ============================================

// After applying OCP
// =============================================================
// The CheckoutService simply passes the payment method to the processor. It
// does not need to know which payment type it is handling, it just delegates.
class PaymentMethod {
public:
  virtual void processPayment(const double amount) = 0;
  virtual ~PaymentMethod() = default;
};

class CreditCardPayment : public PaymentMethod {
public:
  void processPayment(double amount) override {
    cout << fixed << setprecision(2);
    cout << "Processing credit card payment of $" << amount << endl;
    // Complex logic for credit card processing
  }
};

class PayPalPayment : public PaymentMethod {
public:
  void processPayment(double amount) override {
    cout << fixed << setprecision(2);
    cout << "Processing PayPal payment of $" << amount << endl;
    // Logic for PayPal processing
  }
};

class UPIPayment : public PaymentMethod {
public:
  void processPayment(double amount) override {
    cout << fixed << setprecision(2);
    cout << "Processing UPI payment of ₹" << amount * 80 << endl;
    // Logic for UPI processing
  }
};

// PaymentProcessor now depends on the PaymentMethod interface, not concrete
// implementations. It no longer needs to know the specifics of each payment
// type. There are no if-else branches, no switch statements, and no reason to
// change when new payment methods arrive.
class PaymentProcessor {
public:
  void process(PaymentMethod *paymentMethod, double amount) {
    // No more if-else! The processor doesn't care about the specific type.
    // It just knows it can call processPayment.
    paymentMethod->processPayment(amount);
  }
};

// The CheckoutService simply passes the payment method to the processor. It
// does not need to know which payment type it is handling, it just delegates.
class CheckoutService {
public:
  void processPayment(PaymentMethod *method, double amount) {
    PaymentProcessor processor;
    processor.process(method, amount);
  }
};

int main() {
  // Usage
  CheckoutService checkout;
  CreditCardPayment credit;
  PayPalPayment paypal;
  UPIPayment upi;

  checkout.processPayment(&credit, 100.00);
  checkout.processPayment(&paypal, 100.00);
  checkout.processPayment(&upi, 100.00);
}