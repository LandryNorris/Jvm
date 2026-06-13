package java.io;

public class FileOutputStream extends OutputStream {
    private File file;

    public FileOutputStream(File file) throws IOException {
        this(file, false);
    }

    public FileOutputStream(File file, boolean append) throws IOException {
        this.file = file;
        if(!append) {
            file.delete();
        }
        file.createNewFile();
    }

    private native void writeBytesInternal(int fd, byte[] bytes);
    private native void writeByteInternal(int fd, byte b);
    private native void closeInternal(int fd);

    @Override
    public void write(int b) throws IOException {
        writeByteInternal(file.fileDescriptor, (byte) b);
    }

    @Override
    void write(byte[] bytes) throws IOException {
        writeBytesInternal(file.fileDescriptor, bytes);
    }

    @Override
    void close() throws IOException {
        closeInternal(file.fileDescriptor);
    }
}
