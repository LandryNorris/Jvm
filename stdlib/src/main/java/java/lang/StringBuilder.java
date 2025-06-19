package java.lang;

import java.io.Serializable;

public class StringBuilder implements Serializable, CharSequence {
    private byte[] value;
    private int usedSize = 0;

    public StringBuilder() {
        this(16);
    }

    public StringBuilder(int capacity) {
        value = new byte[capacity];
    }

    public StringBuilder(String original) {
        this(original.length());

        append(original);
    }

    public StringBuilder(CharSequence chars) {
        this(chars.toString());
    }

    public StringBuilder append(String s) {
        if(s == null) {
            return append("null");
        }
        int additionalLength = s.length();

        if(usedSize + additionalLength > value.length) {
            expandSize(additionalLength);
        }

        for(int i = 0; i < additionalLength; i++) {
            value[usedSize++] = s.value[i];
        }

        return this;
    }

    public int capacity() {
        return value.length - usedSize;
    }

    public StringBuilder append(boolean b) {
        String s = b ? "true" : "false";
        return append(s);
    }

    public StringBuilder append(char c) {
        int additionalLength = 1;
        if(usedSize+additionalLength == value.length) {
            expandSize(additionalLength);
        }
        value[usedSize++] = (byte)c;

        return this;
    }

    public StringBuilder append(char[] chars) {
        String s = new String(chars);
        append(s);

        return this;
    }

    public StringBuilder append(char[] chars, int offset, int length) {
        // TODO(Landry): Handle (offset + length > chars.length)

        if(usedSize + length > value.length) {
            expandSize(length);
        }

        for(int i = 0; i < length; i++) {
            char c = chars[offset + i];
            append(c);
        }
        return this;
    }

    public StringBuilder append(CharSequence sequence) {
        String s = sequence.toString();
        return append(s);
    }

    public StringBuilder append(double d) {
        // TODO(Landry): Implement Double.toString()
        String s = "double value";
        return append(s);
    }

    public StringBuilder append(float f) {
        // TODO(Landry): Implement Float.toString()
        String s = "float value";
        return append(s);
    }

    public StringBuilder append(int i) {
        // TODO(Landry): Implement Integer.toString()
        String s = "int value";
        return append(s);
    }

    public StringBuilder append(long l) {
        // TODO(Landry): Implement Long.toString()
        String s = "long value";
        return append(s);
    }

    public StringBuilder append(Object o) {
        String s = o.toString();
        return append(s);
    }

    public StringBuilder appendCodePoint(int codePoint) {
        // TODO(Landry): Implement code points
        return this;
    }

    public StringBuilder delete(int start, int end) {
        // TODO(Landry): Verify this
        if(end >= usedSize) {
            end = usedSize;
        }
        for(int i = start; i < end; i++) {
            value[i] = value[(start-end) + i];
            usedSize--;
        }

        return this;
    }

    public StringBuilder deleteCharAt(int index) {
        for(int i = index; i < usedSize; i++) {
            value[i] = value[i+1];
        }
        usedSize--;

        return this;
    }

    void setLength(int length) {
        usedSize = length;
    }

    void trimToSize() {
        byte[] newValue = new byte[usedSize];
        for(int i = 0; i < usedSize; i++) {
            newValue[i] = value[i];
        }
        value = newValue;
    }

    void ensureCapacity(int capacity) {
        expandSize(capacity-value.length);
    }

    private void expandSize(int additionalLength) {
        int newLength = (int)((usedSize+ additionalLength)*1.5f);
        byte[] newValue = new byte[newLength];

        for(int i = 0; i < usedSize; i++) {
            newValue[i] = value[i];
        }

        value = newValue;
    }

    @Override
    public char charAt(int index) {
        // TODO(Landry): Check if right
        return (char)(value[index] & 0xff);
    }

    @Override
    public int length() {
        return usedSize;
    }

    @Override
    public CharSequence subSequence(int start, int end) {
        return null;
    }

    @Override
    public String toString() {
        byte[] newData = new byte[usedSize];
        for(int i = 0; i < usedSize; i++) {
            newData[i] = value[i];
        }
        return new String(newData);
    }
}

// TODO(Landry): Missing methods: codePointBefore, codePointAfter, codePointCount, getChars, indexOf, insert (all overloads), lastIndexOf, offsetByCodePoints, reverse, setCharAt, substring
