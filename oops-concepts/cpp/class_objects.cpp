#include <iostream>
#include <string>
using namespace std;

class Car
{
private:
    // Attribures/properties
    string brand;
    string model;
    int speed{0}; // In-class initialization

public:
    // Constructor
    Car(const string &brand, const string &model) : brand(brand), model(model)
    {
    }

    // Method to accelerate
    void accelerate(int increment)
    {
        this->speed += increment;
    }
    // Method to display info
    void displayStatus() const
    {
        cout << this->brand << " is running at " << this->speed << " km/h." << endl;
    }
};

int main()
{
    // Creating objects instances of the Car class
    Car sierra("Tata", "Sierra");
    Car hilux("Toyota", "Hilux");

    sierra.accelerate(20);
    hilux.accelerate(50);

    // Displaying status of each car
    sierra.displayStatus();
    cout << "-----------------" << endl;
    hilux.displayStatus();
}

/*
📘 Learning Note: const Correctness in This Code

Use const for parameters passed by reference when the function does not modify them
(e.g., const string& brand) — this prevents accidental changes and avoids costly copies.

Mark member functions as const if they do not change the object’s state
(e.g., displayStatus() const) — this enforces read-only behavior and allows calls on const objects.

Do NOT mark member functions as const if they modify data members
(e.g., accelerate() modifies speed).

Pass small fundamental types (like int) by value, not by const reference
(use int increment, not const int& increment) — it is simpler, clearer, and more efficient.

Rule of Thumb:

Pass small types by value, large or complex types by const&.
Use const wherever modification is not intended.
*/