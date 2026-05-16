public class InheritanceApp {
    public static void main(String[] args) {
        Bar bar = new Bar();

        System.out.println("Printing bar fields");
        System.out.println(bar.barStringField);
        System.out.println(bar.barFloatField);

        System.out.println("Printing foo fields of bar");
        System.out.println(bar.fooStringField);
        System.out.println(bar.fooIntField);
    }
}
