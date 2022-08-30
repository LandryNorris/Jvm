
class Main {
    public static void main(String[] args) {
        Averager averager = new Averager();

        averager.addValue(7);
        averager.addValue(14);
        averager.addValue(100);
        averager.addValue(-2000);

        int average = averager.getAverage();
    }
}