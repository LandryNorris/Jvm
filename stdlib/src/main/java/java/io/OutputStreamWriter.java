package java.io;

public abstract class OutputStreamWriter {
    private OutputStream out;

    OutputStreamWriter(OutputStream out) {
        this.out = out;
    }

    public void write(String s) throws IOException {
        out.write(s.getBytes());
    }

    public void close() throws IOException {
        out.close();
    }
    public void flush() throws IOException {
        out.flush();
    }
}
