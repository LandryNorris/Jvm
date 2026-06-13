import java.io.File;
import java.io.IOException;
import java.io.FileWriter;

public class App {
    public static void main(String[] args) throws IOException {
        File file = new File("test.txt");
        System.out.println("File exists? " + file.exists());
        boolean created = file.createNewFile();
        System.out.println("File was created new? " + created);

        FileWriter writer = new FileWriter(file);
        writer.write("Some text");
        writer.close();
        boolean deleted = file.delete();
        System.out.println("File was deleted? " + deleted);
    }
}
