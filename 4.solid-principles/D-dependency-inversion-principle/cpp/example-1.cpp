/*
=========================================
# The Problem: A Tightly Coupled modules
=========================================
When a high-level component(business logic) is tightly coupled(directly
dependent) with the low-level module (implementation details of some provider
api like gmail, outlook etc) and there is a requrement to change the provider
(say from gmail to outlook) then the entire business logic is effected and need
to change significatly to make it work. This is a pain for a developer as almost
half of the high-level class need to changed.

=====================
# The Solution: DIP
=====================
The dependency-inversion-principle (DIP) exactly solves this tight-coupling
issue.

DIP has two golden rules:
- High-level modules should not depend on low-level modules. Both should depend
on abstractions (e.g., interfaces).
- Abstractions should not depend on details. Details (concrete implementations)
should depend on abstractions.

In plain English:

- Business logic should not rely directly on implementation details.
- Instead, both should depend on a common interface or abstraction.

You might wonder what exactly is being "inverted." It is the direction of
dependency. Without DIP, high-level modules depend directly on low-level
modules. With DIP, both the high-level module and the low-level module depend on
a shared abstraction (an interface or abstract class).

The control flow might still go from high to low, but the source code dependency
is inverted. High-level modules define what they need (the contract/interface),
and low-level modules provide the how (the implementation of that interface).
*/

#include <iostream>
#include <memory>
using namespace std;

/*
=========================
Before DIP
=========================
*/

/*
Imagine you are building an EmailService. Your first task is to send emails
using Gmail. So you write something like this.

Here is the low-level module, a GmailClient that knows how to talk to Gmail's
servers.
*/
class GmailClient {
public:
  void sendGmail(const string &toAddress, const string &subjectLine,
                 const string &emailBody) {
    cout << "Connecting to Gmail SMTP server..." << endl;
    cout << "Sending email via Gmail to: " << toAddress << endl;
    cout << "Subject: " << subjectLine << endl;
    cout << "Body: " << emailBody << endl;
    // ... actual Gmail API interaction logic ...
    cout << "Gmail email sent successfully!" << endl;
  }
};

/*
And here is the high-level module, the EmailService that handles business logic
like sending welcome emails and password resets.
*/
class EmailService {
private:
  GmailClient gmailClient;

public:
  void sendWelcomeEmail(const string &userEmail, const string &userName) {
    string subject = "Welcome, " + userName + "!";
    string body = "Thanks for signing up to our awesome platform. We're glad "
                  "to have you!";
    gmailClient.sendGmail(userEmail, subject, body);
  }

  void sendPasswordResetEmail(const string &userEmail) {
    string subject = "Reset Your Password";
    string body = "Please click the link below to reset your password...";
    gmailClient.sendGmail(userEmail, subject, body);
  }
};

/*
Now if you want to change the provider from gmail to outlook.

Suddenly, you have a problem. Your EmailService, a high-level component that
handles business logic, is tightly coupled to GmailClient, a low-level
implementation detail. To switch providers, you would have to:

- Rewrite parts of EmailService
- Replace every gmailClient method call with outlookClient ones
- Change the constructorsss

And that is just for one provider swap. Now imagine needing to support multiple
email providers (Gmail, Outlook, SES) or dynamically select a provider based on
configuration. Your EmailService would quickly turn into a giant if-else soup.

This is exactly the kind of pain the Dependency Inversion Principle (DIP) helps
you avoid.
*/

/*
====================
After DIP
====================

Create an EmailCLient interface and both GmailClient, OutlookClient and the
high-level module EmailService depend on the EmailClient interface.

EmailService is now shielded from implementation details, and you can swap
providers without touching business logic.
*/

/*
=========================================================
                     INTERFACE
=========================================================
*/
class EmailClient {
public:
  virtual void sendEmail(const string &to, const string &subject,
                         const string &body) = 0;
  virtual ~EmailClient() = default;
};

/*
=========================================================
                CONCRETE Inplementations
=========================================================
*/

class GmailClientImpl : public EmailClient {
public:
  void sendEmail(const string &to, const string &subject,
                 const string &body) override {
    cout << "Connecting to Gmail SMTP server..." << endl;
    cout << "Sending email via Gmail to: " << to << endl;
    cout << "Subject: " << subject << endl;
    cout << "Body: " << body << endl;
    cout << "Gmail email sent successfully!" << endl;
  }
};

class OutlookClientImpl : public EmailClient {
public:
  void sendEmail(const string &to, const string &subject,
                 const string &body) override {
    cout << "Connecting to Outlook Exchange server..." << endl;
    cout << "Sending email via Outlook to: " << to << endl;
    cout << "Subject: " << subject << endl;
    cout << "Body: " << body << endl;
    cout << "Outlook email sent successfully!" << endl;
  }
};

class EmailService_DIP {
private:
  shared_ptr<EmailClient> emailClient;

public:
  EmailService_DIP(shared_ptr<EmailClient> client)
      : emailClient(move(client)) {}

  void sendWelcomeEmail(const string &userEmail, const string &userName) {
    string subject = "Welcome, " + userName + "!";
    string body = "Thanks for signing up to our awesome platform. We're glad "
                  "to have you!";
    emailClient->sendEmail(userEmail, subject, body);
  }

  void sendPasswordResetEmail(const string &userEmail) {
    string subject = "Reset Your Password";
    string body = "Please click the link below to reset your password...";
    emailClient->sendEmail(userEmail, subject, body);
  }
};

/*
=========================================================
                Driver Code
=========================================================
*/

int main() {
  // Before DIP
  cout << "--- Before DIP ---" << endl;
  EmailService email;
  email.sendWelcomeEmail("test@example.com", "Alice");

  cout << "\n --- After DIP --- \n" << endl;

  cout << "--- Using Gmail ---" << endl;
  shared_ptr<EmailClient> gmail = make_shared<GmailClientImpl>();
  EmailService_DIP gmailService(gmail);
  gmailService.sendWelcomeEmail("test@example.com", "Alice");

  cout << "\n--- Using Outlook ---" << endl;
  shared_ptr<EmailClient> outlook = make_shared<OutlookClientImpl>();
  EmailService_DIP outlookService(outlook);
  outlookService.sendWelcomeEmail("test@example.com", "Alice");

  return 0;
}