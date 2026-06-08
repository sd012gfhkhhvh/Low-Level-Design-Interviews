/*
=============================================================
                PROTOTYPE DESIGN PATTERN
=============================================================
# Resources:
    - https://algomaster.io/learn/lld/prototype

# Definition:
The Prototype Design Pattern is a creational design pattern that lets you create
new objects by cloning existing ones, instead of instantiating them from
scratch.

It’s particularly useful in situations where:

- Creating a new object is expensive, time-consuming, or resource-intensive.
- You want to avoid duplicating complex initialization logic.
- You need many similar objects with only slight differences.
*/

/*
===============================================
Example-1: Spawning Enemies in a Game
===============================================

# The Priblem:
Let’s say you’re developing a 2D shooting game where enemies appear frequently
throughout the gameplay.

You have several enemy types with distinct attributes:

- BasicEnemy: Low health, slow speed. Used in early levels.
- ArmoredEnemy: High health, medium speed. Harder to defeat, appears later.
- FlyingEnemy: Medium health, fast speed. Harder to hit, used for surprise
attacks.

Now, imagine you need to spawn a FlyingEnemy. You might write code like this:
-----------------------------------------------------------------
Enemy flying1 = new Enemy("Flying", 100, 10.5, false, "Laser");
Enemy flying2 = new Enemy("Flying", 100, 10.5, false, "Laser");
------------------------------------------------------------------
And you’ll do the same for dozens, maybe hundreds, of similar enemies during the
game.

But Here’s the Problem
- Repetitive Code
- Scattered Defaults: If the default speed or weapon of FlyingEnemy
changes, you need to update it in every single place you created one.
- Error-Prone: Forget to set one property? Use a wrong value? Bugs will creep in
silently.
- Cluttered Codebase: Your main game loop or spawn logic becomes bloated
with object construction details.

As your game scales (adding more enemy types, behaviors, or configurations) this
naive approach quickly becomes hard to manage and maintain.

# Goal:
To create a clean, centralized, and reusable way to create enemy instances with
consistent defaults while allowing minor tweaks.

# Solution:
Instead of configuring every new object line-by-line, we define a pre-configured
prototype and simply clone it whenever we need a new instance.

Two ideas define the pattern:

- Self-cloning: The object itself knows how to create a copy of itself. No
external code needs to understand its internal structure.
- Decoupled creation: The client does not need to know the concrete class of the
object it is cloning. It works through a common interface with a clone() method.


Prototype has four participants, three required and one optional:

*/

#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
using namespace std;

/*
===========================
Prototype (Interface)
===========================
Declares the clone() method that all cloneable objects must implement
Defines the contract for self-cloning and allows clients to clone objects
without knowing their concrete class
*/
class EnemyPrototype {
public:
  virtual EnemyPrototype *clone() const = 0;
  virtual ~EnemyPrototype() = default;
};

/*
=================================
ConcretePrototype Implementation
=================================
Implements the clone() method to produce a copy of itself
Copies all fields to the new instance (must be a deep copy)
*/
class Enemy : public EnemyPrototype {
private:
  string type;
  int health;
  double speed;
  bool armored;
  string weapon;
  vector<string> inventory;

public:
  Enemy(string type, int health, double speed, bool armored, string weapon,
        vector<string> inventory)
      : type(move(type)), health(health), speed(speed), armored(armored),
        weapon(move(weapon)), inventory(move(inventory)) {}

  Enemy *clone() const override {
    return new Enemy(type, health, speed, armored, weapon, inventory);
  }

  void setHealth(int h) { health = h; }
  void addItem(const string &item) { inventory.push_back(item); }

  void printStats() const {
    cout << type << " [Health: " << health << ", Speed: " << speed
         << ", Armored: " << (armored ? "true" : "false")
         << ", Weapon: " << weapon << ", Inventory: [";
    for (size_t i = 0; i < inventory.size(); i++) {
      if (i > 0)
        cout << ", ";
      cout << inventory[i];
    }
    cout << "]]" << endl;
  }
};

/*
=====================================
Prototype Registry (Optional)
=====================================
Stores a collection of pre-configured prototypes, indexed by a key (string,
enum, etc.) Returns clones (not originals) when clients request objects by key
*/
class EnemyRegistry {
private:
  map<string, unique_ptr<Enemy>> prototypes;

public:
  void registerPrototype(const string &key, unique_ptr<Enemy> prototype) {
    prototypes[key] = move(prototype);
  }

  unique_ptr<Enemy> get(const string &key) {
    auto it = prototypes.find(key);
    if (it == prototypes.end()) {
      throw invalid_argument("No prototype registered for: " + key);
    }
    return unique_ptr<Enemy>(it->second->clone());
  }
};

/*
================
Client Code
================
Creates new objects by asking a prototype to clone itself
Holds a reference to a prototype instance and calls clone() when a new object is
needed Optionally customize the clone after creation
*/

int main() {
  EnemyRegistry registry;

  registry.registerPrototype(
      "flying",
      std::make_unique<Enemy>("FlyingEnemy", 100, 12.0, false, "Laser",
                              std::vector<std::string>{"Speed Boost"}));
  registry.registerPrototype(
      "armored",
      std::make_unique<Enemy>("ArmoredEnemy", 300, 6.0, true, "Cannon",
                              std::vector<std::string>{"Shield", "Helmet"}));

  auto e1 = registry.get("flying");
  auto e2 = registry.get("flying");
  e2->setHealth(80);
  e2->addItem("Smoke Bomb");

  auto e3 = registry.get("armored");

  e1->printStats();
  e2->printStats();
  e3->printStats();

  return 0;
}