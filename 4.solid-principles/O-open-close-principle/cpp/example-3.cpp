/*
Exercise 2: NotificationService
Refactor NotificationService Class
Problem: A NotificationService has hardcoded notification channels (Email and
SMS). Every time the business wants to add a new channel (Push, Slack,
WhatsApp), the service must be modified. Refactor it to make it extensible so
that new channels can be added without modifying existing code.

Requirements:

Define a NotificationChannel interface with a send(message) method
Create concrete implementations: EmailChannel, SMSChannel, PushChannel,
SlackChannel Refactor NotificationService to accept one or more
NotificationChannel instances The service should delegate sending to the channel
without knowing its type
*/

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// =============
// Before OCP
// =============
// class NotificationService {
// public:
//     void sendNotification(const string& channel, const string& message) {
//         if (channel == "Email") {
//             cout << "Sending EMAIL: " << message << endl;
//             // Email-specific logic
//         } else if (channel == "SMS") {
//             cout << "Sending SMS: " << message << endl;
//             // SMS-specific logic
//         } else {
//             throw invalid_argument("Unknown channel: " + channel);
//         }
//     }
// };

// ===================================
// After OCP
// ===================================

/*
=========================================================
                    STRATEGY INTERFACE
=========================================================
*/

class NotificationChannel {
public:
  virtual void send(const string &message) const = 0;
  virtual ~NotificationChannel() = default;
};

/*
=========================================================
                CONCRETE CHANNELS
=========================================================
*/

class EmailChannel : public NotificationChannel {
public:
  void send(const string &message) const override {
    cout << "Sending EMAIL : " << message << endl;
  }
};

class SMSChannel : public NotificationChannel {
public:
  void send(const string &message) const override {
    cout << "Sending SMS   : " << message << endl;
  }
};

class PushChannel : public NotificationChannel {
public:
  void send(const string &message) const override {
    cout << "Sending PUSH  : " << message << endl;
  }
};

class SlackChannel : public NotificationChannel {
public:
  void send(const string &message) const override {
    cout << "Sending SLACK : " << message << endl;
  }
};

/*
=========================================================
IMPLEMENTATION 1
RAW POINTERS (BORROWING)

Meaning:
- NotificationService does NOT own channels.
- Channels must outlive NotificationService.

Use when:
- Channels are created elsewhere.
- Multiple services may use same channels.

Pros:
- No ownership transfer.
- No heap allocation required.

Cons:
- Lifetime must be managed manually.
=========================================================
*/

class NotificationServiceRaw {
private:
  vector<NotificationChannel *> channels;

public:
  explicit NotificationServiceRaw(const vector<NotificationChannel *> &channels)
      : channels(channels) {}

  void sendNotification(const string &message) const {
    for (const auto *channel : channels) {
      channel->send(message);
    }
  }
};

/*
=========================================================
IMPLEMENTATION 2
UNIQUE_PTR OWNERSHIP

Meaning:
- NotificationService OWNS channels.
- Lifetime managed automatically.

Use when:
- Service is responsible for channels.
- Channels come from factories/configuration.

Pros:
- Clear ownership.
- Automatic cleanup.
- No dangling pointers.

Cons:
- Ownership transfer required.
=========================================================
*/

class NotificationServiceOwned {
private:
  vector<unique_ptr<NotificationChannel>> channels;

public:
  explicit NotificationServiceOwned(
      vector<unique_ptr<NotificationChannel>> channels)
      : channels(move(channels)) {}

  void sendNotification(const string &message) const {
    for (const auto &channel : channels) {
      channel->send(message);
    }
  }
};

/*
=========================================================
DRIVER CODE
=========================================================
*/

int main() {

  cout << "========== RAW POINTER VERSION ==========\n\n";

  EmailChannel email;
  SMSChannel sms;
  PushChannel push;
  SlackChannel slack;

  vector<NotificationChannel *> borrowedChannels;

  borrowedChannels.push_back(&email);
  borrowedChannels.push_back(&sms);
  borrowedChannels.push_back(&push);
  borrowedChannels.push_back(&slack);

  NotificationServiceRaw rawService(borrowedChannels);

  rawService.sendNotification("Your order has shipped!");

  cout << "\n\n========== UNIQUE_PTR VERSION(BEST for the example) "
          "==========\n\n";

  vector<unique_ptr<NotificationChannel>> ownedChannels;

  ownedChannels.push_back(make_unique<EmailChannel>());

  ownedChannels.push_back(make_unique<SMSChannel>());

  ownedChannels.push_back(make_unique<PushChannel>());

  ownedChannels.push_back(make_unique<SlackChannel>());

  NotificationServiceOwned ownedService(move(ownedChannels));

  ownedService.sendNotification("Your payment was successful!");

  return 0;
}

/*
[Read the README](./raw_vs_unuque_ptr_ownership.md)
*/