package java.lang.annotation;

import java.io.Serializable;

public enum RetentionPolicy implements Serializable, Comparable<RetentionPolicy> {
    CLASS,
    RUNTIME,
    SOURCE
}
