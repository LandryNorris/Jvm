package java.io;

public abstract class OutputStreamWriter {
    private OutputStream out;

    OutputStreamWriter(OutputStream out) {
        this.out = out;
    }

    void close() throws IOException {
        out.close();
    }
    void flush() throws IOException {
        out.flush();
    }
}
