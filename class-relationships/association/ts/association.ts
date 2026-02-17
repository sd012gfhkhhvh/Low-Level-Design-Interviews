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
  processPayment(amount: number): void {
    console.log(`💳 Processing payment of $${amount}`);
  }
}

class Order {
  private orderId: string;
  private totalAmount: number;
  private gateway: PaymentGateway; // Order knows about PaymentGateway

  constructor(orderId: string, gateway: PaymentGateway, amount: number) {
    this.orderId = orderId;
    this.gateway = gateway;
    this.totalAmount = amount;
  }

  checkout(): void {
    console.log(`📦 Order ${this.orderId} checking out...`);
    this.gateway.processPayment(this.totalAmount); // Uses the gateway
    console.log("✅ Order complete!");
  }
}

// PaymentGateway doesn't know about Order
// This is unidirectional: Order -> PaymentGateway

// ---- 2.2 BIDIRECTIONAL ASSOCIATION ----
// Both classes know about each other

class Team {
  private name: string;
  private developers: Developer[] = []; // Team knows its developers

  constructor(name: string) {
    this.name = name;
  }

  addDeveloper(dev: Developer): void {
    // Must keep both sides in sync!
    this.developers.push(dev);
    dev.setTeam(this);
  }

  getName(): string {
    return this.name;
  }

  getDevelopers(): Developer[] {
    return this.developers;
  }
}

class Developer {
  private name: string;
  private team: Team | null = null; // Developer knows their team

  constructor(name: string) {
    this.name = name;
  }

  setTeam(team: Team): void {
    this.team = team;
  }

  getTeam(): Team | null {
    return this.team;
  }

  getName(): string {
    return this.name;
  }
}

// Both Team and Developer know about each other
// This is bidirectional: Team <-> Developer

//
// =======================================================
// 3. TYPES BY MULTIPLICITY
// =======================================================

// ---- 3.1 ONE-TO-ONE ASSOCIATION ----
// Each object is linked to exactly one other object

class User {
  private username: string;
  private email: string;
  private profile: Profile | null = null; // Each user has one profile

  constructor(username: string, email: string) {
    this.username = username;
    this.email = email;
  }

  setProfile(profile: Profile): void {
    this.profile = profile;
    profile.setUser(this); // Keep both sides in sync
  }

  getProfile(): Profile | null {
    return this.profile;
  }

  getUsername(): string {
    return this.username;
  }
}

class Profile {
  private avatar: string;
  private bio: string;
  private user: User | null = null; // Each profile belongs to one user

  constructor(avatar: string, bio: string) {
    this.avatar = avatar;
    this.bio = bio;
  }

  setUser(user: User): void {
    this.user = user;
  }

  getUser(): User | null {
    return this.user;
  }

  getBio(): string {
    return this.bio;
  }

  getAvatar(): string {
    return this.avatar;
  }
}

// User (1) <----> (1) Profile
// One-to-one bidirectional association

// ---- 3.2 ONE-TO-MANY ASSOCIATION ----
// One object is linked to multiple objects

class Project {
  private name: string;
  private issues: Issue[] = []; // Project has many issues

  constructor(name: string) {
    this.name = name;
  }

  addIssue(issue: Issue): void {
    this.issues.push(issue);
    issue.setProject(this); // Keep both sides in sync
  }

  getName(): string {
    return this.name;
  }

  getIssues(): Issue[] {
    return this.issues;
  }
}

class Issue {
  private title: string;
  private status: string;
  private project: Project | null = null; // Each issue belongs to one project

  constructor(title: string, status: string) {
    this.title = title;
    this.status = status;
  }

  setProject(project: Project): void {
    this.project = project;
  }

  getProject(): Project | null {
    return this.project;
  }

  getTitle(): string {
    return this.title;
  }

  getStatus(): string {
    return this.status;
  }
}

// Project (1) <----> (*) Issue
// One-to-many bidirectional association

// ---- 3.3 MANY-TO-MANY ASSOCIATION ----
// Multiple objects from both sides are linked

class Student {
  private name: string;
  private courses: Course[] = []; // Student enrolled in many courses

  constructor(name: string) {
    this.name = name;
  }

  enroll(course: Course): void {
    // Check if already enrolled (avoid duplicates)
    if (this.courses.includes(course)) return;

    this.courses.push(course);
    course.addStudent(this); // Keep both sides in sync
  }

  getName(): string {
    return this.name;
  }

  getCourses(): Course[] {
    return this.courses;
  }
}

class Course {
  private name: string;
  private students: Student[] = []; // Course has many students

  constructor(name: string) {
    this.name = name;
  }

  addStudent(student: Student): void {
    // Check if already enrolled (avoid duplicates)
    if (this.students.includes(student)) return;

    this.students.push(student);
    student.enroll(this); // Keep both sides in sync
  }

  getName(): string {
    return this.name;
  }

  getStudents(): Student[] {
    return this.students;
  }
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

class Author {
  private name: string;
  private books: Book[] = [];

  constructor(name: string) {
    this.name = name;
  }

  addBook(book: Book): void {
    if (this.books.includes(book)) return;

    this.books.push(book);
    book.addAuthor(this);
  }

  getName(): string {
    return this.name;
  }

  getBooks(): Book[] {
    return this.books;
  }
}

class Book {
  private isbn: string;
  private title: string;
  private authors: Author[] = [];
  private borrowRecords: BorrowRecord[] = [];

  constructor(isbn: string, title: string) {
    this.isbn = isbn;
    this.title = title;
  }

  addAuthor(author: Author): void {
    if (this.authors.includes(author)) return;

    this.authors.push(author);
    author.addBook(this);
  }

  addBorrowRecord(record: BorrowRecord): void {
    this.borrowRecords.push(record);
  }

  isAvailable(): boolean {
    for (const record of this.borrowRecords) {
      if (!record.isReturned()) {
        return false; // Book is currently borrowed
      }
    }
    return true;
  }

  getISBN(): string {
    return this.isbn;
  }

  getTitle(): string {
    return this.title;
  }

  getAuthors(): Author[] {
    return this.authors;
  }
}

class Member {
  private memberId: string;
  private name: string;
  private borrowRecords: BorrowRecord[] = [];

  constructor(memberId: string, name: string) {
    this.memberId = memberId;
    this.name = name;
  }

  addBorrowRecord(record: BorrowRecord): void {
    this.borrowRecords.push(record);
  }

  getMemberId(): string {
    return this.memberId;
  }

  getName(): string {
    return this.name;
  }

  getBorrowRecords(): BorrowRecord[] {
    return this.borrowRecords;
  }
}

class BorrowRecord {
  private book: Book;
  private member: Member;
  private borrowDate: string;
  private returnDate: string;
  private returned: boolean;

  constructor(book: Book, member: Member, borrowDate: string) {
    this.book = book;
    this.member = member;
    this.borrowDate = borrowDate;
    this.returnDate = "";
    this.returned = false;

    book.addBorrowRecord(this);
    member.addBorrowRecord(this);
  }

  markReturned(returnDate: string): void {
    this.returned = true;
    this.returnDate = returnDate;
  }

  isReturned(): boolean {
    return this.returned;
  }

  getBook(): Book {
    return this.book;
  }

  getMember(): Member {
    return this.member;
  }

  getBorrowDate(): string {
    return this.borrowDate;
  }

  getReturnDate(): string {
    return this.returnDate;
  }
}

//
// =======================================================
// 5. REAL-WORLD EXAMPLE: HOSPITAL APPOINTMENT SYSTEM
// =======================================================
//
// Demonstrates association as intermediary class pattern

class Room {
  private number: string;
  private floor: number;

  constructor(number: string, floor: number) {
    this.number = number;
    this.floor = floor;
  }

  getNumber(): string {
    return this.number;
  }

  getFloor(): number {
    return this.floor;
  }
}

class Doctor {
  private name: string;
  private specialization: string;
  private appointments: Appointment[] = [];

  constructor(name: string, specialization: string) {
    this.name = name;
    this.specialization = specialization;
  }

  addAppointment(appt: Appointment): void {
    this.appointments.push(appt);
  }

  getPatients(): Patient[] {
    const result: Patient[] = [];
    for (const appt of this.appointments) {
      const patient = appt.getPatient();
      if (!result.includes(patient)) {
        result.push(patient);
      }
    }
    return result;
  }

  getName(): string {
    return this.name;
  }

  getSpecialization(): string {
    return this.specialization;
  }

  getAppointments(): Appointment[] {
    return this.appointments;
  }
}

class Patient {
  private name: string;
  private patientId: string;
  private appointments: Appointment[] = [];

  constructor(name: string, patientId: string) {
    this.name = name;
    this.patientId = patientId;
  }

  addAppointment(appt: Appointment): void {
    this.appointments.push(appt);
  }

  getDoctors(): Doctor[] {
    const result: Doctor[] = [];
    for (const appt of this.appointments) {
      const doctor = appt.getDoctor();
      if (!result.includes(doctor)) {
        result.push(doctor);
      }
    }
    return result;
  }

  getName(): string {
    return this.name;
  }

  getPatientId(): string {
    return this.patientId;
  }

  getAppointments(): Appointment[] {
    return this.appointments;
  }
}

class Appointment {
  private doctor: Doctor;
  private patient: Patient;
  private room: Room;
  private dateTime: string;

  constructor(doctor: Doctor, patient: Patient, room: Room, dateTime: string) {
    this.doctor = doctor;
    this.patient = patient;
    this.room = room;
    this.dateTime = dateTime;

    doctor.addAppointment(this);
    patient.addAppointment(this);
  }

  getDoctor(): Doctor {
    return this.doctor;
  }

  getPatient(): Patient {
    return this.patient;
  }

  getRoom(): Room {
    return this.room;
  }

  getDateTime(): string {
    return this.dateTime;
  }
}

//
// =======================================================
// 6. DEMONSTRATION
// =======================================================
//

function main(): void {
  console.log("=== Association in TypeScript Demo ===\n");

  // ---- Unidirectional Association ----
  console.log("1. Unidirectional Association (Order -> PaymentGateway):");
  const gateway = new PaymentGateway();
  const order1 = new Order("ORD001", gateway, 150.0);
  order1.checkout();

  // ---- Bidirectional Association ----
  console.log("\n2. Bidirectional Association (Team <-> Developer):");
  const backend = new Team("Backend Team");
  const dev1 = new Developer("Alice");
  const dev2 = new Developer("Bob");

  backend.addDeveloper(dev1);
  backend.addDeveloper(dev2);

  console.log(`Team: ${backend.getName()}`);
  console.log("Members:");
  for (const dev of backend.getDevelopers()) {
    console.log(`  - ${dev.getName()}`);
  }

  // ---- One-to-One Association ----
  console.log("\n3. One-to-One Association (User <-> Profile):");
  const user = new User("john_doe", "john@example.com");
  const profile = new Profile("avatar.jpg", "Software Engineer");
  user.setProfile(profile);

  console.log(`User: ${user.getUsername()}`);
  console.log(`Bio: ${user.getProfile()?.getBio()}`);

  // ---- One-to-Many Association ----
  console.log("\n4. One-to-Many Association (Project <-> Issues):");
  const project = new Project("Website Redesign");
  const bug1 = new Issue("Login button not working", "Open");
  const bug2 = new Issue("Homepage loads slowly", "In Progress");
  const feature1 = new Issue("Add dark mode", "Open");

  project.addIssue(bug1);
  project.addIssue(bug2);
  project.addIssue(feature1);

  console.log(`Project: ${project.getName()}`);
  console.log("Issues:");
  for (const issue of project.getIssues()) {
    console.log(`  - ${issue.getTitle()} [${issue.getStatus()}]`);
  }

  // ---- Many-to-Many Association ----
  console.log("\n5. Many-to-Many Association (Student <-> Course):");
  const alice = new Student("Alice");
  const bob = new Student("Bob");

  const cpp = new Course("C++ Programming");
  const ds = new Course("Data Structures");

  alice.enroll(cpp);
  alice.enroll(ds);
  bob.enroll(cpp);

  console.log(`Student: ${alice.getName()}`);
  console.log("Enrolled in:");
  for (const course of alice.getCourses()) {
    console.log(`  - ${course.getName()}`);
  }

  console.log(`\nCourse: ${cpp.getName()}`);
  console.log("Students:");
  for (const student of cpp.getStudents()) {
    console.log(`  - ${student.getName()}`);
  }

  // ---- Library System ----
  console.log("\n6. Real-World Example (Library System):");
  const jkRowling = new Author("J.K. Rowling");
  const harryPotter = new Book(
    "978-0439708180",
    "Harry Potter and the Sorcerer's Stone",
  );
  harryPotter.addAuthor(jkRowling);

  const member1 = new Member("M001", "Charlie");
  const record1 = new BorrowRecord(harryPotter, member1, "2024-01-15");

  console.log(`Book: ${harryPotter.getTitle()}`);
  console.log(`Author: ${harryPotter.getAuthors()[0].getName()}`);
  console.log(`Available: ${harryPotter.isAvailable() ? "No" : "Yes"}`);
  console.log(
    `Borrowed by: ${member1.getName()} on ${record1.getBorrowDate()}`,
  );

  // ---- Hospital System ----
  console.log("\n7. Real-World Example (Hospital System):");
  const drSmith = new Doctor("Dr. Smith", "Cardiology");
  const drPatel = new Doctor("Dr. Patel", "Neurology");

  const patientAlice = new Patient("Alice Johnson", "P001");
  const patientDavid = new Patient("David Brown", "P002");

  const room101 = new Room("101", 1);
  const room205 = new Room("205", 2);

  const appt1 = new Appointment(
    drSmith,
    patientAlice,
    room101,
    "2024-02-18 09:00",
  );
  const appt2 = new Appointment(
    drSmith,
    patientDavid,
    room101,
    "2024-02-18 10:00",
  );
  const appt3 = new Appointment(
    drPatel,
    patientAlice,
    room205,
    "2024-02-18 14:00",
  );

  console.log(`${drSmith.getName()}'s patients:`);
  for (const p of drSmith.getPatients()) {
    console.log(`  - ${p.getName()}`);
  }

  console.log(`\n${patientAlice.getName()}'s doctors:`);
  for (const d of patientAlice.getDoctors()) {
    console.log(`  - ${d.getName()} (${d.getSpecialization()})`);
  }

  console.log(`\n${drSmith.getName()}'s schedule:`);
  for (const appt of drSmith.getAppointments()) {
    console.log(
      `  - ${appt.getDateTime()} with ${appt.getPatient().getName()} in Room ${appt.getRoom().getNumber()}`,
    );
  }

  console.log("\n=== Demo Complete ===");
}

// Execute
main();

export {};

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

6. **TypeScript advantages**:
   - Type safety with interfaces
   - Optional types (Type | null)
   - Array.includes() for guard clauses
   - Cleaner syntax than C++

Best Practices:
- Prefer unidirectional when possible (simpler)
- Always synchronize bidirectional associations
- Use intermediary classes for complex many-to-many
- Leverage TypeScript's type system for safety
- Document the multiplicity clearly

When to use association:
✅ Objects need to communicate
✅ Objects can exist independently
✅ No ownership relationship
✅ Loose coupling desired

TypeScript-Specific:
- Use `Type | null` for optional associations
- Leverage `includes()` for guard checks
- Type safety prevents invalid associations
- Interfaces can define association contracts
*/
