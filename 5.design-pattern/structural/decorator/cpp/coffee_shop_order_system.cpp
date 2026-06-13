/*
=========================================================================
  Decorator Pattern — Practical Example: Coffee Shop Order System
=========================================================================

Scenario:
You are building an ordering system for a coffee shop. Customers can order 
a simple coffee, but they can also add a huge variety of condiments: milk, 
sugar, whipped cream, caramel, vanilla, etc. Each condiment adds a specific 
amount to the total cost and alters the description of the order.

The Problem:
If you try to use inheritance to calculate the cost (e.g., `CoffeeWithMilk`, 
`CoffeeWithMilkAndSugar`, `CoffeeWithDoubleMilkAndCream`), the number of 
classes grows exponentially. It becomes completely unmaintainable. Also, 
what happens if someone wants double milk? Inheritance can't handle that cleanly.

Solution:
Use the Decorator Pattern. Start with a base `SimpleCoffee` object, and 
dynamically wrap it with decorator objects (`MilkDecorator`, `SugarDecorator`) 
at runtime. Each decorator adds its own cost and description to the object 
it wraps. You can even wrap the same decorator multiple times (e.g., double milk)!

=========================================================================
  Component Mapping
=========================================================================

  Component         → Coffee (Interface tracking cost and description)
  ConcreteComponent → SimpleCoffee (The base order)
  Decorator         → CoffeeDecorator (Base class for condiments)
  ConcreteDecorator → MilkDecorator, SugarDecorator, WhippedCreamDecorator
  Client            → main() (The barista building the order)

=========================================================================
*/

#include <iostream>
#include <string>

using namespace std;

/*
=========================================================================
  STEP 1: The Component Interface
=========================================================================
This is the shared contract. Both the base coffee and all condiments 
implement this so their costs and descriptions can be summed up.
*/
class Coffee {
public:
  virtual double getCost() = 0;
  virtual string getDescription() = 0;
  virtual ~Coffee() = default;
};

/*
=========================================================================
  STEP 2: The Concrete Component
=========================================================================
This is the base object that provides the starting price and description. 
All orders begin here.
*/
class SimpleCoffee : public Coffee {
public:
  double getCost() override { 
    return 1.00; 
  }

  string getDescription() override { 
    return "Simple coffee"; 
  }
};

/*
=========================================================================
  STEP 3: The Base Decorator
=========================================================================
This abstract class implements the Coffee interface AND holds a reference 
to another Coffee object. It forms the chain of wrapped objects.
*/
class CoffeeDecorator : public Coffee {
protected:
  // The coffee (or previously decorated coffee) being wrapped
  Coffee *inner;

public:
  CoffeeDecorator(Coffee *inner) : inner(inner) {}
};

/*
=========================================================================
  STEP 4: Concrete Decorators (Condiments)
=========================================================================
Each decorator adds its own price and appends its name to the description.
*/

class MilkDecorator : public CoffeeDecorator {
public:
  MilkDecorator(Coffee *inner) : CoffeeDecorator(inner) {}

  double getCost() override { 
    return inner->getCost() + 0.50; 
  }

  string getDescription() override {
    return inner->getDescription() + ", milk";
  }
};

class SugarDecorator : public CoffeeDecorator {
public:
  SugarDecorator(Coffee *inner) : CoffeeDecorator(inner) {}

  double getCost() override { 
    return inner->getCost() + 0.20; 
  }

  string getDescription() override {
    return inner->getDescription() + ", sugar";
  }
};

class WhippedCreamDecorator : public CoffeeDecorator {
public:
  WhippedCreamDecorator(Coffee *inner) : CoffeeDecorator(inner) {}

  double getCost() override { 
    return inner->getCost() + 1.00; 
  }

  string getDescription() override {
    return inner->getDescription() + ", whipped cream";
  }
};

/*
=========================================================================
  STEP 5: Client Code
=========================================================================
The client (barista) creates the base coffee and wraps it with condiments 
based on the customer's order.
*/
int main() {
  cout << "=== Decorator Pattern: Coffee Shop ===" << endl << endl;

  // Order 1: Just a plain coffee
  SimpleCoffee simple;
  printf("Order 1: %-55s | $%.2f\n", simple.getDescription().c_str(), simple.getCost());

  // Order 2: Coffee with Milk and Sugar
  MilkDecorator milk(&simple);
  SugarDecorator milkSugar(&milk);
  printf("Order 2: %-55s | $%.2f\n", milkSugar.getDescription().c_str(), milkSugar.getCost());

  // Order 3: Coffee with Double Milk, Sugar, and Whipped Cream
  // Notice how we can stack the same decorator (Milk) multiple times!
  SimpleCoffee simple2;
  MilkDecorator milk1(&simple2);
  MilkDecorator milk2(&milk1); // Double milk!
  SugarDecorator sugar(&milk2);
  WhippedCreamDecorator luxuryOrder(&sugar);
  
  printf("Order 3: %-55s | $%.2f\n", luxuryOrder.getDescription().c_str(), luxuryOrder.getCost());

  cout << endl;
  return 0;
}