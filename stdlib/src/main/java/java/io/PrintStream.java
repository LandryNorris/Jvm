package java.io;

public class PrintStream {
    void print(String s) {

    }

    void println(String s) {
        print(s);
        print("\n");
    }

    void println(Object o) {
        print(o.toString());
        print("\n");
    }
}
