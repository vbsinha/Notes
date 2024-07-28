### Threads

```java
public interface Runnable {
    void run();
}
```

```java
Runnable r = () -> {
    try {
        Thread.sleep(1);
    } catch (InterruptedException e) {}
}
var t = new Thread(r);
t.start();
```

Do not subclass `Thread` to create threads and call `start`. Keep task and the mechanism running it separate. Also can use thread pool.

Calling `run` executes the task in the same thread.

Thread states:
1. New: `new Thread(r)`. Execution not started.
2. Runnable: On invoking `start`. May or may not be running. Thread can call `static void yield()` to preempt itself.
3. Blocked: When a thread tries to acquire an intrinsic lock (not a `Lock` from `java.util.concurrent`), that is currently held by another thread, it becomes *blocked*. Unblocked when other threads have elinquished the lock and the thread scheduler has allowed this thread to hold it.
4. Waiting: Waiting for another thread to notify the scheduler of a condition. `Object.wait`, `Thread.join`, `Lock` or `Condition` in `java.util.Concurrent`.
5. Timed waiting: `Thread.sleep`, or timed version of `Object.wait`, `Thread.join`, `Lock.tryLock` or `Condition.await`.
6. Terminated:
    * `run` exited normally.
    * uncaught exception from `run`.
    * Do not call `stop` to kill.

`suspend`, `stop`, and `resume` are deprecated.

#### Interrupting Threads

Thread terminates when:
1. `run` returns
2. uncaught exception in `run`.

There is no way to *force* a thread to terminate (except deprecated `stop`).
`interrupt` can be used to *request* termination.

*interrupt* methods sets the interrupted status boolean flag on thread. Each thread should occasionally check whether it has been interrupted. When the `interrupt` method is called on a thread that blocks on a call such as sleep or wait, the blocking call is terminated by an `InterruptedException`. No language requirement to terminate an interrupted thread.

```java
Runnable r = () -> {
      try {
         . . .
         while (!Thread.currentThread().isInterrupted() && more work to do) {
            // do more work
         }
      }
      catch(InterruptedException e) {
         // thread was interrupted during sleep or wait
      }
      finally {
         // cleanup
      }
      // exiting the run method terminates the thread
   };
```

If loop calls `sleep` do not check interrupted status.

```java
Runnable r = () -> {
      try {
         . . .
         while (more work to do) {
            do more work
            Thread.sleep(delay);
         }
      }
      catch(InterruptedException e) {
         // thread was interrupted during sleep
      }
      finally {
         // cleanup
      }
      // exiting the run method terminates the thread
   };
```

Do not squelch `InterruptedException`. Instead either call `Thread.currentThread().interrupt()` from `catch`, or throw to the caller using `throws` on the method.

#### Daemon threads

`t.setDaemon(true)` called before `start`.

A daemon is a thread that has no other role in life than to serve others. When only daemon threads remain, the virtual machine exits.

#### Thread name

`t.setName("Crawler")`

#### Handler for uncaught exceptions

`run` cannot throw checked exceptions`.

No `catch` clause for unchecked exceptions. Just before thread's death, the exception is passed to handler for uncaught exceptions.

Handler implements `Thread.UncaughtExceptionHandler`, which has single method `void uncaughtException(Thread t, Throwable e)`.

Install a handler into a thread using `setUncaughtExceptionHandler`. If you don’t install a default handler, the default handler is `null`. However, if you don’t install a handler for an individual thread, the handler is the thread’s `ThreadGroup` object. (Do not use ThreadGroup due to other better alternatives.)

`ThreadGroup` class implements `Thread.uncaughtExceptionHandler`, whose `uncaughtException`:
1. If thread group has parent, `uncaughtException` of parent group is called.
2. If `Thread.getDefaultUncaughtExceptionHandler` returns non-`null` handler, it is called.
3. Otherwise, if the `Throwable` is an instance of `ThreadDeath`, nothing happens.
4. Otherwise, the name of the thread and the stack trace of the `Throwable` are printed on `System.err` (which we see on crashes).

#### Thread priorities

Every thread has a priority. By default, a thread inherits the priority of the thread that constructed it. Change with `setPriority` method between `MIN_PRIORITY` (1) to `MAX_PRIORITY` (10), `NORM_PRIORITY` (5).

Whenever the thread scheduler has a chance to pick a new thread, it prefers threads with higher priority. However, thread priorities are highly system-dependent. Do not use thread priorities.

### Synchronization

#### Lock Objects

```java
// Cannot use try-with-resources

private Lock l = new ReentrantLock();

l.lock();
try {

} finally {
    l.unlock();
}
```

A thread can repeatedly acquire a lock it already owns. Lock has a *hold* count, every call to lock, must be unlocked. Multiple methods can use the same lock and call each other.

### Condition objects

Also called *condition variables*.

A lock can have one or more associated condition objects. 

```java
private Condition sufficientFunds;

public Bank() {
    sufficientFunds = l.newCondition();
}
```

`sufficientFunds.await()`. Thread enters a *wait set*, is not made runnable when the lock is available. Stays deactivated until other thread calls `signalAll` on the same condition `sufficientFunds.signalAll()`.

Upon signalAll, threads leave wait set and are runnable again. As soon as the lock is available one of them will acquire the lock and continue where it left off, returning from call to `await`. At this time, the thread should test the condition again.
```java
while (not ok to proceed) {
    condition.await();
}
```

Some thread must call `signalAll` or the thread will never awake.

#### Synchronized

Every object has an intrinsic lock.

```java
public synchronized void method() {

}

// same as 

public void method() {
    this.intrinsicLock.lock();
    try {

    } finally {
        this.intrinsicLock.unlock();
    }
}
```

Intrinsic Lock has an associated condition. `wait` adds thread to wait set, `notifyAll` unblocks waiting threads (equivalent to `await` and `signalAll`).

Static methods can be `synchronized`, lock of `ClassName.class` is acquired by the lock.

Preference order:
1. Use mechanisms of `java.util.concurrent`
2. `synchronized`
3. `Lock` / `Condition`

#### Synchronized blocks

```java
private Object o = new Object();

...
synchronized (o) {

}
```

Do not use synchronized blocks with `String` / wrappers since they can share objects. Use `MyClass.class` not `getClass()` to avoid subclass issue.

JVM has built in support for synchronized methods, but synchronized blocks are compiled into lengthy sequence of bytecodes.

#### Monitors

1. A monitor is a class with only private fields.
2. Each object of that class has an associated lock. All methods are locked by that lock. Since all fields are private no thread can access the fields while other thread manipulates them.
3. The lock can have any number of associated conditions.

In Java with `synchronized`:
1. Fields are not required to be private.
2. Methods are not required to be `synchronized`.
3. The intrinsic lock is available to clients.

#### volatile

If you write a variable which may next be read by another thread, or you read a variable which may have last been written by another thread, you must use synchronization.

```java
private volatile boolean done;
public boolean isDone() { return done; }
public void setDone() { done = true; }
```

The compiler will insert the appropriate code to ensure that a change to the done variable in one thread is visible from any other thread that reads the variable.

Creating a non-volatile instance field and setting methods to `synchronized` would use the same lock for the entire class and cause unnecessary blocking.

Volatile variables are not atomic.
```java
public void flip() { done != done;}
```
May not flip, as reading, flipping, and writing, is not guaranteed to be uninterrupted.

#### final

final objects are safe to share across but their internal field values could change (like in a map).

#### atomics

```java
public static AtomicLong nextNumber = new AtomicLong();
// in some thread ... guarantees atomicity
long id = nextNumber.incrementAndGet();
```

```java
public static AtomicLong largest = new AtomicLong();
largest.updateAndGet(x -> Math.max(x, observed));
largest.accumulateAndGet(observed, Math::max);
```

When a large number of threads are accessing, use `LongAdder`, `LongAccumulator`.
A `LongAdder` is composed of multiple variables whose collective sum is the current value.

```java
// For general operations
// Operator must be associative and commutative
var adder = new LongAccumulator(Long::sum, 0);
// in some thread. . .
adder.accumulate(value);
```

#### Deadlocks

Java will not automatically void or break deadlocks. Programs must be designed so that they do not occur.

#### Thread local variables

```java
public static final SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
// If multiple threads call this, data can get messed up since the object is shared
String dateStamp = dateFormat.format(new Date());
```

```java
public static final ThreadLocal<SimpleDateFormat> dateFormat =
   ThreadLocal.withInitial(() -> new SimpleDateFormat("yyyy-MM-dd"));
```

The first call to `get` calls the lambda constructor. Later calls returns the instance belonging to the current thread.
```java
String dateStamp = dateFormat.get().format(new Date());
```

### Thread safe collections

#### Blocking queues

A blocking queue causes a thread to block when you try to add an element when the queue is currently full or to remove an element when the queue is empty. 

`put` and `take`

#### Maps, Sets, Queues

`ConcurrentHashMap`, `ConcurrentSkipListMap`, `ConcurrentSkipListSet`, and `ConcurrentLinkedQueue`.

Data structure will not be corrupted, but result is not predictable since not atomic:
```java
Long oldValue = map.get(word);
Long newValue = oldValue == null ? 1 : oldValue + 1;
map.put(word, newValue); // ERROR--might not replace oldValue
```

```java
map.compute(word, (k, v) -> v == null ? 1 : v + 1);
map.merge(word, 1L, Long::sum);
```

#### Bulk operations on Concurrent Hash Maps

Types:
1. `search`
2. `reduce`
3. `forEach`

Unless you happen to know that the map is not being modified while a bulk operation runs, you should treat its result as an approximation of the map’s state.

*Methods Skipped*

#### Concurrent Set Views

No `ConcurrentHashSet`.

Use the view as a set.
```java
Set<String> words = ConcurrentHashMap.<String>newKeySet();
```

To add elements to a set, create a view with default for value.
```java
Set<String> words = map.keySet(1L);
words.add("Java"); // "Java" -> 1
```

#### Parallel Arrays algorithms

`Arrays.parallelSort`, `Arrays.parallelPrefix`

#### Miscellaneous

```java
List<E> synchArrayList = Collections.synchronizedList(new ArrayList<E>());
Map<K, V> synchHashMap = Collections.synchronizedMap(new HashMap<K, V>());
```

Add iterators in synchronized blocks.
```java
synchronized (synchHashMap)
{
   Iterator<K> iter = synchHashMap.keySet().iterator();
   while (iter.hasNext()) . . .;
}
```

Prefer using the concurrent versions.

## Tasks and Thread Pools