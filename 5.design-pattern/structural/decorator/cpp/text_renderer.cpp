/*
=========================================================================
  Decorator Pattern — Practical Example: Rich Text Renderer
=========================================================================

Scenario:
You are building a rich text rendering system (like a markdown previewer). 
At the core of your system is a `TextView` component that renders plain 
text on the screen.

The Problem:
The requirements grow: you need to support bold, italic, and underlined text, 
as well as any combination of the three. If you use inheritance, you will 
create a "class explosion" (PlainTextView, BoldTextView, BoldItalicTextView, 
etc.). With just 3 features, you need 7 subclasses to cover every combination.

Solution:
Use the Decorator Pattern. Instead of extending the class through inheritance, 
wrap the original object inside "decorator" objects that add the new behavior. 
Decorators implement the same interface as the objects they wrap, meaning 
they can be stacked dynamically at runtime in any order.

=========================================================================
  Component Mapping
=========================================================================

  Component         → TextView (The common interface)
  ConcreteComponent → PlainTextView (The base object to be decorated)
  Decorator         → TextDecorator (Abstract base for all decorators)
  ConcreteDecorator → BoldDecorator, ItalicDecorator, UnderlineDecorator
  Client            → main() (Stacks decorators at runtime)

=========================================================================
*/

#include <iostream>
#include <string>

using namespace std;

/*
=========================================================================
  STEP 1: The Component Interface
=========================================================================
This is the shared contract. Both the base component and all decorators
must implement it. This ensures the client treats them identically.
*/
class TextView {
public:
  virtual void render() = 0;
  virtual ~TextView() = default;
};

/*
=========================================================================
  STEP 2: The Concrete Component
=========================================================================
This is the base object that provides the default behavior. Decorators 
will wrap around this object to add new features.
*/
class PlainTextView : public TextView {
private:
  string text;

public:
  PlainTextView(string text) : text(move(text)) {}

  void render() override { 
    cout << text; 
  }
};

/*
=========================================================================
  STEP 3: The Base Decorator
=========================================================================
This abstract class implements the component interface AND holds a reference 
to another component (the "inner" or "wrappee" object). 
It serves as the bridge for the delegation pattern.
*/
class TextDecorator : public TextView {
protected:
  // The object being wrapped (could be a PlainTextView OR another Decorator)
  TextView *inner;

public:
  TextDecorator(TextView *inner) : inner(inner) {}
};

/*
=========================================================================
  STEP 4: Concrete Decorators
=========================================================================
Each decorator adds one specific formatting layer. It prints its opening tag, 
delegates the rest of the rendering to the wrapped component, then prints 
its closing tag.
*/

class BoldDecorator : public TextDecorator {
public:
  BoldDecorator(TextView *inner) : TextDecorator(inner) {}

  void render() override {
    cout << "<b>";
    inner->render(); // Delegate inward
    cout << "</b>";
  }
};

class ItalicDecorator : public TextDecorator {
public:
  ItalicDecorator(TextView *inner) : TextDecorator(inner) {}

  void render() override {
    cout << "<i>";
    inner->render(); // Delegate inward
    cout << "</i>";
  }
};

class UnderlineDecorator : public TextDecorator {
public:
  UnderlineDecorator(TextView *inner) : TextDecorator(inner) {}

  void render() override {
    cout << "<u>";
    inner->render(); // Delegate inward
    cout << "</u>";
  }
};

/*
=========================================================================
  STEP 5: Client Code
=========================================================================
The client creates the base component and wraps it with any combination of
decorators at runtime. 

Notice: No subclass explosion. We achieve any combination using just 3 
concrete decorator classes.
*/
int main() {
  cout << "=== Decorator Pattern: Text Rendering ===" << endl << endl;

  // 1. Base Component
  PlainTextView text("Hello, World!");
  cout << "Plain:                   ";
  text.render();
  cout << endl;

  // 2. Single Decorator (Bold)
  BoldDecorator boldText(&text);
  cout << "Bold:                    ";
  boldText.render();
  cout << endl;

  // 3. Two Decorators (Italic + Underline)
  ItalicDecorator italicText(&text);
  UnderlineDecorator italicUnderline(&italicText);
  cout << "Italic + Underline:      ";
  italicUnderline.render();
  cout << endl;

  // 4. Three Decorators (Bold + Italic + Underline)
  BoldDecorator boldLayer(&text);
  ItalicDecorator italicLayer(&boldLayer);
  UnderlineDecorator allStyles(&italicLayer);
  cout << "Bold + Italic + Underline: ";
  allStyles.render();
  cout << endl << endl;

  return 0;
}