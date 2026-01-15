#include <iostream>
using namespace std;


// ref - https://www.geeksforgeeks.org/cpp/operator-overloading-cpp/
class Fraction
{
private:
    int neu;
    int den;

public:
    Fraction()
    {
    }
    Fraction(int neu, int den)
    {
        this->neu = neu;
        this->den = den;
    }

    void display()
    {
        cout << this->neu << " / " << this->den << endl;
    }

    Fraction sum(Fraction fr)
    {
        int newNeu = this->neu * fr.den + fr.neu * this->den;
        int newDen = this->den * fr.den;

        Fraction f(newNeu, newDen);
        return f;
    }
    // OR ---------
    // operator overloading
    Fraction operator+(Fraction fr)
    {
        int newNeu = this->neu * fr.den + fr.neu * this->den;
        int newDen = this->den * fr.den;

        Fraction f(newNeu, newDen);
        return f;
    }
    Fraction operator-(Fraction fr)
    {
        int newNeu = this->neu * fr.den - fr.neu * this->den;
        int newDen = this->den * fr.den;

        Fraction f(newNeu, newDen);
        return f;
    }
};

int main()
{
    Fraction f1(1, 2);
    Fraction f2(1, 3);

    Fraction f3 = f1 + f2; // same as Fraction f3 = f1.sum(f2);
    f3.display();
    return 0;
}