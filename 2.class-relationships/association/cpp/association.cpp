#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Ref - https://algomaster.io/learn/lld/association
// Ref - https://www.geeksforgeeks.org/association-composition-aggregation-java/
// Ref -
// https://www.visual-paradigm.com/guide/uml-unified-modeling-language/uml-aggregation-vs-composition/

//
// =======================================================
// 1. WHAT IS ASSOCIATION?
// =======================================================
//
// Association represents a relationship where objects use, know about, or
// communicate with each other.
//
// Key Formula:
// Association = "uses-a" or "has-a" relationship (loosely coupled)
//
// Mental Model:
// Think of association like your relationship with your barber:
// - You use their service
// - You know who they are
// - But you can both exist independently
// - Your barber doesn't cease to exist if you move away
//
// Key Characteristics:
// - Objects are loosely coupled
// - Objects can exist independently
// - Can be unidirectional or bidirectional
// - Can have different multiplicities (1-to-1, 1-to-many, many-to-many)
//
// UML Notation:
// Class1 ------- Class2  (solid line)
// Class1 ------> Class2  (with arrow for direction)
// Class1 1 ----> * Class2  (with multiplicity)

//
// =======================================================
// 2. TYPES BY DIRECTIONALITY
// =======================================================

// ---- 2.1 UNIDIRECTIONAL ASSOCIATION ----
// Only one class knows about the other

class PaymentGateway {
public:
  void processPayment(double amount) {
    std::cout << "💳 Processing payment of $" << amount << "\n";
  }
};

class Order {
private:
  PaymentGateway *gateway; // Order knows about PaymentGateway
  std::string orderId;
  double totalAmount;

public:
  Order(const std::string &id, PaymentGateway *gw, double amount)
      : orderId(id), gateway(gw), totalAmount(amount) {}

  void checkout() {
    std::cout << "📦 Order " << orderId << " checking out...\n";
    gateway->processPayment(totalAmount); // Uses the gateway
    std::cout << "✅ Order complete!\n";
  }
};

// PaymentGateway doesn't know about Order
// This is unidirectional: Order -> PaymentGateway

// ---- 2.2 BIDIRECTIONAL ASSOCIATION ----
// Both classes know about each other

class Team; // Forward declaration

class Developer {
private:
  std::string name;
  Team *team; // Developer knows their team

public:
  Developer(const std::string &n) : name(n), team(nullptr) {}

  void setTeam(Team *t) { team = t; }
  Team *getTeam() const { return team; }
  std::string getName() const { return name; }
};

class Team {
private:
  std::string name;
  std::vector<Developer *> developers; // Team knows its developers

public:
  Team(const std::string &n) : name(n) {}

  void addDeveloper(Developer *dev) {
    // Must keep both sides in sync!
    developers.push_back(dev);
    dev->setTeam(this);
  }

  std::string getName() const { return name; }
  const std::vector<Developer *> &getDevelopers() const { return developers; }
};

// Both Team and Developer know about each other
// This is bidirectional: Team <--> Developer

//
// =======================================================
// 3. TYPES BY MULTIPLICITY
// =======================================================

// ---- 3.1 ONE-TO-ONE ASSOCIATION ----
// Each object is linked to exactly one other object

class User; // Forward declaration

class Profile {
private:
  std::string avatar;
  std::string bio;
  User *user; // Each profile belongs to one user

public:
  Profile(const std::string &av, const std::string &b)
      : avatar(av), bio(b), user(nullptr) {}

  void setUser(User *u) { user = u; }
  User *getUser() const { return user; }
  std::string getAvatar() const { return avatar; }
  std::string getBio() const { return bio; }
};

class User {
private:
  std::string username;
  std::string email;
  Profile *profile; // Each user has one profile

public:
  User(const std::string &user, const std::string &mail)
      : username(user), email(mail), profile(nullptr) {}

  void setProfile(Profile *p) {
    profile = p;
    p->setUser(this); // Keep both sides in sync
  }

  Profile *getProfile() const { return profile; }
  std::string getUsername() const { return username; }
};

// User (1) <----> (1) Profile
// One-to-one bidirectional association

// ---- 3.2 ONE-TO-MANY ASSOCIATION ----
// One object is linked to multiple objects

class Project; // Forward declaration

class Issue {
private:
  std::string title;
  std::string status;
  Project *project; // Each issue belongs to one project

public:
  Issue(const std::string &t, const std::string &s)
      : title(t), status(s), project(nullptr) {}

  void setProject(Project *p) { project = p; }
  Project *getProject() const { return project; }
  std::string getTitle() const { return title; }
  std::string getStatus() const { return status; }
};

class Project {
private:
  std::string name;
  std::vector<Issue *> issues; // Project has many issues

public:
  Project(const std::string &n) : name(n) {}

  void addIssue(Issue *issue) {
    issues.push_back(issue);
    issue->setProject(this); // Keep both sides in sync
  }

  std::string getName() const { return name; }
  const std::vector<Issue *> &getIssues() const { return issues; }
};

// Project (1) <----> (*) Issue
// One-to-many bidirectional association

// ---- 3.3 MANY-TO-MANY ASSOCIATION ----
// Multiple objects from both sides are linked

class Course; // Forward declaration

class Student {
private:
  std::string name;
  std::vector<Course *> courses; // Student enrolled in many courses

public:
  Student(const std::string &n) : name(n) {}

  void enroll(Course *course); // Defined later

  std::string getName() const { return name; }
  const std::vector<Course *> &getCourses() const { return courses; }
};

class Course {
private:
  std::string name;
  std::vector<Student *> students; // Course has many students

public:
  Course(const std::string &n) : name(n) {}

  void addStudent(Student *student) {
    // Check if already enrolled (avoid duplicates)
    for (auto *s : students)
      if (s == student)
        return;

    students.push_back(student);
    student->enroll(this); // Keep both sides in sync
  }

  std::string getName() const { return name; }
  const std::vector<Student *> &getStudents() const { return students; }
};

void Student::enroll(Course *course) {
  // Check if already enrolled  (avoid duplicates)
  for (auto *c : courses)
    if (c == course)
      return;

  courses.push_back(course);
  course->addStudent(this); // Keep both sides in sync
}

// Student (*) <----> (*) Course
// Many-to-many bidirectional association

//
// =======================================================
// 4. REAL-WORLD EXAMPLE: LIBRARY MANAGEMENT SYSTEM
// =======================================================
//
// Demonstrates multiple association types working together:
// - Book and BorrowRecord (one-to-many)
// - Member and BorrowRecord (one-to-many)
// - Author and Book (many-to-many)

class Book;
class Member;
class BorrowRecord;

class Author {
private:
  std::string name;
  std::vector<Book *> books;

public:
  Author(const std::string &n) : name(n) {}

  void addBook(Book *book);

  std::string getName() const { return name; }
  const std::vector<Book *> &getBooks() const { return books; }
};

class Book {
private:
  std::string isbn;
  std::string title;
  std::vector<Author *> authors;
  std::vector<BorrowRecord *> borrowRecords;

public:
  Book(const std::string &isbn, const std::string &title)
      : isbn(isbn), title(title) {}

  void addAuthor(Author *author) {
    for (auto *a : authors)
      if (a == author)
        return;

    authors.push_back(author);
    author->addBook(this);
  }

  void addBorrowRecord(BorrowRecord *record) {
    borrowRecords.push_back(record);
  }

  std::string getISBN() const { return isbn; }
  std::string getTitle() const { return title; }
  const std::vector<Author *> &getAuthors() const { return authors; }
  bool isAvailable() const;
};

class Member {
private:
  std::string memberId;
  std::string name;
  std::vector<BorrowRecord *> borrowRecords;

public:
  Member(const std::string &id, const std::string &n) : memberId(id), name(n) {}

  void addBorrowRecord(BorrowRecord *record) {
    borrowRecords.push_back(record);
  }

  std::string getMemberId() const { return memberId; }
  std::string getName() const { return name; }
  const std::vector<BorrowRecord *> &getBorrowRecords() const {
    return borrowRecords;
  }
};

class BorrowRecord {
private:
  Book *book;
  Member *member;
  std::string borrowDate;
  std::string returnDate;
  bool returned;

public:
  BorrowRecord(Book *b, Member *m, const std::string &date)
      : book(b), member(m), borrowDate(date), returnDate(""), returned(false) {
    book->addBorrowRecord(this);
    member->addBorrowRecord(this);
  }

  void markReturned(const std::string &date) {
    returned = true;
    returnDate = date;
  }

  bool isReturned() const { return returned; }
  Book *getBook() const { return book; }
  Member *getMember() const { return member; }
  std::string getBorrowDate() const { return borrowDate; }
  std::string getReturnDate() const { return returnDate; }
};

void Author::addBook(Book *book) {
  for (auto *b : books)
    if (b == book)
      return;

  books.push_back(book);
  book->addAuthor(this);
}

bool Book::isAvailable() const {
  for (auto *record : borrowRecords) {
    if (!record->isReturned())
      return false; // Book is currently borrowed
  }
  return true;
}

//
// =======================================================
// 5. REAL-WORLD EXAMPLE: HOSPITAL APPOINTMENT SYSTEM
// =======================================================
//
// Demonstrates association as intermediary class pattern

class Room {
private:
  std::string number;
  int floor;

public:
  Room(const std::string &num, int f) : number(num), floor(f) {}

  std::string getNumber() const { return number; }
  int getFloor() const { return floor; }
};

class Doctor;
class Patient;

class Appointment {
private:
  Doctor *doctor;
  Patient *patient;
  Room *room;
  std::string dateTime;

public:
  Appointment(Doctor *doc, Patient *pat, Room *rm, const std::string &dt);

  Doctor *getDoctor() const { return doctor; }
  Patient *getPatient() const { return patient; }
  Room *getRoom() const { return room; }
  std::string getDateTime() const { return dateTime; }
};

class Doctor {
private:
  std::string name;
  std::string specialization;
  std::vector<Appointment *> appointments;

public:
  Doctor(const std::string &n, const std::string &spec)
      : name(n), specialization(spec) {}

  void addAppointment(Appointment *appt) { appointments.push_back(appt); }

  std::vector<Patient *> getPatients() const;

  std::string getName() const { return name; }
  std::string getSpecialization() const { return specialization; }
  const std::vector<Appointment *> &getAppointments() const {
    return appointments;
  }
};

class Patient {
private:
  std::string name;
  std::string patientId;
  std::vector<Appointment *> appointments;

public:
  Patient(const std::string &n, const std::string &id)
      : name(n), patientId(id) {}

  void addAppointment(Appointment *appt) { appointments.push_back(appt); }

  std::vector<Doctor *> getDoctors() const;

  std::string getName() const { return name; }
  std::string getPatientId() const { return patientId; }
  const std::vector<Appointment *> &getAppointments() const {
    return appointments;
  }
};

// Implementation
Appointment::Appointment(Doctor *doc, Patient *pat, Room *rm,
                         const std::string &dt)
    : doctor(doc), patient(pat), room(rm), dateTime(dt) {
  doctor->addAppointment(this);
  patient->addAppointment(this);
}

std::vector<Patient *> Doctor::getPatients() const {
  std::vector<Patient *> result;
  for (auto *appt : appointments) {
    auto *p = appt->getPatient();
    if (std::find(result.begin(), result.end(), p) == result.end())
      result.push_back(p);
  }
  return result;
}

std::vector<Doctor *> Patient::getDoctors() const {
  std::vector<Doctor *> result;
  for (auto *appt : appointments) {
    auto *d = appt->getDoctor();
    if (std::find(result.begin(), result.end(), d) == result.end())
      result.push_back(d);
  }
  return result;
}

//
// =======================================================
// 6. DEMONSTRATION
// =======================================================
//

int main() {
  std::cout << "=== Association in C++ Demo ===\n\n";

  // ---- Unidirectional Association ----
  std::cout << "1. Unidirectional Association (Order -> PaymentGateway):\n";
  PaymentGateway gateway;
  Order order1("ORD001", &gateway, 150.00);
  order1.checkout();

  // ---- Bidirectional Association ----
  std::cout << "\n2. Bidirectional Association (Team <-> Developer):\n";
  Team backend("Backend Team");
  Developer dev1("Alice");
  Developer dev2("Bob");

  backend.addDeveloper(&dev1);
  backend.addDeveloper(&dev2);

  std::cout << "Team: " << backend.getName() << "\n";
  std::cout << "Members:\n";
  for (auto *dev : backend.getDevelopers()) {
    std::cout << "  - " << dev->getName() << "\n";
  }

  // ---- One-to-One Association ----
  std::cout << "\n3. One-to-One Association (User <-> Profile):\n";
  User user("john_doe", "john@example.com");
  Profile profile("avatar.jpg", "Software Engineer");
  user.setProfile(&profile);

  std::cout << "User: " << user.getUsername() << "\n";
  std::cout << "Bio: " << user.getProfile()->getBio() << "\n";

  // ---- One-to-Many Association ----
  std::cout << "\n4. One-to-Many Association (Project <-> Issues):\n";
  Project project("Website Redesign");
  Issue bug1("Login button not working", "Open");
  Issue bug2("Homepage loads slowly", "In Progress");
  Issue feature1("Add dark mode", "Open");

  project.addIssue(&bug1);
  project.addIssue(&bug2);
  project.addIssue(&feature1);

  std::cout << "Project: " << project.getName() << "\n";
  std::cout << "Issues:\n";
  for (auto *issue : project.getIssues()) {
    std::cout << "  - " << issue->getTitle() << " [" << issue->getStatus()
              << "]\n";
  }

  // ---- Many-to-Many Association ----
  std::cout << "\n5. Many-to-Many Association (Student <-> Course):\n";
  Student alice("Alice");
  Student bob("Bob");

  Course cpp("C++ Programming");
  Course ds("Data Structures");

  alice.enroll(&cpp);
  alice.enroll(&ds);
  bob.enroll(&cpp);

  std::cout << "Student: " << alice.getName() << "\n";
  std::cout << "Enrolled in:\n";
  for (auto *course : alice.getCourses()) {
    std::cout << "  - " << course->getName() << "\n";
  }

  std::cout << "\nCourse: " << cpp.getName() << "\n";
  std::cout << "Students:\n";
  for (auto *student : cpp.getStudents()) {
    std::cout << "  - " << student->getName() << "\n";
  }

  // ---- Library System ----
  std::cout << "\n6. Real-World Example (Library System):\n";
  Author jkRowling("J.K. Rowling");
  Book harryPotter("978-0439708180", "Harry Potter and Sorcerer's Stone");
  harryPotter.addAuthor(&jkRowling);

  Member member1("M001", "Charlie");
  BorrowRecord record1(&harryPotter, &member1, "2024-01-15");

  std::cout << "Book: " << harryPotter.getTitle() << "\n";
  std::cout << "Author: " << harryPotter.getAuthors()[0]->getName() << "\n";
  std::cout << "Available: " << (harryPotter.isAvailable() ? "No" : "Yes")
            << "\n";
  std::cout << "Borrowed by: " << member1.getName() << " on "
            << record1.getBorrowDate() << "\n";

  // ---- Hospital System ----
  std::cout << "\n7. Real-World Example (Hospital System):\n";
  Doctor drSmith("Dr. Smith", "Cardiology");
  Doctor drPatel("Dr. Patel", "Neurology");

  Patient patientAlice("Alice Johnson", "P001");
  Patient patientDavid("David Brown", "P002");

  Room room101("101", 1);
  Room room205("205", 2);

  Appointment appt1(&drSmith, &patientAlice, &room101, "2024-02-18 09:00");
  Appointment appt2(&drSmith, &patientDavid, &room101, "2024-02-18 10:00");
  Appointment appt3(&drPatel, &patientAlice, &room205, "2024-02-18 14:00");

  std::cout << drSmith.getName() << "'s patients:\n";
  for (auto *p : drSmith.getPatients()) {
    std::cout << "  - " << p->getName() << "\n";
  }

  std::cout << "\n" << patientAlice.getName() << "'s doctors:\n";
  for (auto *d : patientAlice.getDoctors()) {
    std::cout << "  - " << d->getName() << " (" << d->getSpecialization()
              << ")\n";
  }

  std::cout << "\n" << drSmith.getName() << "'s schedule:\n";
  for (auto *appt : drSmith.getAppointments()) {
    std::cout << "  - " << appt->getDateTime() << " with "
              << appt->getPatient()->getName() << " in Room "
              << appt->getRoom()->getNumber() << "\n";
  }

  std::cout << "\n=== Demo Complete ===\n";
  return 0;
}

/*
📘 Key Insights:

1. **Association is the loosest relationship**:
   - Objects can exist independently
   - No ownership or lifecycle dependency
   - Just "uses" or "knows about"

2. **Directionality matters**:
   - Unidirectional: Only one class knows about the other
   - Bidirectional: Both classes know each other (must keep in sync!)

3. **Multiplicity patterns**:
   - 1-to-1: User <-> Profile
   - 1-to-many: Project <-> Issues
   - many-to-many: Student <-> Course

4. **Synchronization in bidirectional associations**:
   - Always update both sides of the relationship
   - Use guard clauses to prevent infinite loops
   - Keep references consistent

5. **Intermediary class pattern**:
   - Use an intermediary class (like Appointment) for many-to-many
   - Allows adding relationship data
   - Simplifies navigation
   - Analogous to join tables in databases

6. **When to use association**:
   ✅ Objects need to communicate
   ✅ Objects can exist independently
   ✅ No ownership relationship
   ✅ Loose coupling desired

Best Practices:
- Prefer unidirectional when possible (simpler)
- Always synchronize bidirectional associations
- Use intermediary classes for complex many-to-many
- Avoid circular dependencies
- Document the multiplicity

Association vs Other Relationships:
- Association: Loosely coupled, independent lifetimes
- Aggregation: "has-a" with shared ownership
- Composition: "has-a" with exclusive ownership, dependent lifetimes
*/
