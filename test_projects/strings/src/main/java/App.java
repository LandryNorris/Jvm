
public class App {
    public static void main(String[] args) {
        printNull();
        printAppendedText();
        printStringFromCharArray();
        printIntegers();
    }

    private static void testArgPassing(int i1, int i2, int i3) {
        System.out.println(i1);
        System.out.println(i2);
        System.out.println(i3);
    }

    private static void printNull() {
        String s = null;
        StringBuilder builder = new StringBuilder();
        builder.append(s);
        System.out.println(builder.toString());
    }

    private static void printAppendedText() {
        String s = "some text";
        String added = s + " and some other text";
        System.out.println(added);
    }

    private static void printStringFromCharArray() {
        String manual = new String(new char[] { 'a', 'b', 'c' });
        System.out.println(manual);
    }

    private static void printIntegers() {
        Integer zero = Integer.valueOf(0);
        Integer one = Integer.valueOf(1);
        Integer twelve = Integer.valueOf(12);
        Integer fiveHundredFortySix = Integer.valueOf(546);
        Integer negativeSixThousandFourHundredTwentyOne = Integer.valueOf(-6421);

        System.out.println(zero);
        System.out.println(one);
        System.out.println(twelve);
        System.out.println(fiveHundredFortySix);
        System.out.println(negativeSixThousandFourHundredTwentyOne);
    }
}
