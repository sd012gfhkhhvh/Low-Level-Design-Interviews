/*
===================================
Exercise: Bulk Email Template Clone
===================================

Problem:
You are building a bulk email system for your company. The company sends a
monthly newsletter, but each department needs a slightly different version
with a customized subject line and department-specific recipients.

Currently, duplicating the full constructor call for every department email
copies all the shared fields each time, which is inefficient and error-prone.

Your goal is to define a base template once and clone it for each variant using
the Prototype Design Pattern. However, there is a catch: `RecipientList` is a
nested mutable object containing two lists: `to` and `cc`. A shallow clone
would cause all email templates to share the same recipient lists. If you add
a recipient to the marketing email, it would accidentally add them to every
other department's email and the base template!

Requirements:
1. Implement a `clone()` method in the `EmailTemplate` class using the
   Prototype Design Pattern.
2. Ensure that cloning an `EmailTemplate` performs a DEEP COPY of the
   `RecipientList` so that modifying a cloned template's recipients does
   not affect the original template or other clones.
3. Your driver code should demonstrate creating a base template, then cloning
   and modifying it for HR, Marketing, and Engineering.
   - The HR email should have an extra recipient and a CC.
   - The Marketing and Engineering emails should each have their own added recipients.
   - The base template MUST remain completely unaffected.
*/

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

/*
=========================================================
           Nested Mutable Object: RecipientList
=========================================================
This is the object that makes deep copying essential. It holds two mutable
lists (to and cc). If cloned shallowly, all email templates would share the
same lists, and adding a recipient to one would affect all others.
*/
class RecipientList {
public:
  vector<string> to;
  vector<string> cc;

  RecipientList(vector<string> to, vector<string> cc)
      : to(move(to)), cc(move(cc)) {}

  // Creates an independent copy of both lists
  RecipientList deepCopy() const { return RecipientList(to, cc); }

  void addTo(const string &email) { to.push_back(email); }
  void addCc(const string &email) { cc.push_back(email); }
};

/*
=========================================================
                 Prototype (Interface)
=========================================================
Declares the clone() method that all cloneable email templates must implement.
Defines the contract for self-cloning and allows clients to clone objects
without knowing their concrete class.
*/
class EmailPrototype {
public:
  virtual EmailPrototype *clone() const = 0;
  virtual ~EmailPrototype() = default;
};

/*
=========================================================
            ConcretePrototype Implementation
=========================================================
Implements the clone() method to produce a deep copy of itself.
The key detail: clone() calls recipients.deepCopy() to ensure each
clone gets its own independent RecipientList.
*/
class EmailTemplate : public EmailPrototype {
private:
  string subject;
  string body;
  RecipientList recipients;

public:
  EmailTemplate(string subject, string body, RecipientList recipients)
      : subject(move(subject)), body(move(body)),
        recipients(move(recipients)) {}

  // Deep clone: creates a new EmailTemplate with an independent RecipientList
  EmailTemplate *clone() const override {
    return new EmailTemplate(subject, body, recipients.deepCopy());
  }

  void setSubject(const string &s) { subject = s; }
  RecipientList &getRecipients() { return recipients; }

  void print() const {
    cout << "Email: " << subject << " | Recipients: {to=[";
    for (size_t i = 0; i < recipients.to.size(); i++) {
      if (i > 0)
        cout << ", ";
      cout << recipients.to[i];
    }
    cout << "], cc=[";
    for (size_t i = 0; i < recipients.cc.size(); i++) {
      if (i > 0)
        cout << ", ";
      cout << recipients.cc[i];
    }
    cout << "]}" << endl;
  }
};

/*
=========================================================
             Prototype Registry (Optional)
=========================================================
Stores a collection of pre-configured email template prototypes, indexed by a
department key (string). Returns clones (not originals) when clients request
templates by key.
*/
class EmailRegistry {
private:
  map<string, unique_ptr<EmailTemplate>> prototypes;

public:
  void registerPrototype(const string &key,
                         unique_ptr<EmailTemplate> prototype) {
    prototypes[key] = move(prototype);
  }

  unique_ptr<EmailTemplate> get(const string &key) {
    auto it = prototypes.find(key);
    if (it == prototypes.end()) {
      throw invalid_argument("No email template registered for: " + key);
    }
    return unique_ptr<EmailTemplate>(it->second->clone());
  }
};

/*
=========================================================
                     Client Code
=========================================================
Creates new email templates by asking the registry for clones.
Customizes each clone after creation (subject, recipients) without
affecting the originals stored in the registry.
*/
int main() {
  EmailRegistry registry;

  // Register the base newsletter template as the prototype
  registry.registerPrototype(
      "newsletter",
      make_unique<EmailTemplate>(
          "Company Newsletter", "Monthly updates from the team...",
          RecipientList({"all@company.com"}, {"archive@company.com"})));

  // Clone and customize for Marketing
  auto marketingEmail = registry.get("newsletter");
  marketingEmail->setSubject("Marketing Newsletter");
  marketingEmail->getRecipients().addTo("marketing@company.com");

  // Clone and customize for Engineering
  auto engineeringEmail = registry.get("newsletter");
  engineeringEmail->setSubject("Engineering Newsletter");
  engineeringEmail->getRecipients().addTo("eng-team@company.com");

  // Clone and customize for HR (extra recipient + CC)
  auto hrEmail = registry.get("newsletter");
  hrEmail->setSubject("HR Newsletter");
  hrEmail->getRecipients().addTo("hr@company.com");
  hrEmail->getRecipients().addCc("ceo@company.com");

  // Print all cloned variants
  marketingEmail->print();
  engineeringEmail->print();
  hrEmail->print();

  // Prove the base template in the registry is unaffected
  cout << "\nBase template unchanged:" << endl;
  auto baseCheck = registry.get("newsletter");
  baseCheck->print();

  return 0;
}