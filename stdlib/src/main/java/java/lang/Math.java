package java.lang;

public final class Math {
    // Don't let anyone instantiate
    private Math() {}

    public static double E = 2.718281828459045235360287471352;
    public static double PI = 3.14159265358979323846264338;

    public static double abs(double a) {
        if(a >= 0) {
            return a;
        }
        return -a;
    }
    public static float abs(float a) {
        if(a >= 0) {
            return a;
        }
        return -a;
    }
    public static int abs(int a) {
        if(a >= 0) {
            return a;
        }
        return -a;
    }
    public static long abs(long a) {
        if(a >= 0) {
            return a;
        }
        return -a;
    }

    public static native double acos(double a);
    public static native double asin(double a);
    public static native double atan(double a);
    public static native double atan2(double a, double b);
    public static native double cbrt(double a);
    public static native double ceil(double a);
    public static native double cos(double a);
    public static native double cosh(double a);
    public static native double exp(double a);
    public static native double expm1(double a);
    public static native double floor(double a);
    public static native double floorDiv(int x, int y);
    public static native double floorDiv(long x, long y);
    public static native double sqrt(double d);
    public static native double log(double d);
    public static native double log10(double d);
    public static native double log1p(double d);

    public static native double random();

    public static double hypot(double x, double y) {
        // TODO(Landry): The spec requires handling intermediate overflow
        return Math.sqrt(x*x + y*y);
    }

    public static double copySign(double magnitude, double sign) {
        return (magnitude >= 0) == (sign >= 0) ? magnitude : -magnitude;
    }
    public static float copySign(float magnitude, float sign) {
        return (magnitude >= 0) == (sign >= 0) ? magnitude : -magnitude;
    }

    public static double max(double a, double b) {
        return (a > b) ? a : b;
    }
    public static float max(float a, float b) {
        return (a > b) ? a : b;
    }
    public static int max(int a, int b) {
        return (a > b) ? a : b;
    }
    public static long max(long a, long b) {
        return (a > b) ? a : b;
    }

    public static double min(double a, double b) {
        return (a < b) ? a : b;
    }
    public static float min(float a, float b) {
        return (a < b) ? a : b;
    }
    public static int min(int a, int b) {
        return (a < b) ? a : b;
    }
    public static long min(long a, long b) {
        return (a < b) ? a : b;
    }

    public static int negateExact(int a) {
        // TODO(Landry): Handle overflow
        return -a;
    }
    public static long negateExact(long a) {
        // TODO(Landry): Handle overflow
        return -a;
    }

    public static double signum(double d) {
        if(d == 0.0) {
            return 0.0;
        } else if(d > 0.0) {
            return 1.0;
        }
        return -1.0;
    }

    public static float signum(float f) {
        if(f == 0.0f) {
            return 0.0f;
        } else if(f > 0.0f) {
            return 1.0f;
        }
        return -1.0f;
    }

    public static double toDegrees(double radians) {
        return radians * 180.0 / Math.PI;
    }

    public static double toRadians(double degrees) {
        return degrees * Math.PI / 180.0;
    }
}
