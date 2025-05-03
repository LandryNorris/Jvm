import org.junit.Test;
import static org.junit.Assert.*;

public class AverageTest {
    @Test public void averageTest() {
        Averager averager = new Averager();
        averager.addValue(10);
        averager.addValue(20);
        assertEquals(15, averager.getAverage());
    }
}
