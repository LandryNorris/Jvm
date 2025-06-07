package java.lang;

public final class System {
    static {
        setupPrinter();
    }
    static String lineSeparator() {
        // TODO(Landry): Detect Windows
        return "\n";
    }

    private static native void setupPrinter();
}
