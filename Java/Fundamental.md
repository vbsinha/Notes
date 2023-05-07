Java has eight primitive types:
1. int (4 bytes)
2. short (2 bytes)
3. long (8 bytes) `1l`
4. byte (1 byte)
5. float (4 bytes) `1.0f`
6. double (8 bytes) `1.0`
7. boolean `true` or `false`
8. char (2 bytes) `'a'`

##### Integers
Longs suffixed by `L` or `l`.
Hexes, Octal, Bin prefexed by `0x`, `0`, `0b` resp.
Underscores allowed, e.g. `1_000_000`.

##### Floats

Longs suffixed by `F` or `f`. Unsuffixed decimals are double.
Three special floating-points: Positive infinity (`Double.POSITIVE_INFINITY` :  positive number / 0), Negative infinity (`Double.NEGATIVE_INFINITY`), NaN (`Double.NEGATIVE_NAN` 0/0 or sqrt of negative number). All NaNs are different from each other: use `Double.isNaN(x)` not `x == Double.NAN`.

#### Variables

Declaration: `int i` (cannot use declared variables that are uninitialized: compile time error)
Intialization: `i = 2`
Declaration and initialization: `int i = 2`

#### Arithmetic Operations

Integer division by zero raises exception, while float division by zero returns NaN.

##### Conversions

                      char
                       |
                       \/
     byte -> short -> int ->  long
                       .   \  . .
                       .   .\   .
                       .  .  \  .
                    float ->  double

Solid arrows: Conversion without information loss

If either operand is double, the other is converted to double.
If either operand if float, the other is converted to float.
If either operand if long, the other is converted to long.
Otherwise, both are converted to int.

##### Casts

Possible information loss conversions are done by casts: `double d = 2.0; int i = (int) d;`.
No casts possible between boolean and numeric types.

#### Switch

Switch allows int, String, enums. Switch with int, and string must have default.

```java
// Switch expression with no fall through
switch(val){
    case 0 -> { Sytem.out...; yield '0'; }
    case 1 -> '1';
    default -> '-1';
}
```

```java
// Switch expression with fall through (old style)
switch(val){
    case 0: 
        System.out.println('0');
    case 1:
        yield '1';
    default: 
        yield '-1';
}
```
 
```java
// Switch statement with no fall through
switch(val){
    case 0 -> i = 0;
    case 1 -> i = 1;
    default -> i = -1;
}
```

```java
// Switch expression with fall through (old style)
switch(val){
    case 0: 
        i = 0;
    case 1: 
        j = 1;
        break;
    default: 
        i = -1;
}
```

```java
// Switch with multiple values
// If val is null, will raise NullPointerException
switch(val){
    case "A", "B" -> '0';
    case "C" -> '1';
    default -> '-1';
}
```

```java
Size val;
switch(val){
    case SMALL -> '0'; // No need for Size.SMALL
    // ...
}
```

##### Bitwise shift

`>>>` fills top bits with `0`, `>>` extends sign bit.

#### Operator Precedence

| Operator                  | Associativity |
| ------------------        |---------------|
| [ ] . ( *method call* )   | left to right |
| ! ~ ++ -- unary+ unary- (*cast*) new   | right to left |
| * / %                     | left to right |
| + -                       | left to right |
| << >> >>>                 | left to right |
| < <= > >= instanceof      | left to right |
| == !=                     | left to right |
| &                         | left to right |
| ^                         | left to right |
| \|                         | left to right |
| &&                        | left to right |
| \|\|                        | left to right |
| ?:                        | right to left |
| = += -= *= /= %= &= \|= ^= &&= <<= >>= >>>=   | right to left |

#### Strings

Strings are immutable. Use `s.equals(t)` not `s == t`. Virtual Machine may share string literals, that is two string variables with the same string value might be referring to the same location.

`s.length()`

`new StringBuilder().append('a').append('b').build()`

`StringBuffer` has same API as `StringBuilder` but supports multiple threads. It is also less efficient.

#### Input

```java
Scanner in = new Scanner(System.in);
String name = in.nextLine(); // Line with whitespace
String firstName = in.next(); // word
int age = in.nextInt();

// Reading from File
Scanner in = new Scanner(Path.of("myfile.txt"), StandardCharsets.UTF_8);
```

Scanner constructor can throw `IOException`.

#### Output

```java
PrintWriter out = new PrintWriter("myfile.txt", StandardCharsets.UTF_8);
out.println(...)
out.print(...)
out.printf(...)
```

#### Labelled statements

Statements (including block statements) can be labelled and break and continue can takes labels. Can only move out of block, never into a block.

```java
b1:
while(...) {
    while (...) {
        if (...) {
            break b1:
        }
    }
}
```

###  Arrays

Declared as `type[] array_name` (preferred) or `type arrayname[]`.

1. `int[] a = new int[100];`
2. `int[] a = new int[0];`
3. `int[] a = {2, 3, 4};`
4. `int[] a = {2, 3, 4,};`
5. `int[] a; a = new int[] {2, 3, 4};`
6. `int[] a = new int[n];` size determined at runtime.

Array of numbers, booleans, objects is initialized with zero, false, null resp.

`arr.length`

`ArrayIndexOutOfBoundsException`

`arr.toString()` outputs `[1, 2, 3]`.

Array is similar to a pointer to an array allocated on a heap.
`int[] arr2 = arr1;` `arr2` and `arr1` refer to the same array.

Multidimensional array is an array of arrays.
1. `int[][] a = new int[5][6]`.
2. `int[][] a = new int[5][]; a[0] = new int[3];` to create ragged arrays.


#### Arrays API

1. Sorting:
    a. `static void sort(int[])` etc. for all primitives.
    b. `static void sort(int[], int from, int to)` etc.
    c. `static void sort(Object[])`, all objects in the array must be comparable: i.e. `e1.compareTo(e2)`.
    d. `static <T> void sort(T[], Comparator<? super T> c)`
`sort(arr);`, all objects in the array must be comparable: i.e. `e1.compareTo(e2)`.
2. Reverse-sorting: For *primitives* sort and reverse. For object `Arrays.sort(a, Collections.reverseOrder());`.
3. `equals(Object[], Object[])`. Same length and object equality.
4. `static <T> List<T> asList(T... a)`. 
    a. Returns a fixed-size list that is serializable and implements `RandomAccess`. Cannot change length: add / remove. Can modify elements in it.
    b. Returned list is just a wrapper. No data is copied or created. Modifications to list will be reflected in the array.
    c. `List<String> l = Arrays.asList("a", "b");`
5. `int[] arr2 = Arrays.copyOf(arr1, arr1.length)` to copy. If size passed is larger than `arr1.length`, then initialize with defaults, if smaller then initial values are copied.
6. `binarySearch`, `fill` etc.

#### for-each loop

`for (type variable: collection)`. `collection` must be an array or an object of class that implements `Iterable` interface.

#### `assert`

```java
assert condition;
assert condition : expression;
// expression is passed to the constructor of AssertionError and converted to String
```

`java -ea Base`. Recompilation is not required. Enabling or disabling asserts is the job of *class loader*.

### Miscellaneous

1. Cannot declare identically named local variables in two nested block.
2. Else groups with the closest if.