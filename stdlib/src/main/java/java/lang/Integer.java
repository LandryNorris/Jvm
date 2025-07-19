package java.lang;

public class Integer {
    private int value;

    public static int BYTES = 4;
    public static int MAX_VALUE = 0x7FFFFFFF;
    public static int MIN_VALUE = -0x80000000;
    public static int SIZE = 32;

    Integer(int value) {
        this.value = value;
    }

    Integer(String s) {
        this.value = valueOfInternal(s);
    }

    @Override
    public String toString() {
        return toStringInternal(value, 10);
    }

    private static int valueOfInternal(String s) {
        int value = 0;
        int power = 1;
        boolean isNegative = false;
        for(int i = s.length()-1; i >= 0; i--) {
            char c = s.charAt(i);
            if(i == 0 && c == '-') {
                isNegative = true;
                continue;
            }

            // TODO(Landry): Handle invalid string
            if(c >= '0' && c <= '9') {
                int thisDigit = c - '0';
                value |= thisDigit * power;
                power *= 10;
            }
        }

        return value * (isNegative ? -1 : 1);
    }

    private static String toStringInternal(int i, int base) {
        StringBuilder builder = new StringBuilder(11); // integers are at most 10 digits + 1 for -

        while(i > 0) {
            int lowestDigit = i % base;
            i /= base; // TODO(Landry): Use the provided value

            char c = (char) ('0' + lowestDigit);
            builder.append(c);
        }

        if(i < 0) {
            builder.append('-');
        }

        return builder.toString();
    }

    static Integer valueOf(String s) {
        return new Integer(s);
    }

    static Integer valueOf(int i) {
        return new Integer(i);
    }

    static String toString(int i, int base) {
        return toStringInternal(i, base);
    }

    static String toString(int i) {
        return toStringInternal(i, 10);
    }
}
