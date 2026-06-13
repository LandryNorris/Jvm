package java.io;

public class File {
    // TODO(Landry): Handle Windows
    public static final String separator = "/";
    public static final char pathSeparatorChar = '/';
    int fileDescriptor = -1;
    private int permissions;
    private String pathname;

    public File(File parent, String child) {
        // TODO(Landry): Implement
    }

    public File(String parent, String child) {
        this(parent + File.pathSeparatorChar + child);
    }

    public File(String pathname) {
        this.pathname = pathname;
    }

    private native int openFile(String path);
    private native int getPermissions(int fd);
    private native int existsInternal(String path);
    private native int createNewFileInternal(String path);
    private native int deleteInternal(String path);

    public static File[] listRoots() {
        // TODO(Landry): Detect OS and support Windows
        return new File[] { new File("/") };
    }

    public boolean exists() {
        return existsInternal(pathname) != 0;
    }

    public boolean createNewFile() {
        fileDescriptor = createNewFileInternal(pathname);
        return fileDescriptor != -1;
    }

    public boolean delete() {
        return deleteInternal(pathname) != 0;
    }

    public String getPath() {
        return pathname;
    }
}
