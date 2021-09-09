

class ComplexJavaClass implements AJavaInterface {

    static final float staticFinalVariable = 123.456f;
    int internalInt;
    double internalDouble;

    @Deprecated
    long deprecatedLongField;

    @Override
    public void aJavaMethod(int intParam, double doubleParam) {
        internalInt = intParam + 7;
        internalDouble = 2*doubleParam;
    }

    @Deprecated
    int deprecatedMethod() {
        int fieldScopedConstant = 7;
        long fieldScopedLong = 1234L;
        String fieldScopedString = "A Field Scoped String";

        return 0;
    }

    static void aStaticMethod(int anInt, String aString) {
        System.out.println(aString + " Appending to aString");
    }
}

interface AJavaInterface {
    public void aJavaMethod(int intParam, double doubleParam);
}