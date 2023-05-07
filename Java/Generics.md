## Generics

Generic classes act as factory for ordinary classes.

```java
var arr = new ArrayList<String>();
ArrayList<String> arr = new ArrayList<>();
ArrayList<String> passwords = new ArrayList<>() // diamond OK in Java 9
   {
      public String get(int n) { return super.get(n).replaceAll(".", "*"); }
   };
```

### Generic Class

```java
public class Pair<T, U>
{
   private T first;
   private U second;

   public Pair() { first = null; second = null; }
   public Pair(T first, U second) { this.first = first; this.second = second; }

   public T getFirst() { return first; }
   public U getSecond() { return second; }

   public void setFirst(T newValue) { first = newValue; }
   public void setSecond(U newValue) { second = newValue; }
}
```

### Generic method

Type variables are inserted after modifiers and before return type.
Generic methods can be defined inside ordinary or generic classes.
```java
public class ArrayAlg {
    public static <T> T getMiddle(T... a) { return a[a.length/2]; }
}
ArrayAlg.<String>getMiddle("a", "b", "c");
ArrayAlg.getMiddle("a", "b", "c"); // compiler deduces type from parameters
```

Compiler complains if `<String>` is not provided in method call and it finds more than one potential choices.
```java
ArrayAlg.getMiddle(2.0, 2); // Error: Number and Comparable are both candidates
ArrayAlg.getMiddle(2.0, 2.0); // OK: Only Double is a candidate
``` 

#### Bounds for Type variables

`<T extends ClassName & Interface1 & Interface2>`

1. Use `extends` both for interface and class.
2. Only one class is allowed. Class must be first.

### Generics and VM

VM does not have objects of generic types, only ordinary classes.

#### Type Erasure

For every generic type, a *raw* type is automatically provided. The name of raw type is same as generic type, with the type parameters removed. Type variables are *erased* and replaced by their bounding types (or `Object` for unbound types).

```java
public class Interval<T extends Comparable & Serializable> implements Serializable
{
   private T lower;
   private T upper;
   . . .
   public Interval(T first, T second) { ... }
}

// Raw type
public class Interval implements Serializable
{
   private Comparable lower;
   private Comparable upper;
   . . .
   public Interval(Comparable first, Comparable second) { . . . }
}
```

If `Interval<T extends Serializable & Comparable>`, then the raw type would have used `Serializable` and added casts for `Comparable` wherever needed. For efficiency, keep tagging interfaces (interfaces without methods) last.

Call to methods get type casted.
```java
Pair<Employee> es = ...;
// Calls raw Pair.getFirst, and then type casts Object to Employee
Employee e = es.getFirst(); 

// Similar cast for public fields
e = es.first;
```

1. There are no generics in the virtual machine, only ordinary classes and methods.
2. All type parameters are replaced by their bounds.
3. Bridge methods are synthesized to preserve polymorphism.
4. Casts are inserted as necessary to preserve type safety.

```java
class DateInterval extends Pair<LocalDate> {
   public void setSecond(LocalDate second) { ... }

   // Inherited from Pair after Erasure
   // Compiler generates bridge method
   // public void setSecond(Object second) { 
   //   setSecond((LocalDate) second); 
   // }

   public LocalDate getSecond() { ... }

   // Compiler generated bridge allowed by VM
   // In VM param types, and return types define a method
   // public Object getSecond() { ... }
}

var interval = new DateInterval(. . .);
Pair<LocalDate> pair = interval; // OK--assignment to superclass
pair.setSecond(aDate); // Bridge method gets called
```

### Restrictions

##### Type Parameters cannot be primitives 

`List<double> // Error`.

##### Runtime Type Inquiry Only Works with Raw Types

VM only knows raw types.
1. `a instanceOf List<Double> // Error`
2. `a instanceOf List<T> // Error`
3. `Pair<Double> p = (Pair<Double>) a; // warning - can only check that a is a Pair`
4. 
```java
Pair<String> stringPair = ...;
Pair<Employee> empPair =  ...;
if (stringPair.getClass() == empPair.getClass()) // True
```
##### Cannot Create Arrays of Parameterized Types

Variables of type `Pair<Double>[]` can be created, but cannot be initialized with `new Pair<Double>[10]`.
```java
var table = new Pair<String>[10]; // ERROR
Object[] objArray = table;
objArray[0] = new Pair<Double>(); // passes array store check but results in a type error
```

```java
var table = (Pair<String>[]) new Pair<?>[10]; // OK but dangerous
```
Instead use `List<Pair<String>>` etc.

##### varags allows array of parameterized types

`SafeVarags` allowed only for `static`, `final`, or `private`.
```java
@SafeVarargs // Generates warning without this annotation
public static <T> void addAll(Collection<T> coll, T... ts)
```

##### Cannot instantiate type variables

`new T(...) // ERROR`

```java
public static <T> Pair<T> makePair(Supplier<T> constr) {
   return new Pair<>(constr.get(), constr.get());
}

Pair<String> p = Pair.makePair(String::new);
```

`String.class` is the sole instance of `Class<String>`

```java
public static <T> Pair<T> makePair(Class<T> cl) {
   try {
      return new Pair<>(cl.getConstructor().newInstance(),
         cl.getConstructor().newInstance());
   }
   catch (Exception e) { return null; }
}

Pair<String> p = Pair.makePair(String.class);
```

##### Cannot create a array with type variables

Array carry the type with which they are created. But type gets erased.

`T[] m = new T[2] // Error`

```java
public static <T extends Comparable> T[] minmax(IntFunction<T[]> constr, T... a) {
   T[] result = constr.apply(2);
   ...
}

String[] names = ArrayAlg.minmax(String[]::new, "Tom", "Dick", "Harry");
```

##### Static methods and fields are not allowed in generics

##### Cannot throw or catch instances of generic class

1. Can neither not catch objects of generic class.
2. A generic class cannot implement `Throwable`.
3. Type variable cannot be used in catch `catch(T e) // Error`.
4. Type variable can be used in exception specification.

```java
public static <T extends Throwable> void doWork(T t) throws T { // OK
   try { ... }
   catch (Throwable realCause) {
      t.initCause(realCause);
      throw t;
   }
}
```

##### Can defeat Checked Exception checking

Can throw checked exception without wrapping in an unchecked exception from a method that does not allow exceptions.

```java
interface Task {
   void run() throws Exception;

   @SuppressWarnings("unchecked")
   static <T extends Throwable> void throwAs(Throwable t) throws T {
      throw (T) t;
   }

   // Runnable interface is not allowed to throw exceptions
   static Runnable asRunnable(Task task) {
      return () ->
         {
            try {
               task.run();
            }
            catch (Exception e) {
               Task.<RuntimeException>throwAs(e);
            }
         };
   }
}
```

```java
public class Test
{
   public static void main(String[] args)
   {
      var thread = new Thread(Task.asRunnable(() ->
         {
            Thread.sleep(1000);
            System.out.println("Hello, World!");
            throw new Exception("Check this out!");
         }));
      thread.start();
   }
}
```

##### Clashes after erasure

Illegal to create conditions that causes clashes after types are erased.

```java
public class Pair<T>
{
   // Gets erased to equals(Object) which clashes with Object.equals
   public boolean equals(T value) { ... }
   
}
```

To support translation by erasure, we impose the restriction that a class or type variable may not at the same time be a subtype of two interface types which are different parameterizations of the same interface.

```java
class Employee implements Comparable<Employee> { . . . }
class Manager extends Employee implements Comparable<Manager> { . . . } // ERROR
```

Problem occurs when synthesizing the bridge method.
```java
// Cannot have two methods for different X
public int compareTo(Object other) { return compareTo((X) other); }
```

### Inheritance for Generic Types

1. In general, there is no relationship between `Pair<S>` and `Pair<T>`, no matter how `S` and `T` are related. `Pair<Base> p = new Pair<Child>() // Error`.
2. Generic classes can extend or implement other generic classes. `ArrayList<T>` implements `List<T>`.

### Wildcard

#### Bounded Wildcards

Cannot pass `Pair<Child>` object to a method expecting `Pair<Base>` object.

```java
public static void printBuddies(Pair<? extends Employee> p)
```

Types `Pair<Manager>` and `Pair<Employee>` are subtypes of `Pair<? extends Employee>`, which itself is a subtype of raw `Pair`.

```java
var childPair = new Pair<Child>(ceo, cfo);
Pair<? extends Base> basePair = childPair; // OK
wildcardBuddies.setFirst(baseObj); // compile-time error
```

```java
// Methods of Pair<? extends Base>
// Maybe not real java syntax
? extends Base getFirst()
void setFirst(? extends Base)
```

Compiler does not know what the exact type of the passed object is, and so it rejects all arguments except `null`.

`getFirst` works since compiler knows assignment to Employee reference is safe. 

Differentiates between safe accessor methods and unsafe mutator methods.

#### Supertype bounds

Matches superclasses and interfaces.

```java
// Methods of ? super Base
// not java syntax
void setFirst(? super Base)
? super Base getFirst()
```

`setFirst` accepts calls with `Base` or subclasses of `Base`, but not any superclasses of `Base`.

`getFirst` can only be assigned to `Object`.

Wildcards with supertype bounds let you write to a generic object, while wildcards with subtype bounds let you read from a generic object.

`Pair<Base>` and `Pai<Object>` extend `Pair<? super Child>`, which extends `Pair<?>`, which extends raw `Pair`.

```java
// result could now be Pair<Base> or Pair<Object>
public static void minmaxBonus(Child[] a, Pair<? super Child> result)
{
   if (a.length == 0) return;
   Child min = a[0];
   Child max = a[0];
   for (int i = 1; i < a.length; i++)
   {
      if (min.getBonus() > a[i].getBonus()) min = a[i];
      if (max.getBonus() < a[i].getBonus()) max = a[i];
   }
   result.setFirst(min);
   result.setSecond(max);
}
```

```java
// Works for T = Child, where Child extends Base, and Base implements Comparable<Base>
public static <T extends Comparable<? super T>> Pair<T> minmax(T[] a)
```

#### Unbounded Wildcards

```java
// Methods of Pair<?>
// not java syntax
void setFirst(?)
? getFirst()
```

The return value of `getFirst` can only be assigned to an `Object`. The `setFirst` method can only be called with `null` not even with an `Object`.

```java
public static boolean hasNulls(Pair<?> p)
{
   return p.getFirst() == null || p.getSecond() == null;
}
// Similar to
public static <T> boolean hasNulls(Pair<T> p)
```

Wildcard is not a type variable, cannot use `?` as a type.

```java
public static void swap(Pair<?> p) { swapHelper(p); }
public static <T> void swapHelper(Pair<T> p)
{
   T t = p.getFirst();
   p.setFirst(p.getSecond());
   p.setSecond(t);
}
```
`T` of swapHalper captures the wildcard.

Wildcard capture is legal only if compiler can guarantee that the wildcard represents a single, definite type. `ArrayList<Pair<T>>` cannot capture `ArrayList<Pair<?>>` since the list could have two pairs of different types.

### Reflection

`Class<T>` is generic. `String.class` is the only instance of `Class<String>`.

##### Miscellaneous

```java
@SuppressWarnings("unchecked")
```