/*
=========================
Exercise: Shape Cloning
=========================
Implement Shape Cloning

Problem: Implement a Shape prototype system with Circle and Rectangle classes.
Each shape has a color and dimensions. Clone a circle and a rectangle, modify
the clones, and verify the originals are unaffected.

Requirements:
- Circle has color, radius
- Rectangle has color, width, height
- Both implement a Cloneable interface with a clone() method
- After cloning, modifying the clone must not affect the original
*/

#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

class Cloneable {
public:
  virtual Cloneable *clone() const = 0;
  virtual ~Cloneable() = default;
};

class Circle : public Cloneable {
private:
  string color;
  double radius;

public:
  Circle(string color, double radius) : color(move(color)), radius(radius) {}

  Circle *clone() const override { return new Circle(color, radius); }

  void setRadius(double r) { radius = r; }

  void printInfo() const {
    printf("Circle [Color: %s, Radius: %.1f]\n", color.c_str(), radius);
  }
};

class Rectangle : public Cloneable {
private:
  string color;
  double width, height;

public:
  Rectangle(string color, double w, double h)
      : color(move(color)), width(w), height(h) {}

  Rectangle *clone() const override {
    return new Rectangle(color, width, height);
  }

  void setWidth(double w) { width = w; }

  void printInfo() const {
    printf("Rectangle [Color: %s, Width: %.1f, Height: %.1f]\n", color.c_str(),
           width, height);
  }
};

int main() {
  Circle original("Red", 5.0);
  Circle *cloned = original.clone();
  cloned->setRadius(10.0);
  original.printInfo();
  cloned->printInfo();
  delete cloned;

  Rectangle rect("Blue", 4.0, 6.0);
  Rectangle *clonedRect = rect.clone();
  clonedRect->setWidth(8.0);
  rect.printInfo();
  clonedRect->printInfo();
  delete clonedRect;
  return 0;
}