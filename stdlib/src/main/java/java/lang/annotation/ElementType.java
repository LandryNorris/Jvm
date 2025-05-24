package java.lang.annotation;

import java.io.Serializable;

public enum ElementType implements Serializable, Comparable<ElementType> {
    ANNOTATION_TYPE,
    CONSTRUCTOR,
    FIELD,
    LOCAL_VARIABLE,
    METHOD,
    PACKAGE,
    PARAMETER,
    TYPE,
    TYPE_PARAMETER,
    TYPE_USE
}
