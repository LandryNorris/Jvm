package java.lang;

public final class System {
    {
        setupPrinter();
    }
    static String lineSeparator() {
        // TODO(Landry): Detect Windows
        return "\n";
    }

    private static native void setupPrinter();
}
