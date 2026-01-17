#include <iostream>
using namespace std;

/*
======================
    # Abstruct class
        - https://www.scaler.com/topics/cpp/abstract-class-in-cpp/
    # virtual fucntion
        - https://www.geeksforgeeks.org/cpp/virtual-function-cpp/
        - https://www.tutorialspoint.com/cplusplus/cpp_virtual_function.htm
    # pure abstruct class (similar to interface in java)
        - https://www.geeksforgeeks.org/cpp/cpp-program-to-create-an-interface/
*/
class PaymentGateway {
public:
  virtual ~PaymentGateway() {} // Virtual destructor for proper cleanup
  virtual void initiatePayment(double amount) = 0; // Pure virtual function
};

class StripePayment : public PaymentGateway {
public:
  void initiatePayment(double amount) override {
    cout << "Processing payment via Stripe: $" << amount << endl;
  }
};

class RazorpayPayment : public PaymentGateway {
public:
  void initiatePayment(double amount) override {
    cout << "Processing payment via Razorpay: ₹" << amount << endl;
  }
};

class CheckoutService {
private:
  PaymentGateway *paymentGateway;

public:
  CheckoutService(PaymentGateway *gateway) : paymentGateway(gateway) {}

  void setPaymentGateway(PaymentGateway *gateway) { paymentGateway = gateway; }

  void checkout(double amount) {
    if (paymentGateway != nullptr) {
      paymentGateway->initiatePayment(amount);
    }
  }
};

int main() {
  StripePayment stripeGateway;
  CheckoutService service(&stripeGateway);
  service.checkout(120.50); // Output: Processing payment via Stripe: $120.5

  // Switch to Razorpay
  RazorpayPayment razorpayGateway;
  service.setPaymentGateway(&razorpayGateway);
  service.checkout(150.50); // Output: Processing payment via Razorpay: ₹150.5

  return 0;
}