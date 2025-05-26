package java.lang;

import java.io.Serializable;

public class String implements Serializable, Comparable<String>, CharSequence {
    final byte[] value;

    public String() {
        this.value = "".value;
    }

    public String(String original) {
        this.value = original.value;
    }

    public String(byte[] value) {
        this.value = value;
    }

    public String(char[] chars) {
        this.value = new byte[chars.length];

        for(int i = 0; i < chars.length; i++) {
            this.value[i] = (byte) chars[i];
        }
    }

    @Override
    public int compareTo(String o) {
        return 0;
    }

    @Override
    public char charAt(int index) {
        return (char)value[index];
    }

    @Override
    public int length() {
        return value.length;
    }

    @Override
    public CharSequence subSequence(int start, int end) {
        // TODO(Landry): Implement fully
        return this;
    }

    @Override
    public String toString() {
        return this;
    }
}
