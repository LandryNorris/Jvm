package java.io;

public class File {
    // TODO(Landry): Handle Windows
    public static final String separator = "/";
    public static final char pathSeparatorChar = '/';
    private int fileDescriptor = -1;
    private int permissions;

    public File(File parent, String child) {
        // TODO(Landry): Implement
    }

    public File(String parent, String child) {
        this(parent + File.pathSeparatorChar + child);
    }

    public File(String pathname) {
        // TODO(Landry): Open file lazily when I have other methods
        fileDescriptor = openFile(pathname);
    }

    private native int openFile(String path);
    private native int getPermissions(int fd);

    public static File[] listRoots() {
        // TODO(Landry): Detect OS and support Windows
        return new File[] { new File("/") };
    }
}
