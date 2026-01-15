#include <iostream>
#include <string>

// Ref - https://www.geeksforgeeks.org/cpp/enum-classes-in-c-and-their-advantage-over-enum-datatype/

//
// =======================================================
// 1. UNscoped ENUM (Legacy C++ Style)
// =======================================================
//

// Old-style enum (avoid in modern C++)
enum Color {
    Red,
    Green,
    Blue
};

//
// =======================================================
// 2. SCOPED ENUM (enum class — Modern C++)
// =======================================================
//

// Strongly typed enum (recommended)
enum class Coin {
    Penny,
    Nickel,
    Dime,
    Quarter
};

//
// =======================================================
// 3. ENUM + BEHAVIOR (Helper Functions)
// =======================================================
//

// Attach meaning via free functions
constexpr int coinValue(Coin coin) {
    switch (coin) {
        case Coin::Penny:   return 1;
        case Coin::Nickel:  return 5;
        case Coin::Dime:    return 10;
        case Coin::Quarter: return 25;
    }
    return 0; // defensive
}

std::string toString(Coin coin) {
    switch (coin) {
        case Coin::Penny:   return "Penny";
        case Coin::Nickel:  return "Nickel";
        case Coin::Dime:    return "Dime";
        case Coin::Quarter: return "Quarter";
    }
    return "Unknown";
}

//
// =======================================================
// 4. ENUM USED IN OBJECT-ORIENTED CODE
// =======================================================
//

class Wallet {
public:
    void addCoin(Coin coin) {
        total_ += coinValue(coin);
    }

    int total() const {
        return total_;
    }

private:
    int total_ = 0;
};

//
// =======================================================
// 5. JAVA-STYLE SINGLETON ENUM EMULATION (NOT RECOMMENDED)
// =======================================================
//

class CoinObject {
public:
    static const CoinObject Penny;
    static const CoinObject Nickel;
    static const CoinObject Dime;
    static const CoinObject Quarter;

    int value() const { return value_; }

private:
    const int value_;

    // Private constructor prevents new instances
    constexpr explicit CoinObject(int v) : value_(v) {}
};

// Static object definitions
constexpr CoinObject CoinObject::Penny{1};
constexpr CoinObject CoinObject::Nickel{5};
constexpr CoinObject CoinObject::Dime{10};
constexpr CoinObject CoinObject::Quarter{25};

//
// =======================================================
// 6. DEMONSTRATION
// =======================================================
//

int main() {

    // ---- Legacy enum problems ----
    Color c = Red;
    int x = c;  // implicit conversion (dangerous)
    std::cout << "Legacy enum converted to int: " << x << "\n";

    // ---- enum class usage ----
    Coin coin1 = Coin::Dime;
    Coin coin2 = Coin::Quarter;

    std::cout << toString(coin1)
              << " value = "
              << coinValue(coin1)
              << "\n";

    // ---- enum used in OOP ----
    Wallet wallet;
    wallet.addCoin(coin1);
    wallet.addCoin(coin2);

    std::cout << "Wallet total (cents): "
              << wallet.total()
              << "\n";

    // ---- Java-style singleton object usage ----
    int objTotal = CoinObject::Dime.value()
                 + CoinObject::Quarter.value();

    std::cout << "Object-based coin total: "
              << objTotal
              << "\n";

    return 0;
}