

class AdderMain {
    public static void main(String[] args) {
        int var1 = 10;
        int var2 = 12345;

        int sum = add(var1, var2);
        int diff = subtract(var1, var2);
    }

    private static int add(int var1, int var2) {
        return var1 + var2;
    }

    static int subtract(int var1, int var2) {
        return var1 - var2;
    }
}