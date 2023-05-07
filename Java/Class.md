# Classes

All Java objects live on the heap. 
`new` returns a reference.
Name of the file must match the name of the public class. A file can have only one public class but any number of nonpublic classes.
`var` can be used only with local variables inside method. Method parameters, and class fields must declare type.
Class contains:
1. members (fields, methods, classes, and interfaces)
2. instance and static initializers
3. constructors

final instance fields need to be set at the end of every constructor.
Java always uses call by value.

If you don’t set a field explicitly in a constructor, it is automatically set to a default value: numbers to 0, boolean values to false, and object references to null.

In a class with no constructors, a no-argument constructor is automatically provided.
Member field can be initialized in the class definition. This initialization occurs before constructor call and need the initialization can call another method.

```java
class Employee {
   private String name = "";
   . . .
}
```

If the first statement of a constructor has the form `this(. . .)`, then the constructor calls another constructor of the same class.

#### Instance Initializers

Class declarations can contain arbitrary blocks of code. These blocks are executed whenever an object of that class is constructed.

```java
class Employee
{
    private static int nextId; 
    private int id;

    {
      id = nextId;
      nextId++;
    }
}
```

If the first line of the constructor calls a second constructor, then the second constructor executes with the provided arguments. 2. Otherwise,
1. All instance fields are initialized to their default values (0, false, or null).
2. All field initializers and initialization blocks are executed, in the order in which they occur in the class declaration.
3. The body of the constructor is executed.

#### Static Initializers

Static initialization occurs when the class is first loaded. Like instance fields, static fields are 0, false, or null unless explicitly set them to another value.

```java
static
{
   nextId = generator.nextInt(10000);
}
```

#### Record

Class whose state is immutable and readable by public.
Instance fields of records are called components.
Records can have static fields and methods.
Records can add member methods.
Cannot extend records, and a record cannot extend another class. Can implement interfaces.

```java
// Constructor and accessor method auto-defined.
record Point(double x, double y) {
    public double x() { return 2*x };
    public double distanceFromOrigin() { return Math.hypot(x, y); }
}

Point p = new Point(1,2); 
p.x(); p.y();
```

#### Record Constructors: Canonical, Custom, and Compact

Canonical: Automatically defined. Sets all components.
Custom: Must call another constructor in first line, so that the call reaches canonical constructor eventually.
Compact: Canonical constructor without parameter list. Recommended.

```java
record Range(int from, int to) {
   public Range { // Compact form
    if (from > to) { // Swap the bounds
         int temp = from;
         from = to;
         to = temp;
      }
   }
}
```

### Packages

Package is a group of classes to prevent class-name collisions.

For the compiler, there is no relationship between nested packages. For example, the packages java.util and java.util.jar have nothing to do with each other. Each is its own independent collection of classes.

A class can use all classes from its own package and all public classes from other packages.

Using classes from another package:
1. `java.time.LocalDate today = java.time.LocalDate.now();`
2. 
```java
import java.time.*;
LocalDate today = LocalDate.now();
```

Using option 1, import statement can be avoided. 

`*` import instead of class name has no negative impact on code size.
`*` can import only package. e.g. `java.*` or `java.*.*` does not work.

Bytecodes always use full package names to refer to other classes.

Static methods and fields can be imported (in contrast to non-static for which method or field import doesn't work.)

`package com.google.collections` at the top of the file to declare the namespace of the class. 
Place files in the same directory structure as package path. 
Compiler places the class files in the same directory structure.
If the file structure does not match, compilation will go through if the class does not depend on other class. But the resulting program won't run. VM won't find the class.

```
.
- com
    - horse
        - core
            - Emp.java
            - Emp.class
    - mouse
        - Pay.java (depends on Emp.java)
        - Pay.class

Compile from . 
javac com/mouse/Pay.java
java com.mouse.Pay
```

#### Access specifiers

Features tagged public can be used by any class.
Private features can be used only by the class that defines them.
Untagged can be accessed by all methods in same package.

#### Classpath

JAR: Java Archive. Zip file that contains directories and class files. Can contain resources like image etc.
Classpath: List of all locations that contain class files. Has a list of JAR files, and top of package hierarchies.
Compiler finds the dependency class file. If the source file was modified after the class file was created, compiler will automatically recompile the dependency.

```bash
// Setting the classpath when running a class
java -cp /home/user/classdir:.:/home/user/archives/archive.jar MyProg
```

##### Creating JAR

```bash
// jar options jarFileName file1 file2
jar cvf CalculatorClasses.jar *.class icon.gif
```

##### Manifest file

JAR can contain manifest file (`MANIFEST.MF`) located in the `META-INF` subdirectory of the JAR. Manifest describes the features of the archive.

##### Executable JAR

The main class of the JAR can be specified to the manifest, or set on the JAR itself. Then one can execute the JAR
```bash
java -jar MyProgram.jar
```
Or if OS supports, double-click run the JAR.

#### Documentation

`javadoc` tool is a part of JDK.

Class doc:
```
/**
 * A {@code Card} object represents a playing card, such ...
*/
```

Method doc:
```
/**
 * Raises the salary of an employee.
 * @param  byPercent the percentage by which to raise the salary (e.g., 10 means 10%)
 * @return  the amount of the raise
 * @throws class description
 */
```