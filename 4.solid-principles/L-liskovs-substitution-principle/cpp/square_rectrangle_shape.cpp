/*
============================================
       The Rectangle/Square Problem
============================================
Refactor Rectangle/Square Class

Problem: This is the classic LSP example. A Square class extends Rectangle but
overrides setWidth and setHeight to maintain equal sides. This breaks client
code that expects width and height to be independent.

Refactor the design using a Shape interface with a getArea() method, so that
Rectangle and Square are both valid shapes without one inheriting from the
other.

Requirements:

Create a Shape interface with a getArea() method
Implement Rectangle with independent width and height, set via constructor
Implement Square with a single side length, set via constructor
Neither class should extend the other
Client code should work with any Shape without assumptions about mutability
*/

#include <iostream>
using namespace std;

/*
=======================
Before LSP
=======================
*/

// class Rectangle {
// protected:
//   int width = 0;
//   int height = 0;

// public:
//   virtual void setWidth(int w) { width = w; }
//   virtual void setHeight(int h) { height = h; }
//   int getArea() const { return width * height; }
//   virtual ~Rectangle() = default;
// };

// class Square : public Rectangle {
// public:
//   void setWidth(int w) override {
//     width = w;
//     height = w;
//   }
//   void setHeight(int h) override {
//     width = h;
//     height = h;
//   }
// };

// // Client code that breaks with Square
// void resize(Rectangle &rect) {
//   rect.setWidth(5);
//   rect.setHeight(10);
//   cout << "Area: " << rect.getArea() << endl;
// }

/*
=======================
After LSP
=======================
*/

/*
=========================================================
                     INTERFACE
=========================================================
*/
class Shape {
public:
  virtual double getArea() const = 0;
  virtual ~Shape() = default;
};

/*
=========================================================
                CONCRETE Inplementations
=========================================================
*/

class Rectangle : public Shape {
private:
  double width, height;

public:
  Rectangle(const double width, const double height)
      : width(width), height(height) {}

  double getArea() const override { return width * height; }

  // setters
  void setHeight(const double height) { this->height = height; }
  void setWidth(const double width) { this->width = width; }
};

class Square : public Shape {
private:
  double sideLength;

public:
  Square(const double length) : sideLength(length) {}

  double getArea() const override { return sideLength * sideLength; }

  // setters
  void setSide(const double len) { this->sideLength = len; }
};

void resize(Shape &shape) {
  if (auto rect = dynamic_cast<Rectangle *>(&shape)) {
    rect->setWidth(5);
    rect->setHeight(10);
  } else if (auto sq = dynamic_cast<Square *>(&shape)) {
    sq->setSide(5);
  }
}

int main() {
  // Before LSP
  // Rectangle rect;
  // Square sq;
  // resize(rect); // Area: 50
  // resize(sq);   // Area: 100 -- LSP violation!

  // After LSP
  Rectangle rectangle(0, 0);
  Square square(0);

  resize(rectangle);
  resize(square);

  cout << "Rectangle area: " << static_cast<int>(rectangle.getArea()) << endl;
  cout << "Square area: " << static_cast<int>(square.getArea()) << endl;

  return 0;
}
