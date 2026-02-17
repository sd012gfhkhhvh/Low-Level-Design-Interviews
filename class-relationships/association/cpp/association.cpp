#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
Ref - https://algomaster.io/learn/lld/association
Ref -
https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/

1. What is Association?
Association represents a relationship between two classes where one object has,
uses, communicates with, or references another.

If Class A must interact with Class B to fulfill its purpose, then Class A is
associated with Class B.

Key Characteristics of Association:
- Association reflects a "has-a" or "uses-a" relationship.
- Associated objects are loosely coupled and can exist independently of one
another.
- The association can be unidirectional or bidirectional, and can follow
different multiplicity patterns (1-to-1, 1-to-many, etc.).

2. UML Representation
In UML class diagrams, association is represented by a solid line between two
classes.

---------------------------------------------------------------------------------------
Symbol	            Meaning	                              Example Scenario
---------------------------------------------------------------------------------------
Solid line (---)	An association between classes	        Student ---
Teacher

Arrowhead (-->)	    Directionality (who knows whom)	      Order -->
PaymentGateway

No arrowhead	    Bidirectional association	              Team --- Developer

1	                Exactly one	                            Each User
has one Profile

0..1	            Zero or one (optional)	                An Employee may
have a Manager

*	                Many (zero or more)	                    A Project
can have many Tasks

1..*	            At least one	                          Each Course
has one or more Students
------------------------------------------------------------------------------------------------

Multiplicity defines how many instances of one class can be associated with
another. It is written near the class ends in UML diagrams.

(User) 1 ----has----> 1 (Profile)

The solid line is the key. Inheritance uses a solid line with a hollow triangle.
Aggregation adds a hollow diamond. Composition adds a filled diamond. Plain
association is just the line, optionally with an arrowhead for direction and
multiplicity labels at each end.

3. Types of Association
Associations between classes can vary depending on how objects are connected and
in which direction information flows.

In Object-Oriented Design, associations are primarily defined by two key
properties:

Directionality — Who knows about whom?
Multiplicity — How many objects are connected?

*/

/*
3.1 Based on Direction (Directionality)
Directionality determines which class holds a reference to the other and whether
communication is one-way or two-way.

a. Unidirectional Association
In a unidirectional association, only one class is aware of or holds a reference
to the other class. The referenced class has no knowledge of who is referencing
it.

Example:
(Order) ----uses----> 1 (PaymentGateWay)

An Order object uses a PaymentGateway to process transactions, but the
PaymentGateway doesn't keep track of any orders. The order knows about the
gateway. The gateway doesn't know about the order.
*/

class PaymentGateway {
public:
  void processPayment(double amount) {
    cout << "Processing payment of $" << amount << endl;
  }
};

class Order {
private:
  PaymentGateway *gateway;

public:
  Order(PaymentGateway *gateway) { this->gateway = gateway; }

  void checkout() { gateway->processPayment(100.0); }
};

/*
b. Bidirectional Association
In a bidirectional association, both classes are aware of each other. Each class
holds a reference to the other, enabling two-way communication.

Example:
(Team) 1 ----has---- * (Developer)

A Team has a list of Developers, and each Developer knows which Team
they belong to. Either side can navigate to the other.

Notice how addDeveloper() updates both sides of the relationship: it adds the
developer to the team's list and sets the team reference on the developer. This
is important. In a bidirectional association, both references must stay in sync.
If you add a developer to the team but forget to set the developer's team
reference, you'll get inconsistent state where the team thinks it has the
developer, but the developer doesn't know which team it belongs to.

Bidirectional associations are more complex to maintain than unidirectional
ones. You need to keep both sides synchronized, which means more code and more
opportunities for bugs. Use them only when both sides genuinely need to navigate
to the other.
*/

// Forward declaration - tells compiler that Team exists
// We need this because Developer references Team before Team is defined
class Team;

class Developer {
private:
  Team *team;

public:
  void setTeam(Team *team) { this->team = team; }
};

class Team {
private:
  vector<Developer *> developers;

public:
  void addDeveloper(Developer *dev) {
    developers.push_back(dev);
    dev->setTeam(this);
  }
};

/*
3.2 Based on Multiplicity
Multiplicity defines how many instances of one class can be associated with
instances of another class. It describes the quantity and nature of the
connections.

a. One-to-One Association
Each object of one class is linked to exactly one object of the other class.

Example:
(User) 1 ----has----> 1 (Profile)

Each User has exactly one Profile, and each Profile belongs to one
User. This is a bidirectional one-to-one relationship.

One-to-one associations make sense when you want to separate concerns even
though the objects are tightly paired. A User handles authentication (login,
password, roles), while a Profile handles display information (avatar, bio,
preferences). Merging them into one class would work, but separating them keeps
each class focused on a single responsibility.

If you find that two one-to-one associated classes are always created, modified,
and deleted together with no independent use case, that's a signal they might
belong as a single class instead.
*/

class User;

class Profile {
private:
  User *user;

public:
  void setUser(User *user) { this->user = user; }
};

class User {
private:
  Profile *profile;

public:
  void setProfile(Profile *profile) {
    this->profile = profile;
    profile->setUser(this);
  }
};

/*
b. One-to-Many Association
One object of a class is linked to multiple objects of another class. This is
one of the most common patterns in software design.

Example:
(Project) 1 ---has/contains--- * (issue)

Each Project can have many Issues (bug reports, feature requests), but
each Issue belongs to one Project. The project holds a list of issues, and each
issue holds a back-reference to its project.
*/

class Project;

class Issue {
private:
  Project *project;

public:
  void setProject(Project *project) { this->project = project; }
};

class Project {
private:
  vector<Issue *> issues;

public:
  void addIssue(Issue *issue) {
    issues.push_back(issue);
    issue->setProject(this);
  }
};

/*
c. Many-to-Many Association
Multiple objects from one class are associated with multiple objects from
another class. This is common in scenarios involving memberships, enrollments,
or tagging systems.

Example:
(User) * ---member of--- * (Group)

A User can be a member of multiple Groups (WhatsApp groups, Slack
channels), and a Group can have multiple Users. Both sides hold a list of the
other. The joinGroup() and addUser() methods keep both sides in sync.

Notice the guard clause in both joinGroup() and addUser(). Without it, calling
alice.joinGroup(backend) would add backend to Alice's groups, then
backend.addUser(alice) would add Alice to backend's users, then it would call
alice.joinGroup(backend) again, and you'd be stuck in an infinite loop. The
contains check breaks the recursion.

Many-to-many associations are inherently bidirectional and require careful
synchronization. In database design, you'd model this with a join table. In
code, both sides hold a list of the other, and you need helper methods that
update both sides atomically.
*/

class Group; // Forward declaration

class User2 {
private:
  string name;
  vector<Group *> groups;

public:
  User2(const string &name) : name(name) {}

  void joinGroup(Group *group);

  string getName() const { return name; }
  vector<Group *> getGroups() const { return groups; }
};

class Group {
private:
  string name;
  vector<User2 *> users;

public:
  Group(const string &name) : name(name) {}

  void addUser(User2 *user) {
    for (auto u : users)
      if (u == user)
        return;
    users.push_back(user);
    user->joinGroup(this);
  }

  string getName() const { return name; }
  vector<User2 *> getUsers() const { return users; }
};

void User2::joinGroup(Group *group) {
  for (auto g : groups)
    if (g == group)
      return;
  groups.push_back(group);
  group->addUser(this);
}

/*
4. Practical Example: Hospital Appointment System
Let's build a system that combines multiple association types in a realistic
domain. A hospital manages doctors, patients, rooms, and appointments. The
relationships between these entities demonstrate unidirectional, bidirectional,
one-to-many, and many-to-many associations working together.

Here's how the classes connect:

- Appointment holds a reference to a Room (unidirectional, the room doesn't know
about its appointments).
- Doctor has a list of Appointment objects, and each
Appointment points back to its Doctor (bidirectional one-to-many).
- Patient has a list of Appointment objects, and each Appointment points back to
its Patient (bidirectional one-to-many).
- Doctor and Patient are connected many-to-many through Appointment as an
intermediary. A doctor sees many patients, and a patient can visit many doctors,
but they don't reference each other directly.

Why This Design Works ?

- The Appointment class is the intermediary. Instead of Doctor and Patient
holding direct references to each other (which would create a tangled
many-to-many), they connect through Appointment. This is a common pattern for
modeling many-to-many relationships in code, analogous to a join table in a
relational database.

- Navigation works both ways. A doctor can find all their patients by
walking their appointments. A patient can find all their doctors the same way.
Neither class needs to maintain a separate list of the other.

- Room stays simple. The room doesn't need to know about appointments. It's just
a location. This keeps the relationship unidirectional and avoids unnecessary
coupling.

- Adding data to the relationship is natural. Because Appointment is a full
object, you can add fields like time, status, notes, or diagnosis without
modifying Doctor or Patient. Try doing that with a direct many-to-many
reference.
*/

class Room {
private:
  string number;
  int floor;

public:
  Room(const string &number, int floor) : number(number), floor(floor) {}
  string getNumber() const { return number; }
  int getFloor() const { return floor; }
};

class Doctor;
class Patient;

class Appointment {
private:
  Doctor *doctor;
  Patient *patient;
  Room *room;
  string time;

public:
  Appointment(Doctor *doctor, Patient *patient, Room *room, const string &time);
  Doctor *getDoctor() const { return doctor; }
  Patient *getPatient() const { return patient; }
  Room *getRoom() const { return room; }
  string getTime() const { return time; }
};

class Doctor {
private:
  string name;
  string specialization;
  vector<Appointment *> appointments;

public:
  Doctor(const string &name, const string &specialization)
      : name(name), specialization(specialization) {}

  void addAppointment(Appointment *appt) { appointments.push_back(appt); }

  vector<Patient *> getPatients() const;

  string getName() const { return name; }
  string getSpecialization() const { return specialization; }
  vector<Appointment *> getAppointments() const { return appointments; }
};

class Patient {
private:
  string name;
  vector<Appointment *> appointments;

public:
  Patient(const string &name) : name(name) {}

  void addAppointment(Appointment *appt) { appointments.push_back(appt); }

  vector<Doctor *> getDoctors() const;

  string getName() const { return name; }
  vector<Appointment *> getAppointments() const { return appointments; }
};

Appointment::Appointment(Doctor *doctor, Patient *patient, Room *room,
                         const string &time)
    : doctor(doctor), patient(patient), room(room), time(time) {
  doctor->addAppointment(this);
  patient->addAppointment(this);
}

vector<Patient *> Doctor::getPatients() const {
  vector<Patient *> result;
  for (auto *appt : appointments) {
    auto *p = appt->getPatient();
    if (find(result.begin(), result.end(), p) == result.end())
      result.push_back(p);
  }
  return result;
}

vector<Doctor *> Patient::getDoctors() const {
  vector<Doctor *> result;
  for (auto *appt : appointments) {
    auto *d = appt->getDoctor();
    if (find(result.begin(), result.end(), d) == result.end())
      result.push_back(d);
  }
  return result;
}

int main() {
  // Usage - many to many association
  User2 jack("Jack");
  User2 john("John");

  Group backend("Backend");
  Group devOps("DevOps");

  jack.joinGroup(&backend);
  jack.joinGroup(&devOps);
  john.joinGroup(&backend);

  // --------- Practical Example ---------
  Doctor drSmith("Dr. Smith", "Cardiology");
  Doctor drPatel("Dr. Patel", "Neurology");

  Patient alice("Alice");
  Patient bob("Bob");

  Room room101("101", 1);
  Room room205("205", 2);

  Appointment a1(&drSmith, &alice, &room101, "9:00 AM");
  Appointment a2(&drSmith, &bob, &room101, "10:00 AM");
  Appointment a3(&drPatel, &alice, &room205, "2:00 PM");

  cout << drSmith.getName() << "'s patients:" << endl;
  for (auto *p : drSmith.getPatients())
    cout << "  - " << p->getName() << endl;

  cout << alice.getName() << "'s doctors:" << endl;
  for (auto *d : alice.getDoctors())
    cout << "  - " << d->getName() << " (" << d->getSpecialization() << ")"
         << endl;

  cout << drSmith.getName() << "'s schedule:" << endl;
  for (auto *a : drSmith.getAppointments())
    cout << "  - " << a->getTime() << " with " << a->getPatient()->getName()
         << " in Room " << a->getRoom()->getNumber() << endl;

  return 0;
}
