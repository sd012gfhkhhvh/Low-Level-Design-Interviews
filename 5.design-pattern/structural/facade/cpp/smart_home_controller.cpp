/*
=========================================================================
  Facade Pattern — Exercise: Smart Home Controller
=========================================================================

Scenario:
You are building an automation system for a smart home. The home has various
subsystems: lights, a thermostat, and a security system.

The Problem:
When the user leaves the house or arrives back home, they need to manually
turn off/on the lights, adjust the thermostat temperature and mode, and
arm/disarm the security system. If the client app interacts directly with
each of these subsystems, it leads to code duplication and tight coupling.

Solution:
Implement a `SmartHomeFacade` that exposes high-level, simplified methods
like `leaveHome()` and `arriveHome()`. The facade coordinates all three
subsystems internally.

=========================================================================
  Component Mapping
=========================================================================

  Facade      → SmartHomeFacade (the simplified unified interface)
  Subsystems  → SmartLightsSystem, Thermostat, SecuritySystem
                (the individual smart devices)
  Client      → main() (wants to easily manage the house state)

=========================================================================
*/

#include <iostream>
#include <string>

using namespace std;

/*
=========================================================================
  STEP 1: The Subsystems
=========================================================================
These represent the complex internal components (the smart devices).
Individually, they are simple to understand, but coordinating them is tedious.
*/

class SmartLightsSystem {
public:
  void on() { cout << "[Lights] Turned on." << endl; }
  void off() { cout << "[Lights] Turned off." << endl; }
};

class Thermostat {
private:
  string mode;

public:
  void setMode(const string &newMode) { mode = newMode; }

  void setTemperature(int degrees) {
    cout << "[Thermostat] Mode set to '" << mode << "'. Temperature set to "
         << degrees << "°C." << endl;
  }
};

class SecuritySystem {
public:
  void arm() { cout << "[Security] System armed." << endl; }
  void disarm() { cout << "[Security] System disarmed." << endl; }
};

/*
=========================================================================
  STEP 2: The Facade
=========================================================================
The Facade hides the complexity of the subsystems. It exposes two very
simple, high-level operations: leaveHome() and arriveHome().
*/

class SmartHomeFacade {
private:
  SmartLightsSystem &lights;
  Thermostat &thermostat;
  SecuritySystem &security;

public:
  // The Facade is constructed with references to the subsystems it needs to
  // coordinate.
  SmartHomeFacade(SmartLightsSystem &lights, Thermostat &thermostat,
                  SecuritySystem &security)
      : lights(lights), thermostat(thermostat), security(security) {}

  void leaveHome() {
    cout << "\n=== FACADE: Leaving Home ===" << endl;

    // The Facade handles the complex orchestration
    lights.off();
    thermostat.setMode("eco");
    thermostat.setTemperature(18);
    security.arm();

    cout << "=== FACADE: Home secured ===" << endl;
  }

  void arriveHome() {
    cout << "\n=== FACADE: Arriving Home ===" << endl;

    // The Facade handles the complex orchestration
    lights.on();
    thermostat.setMode("comfort");
    thermostat.setTemperature(22);
    security.disarm();

    cout << "=== FACADE: Welcome home! ===" << endl;
  }
};

/*
=========================================================================
  STEP 3: Client Code
=========================================================================
Notice how clean the client code is. The client doesn't need to know the
order of operations or interact with the 3 different subsystems directly.
*/

int main() {
  // 1. Instantiate the subsystems
  SmartLightsSystem lights;
  Thermostat thermostat;
  SecuritySystem security;

  // 2. Instantiate the Facade, passing in the subsystems
  SmartHomeFacade home(lights, thermostat, security);

  // 3. The Client uses the simplified Facade interface
  home.leaveHome();

  // Sometime later...
  home.arriveHome();

  return 0;
}