//
// =======================================================
// 1. NUMERIC ENUM (BASIC / LEGACY STYLE)
// =======================================================
//

enum Color {
    Red,
    Green,
    Blue
}

// Problems:
// - Treated as numbers
// - Allows meaningless arithmetic
// - Reverse mapping exists at runtime

//
// =======================================================
// 2. NUMERIC ENUM WITH EXPLICIT VALUES
// =======================================================
//

enum CoinEnum {
    Penny = 1,
    Nickel = 5,
    Dime = 10,
    Quarter = 25
}

// Simple usage
const enumTotal = CoinEnum.Dime + CoinEnum.Quarter;

//
// =======================================================
// 3. const enum (COMPILE-TIME ONLY ENUM)
// =======================================================
//

const enum CoinConstEnum {
    Penny = 1,
    Nickel = 5,
    Dime = 10,
    Quarter = 25
}

// Inlined at compile time
const constEnumTotal =
    CoinConstEnum.Dime + CoinConstEnum.Quarter;

//
// =======================================================
// 4. ENUM + BEHAVIOR (LOOKUP FUNCTION)
// =======================================================
//

function coinValue(coin: CoinEnum): number {
    return coin;
}

//
// =======================================================
// 5. ENUM USED IN OOP CONTEXT
// =======================================================
//

class Wallet {
    private total = 0;

    addCoin(coin: CoinEnum) {
        this.total += coinValue(coin);
    }

    getTotal(): number {
        return this.total;
    }
}

const wallet = new Wallet();
wallet.addCoin(CoinEnum.Dime);
wallet.addCoin(CoinEnum.Quarter);

//
// =======================================================
// 6. JAVA-STYLE SINGLETON ENUM EMULATION (CLASS-BASED)
// =======================================================
//

class CoinObject {
    static readonly Penny = new CoinObject(1);
    static readonly Nickel = new CoinObject(5);
    static readonly Dime = new CoinObject(10);
    static readonly Quarter = new CoinObject(25);

    private constructor(private readonly value: number) {}

    getValue(): number {
        return this.value;
    }
}

const objectTotal =
    CoinObject.Dime.getValue() +
    CoinObject.Quarter.getValue();

//
// =======================================================
// 7. MODERN TYPESCRIPT ALTERNATIVE (as const OBJECT)
// =======================================================
//

const Coin = {
    Penny: { value: 1 },
    Nickel: { value: 5 },
    Dime: { value: 10 },
    Quarter: { value: 25 }
} as const;

type Coin = typeof Coin[keyof typeof Coin];

const modernTotal =
    Coin.Dime.value + Coin.Quarter.value;

// ---- Outputs ----
console.log(enumTotal);
console.log(constEnumTotal);
console.log(wallet.getTotal());
console.log(objectTotal);
console.log(modernTotal);
