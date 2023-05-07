## Interface

```java
// Using interface like a contract
// Arrays.sort requires that the object implement Comparable
public interface Comparable<T> {
    int compareTo(T other);
}
public class Base implements Comparable<Base> {
    int compareTo(Base other) { ... }
}
```

```java
// Here using the non-generic (old) Comparable interface
public interface Comparable {
    int compareTo(Object other);
}
public class Base implements Comparable {
    // must be public since compareTo from the interface is public, and access cannot be reduced
    public int compareTo(Object other) { ... }
}
```

1. All methods with no access specifiers in interface are automatically `public`.
2. Interfaces cannot have instance fields.
3. Interface variables can be declared `Comparable c;`
4. Interfaces cannot be instantiated `Comparable c = new Comparable(); // Error`.
5. `instanceOf` can check if object's class implements the interface `(c instanceOf Comparable)`.
6. An interface can extend multiple interfaces.
7. Any fields declared in interfaces are automatically `public static final`. 
8. It is legal to explicitly declare `public` for methods, and `public static final` for fields of interface. Java specification recommends redundant keywords be not specified.
9. A class can implement multiple interfaces.
10. Records and Enums cannot extend classes but they can implement interfaces.
11. Interfaces can be `sealed`. All the direct subtypes (classes and interfaces) must be declared in `permits`.
12. Interfaces can have `static` methods.
13. Interfaces can have (static and instance) private methods. These can be used only in the methods of interface, so they are of limited use as helpers to other methods. They can be called from default methods.

#### `default` methods

Can provide default implementation of a method. Can call other methods.
```java
public interface I {
    int size();
    default boolean isEmpty() { return size() == 0; }
}
```

Used for *interface evolution*. If new methods added to interface are not default, all implementers will break. Adding a non-default method to interface is not *source-compatible*.

Adding a method to an interface is *binary compatible*.

##### Resolving `default` method conflicts

1. **Superclass win**. If the same method is in superclass and interface as default, superclass wins. This implies that any method present in `Object` will always take precedence over interface default methods.
```java
public class Base {
    boolean getName () { ... };
}
public interface I {
    default boolean getName () { ... };
}
public class Child extends Base implements I {
}

// new Child().getName() invokes Base's getName
```
2. **Interfaces clash**. If the same method is present in multiple interfaces and at least one of them declares it `default`, compiler reports an error.
```java
public Interface Person {
    default boolean getName() { ... }
}
public Interface Named {
    default boolean getName() { ... }
}
public class Student implements Person, Named {
    default boolean getName() { return Person.super.getName(); }
}
```

#### Callbacks

```java
public interface ActionListener {
    void actionPerformed(ActionEvent event);
}
public TimePrinter implements ActionListener {
    void actionPerformed(ActionEvent event) { ... };
}

var listener = new TimePrinter();
new Timer(1000, listener).start();
```

#### Comparator

Custom sorting a list of strings
```java
public interface Comparator<T> {
    int compare(T first, T second);
}
```
```java
public class CustomComparator implements Comparator<String> {
    int compare(String first, String second) { return first.length() - second.length() };
}
var comparator = new CustomComparator();
String[] arr = { ... };
Arrays.sort(arr, comparator);
```

##### Cloneable

`Cloneable` is a *tagging interface* (or a *marker interface*). It does not have any methods, only used to tag classes. It allows the use of `instanceOf`.