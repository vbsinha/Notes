## Exception

Exception objects are instance of a class derived from `Throwable` interface.

1. `Error`. Implements Throwable. Internal errors and resource exhaustion from Java runtime system. Should not throw objects of this type. In case it is thrown, terminate the program.
2. `Exception`
    a. `RuntimeException` Due to programming error like bad cast, out of bound access, null pointer access.
    b. Not inherited from `RuntimeException`. Use this for custom exceptions.

Exceptions deriving from `Error` or `RuntimeException` are *unchecked exceptions*. All others are *checked exceptions*. Exception handlers for all checked exceptions must be provided.

Exceptions can occur due to:
1. A method call threw a checked exception.
2. The method itself detects an error and throws a checked exception.
3. Programming error causes an unchecked exception.
4. Internal error occurs in VM or runtime library.

Methods must declare all the exceptions it can throw from the first two points above.

```java
public String readFile(String name) throws FileNotFoundException, EOFException { ... }
```

Do not throw unchecked RuntimeExceptions. Instead fix the program.
```java
// bad style
public String readFile(String name) throws ArrayIndexOutOfBoundsException { ... }
```

An overridden method cannot throw a more general exception than the superclass method. If superclass does not throw, subclass cannot throw. If superclass throws, subclass can throw a narrower exception or not throw.

```java
throw new EOFException("Error Message");
```

```java
public class CustomException extends IOException {
    public CustomException() {}
    public CustomException(String error) { super(error); }
}
```

#### Catching exception

```java
try {

} catch (ExceptionType e) {

}
```
If exception is thrown in `try`, remaining code in try is skipped and code in catch is run. If try does not throw an exception then catch is skipped. If catch does not match the type of exception thrown, the method immediately exits, and the process repeats with the caller.

```java
try {

} catch (FileNotFoundException | UnknownHostException e) {
    // types cannot be subclasses of each other
    // e is implicitly final
    e.getMessage();
    e.getClass().getName();
    // Throwable also has getStackTrace() method
} catch (IOException e) {

}
```

#### Rethrowing and chaining exceptions

```java
try {

} catch (SQLException e) {
    var f = new ServletException("error");
    f.initCause(e);
    throw f;
}

Throwable original = caughtException.getCause();
```

If a checked exception occurs in a method that is not allowed to throw further, it can be wrapped as a RuntimeException and thrown.

```java
// Log and rethrow
// Ok to catch Exception and throw a SQLException
// Since compiler knows try only throws SQLException and catch does not change it 
public void method() throws SQLException { 
    try {

    } catch (Exception e) {
        // log
        throw e;
    }
}
```

#### `finally`

```java
var in = new FileInputStream(...);
try {
    // Assume no returns in try
    // 1
    // code that might throw exceptions
    // 2
} catch (IOException e) {
    // 3
    // code that might throw exceptions
    // 4
} finally {
    // 5
    in.close();
}
// 6
```

1. *No exceptions are thrown*. 1, 2, 5, 6
2. *try throws IOException that gets caught in catch, and catch does not throw*.  1 (2 is skipped), 3, 4, 5, 6
3. *try throws IOException that gets caught in catch, but catch throws too*.  1 (2 is skipped), 3 (4 is skipped), 5
4. *try throws an exception not caught in catch*. 1, 5

finally can exist without catch.

finally gets called however the block is exited (except some abrupt abortions or infinite loops). finally get called just before try's return if it has one.

Do not add control flow changing statements to finally like `return`, `break`, `continue`, `throw`. finally's return will mask try's return, and will swallow try's exception.

### `try`-with-Resources

Resource should implement `AutoCloseable`, which has one method `void close() throws Exception`.
```java
// in & out could be previously defined effectively final variables
try (var in = new Scanner(Path.of("in.txt"), StandardCharsets.UTF_8);
      var out = new PrintWriter("out.txt", StandardCharsets.UTF_8)) {
        ...
}
// in.close(), and out.close() get called
```

If both `try` and `close` throw, original exception of `try` is thrown and the exception of `close` is considered "suppressed". They are automatically caught and added to the original exception with the `addSuppressed` method.

A try-with-resources statement can itself have catch clauses and even a finally clause. These are executed after closing the resources.

### Miscellaneous

Exceptions incur performance penalty only when thrown