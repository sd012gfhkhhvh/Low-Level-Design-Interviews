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
// - If the Department closes, the Professors still exist and can join other departments.
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
  private name: string;

  constructor(name: string) {
    this.name = name;
  }

  getName(): string {
    return this.name;
  }
}

class Department {
  private name: string;
  // The Department "has-a" collection of Professors,
  // but stores references to represent weak ownership.
  private professors: Professor[] = [];

  constructor(name: string) {
    this.name = name;
  }

  // Add an existing professor to the department
  addProfessor(prof: Professor): void {
    this.professors.push(prof);
  }

  printProfessors(): void {
    console.log(`🏫 Professors in ${this.name} Department:`);
    this.professors.forEach((prof) => {
      console.log(`  👨‍🏫 ${prof.getName()}`);
    });
  }
}

//
// =======================================================
// 3. SHARING PARTS: PLAYERS AND TEAMS
// =======================================================
//
// In aggregation, the parts can be shared among multiple wholes.
// A Player can belong to a National Team and a Club Team simultaneously.

class Player {
  private name: string;

  constructor(name: string) {
    this.name = name;
  }

  getName(): string {
    return this.name;
  }
}

class Team {
  private teamName: string;
  private roster: Player[] = [];

  constructor(name: string) {
    this.teamName = name;
  }

  addPlayer(player: Player): void {
    this.roster.push(player);
  }

  displayTeam(): void {
    console.log(`⚽ Team: ${this.teamName}`);
    this.roster.forEach((player) => {
      console.log(`  - ${player.getName()}`);
    });
  }
}

//
// =======================================================
// 4. DEPENDENCY INJECTION AS AGGREGATION
// =======================================================
//
// From a design perspective, when you pass dependencies via a constructor,
// you are often setting up an aggregation. The injected dependency exists
// outside the class and is just used by it.

class Logger {
  log(message: string): void {
    console.log(`[LOG] ${message}`);
  }
}

class WebServer {
  private logger: Logger; // WebServer uses Logger, but doesn't own it globally

  // Logger is injected from outside
  constructor(logger: Logger) {
    this.logger = logger;
  }

  handleRequest(): void {
    this.logger.log("Handling an incoming HTTP request");
    console.log("🌐 Response sent.");
  }
}

//
// =======================================================
// 5. REAL-WORLD EXAMPLE: MUSIC LIBRARY SYSTEM
// =======================================================
//
// Let's model a realistic music system demonstrating aggregation at multiple levels.
// - Artists create Songs
// - Playlists aggregate Songs (a Song can be in multiple Playlists)
// - Library acts as the authoritative source of everything.

class Artist {
  private name: string;

  constructor(name: string) {
    this.name = name;
  }

  getName(): string {
    return this.name;
  }
}

class Song {
  private title: string;
  private artist: Artist; // Aggregation: Song points to its Artist
  private durationSecs: number;

  constructor(title: string, artist: Artist, duration: number) {
    this.title = title;
    this.artist = artist;
    this.durationSecs = duration;
  }

  getTitle(): string {
    return this.title;
  }
  getArtist(): Artist {
    return this.artist;
  }
  getDuration(): number {
    return this.durationSecs;
  }

  toString(): string {
    return `🎵 ${this.title} by ${this.artist.getName()} (${this.durationSecs}s)`;
  }
}

class Playlist {
  private name: string;
  // Aggregation: Playlist contains references to Songs.
  private songs: Song[] = [];

  constructor(name: string) {
    this.name = name;
  }

  addSong(song: Song): void {
    this.songs.push(song);
  }

  removeSong(song: Song): void {
    this.songs = this.songs.filter((s) => s !== song);
  }

  getSongCount(): number {
    return this.songs.length;
  }

  getTotalDuration(): number {
    return this.songs.reduce((total, song) => total + song.getDuration(), 0);
  }

  getName(): string {
    return this.name;
  }

  getSongs(): Song[] {
    return this.songs;
  }
}

class Library {
  private masterSongs: Song[] = [];

  addSongToLibrary(song: Song): void {
    this.masterSongs.push(song);
  }

  getTotalSongs(): number {
    return this.masterSongs.length;
  }
}

//
// =======================================================
// 6. DEMONSTRATION
// =======================================================
//

function main() {
  console.log("=== Aggregation in TypeScript Demo ===\n");

  // ---- 1. Department and Professors ----
  console.log("--- 1. Department and Professors ---");
  // 1. Create Parts independently
  const p1 = new Professor("Dr. Smith");
  const p2 = new Professor("Dr. Johnson");

  // 2. Create Whole
  let csDept: Department | null = new Department("Computer Science");

  // 3. Assemble
  csDept.addProfessor(p1);
  csDept.addProfessor(p2);

  csDept.printProfessors();

  // 4. Destroy Whole (Simulating destruction by nullifying the reference)
  console.log("Deleting Department...");
  csDept = null;

  // 5. Parts still exist!
  console.log("Department deleted, but Professors still exist:");
  console.log(`  ${p1.getName()} is looking for a new job.\n`);

  // ---- 2. Shared Parts: Player in Multiple Teams ----
  console.log("--- 2. Shared Parts: Teams & Players ---");
  const messi = new Player("Lionel Messi");

  const national = new Team("Argentina");
  const club = new Team("Inter Miami");

  // The same player is aggregated into multiple teams
  national.addPlayer(messi);
  club.addPlayer(messi);

  national.displayTeam();
  club.displayTeam();
  console.log("");

  // ---- 3. Dependency Injection ----
  console.log("--- 3. Dependency Injection ---");
  const sharedLogger = new Logger();

  const server1 = new WebServer(sharedLogger);
  const server2 = new WebServer(sharedLogger);

  server1.handleRequest();
  server2.handleRequest();
  console.log("");

  // ---- 4. Real-World Music Library ----
  console.log("--- 4. Music Library System ---");

  // Create Artists (Independent entities)
  const coldplay = new Artist("Coldplay");
  const adele = new Artist("Adele");

  // Create Songs
  const yellow = new Song("Yellow", coldplay, 269);
  const clocks = new Song("Clocks", coldplay, 307);
  const hello = new Song("Hello", adele, 295);

  // Add to main library
  const myLib = new Library();
  myLib.addSongToLibrary(yellow);
  myLib.addSongToLibrary(clocks);
  myLib.addSongToLibrary(hello);

  // Create Playlists (Wholes)
  let workout: Playlist | null = new Playlist("Workout Mix");
  const chill = new Playlist("Chill Vibes");

  // Aggregate Songs (Parts) into Playlists
  workout.addSong(yellow);
  workout.addSong(clocks);

  chill.addSong(yellow); // Notice 'Yellow' is shared!
  chill.addSong(hello);

  console.log(`${workout.getName()} (${workout.getSongCount()} songs)`);
  workout.getSongs().forEach((s) => console.log(`  ${s.toString()}`));

  console.log(`${chill.getName()} (${chill.getSongCount()} songs)`);
  chill.getSongs().forEach((s) => console.log(`  ${s.toString()}`));

  // Deleting a playlist doesn't affect the songs or library
  console.log(`Deleting playlist '${workout.getName()}'...`);
  workout = null; // Simulating garbage collection of the playlist

  console.log(
    `Library still has ${myLib.getTotalSongs()} songs. 'Yellow' is still available!`,
  );

  console.log("\n=== Demo Complete ===");
}

main();
export {};

/*
📘 Key Insights:

1. **Weak Ownership**:
   - The "whole" class holds the "parts" but does not own them absolutely.
   - Example: Department has an array of Professor objects.

2. **Independent Lifecycles**:
   - The part can exist before the whole is created.
   - The part survives after the whole is nullified or garbage collected.
   - (In TypeScript, Garbage Collection handles the memory, but architecturally the references persist elsewhere).

3. **Part Sharing**:
   - In aggregation, a single part can belong to multiple wholes simultaneously.
   - Example: A Player object can belong to a Club Team and a National Team.

4. **Implementation Strategy in TypeScript**:
   - Passing arguments to constructors (`Dependency Injection`) is a form of setting up aggregation.
   - It's represented internally as an Array (`Type[]`) or simple property references (`Type`).

5. **Aggregation vs Association vs Composition**:
   - Association: A uses B. (No whole/part context)
   - Aggregation: A has B, but B can exist without A. (Whole/part, weak ownership)
   - Composition: A owns B, B dies if A dies. (Whole/part, strong ownership - often instantiated inside the constructor)
*/
