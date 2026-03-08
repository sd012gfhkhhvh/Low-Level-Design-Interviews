#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Ref - https://algomaster.io/learn/lld/aggregation
// Ref -
// https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/

//
// =======================================================
// 1. WHAT IS AGGREGATION?
// =======================================================
//
// Aggregation is a specialized form of Association that models a "whole-part"
// relationship with "weak ownership".
//
// Key Formula:
// Aggregation = "has-a" relationship + independent lifecycles
//
// Mental Model:
// Think of a University Department and its Professors:
// - A Department (the whole) contains Professors (the parts)
// - If the Department closes, the Professors still exist and can join other
// departments.
// - The Department does not control the lifecycle of the Professors.
//
// Key Characteristics of Aggregation:
// - Whole-part relationship (container-contained)
// - Weak ownership (the "whole" doesn't own the "part")
// - Independent lifecycles (parts exist without the whole)
// - Parts can be shared among multiple wholes
//
// UML Notation:
// Class1 <>------- Class2  (hollow diamond on the "whole" side)
// Whole <>---- Part

//
// =======================================================
// 2. BASIC AGGREGATION: DEPARTMENT AND PROFESSORS
// =======================================================
//
// A classical example of aggregation. The Department contains Professors,
// but Professors are created outside and passed in.

class Professor {
private:
  std::string name;

public:
  Professor(const std::string &name) : name(name) {}

  std::string getName() const { return name; }
};

class Department {
private:
  std::string name;
  // The Department "has-a" collection of Professors,
  // but stores pointers to represent weak ownership.
  std::vector<Professor *> professors;

public:
  Department(const std::string &name) : name(name) {}

  // Add an existing professor to the department
  void addProfessor(Professor *prof) { professors.push_back(prof); }

  void printProfessors() const {
    std::cout << "🏫 Professors in " << name << " Department:\n";
    for (const auto *prof : professors) {
      std::cout << "  👨‍🏫 " << prof->getName() << "\n";
    }
  }

  // Notice there is NO destructor deleting the professors.
  // The department objects don't own the professor objects!
};

//
// =======================================================
// 3. SHARING PARTS: PLAYERS AND TEAMS
// =======================================================
//
// In aggregation, the parts can be shared among multiple wholes.
// A Player can belong to a National Team and a Club Team simultaneously.

class Player {
private:
  std::string name;

public:
  Player(const std::string &n) : name(n) {}
  std::string getName() const { return name; }
};

class Team {
private:
  std::string teamName;
  std::vector<Player *> roster;

public:
  Team(const std::string &name) : teamName(name) {}

  void addPlayer(Player *player) { roster.push_back(player); }

  void displayTeam() const {
    std::cout << "⚽ Team: " << teamName << "\n";
    for (const auto *player : roster) {
      std::cout << "  - " << player->getName() << "\n";
    }
  }
};

//
// =======================================================
// 4. DEPENDENCY INJECTION AS AGGREGATION
// =======================================================
//
// From a design perspective, when you pass dependencies via a constructor,
// you are often setting up an aggregation. The injected dependency exists
// outside the class and is just used by it.

class Logger {
public:
  void log(const std::string &message) {
    std::cout << "[LOG] " << message << "\n";
  }
};

class WebServer {
private:
  Logger *logger; // WebServer uses Logger, but doesn't own it

public:
  // Logger is injected from outside
  WebServer(Logger *l) : logger(l) {}

  void handleRequest() {
    if (logger) {
      logger->log("Handling an incoming HTTP request");
    }
    std::cout << "🌐 Response sent.\n";
  }
};

//
// =======================================================
// 5. REAL-WORLD EXAMPLE: MUSIC LIBRARY SYSTEM
// =======================================================
//
// Let's model a realistic music system demonstrating aggregation at multiple
// levels.
// - Artists create Songs
// - Playlists aggregate Songs (a Song can be in multiple Playlists)
// - Library acts as the authoritative source of everything.
// - Deleting a Playlist leaves the Songs intact.

class Artist {
private:
  std::string name;

public:
  Artist(const std::string &name) : name(name) {}
  std::string getName() const { return name; }
};

class Song {
private:
  std::string title;
  Artist *artist; // Aggregation: Song points to its Artist
  int durationSecs;

public:
  Song(const std::string &t, Artist *a, int duration)
      : title(t), artist(a), durationSecs(duration) {}

  std::string getTitle() const { return title; }
  Artist *getArtist() const { return artist; }
  int getDuration() const { return durationSecs; }

  std::string toString() const {
    return "🎵 " + title + " by " + artist->getName() + " (" +
           std::to_string(durationSecs) + "s)";
  }
};

class Playlist {
private:
  std::string name;
  // Aggregation: Playlist contains references to Songs.
  // It does not own the Songs.
  std::vector<Song *> songs;

public:
  Playlist(const std::string &n) : name(n) {}

  void addSong(Song *song) { songs.push_back(song); }

  void removeSong(Song *song) {
    songs.erase(std::remove(songs.begin(), songs.end(), song), songs.end());
  }

  int getSongCount() const { return songs.size(); }

  int getTotalDuration() const {
    int total = 0;
    for (const auto *song : songs) {
      total += song->getDuration();
    }
    return total;
  }

  std::string getName() const { return name; }
  const std::vector<Song *> &getSongs() const { return songs; }
};

class Library {
private:
  // Weak ownership of songs in this specific implementation,
  // assuming songs are created/managed somewhere top-level,
  // or we could use Composition here and Aggregation in Playlist.
  // For simplicity, Library just aggregates pointers.
  std::vector<Song *> masterSongs;

public:
  void addSongToLibrary(Song *s) { masterSongs.push_back(s); }

  int getTotalSongs() const { return masterSongs.size(); }
};

//
// =======================================================
// 6. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Aggregation in C++ Demo ===\n\n";

  // ---- 1. Department and Professors ----
  std::cout << "--- 1. Department and Professors ---\n";
  // 1. Create Parts independently
  Professor *p1 = new Professor("Dr. Smith");
  Professor *p2 = new Professor("Dr. Johnson");

  // 2. Create Whole
  Department *csDept = new Department("Computer Science");

  // 3. Assemble
  csDept->addProfessor(p1);
  csDept->addProfessor(p2);

  csDept->printProfessors();

  // 4. Destroy Whole
  std::cout << "Deleting Department...\n";
  delete csDept; // Automatically handled if we used smart pointers, but here
                 // manual

  // 5. Parts still exist!
  std::cout << "Department deleted, but Professors still exist:\n";
  std::cout << "  " << p1->getName() << " is looking for a new job.\n\n";

  // Clean up
  delete p1;
  delete p2;

  // ---- 2. Shared Parts: Player in Multiple Teams ----
  std::cout << "--- 2. Shared Parts: Teams & Players ---\n";
  Player messi("Lionel Messi");

  Team national("Argentina");
  Team club("Inter Miami");

  // The same player is aggregated into multiple teams
  national.addPlayer(&messi);
  club.addPlayer(&messi);

  national.displayTeam();
  club.displayTeam();
  std::cout << "\n";

  // ---- 3. Dependency Injection ----
  std::cout << "--- 3. Dependency Injection ---\n";
  Logger sharedLogger;

  WebServer server1(&sharedLogger);
  WebServer server2(&sharedLogger);

  server1.handleRequest();
  server2.handleRequest();
  std::cout << "\n";

  // ---- 4. Real-World Music Library ----
  std::cout << "--- 4. Music Library System ---\n";

  // Create Artists (Independent entities)
  Artist coldplay("Coldplay");
  Artist adele("Adele");

  // Create Songs
  Song yellow("Yellow", &coldplay, 269);
  Song clocks("Clocks", &coldplay, 307);
  Song hello("Hello", &adele, 295);

  // Add to main library
  Library myLib;
  myLib.addSongToLibrary(&yellow);
  myLib.addSongToLibrary(&clocks);
  myLib.addSongToLibrary(&hello);

  // Create Playlists (Wholes)
  Playlist *workout = new Playlist("Workout Mix");
  Playlist *chill = new Playlist("Chill Vibes");

  // Aggregate Songs (Parts) into Playlists
  workout->addSong(&yellow);
  workout->addSong(&clocks);

  chill->addSong(&yellow); // Notice 'Yellow' is shared!
  chill->addSong(&hello);

  std::cout << workout->getName() << " (" << workout->getSongCount()
            << " songs)\n";
  for (auto *s : workout->getSongs())
    std::cout << "  " << s->toString() << "\n";

  std::cout << chill->getName() << " (" << chill->getSongCount() << " songs)\n";
  for (auto *s : chill->getSongs())
    std::cout << "  " << s->toString() << "\n";

  // Deleting a playlist doesn't affect the songs or library
  std::cout << "Deleting playlist '" << workout->getName() << "'...\n";
  delete workout;

  std::cout << "Library still has " << myLib.getTotalSongs()
            << " songs. 'Yellow' is still available!\n";

  delete chill;

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Weak Ownership**:
   - The "whole" class holds the "parts" but does not own them absolutely.
   - Example: Department has a list of Professor pointers.

2. **Independent Lifecycles**:
   - The part can exist before the whole is created.
   - The part survives after the whole is destroyed.

3. **Part Sharing**:
   - In aggregation, a single part can belong to multiple wholes simultaneously.
   - Example: A Player can be in a Club Team and a National Team.

4. **Implementation Strategy in C++**:
   - Often implemented via Raw Pointers (`T*`), References (`T&`), or
`std::shared_ptr`.
   - Never use `std::unique_ptr` for simple aggregation (because it implies
exclusive ownership = Composition).
   - The container class does not `delete` the contents inside its destructor.

5. **Aggregation vs Association vs Composition**:
   - Association: A uses B. (No whole/part context)
   - Aggregation: A has B, but B can exist without A. (Whole/part, weak
ownership)
   - Composition: A owns B, B dies if A dies. (Whole/part, strong ownership)
*/