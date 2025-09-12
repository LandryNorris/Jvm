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

    private native void writeBytesInternal(String path, byte[] bytes);
    private native void writeByteInternal(String path, byte b);

    @Override
    public void write(int b) throws IOException {
        writeByteInternal(file.getPath(), (byte) b);
    }

    @Override
    void write(byte[] bytes) throws IOException {
        writeBytesInternal(file.getPath(), bytes);
    }
}
