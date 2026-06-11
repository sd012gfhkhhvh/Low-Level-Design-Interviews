/*
=========================================================================
  Adapter Pattern — Practical Example: Payment Gateway Integration
=========================================================================

Scenario:
You're a developer on an e-commerce team. Your checkout system processes
payments through a clean PaymentProcessor interface. Everything works great
with your in-house payment processor.

Then management drops a new requirement:

  "Integrate with ExternalPayGateway — a battle-tested legacy payment
   provider used by our partner companies."

The catch? ExternalPayGateway has a completely different interface:

  Your interface          │  Legacy interface
  ────────────────────────┼──────────────────────────────
  processPayment()        │  executeTransaction()
  isPaymentSuccessful()   │  checkStatus(refNumber)
  getTransactionId()      │  getReferenceNumber()

Constraints:
  - You CANNOT modify CheckoutService — it's used system-wide.
  - You CANNOT modify ExternalPayGateway — it's from an external vendor SDK.
  - You MUST make them work together.

Solution:
  Build an Adapter — a translator class that implements your PaymentProcessor
  interface but internally delegates to ExternalPayGateway. CheckoutService
  never knows it's talking to a legacy system.

=========================================================================
  Component Mapping
=========================================================================

  Target    → PaymentProcessor (the interface your system expects)
  Adaptee   → ExternalPayGateway (the legacy system you can't modify)
  Adapter   → ExternalPayAdapter (the translator bridge)
  Client    → CheckoutService (your business logic, unchanged)

=========================================================================
*/

#include <chrono>
#include <iostream>
#include <string>
using namespace std;

/*
=========================================================================
  STEP 1: Target Interface — PaymentProcessor
=========================================================================
This is the contract your application was built around. Every payment
provider must conform to this interface. CheckoutService depends ONLY
on this abstraction — it has no knowledge of any concrete provider.
*/
class PaymentProcessor {
public:
  virtual void processPayment(double amount, string currency) = 0;
  virtual bool isPaymentSuccessful() = 0;
  virtual string getTransactionId() = 0;
  virtual ~PaymentProcessor() = default;
};

/*
=========================================================================
  STEP 2: Existing Implementation — InHousePaymentProcessor
=========================================================================
Your team's own payment processor. It already implements PaymentProcessor
perfectly — no adapter needed here. This is what your system has been
using successfully.
*/
class InHousePaymentProcessor : public PaymentProcessor {
private:
  string transactionId;
  bool success = false;

public:
  void processPayment(double amount, string currency) override {
    cout << "[InHouse] Processing " << amount << " " << currency << endl;

    // Generate a unique transaction ID using current timestamp
    auto now = chrono::duration_cast<chrono::milliseconds>(
                   chrono::system_clock::now().time_since_epoch())
                   .count();
    transactionId = "TXN_" + to_string(now);
    success = true;

    cout << "[InHouse] Success. Txn ID: " << transactionId << endl;
  }

  bool isPaymentSuccessful() override { return success; }

  string getTransactionId() override { return transactionId; }
};

/*
=========================================================================
  STEP 3: Client — CheckoutService
=========================================================================
This is your core business logic. It accepts ANY PaymentProcessor and
orchestrates the checkout flow: process → verify → report.

Key point: This class is used across the entire system. You cannot and
should not modify it to accommodate a new payment provider. It only
knows about PaymentProcessor — and that's by design (Dependency Inversion).
*/
class CheckoutService {
private:
  PaymentProcessor *paymentProcessor;

public:
  CheckoutService(PaymentProcessor *processor) : paymentProcessor(processor) {}

  void checkout(double amount, string currency) {
    cout << "Checkout: Processing order for " << amount << " " << currency
         << endl;

    paymentProcessor->processPayment(amount, currency);

    if (paymentProcessor->isPaymentSuccessful()) {
      cout << "Checkout: Order successful! Txn: "
           << paymentProcessor->getTransactionId() << endl;
    } else {
      cout << "Checkout: Order failed." << endl;
    }
  }
};

/*
=========================================================================
  STEP 4: Adaptee — ExternalPayGateway (Legacy / Third-Party)
=========================================================================
This is the external vendor's payment SDK. It works perfectly well, but
its interface is completely different from your PaymentProcessor:

  - executeTransaction() instead of processPayment()
  - checkStatus(ref)     instead of isPaymentSuccessful()
  - getReferenceNumber() instead of getTransactionId()

You cannot modify this class. It's from an external vendor.
*/
class ExternalPayGateway {
private:
  long transactionRef = 0;
  bool success = false;

public:
  // Different method name: executeTransaction vs processPayment
  void executeTransaction(double totalAmount, string currencyCode) {
    cout << "[ExternalPay] Executing " << currencyCode << " " << totalAmount
         << endl;

    // Uses nanosecond timestamp as a reference number (not a string ID)
    transactionRef = chrono::duration_cast<chrono::nanoseconds>(
                         chrono::system_clock::now().time_since_epoch())
                         .count();
    success = true;

    cout << "[ExternalPay] Done. Ref: " << transactionRef << endl;
  }

  // Different signature: requires a reference number parameter
  bool checkStatus(long ref) {
    cout << "[ExternalPay] Checking status for ref: " << ref << endl;
    return success;
  }

  // Returns a long, not a string
  long getReferenceNumber() { return transactionRef; }
};

/*
=========================================================================
  STEP 5: Adapter — ExternalPayAdapter
=========================================================================
This is the core of the pattern. The adapter:

  1. IMPLEMENTS PaymentProcessor (so CheckoutService can use it)
  2. HOLDS an instance of ExternalPayGateway (composition, not inheritance)
  3. TRANSLATES each PaymentProcessor method call into the corresponding
     ExternalPayGateway method call

Method translation map:
  processPayment(amt, cur)  →  executeTransaction(amt, cur)
  isPaymentSuccessful()     →  checkStatus(ref)
  getTransactionId()        →  "LEGACY_" + to_string(getReferenceNumber())
*/
class ExternalPayAdapter : public PaymentProcessor {
private:
  ExternalPayGateway *gateway; // Holds the adaptee (composition)
  long currentRef;             // Caches the reference for status checks

public:
  ExternalPayAdapter(ExternalPayGateway *gateway)
      : gateway(gateway), currentRef(0) {}

  // Translate: processPayment → executeTransaction
  void processPayment(double amount, string currency) override {
    gateway->executeTransaction(amount, currency);
    currentRef = gateway->getReferenceNumber();
  }

  // Translate: isPaymentSuccessful → checkStatus(ref)
  bool isPaymentSuccessful() override {
    return gateway->checkStatus(currentRef);
  }

  // Translate: getTransactionId (string) → getReferenceNumber (long)
  string getTransactionId() override {
    return "LEGACY_" + to_string(currentRef);
  }
};

/*
=========================================================================
  STEP 6: Client Code — Putting It All Together
=========================================================================
Notice how CheckoutService is used identically in both cases. It has
no idea whether it's talking to InHousePaymentProcessor or to
ExternalPayGateway through the adapter. That's the whole point.
*/
int main() {
  // --- Scenario 1: Using the in-house processor (no adapter needed) ---
  cout << "=== Scenario 1: In-House Payment Processor ===" << endl;
  InHousePaymentProcessor inHouse;
  CheckoutService checkout1(&inHouse);
  checkout1.checkout(199.99, "USD");

  cout << endl;

  // --- Scenario 2: Using the legacy gateway via adapter ---
  cout << "=== Scenario 2: Legacy Gateway via Adapter ===" << endl;
  ExternalPayGateway legacyGateway;           // The adaptee (can't modify)
  ExternalPayAdapter adapter(&legacyGateway); // The adapter (translator)
  CheckoutService checkout2(&adapter);        // Same CheckoutService, unchanged
  checkout2.checkout(75.50, "EUR");

  return 0;
}