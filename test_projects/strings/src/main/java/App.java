
public class App {
    public static void main(String[] args) {
        Integer i = 60;
        String s = i.toString();
        //System.out.println("Starting");
        //printNull();
        //printAppendedText();
        //printStringFromCharArray();
        // printIntegers();
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
        Integer i = new Integer(243843);
        System.out.println(i);
//         Integer zero = Integer.valueOf("0");
//         Integer one = Integer.valueOf("1");
//         Integer twelve = Integer.valueOf("12");
//         Integer fiveHundredFortySix = Integer.valueOf("546");
//         Integer negativeSixThousandFourHundredTwentyOne = Integer.valueOf("-6421");
//
//         System.out.println(zero);
//         System.out.println(one);
//         System.out.println(twelve);
//         System.out.println(fiveHundredFortySix);
//         System.out.println(negativeSixThousandFourHundredTwentyOne);
    }
}
