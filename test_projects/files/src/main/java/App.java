import java.io.File;

public class App {
    public static void main(String[] args) {
        File file = new File("test.txt");
        System.out.print("File exists? " + file.exists() + '\n');
    }
}
