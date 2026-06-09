/*
===============================================================================
                    NOTIFICATION SERVICE
                    IMPLEMENTATION: SIMPLE FACTORY + STRATEGY
===============================================================================
# Resources:
  - https://algomaster.io/learn/lld/factory-method

===============================================================================
PROBLEM STATEMENT
===============================================================================

We need to build a Notification Service that can send notifications through
multiple channels:

1. Email
2. SMS
3. Push Notification

A naive implementation might look like:

    if(type == "EMAIL")
        sender = new EmailSender();
    else if(type == "SMS")
        sender = new SmsSender();
    else if(type == "PUSH")
        sender = new PushSender();

This creates two major problems:

1. Tight Coupling
   - Client code must know every concrete notification class.
   - Adding WhatsAppNotification requires modifying existing code.

2. Violates Open/Closed Principle
   - Every new notification type forces changes in client code.

===============================================================================
WHY FACTORY METHOD?
===============================================================================

Factory Method solves the OBJECT CREATION problem.

Instead of clients deciding:

    "Which concrete notification object should I create?"

we delegate that responsibility to a Factory.

Benefits:
- Encapsulates object creation.
- Client depends on abstractions, not concrete classes.
- Easier to extend with new notification channels.

===============================================================================
WHY STRATEGY PATTERN AS WELL?
===============================================================================

Factory creates the correct notification object.

Strategy executes the correct notification behavior.

Factory answers:
    "What object should I create?"

Strategy answers:
    "How should the notification be sent?"

Real-world systems typically use BOTH patterns together.

===============================================================================
DESIGN
===============================================================================

                        NotificationFactory
                                 |
                                 |
                 --------------------------------
                 |              |               |
                 v              v               v
          EmailSender      SmsSender      PushSender
                 ^              ^               ^
                 |              |               |
                 --------------------------------
                                 |
                                 |
                        NotificationStrategy
                                 |
                                 |
                                 v
                       NotificationService

===============================================================================
*/

#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
===============================================================================
STRATEGY INTERFACE
===============================================================================

Defines the contract for all notification channels.

Client code will depend on this abstraction rather than concrete classes.
*/
class NotificationStrategy {
public:
  virtual void send(const string &message) = 0;
  virtual ~NotificationStrategy() = default;
};

/*
===============================================================================
CONCRETE STRATEGIES
===============================================================================
*/

class EmailNotification : public NotificationStrategy {
public:
  void send(const string &message) override {
    cout << "[EMAIL] Sending: " << message << endl;
  }
};

class SmsNotification : public NotificationStrategy {
public:
  void send(const string &message) override {
    cout << "[SMS] Sending: " << message << endl;
  }
};

class PushNotification : public NotificationStrategy {
public:
  void send(const string &message) override {
    cout << "[PUSH] Sending: " << message << endl;
  }
};

/*
===============================================================================
FACTORY METHOD
===============================================================================

Responsibility:
    Create the correct notification strategy.

Why?
    Object creation logic should not be scattered throughout the codebase.

When a new channel arrives (e.g. WhatsAppNotification),
we only extend the factory rather than modifying client code.
*/
class NotificationFactory {
public:
  static unique_ptr<NotificationStrategy>
  createNotificationChannel(const string &channel) {
    if (channel == "EMAIL")
      return make_unique<EmailNotification>();

    if (channel == "SMS")
      return make_unique<SmsNotification>();

    if (channel == "PUSH")
      return make_unique<PushNotification>();

    return nullptr;
  }
};

/*
===============================================================================
CONTEXT CLASS (STRATEGY PATTERN)
===============================================================================

NotificationService does not know HOW notifications are sent.

It simply delegates the responsibility to the chosen strategy.

This follows:
    "Program to an interface, not an implementation."
*/
class NotificationService {
private:
  unique_ptr<NotificationStrategy> strategy;

public:
  explicit NotificationService(unique_ptr<NotificationStrategy> strategy)
      : strategy(move(strategy)) {}

  void notify(const string &message) {
    if (!strategy) {
      cout << "Invalid notification channel." << endl;
      return;
    }

    strategy->send(message);
  }
};

/*
===============================================================================
DRIVER CODE
===============================================================================

Flow:

1. Factory decides which object to create.
2. Service receives strategy abstraction.
3. Service delegates notification delivery to strategy.

Client never directly creates EmailNotification,
SmsNotification, or PushNotification.
*/
int main() {
  auto emailChannel = NotificationFactory::createNotificationChannel("EMAIL");

  NotificationService emailService(move(emailChannel));

  emailService.notify("Welcome to our platform!");

  cout << endl;

  auto smsChannel = NotificationFactory::createNotificationChannel("SMS");

  NotificationService smsService(move(smsChannel));

  smsService.notify("Your OTP is 123456");

  cout << endl;

  auto pushChannel = NotificationFactory::createNotificationChannel("PUSH");

  NotificationService pushService(move(pushChannel));

  pushService.notify("You have a new friend request.");

  return 0;
}

/*
===============================================================================
INTERVIEW NOTES
===============================================================================

Patterns Used:
- Strategy
- Simple Factory

Factory Responsibility:
- Create correct notification strategy

Strategy Responsibility:
- Execute correct notification behavior

Advantages:
- Low coupling
- Easy to maintain
- Production-friendly
- Simple architecture

This is NOT the formal GoF Factory Method Pattern.

This is a Simple Factory implementation, which is more common in
real-world business applications.
===============================================================================
*/

/*
===============================================================================
INTERVIEW EXPLANATION
===============================================================================

Why Factory Method?
-------------------
Factory Method encapsulates object creation and removes the need for clients
to instantiate concrete notification classes directly.

Why Strategy Pattern?
---------------------
Each notification channel has a different sending algorithm.
Strategy allows us to swap notification behavior at runtime.

Why use both together?
----------------------
Factory solves object creation.
Strategy solves behavioral variation.

Factory -> Creates the correct strategy.
Strategy -> Executes the correct notification logic.

This combination is commonly used in production-grade systems because it
maintains low coupling, high extensibility, and follows SOLID principles.
===============================================================================
*/