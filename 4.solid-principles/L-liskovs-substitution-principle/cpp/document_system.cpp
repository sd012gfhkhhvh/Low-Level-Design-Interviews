/*

# LSP: If a class S extends or implements class T, then you should be
able to use S anywhere T is expected, without breaking the program's behavior or
logic.

Subtypes must honor the expectations set by their base types. The client code
should not need to know or care which specific subtype it is dealing with.
Everything should "just work."

In ither term: A child class should always extend/maintain the capabilities of
its parent class. It should never downsize its parent's capabilities.
*/

/*
============================================
The Problem: A Document System Gone Wrong
===========================================
Imagine you are building a system to manage different types of documents. You
start with a simple base class.

Now, a new requirement comes in: "We need a read-only document type for
sensitive content like government reports or signed contracts."

You think: a ReadOnlyDocument is still a kind of Document, so inheritance makes
sense. You extend the Document class and override save() to block writes.

Seems reasonable, right?

But Then Reality Hits…

Let's see how this plays out in client code. A DocumentProcessor class takes any
Document and tries to process and save it.

The client code expected any Document to be savable. But when it received a
ReadOnlyDocument, that assumption exploded into a runtime exception.

What Went Wrong?
At the heart of this failure is a violation of a fundamental design principle:
the Liskov Substitution Principle. Our subtype (ReadOnlyDocument) cannot be
seamlessly substituted for its base type (Document) without altering the desired
behavior of the program.

If you ever find yourself overriding a method just to throw an exception, or
adding subtype-specific conditions in client code, you are likely violating LSP.

==========================
Solution : apply LSP
==========================
The root problem was that the base class Document assumed all documents are
editable, but not all documents should be. To fix this, we need to:

- Separate editable behavior from read-only behavior, create two interfaces
Document(read-only) & Editable(editable) extends Document

- Use interfaces or abstract types to model capabilities explicitly,
Implement two concrete types. The EditableDocument implements
Editable (which already includes everything from Document), while the
ReadOnlyDocument implements only Document.
*/

#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

// ==========================
// Before applying LSP
// ==========================
// class Document {
// protected:
//   string data;

// public:
//   Document(const string &data) : data(data) {}

//   virtual void open() const {
//     cout << "Document opened. Data: "
//          << data.substr(0, min((size_t)20, data.length())) << "..." << endl;
//   }

//   virtual void save(const string &newData) {
//     data = newData;
//     cout << "Document saved." << endl;
//   }

//   string getData() const { return data; }

//   virtual ~Document() = default;
// };

// class ReadOnlyDocument : public Document {
// public:
//   ReadOnlyDocument(const string &data) : Document(data) {}

//   void save(const string & /*newData*/) override {
//     throw runtime_error("Cannot save a read-only document!");
//   }
// };

// class DocumentProcessor {
// public:
//   void processAndSave(Document *doc, const string &additionalInfo) {
//     doc->open();
//     string currentData = doc->getData();
//     string newData = currentData + " | Processed: " + additionalInfo;
//     doc->save(newData); // Assumes all Documents are savable
//     cout << "Document processing complete." << endl;
//   }
// };

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
class Document {
public:
  virtual void open() const = 0;
  virtual string getData() const = 0;
  virtual ~Document() = default;
};

class Editable : public Document {
public:
  virtual void save(const string &newData) = 0;
};

/*
=========================================================
                CONCRETE Inplementations
=========================================================
*/

class EditableDocument : public Editable {
private:
  string data;

  string preview() const {
    return data.substr(0, min((size_t)20, data.length())) + "...";
  }

public:
  EditableDocument(const string &data) : data(data) {}

  void open() const override {
    cout << "Editable Document opened. Data: " << preview() << endl;
  }

  void save(const string &newData) override {
    data = newData;
    cout << "Document saved." << endl;
  }

  string getData() const override { return data; }
};

class ReadOnlyDocument : public Document {
private:
  string data;

  string preview() const {
    return data.substr(0, min((size_t)20, data.length())) + "...";
  }

public:
  ReadOnlyDocument(const string &data) : data(data) {}

  void open() const override {
    cout << "Read-Only Document opened. Data: " << preview() << endl;
  }

  string getData() const override { return data; }
};

class DocumentProcessor {
public:
  void process(const Document *doc) const {
    doc->open();
    cout << "Document processed." << endl;
  }

  void processAndSave(Editable &doc, const string &additionalInfo) {
    doc.open();
    string currentData = doc.getData();
    string newData = currentData + " | Processed: " + additionalInfo;
    doc.save(newData);
    cout << "Editable document processed and saved." << endl;
  }
};

int main() {
  /*
  =======================
  Before LSP
  =======================
  */

  //   Document *regularDoc = new Document("Initial project proposal content.");
  //   Document *confidentialReport =
  //       new ReadOnlyDocument("Top secret government data.");

  //   DocumentProcessor processor;

  //   cout << "--- Processing Regular Document ---" << endl;
  //   processor.processAndSave(regularDoc, "Reviewed by Alice");

  //   cout << "\n--- Processing ReadOnly Document ---" << endl;
  //   try {
  //     processor.processAndSave(confidentialReport, "Reviewed by Bob");
  //   } catch (const exception &e) {
  //     cerr << "Error: " << e.what() << endl;
  //   }

  //   delete regularDoc;
  //   delete confidentialReport;

  /*
  =======================
  After LSP
  =======================
  */

  EditableDocument editable("Draft proposal for Q3.");
  ReadOnlyDocument readOnly("Top secret strategy.");
  DocumentProcessor processor;

  cout << "--- Processing Editable Document ---" << endl;
  processor.processAndSave(editable, "Reviewed by Alice");

  cout << "\n--- Processing Read-Only Document ---" << endl;
  processor.process(&readOnly); // Works fine

  //   processor.processAndSave(readOnly, "Reviewed by Bob");
  // Won't compile! ReadOnlyDocument doesn't have save().

  return 0;
}