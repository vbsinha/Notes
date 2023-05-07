## Inner Class

Creating an Inner class does not create an instance filed of type Inner in the Outer class.
```java
public class Outer {
    private boolean beep;
    public class Inner {
        int j;
        public void m() {
            beep = true; // OK
            Outer.this.beep = true; // OK
        }
    }
}
```

1. Inner class can access both its own and outer class's instance fields, even private ones. Compiler inserts a Outer object to every constructor of the inner class, and the outer fields are accessed through this reference.
2. Only inner classes can be private, in which case only it's outer class can create Inner instances. Regular classes have to be either public or package.
3. Construct Inner class instances more explicitly using `outer.new Inner(params)` like `this.new Inner()` (`this` is redundant here). For any object `o` of Outer, we can construct Inner object as `o.new Inner()`.
4. Use `Outer.Inner` to refer to `Inner` outside `Outer`.
5. Any static field declared in inner class must be final and initialized with a compile time constant.
6. Inner class cannot have static method.
7. Inner class gets translated into regular class regular files with`$` separating the outer and inner class names.
8. Inner classes are more powerful than extracting them out, and storing a reference to the outer class, since such an extracted class will not be able to access Outer's private fields.

#### Local inner class

Inner class defined in a method is called local inner class.

1. No access specifier, only the method that created it knows about it.
2. Can access local variables, which are effectively final. The field persists in inner class even if the local variable goes out of scope. (Compiler inserts a field in Inner class for all used local variables.)

#### Anonymous inner class

If only one object of the local inner class is needed, then it can be anonymous.

```java
new SuperType(construction params) {
    { ... initialization block ... }
    // inner class methods and data
}

// a Person object
new Person("Alice"); 

// an object of an inner class extending Person
var bob = new Person("Bob") { int age = 2; };
System.out.println(bob.age);

new ArrayList<String>() {{add("a"); add(b);}}
```
```java
// no params allowed
new Interface() {
    // methods and data
}


```
Anonymous Inner class cannot have constructor since it does not have a name.

#### Static inner class

Static inner classes do not have reference of the outer class.

Only inner classes can be static. A static inner class is same as normal inner class, except the reference to the outer class.

Static inner classes can have static fields and methods (unlike normal inner class).

Classes defined inside interfaces are automatically `static` and `public`.

```java
public class C {
    public static class Inner {
        private int val;
    }
    public static Inner getInner() {
        return new Inner();
    }
}
```

### Inner interfaces, records and enums

Interfaces, records, enums declared inside a class are automatically static.