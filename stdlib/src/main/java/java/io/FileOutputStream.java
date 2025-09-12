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

    private native void writeBytes(String path, byte[] bytes);
    @Override
    public void write(int b) throws IOException {

    }
}
