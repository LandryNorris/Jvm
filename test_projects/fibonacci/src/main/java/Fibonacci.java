
class Fibonacci {

    public static void main(String[] args) {

    }

    static int fibonacciRecursive(int n) {
        if(n <= 1) return n;
        return fibonacciRecursive(n-1) + fibonacciRecursive(n-2);
    }
}