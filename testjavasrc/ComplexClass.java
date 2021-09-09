

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

    void methodWithAnIntSwitch() {
        int x = 0;
        int result;
        switch(x) {
            case 0:
                result = 1;
                break;
            case 1:
                result = 5;
                break;
            case 3:
                result = 27;
                break;
            case 4:
                result = 12345;
            case 25:
                result = -2;
                break;
            case 28:
                result = -20;
                break;
            case 48:
                result = 100-x;
        }
    }

    String methodWithAStringSwitch(String s) {
        switch(s) {
            case "":
                return "Empty String";
            case "A":
                return "Letter A";
            case "b":
                return "Letter b";
            case "c":
            case "d":
            case "abcd":
                return "abcd";
        }
        return "No cases hit";
    }
}

interface AJavaInterface {
    public void aJavaMethod(int intParam, double doubleParam);
}