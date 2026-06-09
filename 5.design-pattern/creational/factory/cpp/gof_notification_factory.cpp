/*
===============================================================================
                NOTIFICATION SERVICE
                IMPLEMENTATION: GOF FACTORY METHOD + STRATEGY
===============================================================================
# Resources:
  - https://algomaster.io/learn/lld/factory-method

PROBLEM
-------------------------------------------------------------------------------
We need to support multiple notification channels while avoiding direct
instantiation of concrete notification classes.

Unlike Simple Factory, we want creation responsibility delegated to
specialized creator subclasses.

-------------------------------------------------------------------------------
DESIGN DECISION
-------------------------------------------------------------------------------

Patterns Used:

1. Strategy Pattern
   Encapsulates notification delivery logic.

2. Factory Method Pattern (GoF)
   Delegates object creation to subclasses.

Key Difference:

Simple Factory:
    NotificationFactory::create("EMAIL")

GoF Factory Method:
    EmailCreator::createNotification()

Creation is achieved through inheritance rather than a centralized
conditional block.

===============================================================================
*/

#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
===============================================================================
PRODUCT HIERARCHY (STRATEGY)
===============================================================================
*/

class Notification {
public:
  virtual void send(const string &message) = 0;
  virtual ~Notification() = default;
};

class EmailNotification : public Notification {
public:
  void send(const string &message) override {
    cout << "[EMAIL] " << message << endl;
  }
};

class SmsNotification : public Notification {
public:
  void send(const string &message) override {
    cout << "[SMS] " << message << endl;
  }
};

class PushNotification : public Notification {
public:
  void send(const string &message) override {
    cout << "[PUSH] " << message << endl;
  }
};

/*
===============================================================================
CREATOR

Factory Method lives here.

Subclasses decide which concrete product gets created.
===============================================================================
*/
class NotificationCreator {
public:
  virtual unique_ptr<Notification> createNotification() = 0;

  virtual ~NotificationCreator() = default;
};

/*
===============================================================================
CONCRETE CREATORS
===============================================================================
*/

class EmailNotificationCreator : public NotificationCreator {
public:
  unique_ptr<Notification> createNotification() override {
    return make_unique<EmailNotification>();
  }
};

class SmsNotificationCreator : public NotificationCreator {
public:
  unique_ptr<Notification> createNotification() override {
    return make_unique<SmsNotification>();
  }
};

class PushNotificationCreator : public NotificationCreator {
public:
  unique_ptr<Notification> createNotification() override {
    return make_unique<PushNotification>();
  }
};

/*
===============================================================================
SERVICE

Depends only on abstractions.

Does not know which creator or notification implementation is used.
===============================================================================
*/
class NotificationService {
private:
  unique_ptr<Notification> notification;

public:
  explicit NotificationService(unique_ptr<Notification> notification)
      : notification(move(notification)) {}

  void notify(const string &message) { notification->send(message); }
};

/*
===============================================================================
DRIVER
===============================================================================
*/
int main() {
  unique_ptr<NotificationCreator> emailCreator =
      make_unique<EmailNotificationCreator>();

  auto emailNotification = emailCreator->createNotification();

  NotificationService emailService(move(emailNotification));

  emailService.notify("Welcome User!");

  cout << endl;

  unique_ptr<NotificationCreator> smsCreator =
      make_unique<SmsNotificationCreator>();

  auto smsNotification = smsCreator->createNotification();

  NotificationService smsService(move(smsNotification));

  smsService.notify("Your OTP is 123456");

  cout << endl;

  unique_ptr<NotificationCreator> pushCreator =
      make_unique<PushNotificationCreator>();

  auto pushNotification = pushCreator->createNotification();

  NotificationService pushService(move(pushNotification));

  pushService.notify("You have a new message.");

  return 0;
}

/*
===============================================================================
INTERVIEW NOTES
===============================================================================

Patterns Used:
- Strategy
- Factory Method (GoF)

Creator Hierarchy:
    NotificationCreator
        |
        +---- EmailNotificationCreator
        +---- SmsNotificationCreator
        +---- PushNotificationCreator

Advantages:
- Follows formal GoF definition
- Creation delegated to subclasses
- Extensible through inheritance

Tradeoff:
- More classes
- More boilerplate
- Often overkill for simple creation logic

Use When:
- Interview explicitly asks for Factory Method
- Different creators have distinct creation workflows

Most production systems would prefer the Simple Factory version because
it is easier to maintain and understand.
===============================================================================
*/