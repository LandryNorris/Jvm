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

    public StringBuilder append(String s) {
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
