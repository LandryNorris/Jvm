package java.io;

public abstract class OutputStream {
    void close() throws IOException {}
    void flush() throws IOException {}

    void write(byte[] bytes) throws IOException {}
    abstract void write(int b) throws IOException;
}
