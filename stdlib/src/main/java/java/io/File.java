package java.io;

public class File {
    private int fileDescriptor = -1;
    private int permissions;

    public File(File parent, String child) {
        // TODO(Landry): Implement
    }

    public File(String parent, String child) {
        // TODO(Landry): Use System.lineSeparator
        this(parent + "/" + child);
    }

    public File(String pathname) {
        // TODO(Landry): Open file lazily when I have other methods
        int fd = openFile(pathname);
    }

    private native int openFile(String path);
    private native int getPermissions(int fd);

    public static File[] listRoots() {
        // TODO(Landry): Detect OS and support Windows
        return new File[] { new File("/") };
    }
}
