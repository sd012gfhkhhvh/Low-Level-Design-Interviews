#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

/*
Ref - https://algomaster.io/learn/lld/polymorphism
Ref - https://www.geeksforgeeks.org/cpp/vtable-and-vptr-in-cpp/

Polymorphism allows the same method name or interface to exhibit different
behaviors depending on the object that is invoking it.

it allows us to write code that is generic, extensible, and reusable, while the
specific behavior is determined at runtime or compile-time based on the object’s
actual type.

Why Polymorphism Matters ?
 Here are four concrete benefits that polymorphism provides.

Encourages loose coupling: You interact with abstractions (interfaces or base
classes), not specific implementations.

Enhances flexibility: You can introduce new behaviors without modifying existing
code, supporting the Open/Closed Principle.

Promotes scalability: Systems can grow to support more features with minimal
impact on existing code.

Enables extensibility: You can “plug in” new implementations without touching
the core business logic.

How Polymorphism Works
Polymorphism in OOP comes in two forms: compile-time (decided before the program
runs) and runtime (decided while the program runs). Both allow the same method
name to behave differently, but the mechanism is fundamentally different.
*/

/*
===== 1. Example: Compile-time Polymorphism (Method Overloading) =======

Compile-time polymorphism, also called method overloading, happens
when you have multiple methods with the same name in the same class but with
different parameter lists.

The compiler determines which version to call based on the number, types, or
order of arguments at the call site. The decision is made before the program
runs.

The compiler resolves which add() to call based on the arguments. Pass two ints,
you get add(int, int). Pass two doubles, you get add(double, double). Pass three
ints, you get add(int, int, int). No runtime decision needed.
*/

class Calculator {
public:
  // Two ints
  int add(int a, int b) { return a + b; }

  // Two doubles
  double add(double a, double b) { return a + b; }

  // Three ints
  int add(int a, int b, int c) { return a + b + c; }
};

/*
=== 2. Example: Runtime Polymorphism (Method Overriding / Dynamic Dispatch) ===

Runtime polymorphism in C++ is implemented using virtual functions and
inheritance.  It allows the correct function to be called at runtime based on
the actual object type, rather than the pointer or reference type.

NOTE -
Runtime polymorphism (late binding) occurs only through pointers or references
to a base class, not with direct objects of the derived class.

The core reason pointers (or references) enable late binding while direct calls
do not lies in type information availability at compile time.

Direct Object Calls (Early Binding)
When you call a method on a direct object, the type is known at compile time.
The compiler sees derivedObject.virtualFunction() and knows exactly which class
it belongs to. So, it performs early binding: it directly resolves the function
address and generates a fixed call — no runtime overhead. Pointer/Reference
Calls (Late Binding) When calling via a base class pointer or reference (e.g.,
Base* ptr = &derivedObj; ptr->virtualFunction();), the static type of ptr is
Base*, but the dynamic (actual) type of the object may be Derived. Since the
compiler cannot know at compile time which derived class object the pointer will
point to (especially across functions or user input), it defers the decision to
runtime. This is where the vtable mechanism kicks in: Every object with virtual
functions contains a hidden vptr pointing to its class’s vtable. The vtable
holds function pointers to the correct (possibly overridden) versions of virtual
functions. At runtime, the call ptr->virtualFunction() follows: ptr → vptr →
vtable → correct function address.

*/

class Notification {
protected:
  string recipient;
  string message;

public:
  Notification(const string &recipient, const string &message)
      : recipient(recipient), message(message) {}

  virtual ~Notification() {
  } // virtual destructure is must needed to avoid memory leak

  virtual void send() {
    cout << "Sending generic notification to " << recipient << endl;
  }
};

class EmailNotification : public Notification {
  string subject;

public:
  EmailNotification(const string &recipient, const string &message,
                    const string &subject)
      : Notification(recipient, message), subject(subject) {}

  void send() override {
    cout << "Sending EMAIL to " << recipient << " | Subject: " << subject
         << endl;
  }
};

class SMSNotification : public Notification {
  string phoneNumber;

public:
  SMSNotification(const string &recipient, const string &message,
                  const string &phoneNumber)
      : Notification(recipient, message), phoneNumber(phoneNumber) {}

  void send() override {
    cout << "Sending SMS to " << phoneNumber << " | Message: " << message
         << endl;
  }
};

class PushNotification : public Notification {
  string deviceToken;

public:
  PushNotification(const string &recipient, const string &message,
                   const string &deviceToken)
      : Notification(recipient, message), deviceToken(deviceToken) {}

  void send() override {
    cout << "Sending PUSH to device " << deviceToken.substr(0, 8)
         << "... | Alert: " << message << endl;
  }
};

// The key thing to notice: every element in the list is stored as a
// Notification reference, but the runtime calls the correct child class's
// send(). The variable type says Notification. The behavior says
// EmailNotification, SMSNotification, or PushNotification. That's runtime
// polymorphism.

int main() {
  // Exampel -1
  Calculator calc;
  std::cout << calc.add(2, 3) << std::endl; // Calls add(int, int) -> 5
  std::cout << calc.add(2.5, 3.5)
            << std::endl;                      // Calls add(double, double) -> 6
  std::cout << calc.add(1, 2, 3) << std::endl; // Calls add(int, int, int) -> 6

  // Example 2
  vector<unique_ptr<Notification>> notifications;
  notifications.push_back(make_unique<EmailNotification>(
      "alice@example.com", "Your order shipped!", "Order Update"));
  notifications.push_back(
      make_unique<SMSNotification>("Bob", "Code: 482910", "+1-555-0123"));
  notifications.push_back(make_unique<PushNotification>(
      "Charlie", "New message", "d8a3f4b2c1e5a9b7"));

  for (auto &n : notifications) {
    n->send();
  }
  return 0;
}

/*
Note-
Polymorphism with Interfaces vs Abstract Classes
Both interfaces and abstract classes enable polymorphism. In the notification
example, you could define Notification as either an abstract class or an
interface. The polymorphic behavior, calling send() on a base reference and
having the child's version execute, works the same either way. So when should
you use which?

Aspect	Interface	Abstract Class
Relationship	"can do" (capability)	"is a" (family)
Shared behavior	None (contract only)	Yes (concrete methods + fields)
Multiple	A class can implement many	A class can extend only one
When to use	Unrelated classes share a capability	Related classes share
logic Example	Sendable implemented by Email, Invoice, Report	Notification
extended by Email, SMS, Push Use an interface when the implementing classes are
fundamentally different but share a capability. Email, Invoice, and Report have
nothing in common structurally, but they can all send(). An interface defines
that contract without forcing a shared hierarchy.

Use an abstract class when the implementing classes are a family with shared
logic. All notifications need the same formatHeader() method, the same recipient
and message fields, and the same constructor pattern. An abstract class provides
all of that, plus the abstract send() that each child implements differently.

In practice, many designs use both. An abstract Notification class provides
shared fields and formatting, while a Sendable interface marks anything that can
be sent (notifications, reports, alerts).
*/