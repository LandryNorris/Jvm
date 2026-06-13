package java.io;

public class FileWriter extends OutputStreamWriter {
    public FileWriter(File file) throws IOException {
        super(new FileOutputStream(file));
    }

    public FileWriter(String path) throws IOException {
        this(new File(path));
    }
}

