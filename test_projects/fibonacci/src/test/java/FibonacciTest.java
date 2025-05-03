import org.junit.Test;
import static org.junit.Assert.*;

public class FibonacciTest {
    @Test public void fibonacciTest() {
        int n = Fibonacci.fibonacciRecursive(20);
        assertEquals(6765, n);
    }
}
