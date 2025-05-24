package java.lang;

public interface CharSequence {
    char charAt(int index);
    // TODO(Landry): Implement chars() and codePoints()

    int length();
    CharSequence subSequence(int start, int end);
    String toString();
}
