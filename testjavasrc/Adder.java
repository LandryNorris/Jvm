

class AdderMain {

	int classLevelVar1 = 123;
	long classLevelLong = 9223372036854775800L;
	double classLevelDouble = 123.456;
	float classLevelFloat = 789.012f;
	String classLevelString = "Testing a String";

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
