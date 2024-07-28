## Annotation

Tools can process on source level or on the class files. Annotations do not change the way in which programs are compiled. The Java compiler generates the same virtual machine instructions with or without the annotations.

Annotation is used like modifier.

### Syntax

#### Annotation Interfaces

```java
modifiers @interface AnnotationName {
     type elementName();
     type elementName() default value;
}
```

1. All annotation interfaces implicitly extend the `java.lang.annotation.Annotation` interface, which is a regular interface (not annotation interface).
2. Annotation interfaces cannot be extended, and cannot have classes implementing them.
3. Methods of annotation interfaces have no parameters or `throws`. They cannot be `default` or `static` and cannot have type parameters.
4. Types of annotation element:
    * A primitive type (`int`, `short`, `long`, `byte`, `char`, `double`, `float`, or `boolean`).
    * `Class` (with optional type parameter like `Class<? extends MyClass>`)
    * `String`
    * enum
    * annotation type
    * Array of above types (array of arrays is not allowed)
```java
public @interface BugReport
{
   enum Status { UNCONFIRMED, CONFIRMED, FIXED, NOTABUG };
   boolean showStopper() default false;
   String assignedTo() default "[none]";
   Class<?> testCase() default Void.class;
   Status status() default Status.UNCONFIRMED;
   Reference ref() default @Reference(); // an annotation type
   String[] reportedBy();
}
```

#### Annotations

```java
@BugReport(assignedTo="Harry", severity=10)
// Order does not matter
@BugReport(severity=10, assignedTo="Harry")
// default used if element value is not specified
@BugReport(severity=10)
```

If no elements are specified or only default values are to used, `@BugReport`. Called *marker* annotation.
*single-valued* annotation, if the only element is value, element name can be skipped.
```java
public @interface ActionListenerFor {
   String value();
}
@ActionListenerFor("yellowButton")
```

An item can have multiple annotations.
```java
@Test
@BugReport(showStopper=true, reportedBy="Joe")
public void checkRandomInsertions()
```

If annotation is declared repeatable, it can appear multiple times.
```java
@BugReport(showStopper=true, reportedBy="Joe")
@BugReport(reportedBy={"Harry", "Carl"})
public void checkRandomInsertions()
```

All element values must be compile time constants.
An annotation element can never be set to `null`. Not even a default of `null` is permissible. 

```java
@BugReport(. . ., reportedBy={"Harry", "Carl"}) // reportedBy is array
@BugReport(. . ., reportedBy="Joe") // OK, same as {"Joe"}
@BugReport(ref=@Reference(id="3352627"), ...)
```
Cannot have circular dependencies in annotations by having annotation type as element.

#### Annotating declarations

Annotations fall into two categories: *declaration* and *type uses*.
Declaration annotation can appear at declaration of:
* Packages
* Classes (including enum)
* Interfaces (including annotation interfaces)
* Methods
* Constructors
* Instance fields (including enum constants)
* Local variables
* Parameter variables
* Type parameters

```java
@Entity public class User { . . . }
@SuppressWarnings("unchecked") List<User> users = . . .;
public User getUser(@Param("id") String userId)
public class Cache<@Immutable V> { . . . }
@GPL(version="3")
package com.horstmann.corejava;
```

All annotations of local variables and packages are discarded when a class is compiled.

#### Annotating type uses

Declaration annotation provide information about the item.
`public User getUser(@NonNull String userId)`

1. `List<@NonNull String>` list of non-null strings. `Comparator.<@NonNull String> reverseOrder()`
2. `@NonNull String[][]` words (`words[i][j]` is not `null`), `String @NonNull [][] words` (`words` is not `null`), `String[] @NonNull [] words` (`words[i]` is not `null`).
3. `class Warning extends @Localized Message`
4. `new @Localized String(...)`
5. `(@Localized String) text`, `if (text instanceof @Localized String)`. Annotation has no influence on cast or `instanceof`.
6. `public String read() throws @Localized IOException`
7. `List<@Localized ? extends Message>`, `List<? extends @Localized Message>`
8. `@Localized Message::getText`

Cannot use:
1. `@NonNull String.class // ERROR: Cannot annotate class literal`
2. `import java.lang.@NonNull String; // ERROR: Cannot annotate import`

Annotations can be place before or after other modifiers like `private` etc. Customary to put declaration modifiers before other modifiers and type annotations after modifiers.
```java
private @NonNull String text; // Annotates the type use
@Id private String userId; // Annotates the variable
```

When annotating a record component, the annotation also applies to the generated field, getter method, and constructor parameter.

Annotation specifies where it can appear. If an annotation is permissible both for a variable and a type use, and it is used in a variable declaration, then both the variable and the type use are annotated. `public User getUser(@NonNull String userId)`. If `@NonNull` can apply both to parameters and to type uses, the `userId` parameter is annotated, and the parameter type is `@NonNull String`.

#### Annotating `this`

First parameter is called *receiver* parameter, must be `this`. Constructors cannot have receiver parameter.
```java
public class Point {
   public boolean equals(@ReadOnly Point this, @ReadOnly Object other) { ... }
}
```

The parameter of enclosing class can be specified in inner class constructor.

```java
public class Sequence {
    class Iterator implements java.util.Iterator<Integer> {
        private int current;
        public Iterator(@ReadOnly Sequence Sequence.this) {
            this.current = Sequence.this.from;
        }
    }
}
```

### Standard Annotations

1. `@Deprecated`. Applicable to all.
2. `@SuppressWarnings("unchecked")`. All but packages and annotations.
3. `@Override`. Methods.
4. `@SafeVarargs`. Methods and constructors. Asserts that the varargs parameter is safe to use.
5. `@Serial`. Methods. Checks that this method is a correct serialization method.
6. `@FunctionalInterface`. Interfaces. Marks an interface as functional (with a single abstract method).
7. `@Generated`. All. Marks an item as source code that has been generated by a tool.

### Meta Annotations

Meta annotations are annotations for annotation interfaces.
`@Target` restricts where the annotation can be applied.
```java
@Target({ElementType.TYPE, ElementType.METHOD})
public @interface BugReport
```

Element Types: `ANNOTATION_TYPE`, `PACKAGE`, `TYPE`, `METHOD`, `CONSTRUCTOR`, `FIELD`, `PARAMETER`, `LOCAL_VARIABLE`, `TYPE_PARAMETER`, `TYPE_USE`.

`@Retention` specifies how long an annotation is retained.
Possible choices:
1. `SOURCE`. Annotations not included in class files.
2. `CLASS`. Annotations are included in class files, but the virtual machine does not load them. Default.
3. `RUNTIME`. Annotations are included in class files and loaded by the virtual machine. They are available through the reflection API.

`@Documented` specifies that the specific annotation will be present in documentation.
Legal to apply an annotation to itself, `Documented` does so.

`@Inherited` applied to annotation of class. When a class has an inherited annotation, then all of its subclasses automatically have the same annotation. Can work in place of marker interfaces.

```java
@Inherited @interface Persistent { }
@Persistent class Employee { ... }
class Manager extends Employee { ... } // also @Persistent
```

```java
@Repeatable(TestCases.class)
@interface TestCase {
   String params();
   String expected();
}

@interface TestCases {
   TestCase[] value();
}
```

Whenever two or more @TestCase annotations are supplied, they are automatically wrapped into a @TestCases annotation.

#### Processing annotations at runtime

Both source file and class file will have annotation.
```java
// ActionListenerFor.java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface ActionListenerFor {
   String source();
}
```

```java
// ActionListenerInstaller.java
public class ActionListenerInstaller {
  public static void processAnnotations(Object obj) {
    try {
       Class<?> cl = obj.getClass();
       for (Method m : cl.getDeclaredMethods()) {
          ActionListenerFor a = m.getAnnotation(ActionListenerFor.class);
          if (a != null) {
             Field f = cl.getDeclaredField(a.source());
             f.setAccessible(true);
             addListener(f.get(obj), obj, m);
          }
        }
     }
     catch (ReflectiveOperationException e) { ... }
  }
}
```

```java
// ButtonFrame
public class ButtonFrame extends JFrame {
    public ButtonFrame() { ActionListenerInstaller.processAnnotations(this); }

   @ActionListenerFor(source = "yellowButton")
   public void yellowBackground() { panel.setBackground(Color.YELLOW); }
}
```

#### Source level annotation processing

Can be used to produce more source code, configuration files, scripts, etc. 

##### Annotations Processors

```bash
javac -processor ProcessorClassName1,ProcessorClassName2,... sourceFiles
```

Compiler locates the annotations of the source files. Each annotation processor is executed in turn and given the annotations in which it expressed an interest. If an annotation processor creates a new source file, the process is repeated. Once a processing round yields no further source files, all source files are compiled.

1. An annotation processor can only generate new source files. It cannot modify an existing source file.
2. Annotation processor implements the `Processor` interface, generally by extending the `AbstractProcessor` class. It declares which annotations it supports.

```java
@SupportedAnnotationTypes("com.horstmann.annotations.ToString") // com.horstmann.* etc. works too
@SupportedSourceVersion(SourceVersion.RELEASE_8)
public class ToStringAnnotationProcessor extends AbstractProcessor
{
   public boolean process(Set<? extends TypeElement> annotations,
         RoundEnvironment currentRound) {
      ...
   }
}
```

`process` is called once for each round, with all annotations found in that round. `RoundEnvironment` contains information about the current processing round.

*Language model* API is used to analyze source level annotations. Compiler produces a tree whose nodes are instances of classes that implement `javax.lang.model.element.Element` interface: `TypeElement`, `VariableElement` etc. analogous to `Classes`, `Field`/`Parameter` reflection classes. 

To see rounds use `javac` with `-XprintRounds`.

#### Bytecode engineering

Besides runtime, and source level, annotations can be processed at bytecode level since class files have annotations. This can be used to insert method calls etc.

`@LogEntry(logger=loggerName)` could add `Logger.getLogger(loggerName).entering(className, methodName);` at the beginning of method. 

Use ASM library.
```bash
javac set/Item.java
javac -classpath .:asm/\* bytecodeAnnotations/EntryLogger.java
java -classpath .:asm/\* bytecodeAnnotations.EntryLogger set.Item

javap -c set.Item; ; 
```

#### Modifying bytecodes at load time

...