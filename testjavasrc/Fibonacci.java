
class Fibonacci {
    public static void main(String[] args) {
        int n = 20;
        int result = fibonacciRecursive(n);
        //System.out.println(result);
    }

    static int fibonacciRecursive(int n) {
        if(n <= 1) return n;
        return fibonacciRecursive(n-1) + fibonacciRecursive(n-2);
    }
}