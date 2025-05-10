package java.lang;

public class Object {
    public Object() {}

    public int hashCode() {
        return 0;
    }

    protected Object clone() {
        // TODO(Landry): Proper clone
        return this;
    }

    boolean equals(Object other) {
        return this == other;
    }

    public String toString() {
        //TODO(Landry): Implement standard
        // toString() = getClass().getName() + '@' + Integer.toHexString(hashCode())
        return "[Object object]";
    }
}
