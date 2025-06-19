
public class App {
    public static void main(String[] args) {
        System.out.println("Starting");
        printNull();
        printAppendedText();
        printStringFromCharArray();
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
}
