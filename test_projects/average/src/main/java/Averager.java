
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
}
