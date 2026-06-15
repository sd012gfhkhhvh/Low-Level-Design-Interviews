/*
=========================================================================
  Strategy Pattern — Practical Example: Shipping Cost Calculator
=========================================================================

Scenario:
You are building an e-commerce platform. You need a shipping cost 
calculator, but shipping costs can be calculated in many different ways 
depending on business rules: Flat Rate, Weight-Based, Distance-Based, 
or via a Third-Party API (like FedEx or UPS).

The Problem:
If you try to put all this logic into a single `ShippingCostCalculator` 
class using a massive `if-else` or `switch` statement (as seen in V1), 
the class will violate the Open/Closed Principle. Every time you add a 
new shipping method, you have to modify the core class. This makes the 
code fragile, hard to read, and difficult to test.

Solution:
Apply the Strategy Pattern (as seen in V2). Extract each shipping calculation 
algorithm into its own dedicated class (a Concrete Strategy) that implements a 
common interface. The main calculator class (the Context) holds a reference 
to a strategy object and delegates the calculation to it. Now, adding a new 
shipping method simply means creating a new class—no existing code needs 
to be touched!

=========================================================================
  Component Mapping (V2)
=========================================================================
  Strategy          → ShippingStrategy (Interface for algorithms)
  ConcreteStrategy  → FlatRate, WeightBased, DistanceBased, ThirdPartyApi
  Context           → ShippingCostService (Delegates to the strategy)
  Client            → ecommerceAppV2() (Selects the strategy and passes it)
=========================================================================
*/

#include <iostream>
#include <string>

using namespace std;

// A simple Order class to provide data to our calculators
class Order {
private:
  double totalWeight;
  string destinationZone;
  double orderValue;

public:
  Order(double weight, string zone, double value) 
    : totalWeight(weight), destinationZone(move(zone)), orderValue(value) {}

  double getTotalWeight() const { return totalWeight; }
  string getDestinationZone() const { return destinationZone; }
  double getOrderValue() const { return orderValue; }
};

/*
=========================================================================
  V1: THE NAIVE APPROACH (Anti-Pattern)
=========================================================================
All calculation logic is stuffed into a single class using if-else chains.
Violates Open/Closed Principle (OCP) and Single Responsibility Principle (SRP).
*/
class ShippingCostCalculatorNaive {
public:
  double calculateShippingCost(const Order &order, const string &strategyType) {
    double cost = 0.0;

    if (strategyType == "FLAT_RATE") {
      cout << "[Naive] Calculating with Flat Rate strategy." << endl;
      cost = 10.0;
    } else if (strategyType == "WEIGHT_BASED") {
      cout << "[Naive] Calculating with Weight-Based strategy." << endl;
      cost = order.getTotalWeight() * 2.5;
    } else if (strategyType == "DISTANCE_BASED") {
      cout << "[Naive] Calculating with Distance-Based strategy." << endl;
      if (order.getDestinationZone() == "ZoneA") {
        cost = 5.0;
      } else if (order.getDestinationZone() == "ZoneB") {
        cost = 12.0;
      } else {
        cost = 20.0; // fallback
      }
    } else if (strategyType == "THIRD_PARTY_API") {
      cout << "[Naive] Calculating with Third-Party API strategy." << endl;
      // Simulated external call
      cost = 7.5 + (order.getOrderValue() * 0.02);
    } else {
      throw invalid_argument("Unknown shipping strategy: " + strategyType);
    }

    return cost;
  }
};

void ecommerceAppV1() {
  ShippingCostCalculatorNaive calculator;
  Order order1(10.0, "ZoneB", 100.00);

  cout << "--- Order 1 ---" << endl;
  double cost = calculator.calculateShippingCost(order1, "FLAT_RATE");
  cout << "Calculated Cost: $" << cost << endl << endl;

  cost = calculator.calculateShippingCost(order1, "WEIGHT_BASED");
  cout << "Calculated Cost: $" << cost << endl << endl;

  cost = calculator.calculateShippingCost(order1, "DISTANCE_BASED");
  cout << "Calculated Cost: $" << cost << endl << endl;

  // What if we want to try a new "PremiumZone" strategy?
  // We have to go modify the ShippingCostCalculatorNaive class again...
}


/*
=========================================================================
  V2: STRATEGY PATTERN APPROACH
=========================================================================
Logic is extracted into separate strategy classes. Context delegates to them.
*/

// STEP 1: The Strategy Interface
class ShippingStrategy {
public:
  virtual ~ShippingStrategy() = default;
  virtual double calculateCost(const Order &order) = 0;
};

// STEP 2: Concrete Strategies
class FlatRateShipping : public ShippingStrategy {
private:
  double rate;

public:
  FlatRateShipping(double r) : rate(r) {}

  double calculateCost(const Order &order) override {
    cout << "[Strategy] Using Flat Rate ($" << rate << ")" << endl;
    return rate;
  }
};

class WeightBasedShipping : public ShippingStrategy {
private:
  double ratePerKg;

public:
  WeightBasedShipping(double rateKg) : ratePerKg(rateKg) {}

  double calculateCost(const Order &order) override {
    cout << "[Strategy] Using Weight-Based ($" << ratePerKg << "/kg)" << endl;
    return order.getTotalWeight() * ratePerKg;
  }
};

class DistanceBasedShipping : public ShippingStrategy {
private:
  double ratePerKm;

public:
  DistanceBasedShipping(double rateKm) : ratePerKm(rateKm) {}

  double calculateCost(const Order &order) override {
    cout << "[Strategy] Using Distance-Based for zone: " << order.getDestinationZone() << endl;

    if (order.getDestinationZone() == "ZoneA") {
      return ratePerKm * 5.0;
    } else if (order.getDestinationZone() == "ZoneB") {
      return ratePerKm * 7.0;
    } else {
      return ratePerKm * 10.0;
    }
  }
};

class ThirdPartyApiShipping : public ShippingStrategy {
private:
  double baseFee;
  double percentageFee;

public:
  ThirdPartyApiShipping(double base, double percentage)
      : baseFee(base), percentageFee(percentage) {}

  double calculateCost(const Order &order) override {
    cout << "[Strategy] Using Third-Party API" << endl;
    return baseFee + (order.getOrderValue() * percentageFee);
  }
};

// STEP 3: The Context
class ShippingCostService {
private:
  ShippingStrategy *strategy;

public:
  ShippingCostService(ShippingStrategy *initialStrategy) : strategy(initialStrategy) {}

  void setStrategy(ShippingStrategy *newStrategy) {
    strategy = newStrategy;
  }

  double executeShippingCalculation(const Order &order) {
    if (strategy == nullptr) {
      throw invalid_argument("Shipping strategy is not set.");
    }
    return strategy->calculateCost(order);
  }
};

// STEP 4: Client Code
void ecommerceAppV2() {
  Order order1(10.0, "ZoneB", 100.00);

  FlatRateShipping flatRate(15.0);
  WeightBasedShipping weightBased(2.5);
  DistanceBasedShipping distanceBased(5.0);
  ThirdPartyApiShipping thirdParty(7.5, 0.05);

  ShippingCostService shippingService(&flatRate);
  
  cout << "--- Checkout: Option 1 ---" << endl;
  double cost1 = shippingService.executeShippingCalculation(order1);
  cout << "Calculated Cost: $" << cost1 << endl << endl;

  shippingService.setStrategy(&weightBased);
  cout << "--- Checkout: Option 2 ---" << endl;
  double cost2 = shippingService.executeShippingCalculation(order1);
  cout << "Calculated Cost: $" << cost2 << endl << endl;

  shippingService.setStrategy(&distanceBased);
  cout << "--- Checkout: Option 3 ---" << endl;
  double cost3 = shippingService.executeShippingCalculation(order1);
  cout << "Calculated Cost: $" << cost3 << endl << endl;
}

int main() {
  cout << "========================================" << endl;
  cout << "  V1: NAIVE APPROACH (If/Else Anti-Pattern) " << endl;
  cout << "========================================" << endl;
  ecommerceAppV1();

  cout << "========================================" << endl;
  cout << "  V2: STRATEGY PATTERN APPROACH         " << endl;
  cout << "========================================" << endl;
  ecommerceAppV2();

  return 0;
}