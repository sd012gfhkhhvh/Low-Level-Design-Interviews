/*
==================================
Exercise 2: ShippingCostCalculator
==================================
Refactor ShippingCostCalculator Class

Problem: A ShippingCostCalculator uses if-else to determine the shipping cost
based on the shipping type (Standard, Express, Overnight, International). Every
time a new shipping type is added, someone has to modify the calculator.
Refactor this so that new shipping types can be added without modifying the
calculator.

Requirements:

Define a ShippingStrategy interface with a calculateCost(weight) method
Create concrete implementations: StandardShipping, ExpressShipping,
OvernightShipping, InternationalShipping Refactor ShippingCostCalculator to
accept a ShippingStrategy instead of a string The calculator should delegate
cost computation to the strategy
*/

#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

using namespace std;

// =========================================
// Before applying OCP
// ==========================================
// class ShippingCostCalculator {
// public:
//     double calculate(const string& shippingType, double weight) {
//         if (shippingType == "Standard") {
//             return weight * 1.5;
//         } else if (shippingType == "Express") {
//             return weight * 3.0;
//         } else if (shippingType == "Overnight") {
//             return weight * 5.0;
//         } else if (shippingType == "International") {
//             return weight * 10.0;
//         }
//         throw invalid_argument("Unknown shipping type: " + shippingType);
//     }
// };

// =======================================
// After applyiing OCP
// =======================================
/*
=========================================================
                    STRATEGY INTERFACE
=========================================================
*/

class ShippingStrategy {
public:
  virtual ~ShippingStrategy() = default;
  virtual double calculateCost(double weight) const = 0;
};

/*
=========================================================
                CONCRETE STRATEGIES
=========================================================
*/

class StandardShipping : public ShippingStrategy {
public:
  double calculateCost(double weight) const override { return weight * 1.5; }
};

class ExpressShipping : public ShippingStrategy {
public:
  double calculateCost(double weight) const override { return weight * 3.0; }
};

class OvernightShipping : public ShippingStrategy {
public:
  double calculateCost(double weight) const override { return weight * 5.0; }
};

class InternationalShipping : public ShippingStrategy {
public:
  double calculateCost(double weight) const override { return weight * 10.0; }
};

/*
=========================================================
IMPLEMENTATION 1
Stateless Calculator (Strategy passed per call)

WHEN TO USE:
- Calculator doesn't need to remember a strategy.
- Different strategies may be used for every call.
- Maximum flexibility.

NOTE:
This is often close to being just a utility function.
=========================================================
*/

class ShippingCostCalculator_V1 {
public:
  double calculate(const ShippingStrategy &strategy, double weight) const {
    return strategy.calculateCost(weight);
  }
};

/*
=========================================================
IMPLEMENTATION 2
Reference-based Dependency Injection

WHEN TO USE:
- Strategy already exists elsewhere.
- Calculator should NOT own the strategy.
- Lifetime relationship is obvious.

BEST CHOICE:
For simple applications where strategies are stack objects.
=========================================================
*/

class ShippingCostCalculator_V2 {
private:
  const ShippingStrategy &strategy;

public:
  explicit ShippingCostCalculator_V2(const ShippingStrategy &strategy)
      : strategy(strategy) {}

  double calculate(double weight) const {
    return strategy.calculateCost(weight);
  }
};

/*
=========================================================
IMPLEMENTATION 3
Ownership via unique_ptr

WHEN TO USE:
- Strategy is created dynamically.
- Factory creates strategies.
- Calculator should own strategy lifetime.

BEST CHOICE:
When ownership transfer is required.
=========================================================
*/

class ShippingCostCalculator_V3 {
private:
  unique_ptr<ShippingStrategy> strategy;

public:
  explicit ShippingCostCalculator_V3(unique_ptr<ShippingStrategy> strategy)
      : strategy(move(strategy)) {
    if (!this->strategy) {
      throw invalid_argument("Strategy cannot be null");
    }
  }

  double calculate(double weight) const {
    return strategy->calculateCost(weight);
  }
};

/*
=========================================================
Factory Example

Shows where V3 becomes useful.
=========================================================
*/

unique_ptr<ShippingStrategy> createStrategy(const string &type) {
  if (type == "standard")
    return make_unique<StandardShipping>();

  if (type == "express")
    return make_unique<ExpressShipping>();

  if (type == "overnight")
    return make_unique<OvernightShipping>();

  if (type == "international")
    return make_unique<InternationalShipping>();

  throw invalid_argument("Unknown strategy type");
}

/*
=========================================================
DRIVER CODE
=========================================================
*/

int main() {
  // Before OCP
  // ShippingCostCalculator calculator;
  // cout << "Standard: $" << calculator.calculate("Standard", 2.0) << endl;
  // cout << "Express: $" << calculator.calculate("Express", 2.0) << endl;

  // After OCP
  double weight = 10.0;

  StandardShipping standard;
  ExpressShipping express;
  OvernightShipping overnight;
  InternationalShipping international;

  /*
  -----------------------------------------------------
  V1 : Stateless
  -----------------------------------------------------
  */
  cout << fixed << setprecision(1);
  cout << "===== V1 : Stateless Calculator =====\n";

  ShippingCostCalculator_V1 calc1;

  cout << "Standard      : $" << calc1.calculate(standard, weight) << '\n';

  cout << "Express       : $" << calc1.calculate(express, weight) << '\n';

  cout << "Overnight     : $" << calc1.calculate(overnight, weight) << '\n';

  cout << "International : $" << calc1.calculate(international, weight) << '\n';

  /*
  -----------------------------------------------------
  V2 : Reference Injection
  -----------------------------------------------------
  */

  cout << "\n===== V2 : Reference Injection =====\n";

  ShippingCostCalculator_V2 calc2(express);

  cout << "Express Cost  : $" << calc2.calculate(weight) << '\n';

  /*
  -----------------------------------------------------
  V3 : Ownership using unique_ptr
  -----------------------------------------------------
  */

  cout << "\n===== V3 : unique_ptr Ownership =====\n";

  ShippingCostCalculator_V3 calc3(make_unique<ExpressShipping>());

  cout << "Express Cost  : $" << calc3.calculate(weight) << '\n';

  /*
  -----------------------------------------------------
  V3 + Factory Example
  -----------------------------------------------------
  */

  cout << "\n===== V3 + Factory =====\n";

  auto strategy = createStrategy("international");

  ShippingCostCalculator_V3 calc4(move(strategy));

  cout << "International Cost : $" << calc4.calculate(weight) << '\n';

  return 0;
}

/*
# ShippingCostCalculator Choices

### V1 — Pass Strategy Per Call
```cpp
calc.calculate(strategy, weight);
```
✅ Stateless, strategy can change every call
❌ Calculator doesn't own/use a fixed strategy

### V2 — Reference Injection
```cpp
ShippingCostCalculator calc(strategy);
```
✅ Best when strategy already exists
✅ No ownership, no heap allocation
⚠️ Strategy must outlive calculator

### V3 — unique_ptr Ownership
```cpp
ShippingCostCalculator calc(
    make_unique<Strategy>()
);
```
✅ Calculator owns strategy
✅ Ideal for factories/runtime configuration
⚠️ Heap allocation + ownership transfer

## Quick Rule

| Scenario | Choose |
|----------|---------|
| Strategy changes per call | V1 |
| Strategy exists elsewhere | V2 ⭐ |
| Calculator owns strategy | V3 |
| Factory-created strategy | V3 |
*/