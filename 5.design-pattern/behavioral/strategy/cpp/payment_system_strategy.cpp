/*
=========================================================================
  Strategy Pattern — Practical Example: Payment Processing System
=========================================================================

Scenario:
You are building the checkout process for an e-commerce platform. Users 
must be able to pay using multiple methods: Credit Card, PayPal, and 
Cryptocurrency. 

The Problem:
Each payment method has an entirely different processing flow. The 
`CheckoutService` shouldn't care about the intricacies of blockchain 
transactions or credit card APIs. If it did, it would be massive, fragile, 
and constantly changing. 
Furthermore, in a real application, users select their payment method via 
a string or enum from the UI (e.g., "PAYPAL"). We need a clean way to 
instantiate the correct strategy based on that user input without polluting 
the core logic.

Solution:
Use the Strategy Pattern combined with the Factory Pattern.
1. The Strategy Pattern extracts the specific payment logic into separate 
   classes (`CreditCardPayment`, `PayPalPayment`).
2. The Factory Pattern looks at the user's input string ("CRYPTO") and 
   automatically instantiates the correct Strategy.
The `CheckoutService` (Context) simply receives the created strategy and 
executes it.

=========================================================================
  Component Mapping
=========================================================================
  Strategy          → PaymentStrategy (Interface for all payment methods)
  ConcreteStrategy  → CreditCardPayment, PayPalPayment, CryptoPayment
  Context           → CheckoutService (Executes the strategy)
  Factory           → PaymentStrategyFactory (Creates strategies from strings)
=========================================================================
*/

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>

using namespace std;

/*
=========================================================================
  STEP 1: The Strategy Interface
=========================================================================
*/
class PaymentStrategy {
public:
  virtual ~PaymentStrategy() = default;
  virtual bool pay(double amount) = 0;
};

/*
=========================================================================
  STEP 2: Concrete Strategies
=========================================================================
*/
class CreditCardPayment : public PaymentStrategy {
private:
  string cardNumber;
  string expiryDate;

public:
  CreditCardPayment(const string &card, const string &expiry)
      : cardNumber(card), expiryDate(expiry) {}

  bool pay(double amount) override {
    cout << "[Strategy: Credit Card] Charging $" << amount 
         << " to card ending in " << cardNumber.substr(cardNumber.length() - 4) << endl;
    return true;
  }
};

class PayPalPayment : public PaymentStrategy {
private:
  string email;

public:
  PayPalPayment(const string &e) : email(e) {}

  bool pay(double amount) override {
    cout << "[Strategy: PayPal] Sending $" << amount 
         << " via PayPal to " << email << endl;
    return true;
  }
};

class CryptoPayment : public PaymentStrategy {
private:
  string walletAddress;

public:
  CryptoPayment(const string &wallet) : walletAddress(wallet) {}

  bool pay(double amount) override {
    cout << "[Strategy: Crypto] Transferring $" << amount 
         << " in crypto to " << walletAddress << endl;
    return true;
  }
};

/*
=========================================================================
  STEP 3: The Context
=========================================================================
*/
class CheckoutService {
private:
  PaymentStrategy *paymentStrategy;

public:
  CheckoutService() : paymentStrategy(nullptr) {}
  CheckoutService(PaymentStrategy *strategy) : paymentStrategy(strategy) {}

  void setPaymentStrategy(PaymentStrategy *strategy) {
    paymentStrategy = strategy;
  }

  bool checkout(double amount) {
    if (!paymentStrategy) {
        throw logic_error("Payment strategy not set!");
    }
    return paymentStrategy->pay(amount); 
  }
};

/*
=========================================================================
  STEP 4: Real-World Addition (Strategy + Factory)
=========================================================================
In the real world, the UI sends a string like "PAYPAL" or "CREDIT_CARD".
We use a Simple Factory to dynamically create the correct Strategy.
*/
class PaymentStrategyFactory {
public:
    static unique_ptr<PaymentStrategy> createStrategy(const string& type) {
        if (type == "CREDIT_CARD") {
            // In reality, these details would come from a secure form payload
            return make_unique<CreditCardPayment>("4111111111111111", "12/26");
        } 
        else if (type == "PAYPAL") {
            return make_unique<PayPalPayment>("user@example.com");
        } 
        else if (type == "CRYPTO") {
            return make_unique<CryptoPayment>("0xABC123987XYZ");
        }
        else {
            throw invalid_argument("Unknown payment method");
        }
    }
};

/*
=========================================================================
  STEP 5: Client Code
=========================================================================
*/
void manualConfigurationV1() {
  // Manual instantiation (Standard Strategy Pattern)
  CreditCardPayment card("4111111111111111", "12/26");
  CheckoutService checkout(&card);
  checkout.checkout(99.99);

  PayPalPayment paypal("user@example.com");
  checkout.setPaymentStrategy(&paypal);
  checkout.checkout(49.99);
}

void factoryIntegrationV2() {
  // Real-world dynamic instantiation (Strategy + Factory)
  CheckoutService checkout;
  
  // Simulated UI inputs
  string userInput1 = "PAYPAL";
  string userInput2 = "CRYPTO";

  cout << "User selected: " << userInput1 << endl;
  auto strategy1 = PaymentStrategyFactory::createStrategy(userInput1);
  checkout.setPaymentStrategy(strategy1.get());
  checkout.checkout(150.00);
  
  cout << "\nUser selected: " << userInput2 << endl;
  auto strategy2 = PaymentStrategyFactory::createStrategy(userInput2);
  checkout.setPaymentStrategy(strategy2.get());
  checkout.checkout(300.00);
}

int main() {
  cout << "========================================" << endl;
  cout << "  V1: MANUAL STRATEGY CONFIGURATION     " << endl;
  cout << "========================================" << endl;
  manualConfigurationV1();
  cout << endl;

  cout << "========================================" << endl;
  cout << "  V2: REAL WORLD (STRATEGY + FACTORY)   " << endl;
  cout << "========================================" << endl;
  factoryIntegrationV2();

  return 0;
}