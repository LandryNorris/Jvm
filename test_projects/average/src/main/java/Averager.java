
class Averager {
    int sum = 0;
    int count = 0;

    void addValue(int value) {
        sum += value;
        count++;
    }

    int getAverage() {
        return sum/count;
    }

    public static void main(String[] args) {
        Averager averager = new Averager();

        averager.addValue(10);
        averager.addValue(20);

        int average = averager.getAverage();
    }
}

