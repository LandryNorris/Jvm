package java.lang;

import java.io.PrintStream;

public final class System {
    static {
        setupPrinter();
    }
    static String lineSeparator() {
        // TODO(Landry): Detect Windows
        return "\n";
    }

    static PrintStream out;

    private static native void setupPrinter();
}
