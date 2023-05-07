## Generics

Use concrete classes for initialization only.

### Collection Interface

Fundamental interface for collection classes.

```java
public interface Collection<E> {
    boolean add(E element); // true if collection changed
    Iterator<E> iterator();
    int size()
    boolean isEmpty()
    boolean contains(Object obj)
    boolean containsAll(Collection<?> c)
    boolean equals(Object other)
    boolean addAll(Collection<? extends E> from)
    boolean remove(Object obj)
    boolean removeAll(Collection<?> c)
    void clear()
    boolean retainAll(Collection<?> c)
    Object[] toArray()
    ...
}
```

For creating a new collection use the `Asbtract...` class like `AbstarctCollection`.

```java
public abstract class AbstractCollection<E>
      implements Collection<E> {
    boolean add(E element);
    public abstract Iterator<E> iterator();

    // default implementation available for other methods.
    public boolean contains(Object obj)
    {
        for (E element : this) // calls iterator()
            if (element.equals(obj))
                return true;
        return false;
    }
    ...
}
```

#### Iterator interface

```java
public interface Iterator<E> {
   E next(); // collections throw NoSuchElementException if end is reached
   boolean hasNext();
   void remove();
   default void forEachRemaining(Consumer<? super E> action);
}
```

Think of iterator as between elements not like an index pointing to an element.

for-each loop works for any object that implements `Iterable`.
```java
public interface Iterable<E> {
   Iterator<E> iterator();
   ...
}
```
`Collection` interface extends `Iterable` interface.

### Interfaces in Collections Framework

```
        Iterable
            |
        Collection
            |
    ------------------
    |       |         |
  List     Set      Queue
            |         |
     SortedSet      Deque

Map -> SortedMap -> NavigableMap
Iterator -> ListIterator
RandomAccess (tagging interface to check if RandomAccess is allowed)
```

Fundamental interfaces: `List` and `Map`.

```java
// From Map
V get(K key)
V put(K key, V value)
```

```java
// From List
void add(int index, E element)
void remove(int index)
E get(int index)
E set(int index, E element)
```

`SortedSet` and `SortedMap` interfaces expose the comparator object used for sorting, and they define methods to obtain views of subsets of the collections. 

`NavigableSet` and `NavigableMap` contain additional methods for searching and traversal in sorted sets and maps. `TreeSet` and `TreeMap` classes implement these interfaces.

### Concrete Collections

```
AbstractCollection -> AbstractList -> AbstractSequentialList -> LinkedList
                                   -> ArrayList
                   -> AbstractSet -> HashSet -> LinkedHashSet
                                  -> EnumSet
                                  -> TreeSet
                   -> AbstractQueue -> PriorityQueue
                   -> ArrayQueue

AbstractMap -> HashMap -> LinkedHashMap
            -> TreeMap
            -> EnumMap
            -> WeakHashMap
            -> IdentityHashMap
```

Class must implement `Comparable` or a `Comparator` needs to be supplied for `TreeSet` or `PriorityQueue`.

##### HashSet

If an element in a HashSet is mutated it may no longer be in the correct bucket.

##### ArrayList

```java
var arr = new ArrayList<Base>(100); // capacity 100, size 0
arr.set(0, new Base()); // Error
```

#### Map

```java
map.forEach((k, v) -> ...)
```

```java
counts.put(word, counts.getOrDefault(word, 0) + 1);

counts.putIfAbsent(word, 0);
counts.put(word, counts.get(word) + 1); // now we know that get will succeed

counts.merge(word, 1, Integer::sum); // 1 if not present, sum otherwise
```

##### Map views

```java
Set<K> keySet()
Collection<V> values()
Set<Map.Entry<K, V>> entrySet()
```

##### WeakHashMap

Uses *WeakReferences*. If there is a key in the map whose last reference does not exist, then it will be garbage collected.

##### LinkedHashSet and LinkedHashMap

`LinkedHashSet` and `LinkedHashMap` remember the order in which data was inserted, using a linked list internally.

##### EnumSet and EnumMap

Useful when the key set is from an Enum. No public constructor, use factor method like `EnumSet.of`.

#### Copies and Views

##### Small Collections

```java
// All return unmodifiable collections
List.of("a", "b"); // non null values only
Set.of(1,2); // 0 elements also allowed
Map.of("a", 1, "b", 2); // keys cannot be duplicate
Collections.nCopies(n, obj);
Collections.emptySet // legacy
Collections.singleton // legacy
```

##### Unmodifiable copy

Unmodifiable copy that does not change if the original collection is changed.
```java
Set<String> nameSet = Set.copyOf(names);
List<String> nameList = List.copyOf(names);
```

If the original collection is unmodifiable and of the correct type, then copyOf simply returns it.

##### Views

Methods from `Collections` to produce *unmodifiable views*:
```java
Collections.unmodifiableCollection
Collections.unmodifiableList
Collections.unmodifiableSet
Collections.unmodifiableSortedSet
Collections.unmodifiableNavigableSet
Collections.unmodifiableMap
Collections.unmodifiableSortedMap
Collections.unmodifiableNavigableMap
```

Views can access accessor methods of the collection. Mutator methods throw `UnsupportedOperationException`. The original collection is modifiable and changes to it are reflected in the views.

Views wrap the interface not the collection object, so only methods defined at collection level are visible.

##### Subranges

Changes to subrange are reflected to the original collection.

```java
List<Employee> group2 = staff.subList(10, 20); // [10, 20)
```

```java
SortedSet<E> subSet(E from, E to)
SortedSet<E> headSet(E to)
SortedSet<E> tailSet(E from)
SortedMap<K, V> subMap(K from, K to)
SortedMap<K, V> headMap(K to)
SortedMap<K, V> tailMap(K from)
NavigableSet<E> subSet(E from, boolean fromInclusive, E to, boolean toInclusive)
NavigableSet<E> headSet(E to, boolean toInclusive)
NavigableSet<E> tailSet(E from, boolean fromInclusive)
```

##### Checked views

```java
var strings = new ArrayList<String>();
ArrayList rawList = strings; // warning only, not an error
rawList.add(new Date());
```

```java
List<String> safeStrings = Collections.checkedList(strings, String.class);
ArrayList rawList = safeStrings;
rawList.add(new Date()); // checked list throws a ClassCastException
```

##### Synchronized views

Views to make regular collections thread-safe.

```java
var map = Collections.synchronizedMap(new HashMap<String, Employee>());
```

### Algorithms

##### Sorting

```java
var l = new List<String>();
Collections.sort(l);
l.sort(Comparator.comparing(Employee::getSalary));
l.sort(Comparator.reverseOrder());
l.sort(Comparator.comparing(Employee::getSalary).reversed());
```

##### Binary Search

```java
Collections.binarySearch(c, element);
```

Non negative return value indicates the index. Negative value can used to compute where to insert the element.

```java
if (i < 0)
   c.add(-i - 1, element);
```

##### Bulk Operations

```java
// Set intersection
Set<String> intersection = new Set<String>(set1);
intersection.retainAll(set2);
```

```java
// Bulk operation on views
map.keySet().removeAll(set);
```

```java
// Bulk operations with subranges
l1.addAll(l2.subList(0, 10));
l1.subList(0,10).clear();
```

##### Conversion between Collections and Arrays

```java
String[] names = ...;
List<String> nameList = List.of(names);
String[] namesArray = nameList.toArray(String[]::new);
```
