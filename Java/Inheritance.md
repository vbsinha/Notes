### Inheritance

Child class can add fields and methods and can override methods.

*Dynamic Bonding* is default. JVM knows the type of each object can so invokes the overridden method.

Object variables are polymorphic: variable of parent class and refer to child class objects.

```java
class Parent {

    private int p;

    public Parent (int p) { this.p = p; }

    public void m1() {}
}

class Child extends Parent {
    // private fields and methods of Parent not visible
    // public and protected fields of Parent are visible

    public Child (int p) {
        // Call to parent constructor must be first line.
        super(p);
    }

    // overriding m1
    @Override // Optional annotation
    public void m1() {
        super.m1(); // need not be first line, can be used anywhere
    }

    public void m2() {}
}

Child c = new Child(4);
Parent p = c;
p.m1(); // Calls child's m1
c.m2(); // OK
p.m2() // Error: Parent does not define m2

Child c = new Parent(5); // ERROR
```

##### Constructor

If child constructor does not explicitly invoke parent constructor, then parent must have no argument constructor, which is run first automatically.

##### Overridden method

Return type of overridden method in Child must be a subtype of the return type of the original method in Parent, due to the Liskov Substitution Principle.

Overridden method must be at least as visible (by public / private) as the parent method.

Overridden methods cannot be inlined by the compiler.

##### Arrays and Polymorphism

Arrays remember the type with which they are created.
```java
Child[] children = new Child[10];
Parent[] parents = children; // OK
parents[0] = new Parent(3); // ArrayStoreException
```

##### Annotation
`@Override` annotation is optional but should be used: throws error if the method signature does not really override, or if the parent changes method signature all overridden methods will automatically error.

private and static methods cannot be overridden.

##### Method Calls

For call `x.f(args)`, where is of type `C`:
1. Compiler enumerates all accessible (i.e. public etc. depending on context) methods of name `f` in `C`, and all accessible methods of name `f` in superclasses of `C`.
2. **Overloading Resolution** Compiler determines the types of arguments passed in the function call, and finds a best match. If no / multiple methods found, compiler reports an error. 
3. **Static Binding** If the method is private, static, final, or a constructor then the compiler knows which method to call.
4. **Dynamic Binding** When the program runs appropriate method is called based on type of `x`. Virtual Machine precomputes a *method table* for each class. The method table lists all method signatures and the actual methods to be called.

Due to dynamic binding, the code of / using Parent need not be recompiled when a new child is added. At runtime, if the variable of type Parent got passed the new child, overridden methods will be called automatically.

##### final

```java
// Cannot extend Child
public final class Child extends Parent {
    // All methods (not fields) are automatically final
}


public class C {
    // Cannot override m1
    public final String m1() {}
}
```

`String` is a final class. Guaranteed that a reference of String will always have a String.

Enums and Records are always final.

##### Cast and instanceof

Can only cast within an inheritance hierarchy. Impossible casts due to inheritance hierarchy are compile time error. If at runtime cast is not possible, raises a `ClassCastException`.

```java
Parent p = new Child(0);
// Old style
if (p instanceof Child) {
    Child c = (Child) p;
    c.m2(); // m2 is not overridden
}
// New style
if (p instanceof Child c) {
    c.m2();
}

if (p instanceof Child c && c.m2()) // OK
if (p instanceof Child c || c.m2()) // Error
p instanceof Child c ? c.m2() : 0;
```

`null instanceof C` returns false.


```java
Child c = ...;
if (c instanceof Parent p) { } // Error useless type cast
if (c instanceof Parent) {} // Useless but no error for backward compatibility
```

```java
// Pattern matching switch: preview in Java 20
switch (e) {
    case Child c -> ...
    case Parent p -> ...
    case default -> ...
}
```

##### Access control modifiers

1. `private`: Only in the same class.
2. No-modifier default: Only in the same package.
3. `protected`: Only in the same package and the subclasses.
4. `public`: Everyone.

### `Object` class

Every Java class extends `Object`. All array types, array of objects or primitives, are class types that extend `Object`.

`"anyString" + x` and `System.out.println(x)` invokes `x.toString()` when `x` is an object.
Use `Arrays.toString(arr)` to print array.

#### API from Objects class

1. `Objects.equals(a, b)`. True if both arguments are null. False if only one is null. `a.equals(b)` otherwise.

#### Wrappers and Autoboxing

Wrapper Classes: Integer, Long, Float, Double, Short, Byte, Character, and Boolean. First six inherit from Number. Wrapper classes are *immutable*: cannot change a wrapped value after the wrapper has been constructed. They are also final.

```java
var list = new ArrayList<Integer>();

// Autoboxing: translated to list.add(Integer.valueOf(3));
list.add(3); 

// Unboxing: compiler translates to int v = list.get(0).intValue();
int v = list.get(0);

Integer n = 3;
n++; // OK uses autoboxing and unboxing

Integer a = ... , b = ... ;
if (a == b) // Do not use. Object equality. Use equals.
```

Don't use wrapper class constructors. They are deprecated. Use `Integer.valueOf(100)` or `Integer i = 100;`.

Boxing and unboxing is courtesy of compiler which adds the necessary code when generating the bytecode. The VM simply executes the bytecode.

Wrappers also contain other convenience methods e.g. `Interger.parseInt(String)`.

### varargs

`foo(Base ...)` available to the method as an array. The caller can pass an array too.

### Abstract Classes

A class with abstract methods must be declared abstract. Abstract classes can have fields and concrete methods.

```java
public abstract class Base {
    private String name;
    public String getName() { ... }
    public abstract String m1();
}
```

Subclasses of abstract class must implement the abstract methods or be declared abstract classes themselves.

A class can be abstract even if it has no abstract methods.

Abstract classes cannot be instantiated. Object variables of abstract classes must refer to an object of non-abstract subclass.

### Enums

Enums are classes.
Can use `==` to compare enum instances.

```java
public enum Size {
    SMALL("s"), LARGE("l");
    private String abbr;
    Size(String abbr) { this.abbr = abbr; }
    public String getAbbr() { ... } 
}
```

Enum constructors is always private. Private modifier can be omitted.

Every enum has `values` method which returns all values of enum `Size[] sizes = Size.values();`.

All enums are subclasses of `Enum`. So enums cannot extend any other class. Enums can implement interfaces.

Methods inherited from `Enum<E>`:
1. static Enum valueOf(Class enumclass, String name). `Enum.valueOf(Size.class, "SMALL")` returns `Size.SMALL`.
2. String toString(). `Size.SMALL.toString()` returns `"SMALL"`.

### Sealed classes

Sealed classes list down the classes that can extend them.

```java
public sealed class Child extends Base permits GrandChildOne, GrandChildTwo {
    ...
}
```

All subclasses of a sealed class must be declared final, sealed, or non-sealed.

## Reflection

While a program is running, Java runtime system maintains runtime type identification on all objects. This information keeps track of the class to which each object belongs. Runtime type information is used by the virtual machine to select the correct methods to execute.

Three ways to obtain `Class` variable.

```java
Base b;
Class c = b.getClass();
c.getName(); // Name with package
```

```java
Class c1 = Class.forName("java.util.Random");
```

```java
Class c1 = Random.class;
Class c2 = int.class; 
Class c3 = Double[].class;
```

VM maintains a unique `Class` object for each type. So we can use `b.getClass() == Base.class`.