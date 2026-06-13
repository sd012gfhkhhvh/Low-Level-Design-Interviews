/*
=========================================================================
  Facade Pattern — Practical Example: Home Theater System
=========================================================================

Scenario:
Imagine you have a home theater with multiple components: an amplifier, a DVD
player, a projector, a streaming service, and smart lights. Watching a movie
requires turning on the projector, dimming the lights, powering up the
amplifier, setting the volume, and starting the movie. That is five subsystems
with specific sequencing requirements.

The Problem:
Without a facade, every part of your application that wants to play a movie (a
remote control app, a voice assistant integration, a scheduled movie night
feature) needs to know all those steps. This means duplicating complex orchestration 
code across multiple clients.

Solution:
Introduce a Facade (`HomeTheaterFacade`). This class wraps all the complex 
subsystem interactions into simple methods like `watchMovie()` and `endMovie()`.
The client only interacts with the Facade, completely decoupled from the 
underlying components.

=========================================================================
  Component Mapping
=========================================================================

  Facade      → HomeTheaterFacade (the simplified unified interface)
  Subsystems  → Amplifier, DvdPlayer, Projector, SmartLights, 
                StreamingService (the complex moving parts)
  Client      → main() (wants to watch a movie easily)

=========================================================================
*/

#include <iostream>
#include <string>

using namespace std;

/*
=========================================================================
  STEP 1: The Subsystems
=========================================================================
These represent the complex internal components. Individually, they are 
simple to understand, but coordinating them is tedious.
*/

class Amplifier {
public:
  void on() { cout << "[Amplifier] Powering on." << endl; }
  void off() { cout << "[Amplifier] Shutting down." << endl; }
  void setVolume(int level) { cout << "[Amplifier] Volume set to " << level << "." << endl; }
};

class DvdPlayer {
public:
  void on() { cout << "[DVD Player] Powering on." << endl; }
  void off() { cout << "[DVD Player] Shutting down." << endl; }
  void play(string movie) { cout << "[DVD Player] Playing '" << movie << "'." << endl; }
  void stop() { cout << "[DVD Player] Stopped." << endl; }
};

class Projector {
public:
  void on() { cout << "[Projector] Warming up." << endl; }
  void off() { cout << "[Projector] Cooling down." << endl; }
  void wideScreenMode() { cout << "[Projector] Widescreen mode enabled." << endl; }
};

class SmartLights {
public:
  void dim(int level) { cout << "[Lights] Dimmed to " << level << "%." << endl; }
  void on() { cout << "[Lights] Full brightness." << endl; }
};

class StreamingService {
public:
  void connect() { cout << "[Streaming] Connected to service." << endl; }
  void disconnect() { cout << "[Streaming] Disconnected." << endl; }
  void stream(string movie) { cout << "[Streaming] Now streaming '" << movie << "'." << endl; }
};

/*
=========================================================================
  STEP 2: The Facade
=========================================================================
The Facade hides the complexity of the subsystems. It exposes two very
simple, high-level operations: watchMovie() and endMovie().
*/

class HomeTheaterFacade {
private:
  Amplifier &amp;
  DvdPlayer &dvd;
  Projector &projector;
  SmartLights &lights;
  StreamingService &streaming;

public:
  // The Facade is constructed with references to the subsystems it needs to coordinate.
  HomeTheaterFacade(Amplifier &amp, DvdPlayer &dvd, Projector &projector,
                    SmartLights &lights, StreamingService &streaming)
      : amp(amp), dvd(dvd), projector(projector), lights(lights),
        streaming(streaming) {}

  void watchMovie(string movie) {
    cout << "\n=== FACADE: Preparing to watch '" << movie << "' ===" << endl;
    
    // The Facade handles the complex orchestration
    lights.dim(15);
    projector.on();
    projector.wideScreenMode();
    amp.on();
    amp.setVolume(20);
    streaming.connect();
    streaming.stream(movie);
    
    cout << "=== FACADE: Enjoy the movie! ===\n" << endl;
  }

  void endMovie() {
    cout << "=== FACADE: Shutting down home theater ===" << endl;
    
    // The Facade handles the cleanup sequence
    streaming.disconnect();
    amp.off();
    projector.off();
    lights.on();
    
    cout << "=== FACADE: Home theater off ===\n" << endl;
  }
};

/*
=========================================================================
  STEP 3: Client Code
=========================================================================
Notice how clean the client code is. The client doesn't need to know the
order of operations or interact with the 5 different subsystems directly.
*/

int main() {
  // 1. Instantiate the subsystems 
  // (In a real app, this might be handled by Dependency Injection)
  Amplifier amp;
  DvdPlayer dvd;
  Projector projector;
  SmartLights lights;
  StreamingService streaming;

  // 2. Instantiate the Facade, passing in the subsystems
  HomeTheaterFacade theater(amp, dvd, projector, lights, streaming);

  // 3. The Client uses the simplified Facade interface
  theater.watchMovie("Interstellar");
  
  // Sometime later...
  theater.endMovie();

  return 0;
}