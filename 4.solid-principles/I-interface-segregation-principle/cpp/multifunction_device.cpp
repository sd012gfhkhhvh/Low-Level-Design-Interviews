/*
================================================
          Fat MultiFunctionDevice
================================================
Refactor MultiFunctionDevice

Problem: You have a MultiFunctionDevice interface with print(), scan(), fax(),
and staple() methods. A BasicPrinter only prints. Refactor into separate
Printable, Scannable, and Faxable interfaces so that BasicPrinter only
implements the capabilities it actually supports.

Requirements:

Create four focused interfaces: Printable, Scannable, Faxable, and Stapleable
BasicPrinter implements only Printable
OfficePrinter implements Printable, Scannable, and Faxable
FullDevice implements all four interfaces
No class should have empty methods or throw UnsupportedOperationException
*/

#include <iostream>
#include <string>

using namespace std;

// ====================
// Before ISP
// ====================
// Before: Fat interface forces BasicPrinter to implement everything
// class MultiFunctionDevice {
// public:
//   virtual void print(const string &document) = 0;
//   virtual void scan(const string &document) = 0;
//   virtual void fax(const string &document, const string &number) = 0;
//   virtual void staple(const string &document) = 0;
//   virtual ~MultiFunctionDevice() = default;
// };

// class BasicPrinter : public MultiFunctionDevice {
// public:
//   void print(const string &document) override {
//     cout << "Printing: " << document << endl;
//   }

//   void scan(const string & /*document*/) override {
//     throw runtime_error("BasicPrinter cannot scan.");
//   }

//   void fax(const string & /*document*/, const string & /*number*/) override {
//     throw runtime_error("BasicPrinter cannot fax.");
//   }

//   void staple(const string & /*document*/) override {
//     throw runtime_error("BasicPrinter cannot staple.");
//   }
// };

// ===========================
// After ISP
// ===========================

/*
=========================================================
                     INTERFACE
=========================================================
*/
class Printable {
public:
  virtual void print(const string &document) const = 0;
  virtual ~Printable() = default;
};

class Scannable {
public:
  virtual void scan(const string &document) const = 0;
  virtual ~Scannable() = default;
};

class Faxable {
public:
  virtual void fax(const string &document, const string &number) const = 0;
  virtual ~Faxable() = default;
};

class Stapleable {
public:
  virtual void staple(const string &document) const = 0;
  virtual ~Stapleable() = default;
};

/*
=========================================================
                CONCRETE Inplementations
=========================================================
*/

class BasicPrinter : public Printable {
public:
  void print(const string &document) const override {
    cout << "BasicPrinter -> " << "Printing: " << document << endl;
  }
};

class OfficePrinter : public Printable, public Scannable, public Faxable {
public:
  void print(const string &document) const override {
    cout << "OfficePrinter -> " << "Printing: " << document << endl;
  }

  void scan(const string &document) const override {
    cout << "OfficePrinter -> " << "Scanning: " << document << endl;
  }

  void fax(const string &document, const string &number) const override {
    cout << "OfficePrinter -> " << "Faxing: " << document << " to " << number
         << endl;
  }
};

class FullDevice : public Printable,
                   public Scannable,
                   public Faxable,
                   public Stapleable {
public:
  void print(const string &document) const override {
    cout << "FullDevice -> " << "Printing: " << document << endl;
  }

  void scan(const string &document) const override {
    cout << "FullDevice -> " << "Scanning: " << document << endl;
  }

  void fax(const string &document, const string &number) const override {
    cout << "FullDevice -> " << "Faxing: " << document << " to " << number
         << endl;
  }

  void staple(const string &document) const override {
    cout << "FullDevice -> " << "Stapling: " << document << endl;
  }
};

/*
=========================================================
                Driver Code
=========================================================
*/

int main() {
  // Before ISP
  //   BasicPrinter printer;
  //   printer.print("report.pdf");

  // After ISP
  BasicPrinter basicPrinter;
  basicPrinter.print("report.pdf");

  OfficePrinter officePrinter;
  officePrinter.print("memo.pdf");
  officePrinter.scan("memo.pdf");
  officePrinter.fax("memo.pdf", "555-1234");

  FullDevice fullDevice;
  fullDevice.print("contract.pdf");
  fullDevice.scan("contract.pdf");
  fullDevice.fax("contract.pdf", "555-5678");
  fullDevice.staple("contract.pdf");

  return 0;
}