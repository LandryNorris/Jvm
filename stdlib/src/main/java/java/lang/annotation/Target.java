package java.lang.annotation;

import static java.lang.annotation.ElementType.ANNOTATION_TYPE;

@Retention(RetentionPolicy.RUNTIME)
@Target(ANNOTATION_TYPE)
public @interface Target {
    ElementType value();
}
