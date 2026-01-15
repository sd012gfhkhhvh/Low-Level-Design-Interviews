class Enum {
    public enum Coin {
        PENNY(1),
        NICKEL(5),
        DIME(10),
        QUARTER(25);

        private final int value;

        Coin(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }
    }

    public static void main(String[] args) {
        int total = Coin.DIME.getValue() + Coin.QUARTER.getValue(); // 35
        System.out.println("Total: " + total);
    }
}