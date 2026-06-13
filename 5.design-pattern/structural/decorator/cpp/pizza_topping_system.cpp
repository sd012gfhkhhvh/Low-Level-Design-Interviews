/*
=========================================================================
  Decorator Pattern — Exercise: Pizza Topping System
=========================================================================

Scenario:
You are building an ordering system for a pizzeria. Customers start with a
plain pizza base and can customize it by adding various toppings like
cheese, olives, and mushrooms. Each topping increases the total cost and
must be added to the final description of the pizza.

The Problem:
Using subclassing for every possible combination (e.g., `CheesePizza`,
`CheeseMushroomPizza`, `OliveMushroomPizza`) leads to a massive class
explosion. Furthermore, it's difficult to handle requests like "extra
cheese" (double cheese) using a rigid inheritance hierarchy.

Solution:
Apply the Decorator Pattern. Create a base `PlainPizza` object that
implements the `Pizza` interface. Then, create topping decorators that
also implement the `Pizza` interface and wrap around the base pizza.
This allows you to dynamically stack toppings at runtime, calculating
the total cost and building the description cleanly.

=========================================================================
  Component Mapping
=========================================================================

  Component         → Pizza (Interface for cost and description)
  ConcreteComponent → PlainPizza (The base crust/sauce)
  Decorator         → PizzaDecorator (Abstract base for all toppings)
  ConcreteDecorator → CheeseDecorator, OliveDecorator, MushroomDecorator
  Client            → main() (The customer building their order)

=========================================================================
*/

#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

/*
=========================================================================
  STEP 1: The Component Interface
=========================================================================
Both the base pizza and the toppings implement this contract.
*/
class Pizza {
public:
  virtual double getCost() = 0;
  virtual string getDescription() = 0;
  virtual ~Pizza() = default;
};

/*
=========================================================================
  STEP 2: The Concrete Component
=========================================================================
The starting point for every order. It has a base cost and description.
*/
class PlainPizza : public Pizza {
public:
  double getCost() override { return 5.00; }

  string getDescription() override { return "Plain pizza"; }
};

/*
=========================================================================
  STEP 3: The Base Decorator
=========================================================================
Holds a reference to the inner Pizza (or previously decorated Pizza).
It delegates the cost and description calls inward.
*/
class PizzaDecorator : public Pizza {
protected:
  Pizza *pizza; // The wrapped object

public:
  PizzaDecorator(Pizza *pizza) : pizza(pizza) {}

  double getCost() override { return pizza->getCost(); }

  string getDescription() override { return pizza->getDescription(); }
};

/*
=========================================================================
  STEP 4: Concrete Decorators (Toppings)
=========================================================================
Each topping adds its specific cost and appends its name to the description.
*/

class CheeseDecorator : public PizzaDecorator {
public:
  CheeseDecorator(Pizza *pizza) : PizzaDecorator(pizza) {}

  double getCost() override { return pizza->getCost() + 1.50; }

  string getDescription() override {
    return pizza->getDescription() + ", cheese";
  }
};

class OliveDecorator : public PizzaDecorator {
public:
  OliveDecorator(Pizza *pizza) : PizzaDecorator(pizza) {}

  double getCost() override { return pizza->getCost() + 2.00; }

  string getDescription() override {
    return pizza->getDescription() + ", olives";
  }
};

class MushroomDecorator : public PizzaDecorator {
public:
  MushroomDecorator(Pizza *pizza) : PizzaDecorator(pizza) {}

  double getCost() override { return pizza->getCost() + 1.00; }

  string getDescription() override {
    return pizza->getDescription() + ", mushrooms";
  }
};

/*
=========================================================================
  STEP 5: Client Code
=========================================================================
The client constructs the pizza by stacking decorators dynamically.
*/
int main() {
  cout << "=== Decorator Pattern: Pizzeria ===" << endl << endl;

  // Order 1: Just a plain pizza
  PlainPizza plain;
  printf("Order 1: %-45s | $%.2f\n", plain.getDescription().c_str(),
         plain.getCost());

  // Order 2: Plain pizza + Cheese + Olives
  CheeseDecorator cheese(&plain);
  OliveDecorator cheeseOlive(&cheese);
  printf("Order 2: %-45s | $%.2f\n", cheeseOlive.getDescription().c_str(),
         cheeseOlive.getCost());

  // Order 3: The "Loaded" Pizza (Extra Cheese, Olives, Mushrooms)
  // Notice we can stack the same decorator (Cheese) multiple times for "extra"!
  PlainPizza plain2;
  CheeseDecorator cheeseBase(&plain2);
  CheeseDecorator extraCheese(&cheeseBase); // Double cheese
  OliveDecorator olives(&extraCheese);
  MushroomDecorator loaded(&olives);

  printf("Order 3: %-45s | $%.2f\n", loaded.getDescription().c_str(),
         loaded.getCost());

  cout << endl;
  return 0;
}