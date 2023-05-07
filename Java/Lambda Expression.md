## Lambda Expression

Lambdas are used as code passed around to be called later (callback).

1. Lambda expression has parameters, `->`, and expression.
```java
(String first, String second) -> first.length() - second.length()

// multiline expression, should be in {} with explicit return
(String first, String second) -> {
    if (first.length() > second.length()) return 1;
    return -1;
}

// no parameter expression
() -> { System.out.println("Hello"); }
```
2. If the type of parameters can be inferred, they can be omitted. If the method has only parameter and its type can be inferred, then parenthesis can be omitted too.
```java
Comparator<String> comp = (first, second) -> first.length() - second.length();

ActionListener listener = event -> System.out.println(event);
```
3. Return type of lambda expression is never specified, it is inferred from context. It is illegal to return in some branches and not in others.
4. `var` can be used to denote inferred type. Used for attaching annotations.
`(@NonNull var first, @NonNull var second) -> ... `

### Functional Interfaces

Can supply a lambda expression whenever an object of an interface with a single abstract method is expected. Such an interface is called a *functional interface*. Interface can have other non abstract methods.

```java
Arrays.sort(arr, (first, second) -> first.length() - second.length())
```

Above, `Arrays.sort()` receives an object of some class that implements `Comparator<String>`. Invoking the compare method on that object executes the body of the lambda expression. Management of these objects and classes is completely implementation-dependent, and it can be much more efficient than using traditional inner classes.

Conversion to functional interface is the *only* thing we can do with lambda expressions.

Lambda expression cannot be assigned to variable of type `Object` since `Object` is not a functional interface.

#### Functional Interfaces defined in `java.util.function`

1. `BiFunction<T, U, R>`: Function with parameters of type `T`, and `U`, and return type `R`.
2.
```java
public interface Predicate<T> {
    boolean test(T t);
    // other default and static methods
}
new ArrayList().removeIf(/* Expects Predicate */ e -> e == null)
```
3. Suppliers are used for *lazy evaluation*.
```java
public interface Supplier<T> {
    T get();
}
// Not optimal. Base() gets constructed even if b is not null
Objects.requireNonNullElse(b, new Base());
// Supplier is called only if needed
Objects.requireNonNullElseGet(b, () -> new Base());
```

### Method Reference

```java
public interface ActionListener {
    void actionPerformed(ActionEvent e);
}
public class Timer {
    Timer(int delay, ActionListener listener);
}

new Timer(1000, System.out::println);
// Equivalent to
new Timer(1000, event -> System.out.println(event));
```
`System.out::println` is a *method reference*. It directs the compiler to produce an instance of a functional interface, overriding the single abstract method of the interface to call the given method.

`::` operator separates method name from class or object name. Variants:
1. object`::`instanceMethod. Method reference is equivalent to a lambda expression whose parameters are passed to the method. `System.out::println` same as `x -> System.out.println(x)`.
2. Class`::`instanceMethod. First parameter becomes the implicit parameter of the method. `String::compareToIgnoreCase` same as `(x,y) -> x.compareToIgnoreCase(y)`.
3. Class`::`staticMethod. All parameters are passed to the static method. `Math::pow` same as `(x,y) -> Math.pow(x,y)`.

Method reference only works if lambda expression only calls the method and does not do anything else. `s -> s.length() == 0` cannot be converted to a method reference.

API contains methods specifically intended to be used for method interfaces, e.g. `Objects::isNull` for `Predicate<T>`.

Forming `separator::equals` throws `NullPointerException` immediately if `separator` is `null`, while `x -> separator.equals(x)` throws only if invoked.

### Constructor Reference

Same as method reference but called with `new` (e.g. `Person::new`). `int[]::new` is same as `n -> new int[n]`.

Cannot construct array of generic type `T`. `new T[n]` is erased to `new Object[n]`.
```java
Object[] person = stream.toArray();
Person[] person = stream.toArray(Person::new);
```

#### Variable Scope

Lambda expression has:
1. A block of code
2. Parameters
3. Values of *free* variables

Free variables are *captured*. The block of code with the free variables is called *closure*.

It is illegal to refer to variables that are mutated outside the lambda function or change the value of a free variable. 

The rule is that any captured variable in a lambda expression must be *effectively final*. An effectively final variable is a variable that is never assigned a new value after it has been initialized.

The body of a lambda expression has the same scope as a nested block. 

`this` in a lambda expression refers to the `this` parameter of the method that creates the lambda. `super` is also allowed, and works the same way as a normal super. `this` and `super` can be used in method references `super::greet`.

### Processing Lambda Expressions

The point of using lambda expressions is *deferred execution*:
1. Running in a separate thread
2. Running multiple times
3. Running at the right point in an algorithm (e.g., comparison in sorting)
4. Running when something happens (e.g. button click, data arrival)
5. Running the code only when necessary (e.g. lazy evaluation)

Pass a lambda function and accept a functional interface.

##### Functional Interfaces

| Functional Interface  | Abstract Method       | Description                                       | Other Methods |
|-----------------------|-----------------------|---------------------------------------------------|---------------|
| `Runnable`            | `void run()`          | Runs an action without arguments or return value  |               |
| `Supplier<T>`         | `T get()`             | Supplies a value of type `T`                      |               |
| `Consumer<T>`         | `void accept(T)`      | Consumes a value of type `T`                      | `andThen`     |
| `BiConsumer<T, U>`    | `void accept(T, U)`   | Consumes values of type `T` and `U`               | `andThen`     |
| `Function<T, R>`      | `R apply(T)`          | A function with argument of type `T`              | `compose` `andThen` `identity` |
| `BiFunction<T, U, R>` | `R apply(T, U)`       | A function with argument of types `T`, `U`        | `andThen`     |
| `UnaryOperator<T>`    | `T apply(T)`          | A unary operator on the type `T`                  | `compose` `andThen` `identity` |
| `BinaryOperator<T>`   | `T apply(T, T)`       | A binary operator on the type `T`                 | `andThen` `maxBy` `minBy` |
| `Predicate<T>`        | `boolean test(T)`     | A boolean-valued function                         | `and` `or` `negate` `isEqual` `not` |
| `BiPredicate<T, U>`   | `boolean test(T, U)`  | A boolean-valued function                         | `and` `or` `negate` |

`Predicate.isEqual(a)` is same as `a::equals` but works for `null` `a` too. Predicates can be combined `Predicate.isEqual(a). or(Predicate.isEqual(b))` is same as `x -> a.equals(x) || b.equals(x)`.

##### Functional Interfaces for primitive types

p, q is `int`, `long`, `double` and P, Q is `Int`, `Long`, `Double`. More efficient to use these specializations than generic ones.

| Functional Interface  | Abstract Method       | 
|-----------------------|-----------------------|
| `BooleanSupplier`     | `boolean getAsBoolean()` | 
| `PSupplier`           | `p getAsP()`          | 
| `PConsumer`           | `void accept(p)`      | 
| `ObjPConsumer<T>`     | `void accept(T, p)`   |
| `PFunction<T>`        | `T apply(p)`          | 
| `PToQFunction`        | `q applyAsQ(p)`       |
| `ToPFunction<T>`      | `p applyAsP(T)`       |
| `ToPBiFunction<T, U>` | `p applyAsP(T, U)`    |
| `PUnaryOperator`      | `p applyAsP(p)`       |
| `PBinaryOperator`     | `p applyAsP(p, p)`    |
| `PPredicate`          | `boolean test(p)`     |

##### Custom Functional Interfaces

Any interface with single abstract method is functional interface. Functional interfaces can be tagged with `@Functionalnterface` to produce compile time error if there are multiple abstract methods, and to produce appropriate java docs.

##### Comparator

`comparing` static method from `Comparator` takes a "key extractor" that maps `T` to a comparable type.

```java
Arrays.sort(people, Comparator.comparing(Person::getFirstName()).thenComparing(Person::getLastName));
```

Can specify lambda method to use on first name too. Can also specify `nullsFirst(naturalOrder())` etc.