/*
=========================================================================
  Observer Pattern — Practical Example: Fitness Tracker App
=========================================================================

Scenario:
You are building a Fitness Tracker App that connects to a wearable device.
The device continuously streams real-time fitness data (steps, active 
minutes, calories) to a central `FitnessData` object. Multiple modules 
(like a LiveDisplay, a ProgressLogger, and a GoalNotifier) need to react 
to these updates.

The Problem (The Naive Approach):
If `FitnessData` directly instantiates and calls these dependent modules,
it becomes tightly coupled to them. Every time you want to add a new 
feature (like a SocialShare module), you must modify the core `FitnessData` 
class. You also cannot dynamically subscribe or unsubscribe modules at runtime.

Solution:
Apply the Observer Pattern. Have `FitnessData` act as a "Subject" and the 
dependent modules act as "Observers". Observers register themselves with 
the Subject. When the state changes, the Subject iterates through its list 
of generic Observers and calls their `update()` method. The Subject doesn't 
need to know the concrete classes of its observers.

=========================================================================
  Component Mapping
=========================================================================

  Subject         → FitnessDataSubject (Interface for managing observers)
  ConcreteSubject → FitnessData (The core object that changes state)
  Observer        → FitnessDataObserver (Interface for receiving updates)
  ConcreteObserver→ LiveActivityDisplay, ProgressLogger, GoalNotifier
  Client          → main() (Registers observers to the subject)

=========================================================================
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/*
=========================================================================
  STEP 0: The Naive Approach (For Reference)
=========================================================================
Notice how FitnessDataNaive is tightly coupled to specific concrete classes.
If we want to add a new module, we MUST modify FitnessDataNaive.
*/

// Dummy classes to make the naive approach compile
class LiveActivityDisplayNaive {
public:
  void showStats(int steps, int activeMins, int calories) {
    cout << "Live Display → Steps: " << steps 
         << " | Active Minutes: " << activeMins 
         << " | Calories: " << calories << endl;
  }
};

class ProgressLoggerNaive {
public:
  void logDataPoint(int steps, int activeMins, int calories) {
    cout << "Logger → Saving to DB: Steps=" << steps 
         << ", ActiveMinutes=" << activeMins 
         << ", Calories=" << calories << endl;
  }
};

class NotificationServiceNaive {
private:
  bool goalReached = false;
public:
  void checkAndNotify(int steps) {
    if (steps >= 10000 && !goalReached) {
      cout << "Notifier → 🎉 Goal Reached! You've hit 10000 steps!" << endl;
      goalReached = true;
    }
  }
  void resetDailyNotifications() {
    goalReached = false;
    cout << "Notifier → Resetting goal state." << endl;
  }
};

class FitnessDataNaive {
private:
  int steps;
  int activeMinutes;
  int calories;

  // Direct, hardcoded references to all dependent modules (Tight Coupling!)
  LiveActivityDisplayNaive liveDisplay;
  ProgressLoggerNaive progressLogger;
  NotificationServiceNaive notificationService;

public:
  FitnessDataNaive() : steps(0), activeMinutes(0), calories(0) {}

  void newFitnessDataPushed(int newSteps, int newActiveMinutes, int newCalories) {
    steps = newSteps;
    activeMinutes = newActiveMinutes;
    calories = newCalories;

    cout << "\n[Naive] FitnessData: New data received - Steps: " << steps
         << ", ActiveMins: " << activeMinutes << ", Calories: " << calories
         << endl;

    // Manually notify each dependent module
    liveDisplay.showStats(steps, activeMinutes, calories);
    progressLogger.logDataPoint(steps, activeMinutes, calories);
    notificationService.checkAndNotify(steps);
  }

  void dailyReset() {
    notificationService.resetDailyNotifications();
    cout << "[Naive] FitnessData: Daily data reset." << endl;
    newFitnessDataPushed(0, 0, 0); 
  }
};

/*
=========================================================================
  STEP 1: Observer Interface
=========================================================================
All subscriber modules implement this interface.
*/
class FitnessData; // Forward declaration

class FitnessDataObserver {
public:
  virtual ~FitnessDataObserver() = default;
  virtual void update(FitnessData *data) = 0;
};

/*
=========================================================================
  STEP 2: Subject Interface
=========================================================================
Declares methods for registering, removing, and notifying observers.
*/
class FitnessDataSubject {
public:
  virtual ~FitnessDataSubject() = default;
  virtual void registerObserver(FitnessDataObserver *observer) = 0;
  virtual void removeObserver(FitnessDataObserver *observer) = 0;
  virtual void notifyObservers() = 0;
};

/*
=========================================================================
  STEP 3: Concrete Subject
=========================================================================
Holds the state and a list of observers. Notifies them on state change.
*/
class FitnessData : public FitnessDataSubject {
private:
  int steps;
  int activeMinutes;
  int calories;
  vector<FitnessDataObserver *> observers;

public:
  FitnessData() : steps(0), activeMinutes(0), calories(0) {}

  void registerObserver(FitnessDataObserver *observer) override {
    observers.push_back(observer);
  }

  void removeObserver(FitnessDataObserver *observer) override {
    observers.erase(remove(observers.begin(), observers.end(), observer),
                    observers.end());
  }

  void notifyObservers() override {
    for (FitnessDataObserver *observer : observers) {
      observer->update(this); // Pass itself so observers can pull data
    }
  }

  void newFitnessDataPushed(int newSteps, int newActiveMinutes, int newCalories) {
    steps = newSteps;
    activeMinutes = newActiveMinutes;
    calories = newCalories;

    cout << "\n[Pattern] FitnessData: New data received – Steps: " << steps
         << ", Active Minutes: " << activeMinutes << ", Calories: " << calories
         << endl;

    // Automatically notify all dynamically registered observers
    notifyObservers();
  }

  void dailyReset() {
    steps = 0;
    activeMinutes = 0;
    calories = 0;

    cout << "\n[Pattern] FitnessData: Daily reset performed." << endl;
    notifyObservers();
  }

  // Getters for observers to pull data
  int getSteps() const { return steps; }
  int getActiveMinutes() const { return activeMinutes; }
  int getCalories() const { return calories; }
};

/*
=========================================================================
  STEP 4: Concrete Observers
=========================================================================
Implement specific reactions to state changes.
*/

class LiveActivityDisplay : public FitnessDataObserver {
public:
  void update(FitnessData *data) override {
    cout << "Live Display → Steps: " << data->getSteps()
         << " | Active Minutes: " << data->getActiveMinutes()
         << " | Calories: " << data->getCalories() << endl;
  }
};

class ProgressLogger : public FitnessDataObserver {
public:
  void update(FitnessData *data) override {
    cout << "Logger → Saving to DB: Steps=" << data->getSteps()
         << ", ActiveMinutes=" << data->getActiveMinutes()
         << ", Calories=" << data->getCalories() << endl;
  }
};

class GoalNotifier : public FitnessDataObserver {
private:
  int stepGoal;
  bool goalReached;

public:
  GoalNotifier() : stepGoal(10000), goalReached(false) {}

  void update(FitnessData *data) override {
    if (data->getSteps() >= stepGoal && !goalReached) {
      cout << "Notifier → 🎉 Goal Reached! You've hit " << stepGoal << " steps!" << endl;
      goalReached = true;
    }
  }

  void reset() { goalReached = false; }
};

/*
=========================================================================
  STEP 5: Client Code
=========================================================================
*/
int main() {
  cout << "=== NAIVE APPROACH ===" << endl;
  FitnessDataNaive fitnessDataNaive;
  fitnessDataNaive.newFitnessDataPushed(500, 5, 20);
  fitnessDataNaive.newFitnessDataPushed(9800, 85, 350);
  fitnessDataNaive.newFitnessDataPushed(10100, 90, 380);
  fitnessDataNaive.dailyReset();

  cout << "\n==========================================\n" << endl;

  cout << "=== OBSERVER PATTERN APPROACH ===" << endl;
  FitnessData fitnessData;

  // Instantiate observers
  LiveActivityDisplay display;
  ProgressLogger logger;
  GoalNotifier notifier;

  // Register observers dynamically at runtime
  fitnessData.registerObserver(&display);
  fitnessData.registerObserver(&logger);
  fitnessData.registerObserver(&notifier);

  // Simulate updates
  fitnessData.newFitnessDataPushed(500, 5, 20);
  fitnessData.newFitnessDataPushed(9800, 85, 350);
  fitnessData.newFitnessDataPushed(10100, 90, 380);

  // Demonstrate dynamic nature: remove logger and reset notifier
  cout << "\n[System] User disabled database logging." << endl;
  fitnessData.removeObserver(&logger);
  notifier.reset();
  
  fitnessData.dailyReset();

  return 0;
}