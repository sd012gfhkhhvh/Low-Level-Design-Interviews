class Car {
    // Attributes (private by default in TypeScript with private keyword)
    private brand: string;
    private model: string;
    private speed: number = 0;

    // Constructor
    constructor(brand: string, model: string) {
        this.brand = brand;
        this.model = model;
    }

    //   ---- Or use 'parameter property constructor' syntax to declares and initializes class properties directly from constructor parameters ----
    //  constructor(
    //     private brand: string,
    //     private model: string,
    //     private speed: number = 0
    // ) {}

    // Method to accelerate
    accelerate(increment: number): void {
        this.speed += increment;
    }

    // Method to display info
    displayStatus(): void {
        console.log(`${this.brand} is running at ${this.speed} km/h.`);
    }
}

// Creating and using objects
function main(): void {
    // Creating objects of the Car class
    const sierra = new Car("Toyota", "Sierra");
    const hilux = new Car("Ford", "Hilux");

    sierra.accelerate(20);
    hilux.accelerate(40);

    // Displaying status of each car
    sierra.displayStatus();
    console.log("-----------------");
    hilux.displayStatus();
}

// Execute the main function
main();
