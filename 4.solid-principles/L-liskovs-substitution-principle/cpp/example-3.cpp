/*
====================================
Exercise 2: The Bird/Penguin Problem
====================================
Refactor Bird/Penguin Class

Problem:  A Bird class has both eat() and fly() methods. A Penguin subclass
extends Bird but overrides fly() to throw an exception, since penguins cannot
fly. Any client code that calls fly() on a Bird reference will crash at runtime
when it gets a Penguin.

Requirements:

Create a Bird interface with only an eat() method
Create a FlyingBird interface that extends Bird and adds a fly() method
Sparrow implements FlyingBird (it can eat and fly)
Penguin implements Bird (it can eat, but not fly)
Client code that works with Bird should never call fly(), and code that needs
flight should accept FlyingBird
*/

#include <iostream>
#include <stdexcept>
#include <string>

// Before: Penguin extends Bird but can't fly
class Bird {
public:
  virtual void eat() { std::cout << "Bird is eating" << std::endl; }

  virtual void fly() { std::cout << "Bird is flying" << std::endl; }

  virtual ~Bird() = default;
};

class Sparrow : public Bird {
public:
  void eat() override { std::cout << "Sparrow is eating" << std::endl; }
  void fly() override { std::cout << "Sparrow is flying" << std::endl; }
};

class Penguin : public Bird {
public:
  void eat() override { std::cout << "Penguin is eating" << std::endl; }
  void fly() override { throw std::runtime_error("Penguins can't fly!"); }
};

void makeBirdFly(Bird &bird) {
  bird.fly(); // Crashes for Penguin!
}

int main() {
  Sparrow sparrow;
  Penguin penguin;
  makeBirdFly(sparrow); // Works fine
  makeBirdFly(penguin); // runtime_error!
  return 0;
}

// TODO: Split Bird into a Bird interface (eat) and a FlyingBird interface
// (fly).
// TODO: Sparrow implements FlyingBird, Penguin implements only Bird.