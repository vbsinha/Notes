# Streams

What, not how, to compute.

1. Stream does not store its elements. They may be stored in an underlying collection or generated on demand.
2. Stream operations don't mutate their source.  `filter` method does not remove elements from a stream but yields a new stream in which they are not present.
3. Stream operations are *lazy* when possible. They are not executed until their result is needed.

Typical workflow:
1. Create a stream.
2. Specify operations to transform the stream into another.
3. Apply a terminal operation to produce a result. Afterwords the stream cannot be used.

We do not specify an explicit loop of how to compute the value, instead, the language optimizes.

```java
java.util.stream.Stream<T>
Stream<T> filter(Predicate<? super T> p)

java.util.Collection<E>
default Stream<E> stream()
default Stream<E> parallelStream()
```

### Constructing Streams

```java
Stream.of("a", "b"); // or pass an array
Stream<String> s = Stream.empty();

// infinite stream
Stream.generate(() -> "a");
Stream<BigInteger> integers = Stream.iterate(BigInteger.ZERO, n -> n.add(BigInteger.ONE));
```

```java
Stream.generate(Math::random).limit(10).toList(); // convert to list
```

Do not modify the collection backing the stream while carrying out a stream operation.

#### `filter`, `map`, and `flatmap`

```java
List<String> words = ...;
words.stream().filter(w -> w.length() > 12);
words.stream().map(String::toLowerCase);
words.stream().map(s -> s.substring(0, 1));

Stream<Stream<String>> result = words.stream().map(w -> codePoints(w));
// would produce [. . . ["y", "o", "u", "r"], ["b", "o", "a", "t"], . . .]

Stream<String> flatResult = words.stream().flatMap(w -> codePoints(w));
   // Calls codePoints on each word and flattens the results
```

#### Extracting and combining substreams

```java
Stream.generate(Math::random).limit(100); // first 100
Stream.of(contents.split("\\PL+")).skip(100); // skip first 100
codePoints(str).takeWhile(s -> "0123456789".contains(s)); // take until Predicate becomes false first time
codePoints(str).dropWhile(s -> s.strip().length() == 0); // drop until Predicate becomes false first time
Stream.concat(codePoints("Hello"), codePoints("World"));
```

#### Other transformations

```java
Stream.of("merrily", "merrily", "merrily", "gently").distinct();
words.stream().sorted(Comparator.comparing(String::length).reversed());
Stream.iterate(1.0, p -> p * 2)
   .peek(e -> System.out.println("Fetching " + e))
   .limit(20).toArray();
```

#### Simple reductions

```java
Optional<String> largest = words.max(String::compareToIgnoreCase);
System.out.println("largest: " + largest.orElse(""));
// Similarly min, count, etc.

Optional<String> startsWithQ
   = words.filter(s -> s.startsWith("Q")).findFirst();

Optional<String> startsWithQ
   = words.parallel().filter(s -> s.startsWith("Q")).findAny();

boolean aWordStartsWithQ
   = words.parallel().anyMatch(s -> s.startsWith("Q"));
// Similarly allMatch, noneMatch
```

#### Optional

```java
// Getting values from Optional
String result = optionalString.orElse("");
String result = optionalString.orElseGet(() -> System.getProperty("myapp.default"));
String result = optionalString.orElseThrow(IllegalStateException::new);
```

```java
optionalValue.ifPresent(results::add);
optionalValue.ifPresentOrElse(
   v -> System.out.println("Found " + v),
   () -> logger.warning("No match"));
```

```java
Optional<String> transformed = optionalString.map(String::toUpperCase);
optionalValue.map(results::add);

Optional<String> transformed = optionalString
   .filter(s -> s.length() >= 8)
   .map(String::toUpperCase);
Optional<String> result = optionalString.or(() -> // Supply an Optional
   alternatives.stream().findFirst());
```

```java
optionalValue.get() // throws NoSuchElementException
optionalValue.isPresent()
```

1. A variable of `Optional` should never be null.
2. Don't use fields or method parameters of type `Optional`.
3. Don't put `Optional` objects in a set, and don't use them as keys for a map.

```java
Optional.empty();
Optional.of(x);
Optional.ofNullable(obj); // returns Optional.empty() if obj == null
```

```java
// f returns Optional<T>, T has method g which returns Optional<U>
Optional<U> result = s.f().flatMap(T::g);
```

`stream` returns a `Stream` from `Optional` with 0 or 1 element.
```java
Optional<User> lookup(String id)
Stream<User> users = ids.map(Users::lookup)
   .filter(Optional::isPresent)
   .map(Optional::get);
// Can be replaced by
Stream<User> users = ids.map(Users::lookup)
   .flatMap(Optional::stream);
```

```java
User lookup(String id) // returns null when not found
Stream<User> users = ids.map(Users::classicLookup)
   .filter(Objects::nonNull);
Stream<User> users = ids.map(Users::classicLookup)
   .flatMap(Stream::ofNullable);
```

#### Collecting results

```java
stream.forEach(System.out::println);
// toList to get list
Object[] result = stream.toArray();
String[] result = stream.toArray(String[]::new);

List<String> result = stream.collect(Collectors.toList());
List<String> result = stream.collect(Collectors.toSet());
TreeSet<String> result = stream.collect(Collectors.toCollection(TreeSet::new));
String result = stream.collect(Collectors.joining());
String result = stream.collect(Collectors.joining(", "));

IntSummaryStatistics summary = stream.collect(Collectors.summarizingInt(String::length));
double averageWordLength = summary.getAverage();
double maxWordLength = summary.getMax();
```

#### Collecting to maps

```java
public record Person(int id, String name) {}
// Throws IllegalStateException if a key is found twice 
Map<Integer, String> idToName = people.collect(
   Collectors.toMap(Person::id, Person::name));
Map<Integer, Person> idToPerson = people.collect(
   Collectors.toMap(Person::id, Function.identity()));

// Resolve duplicate key with third argument
Stream<Locale> locales = Stream.of(Locale.getAvailableLocales());
Map<String, String> languageNames = locales.collect(
   Collectors.toMap(
      Locale::getDisplayLanguage,
      loc -> loc.getDisplayLanguage(loc),
      (existingValue, newValue) -> existingValue));

// To get TreeMap
Map<Integer, Person> idToPerson = people.collect(
   Collectors.toMap(
      Person::id,
      Function.identity(),
      (existingValue, newValue) -> { throw new IllegalStateException(); },
      TreeMap::new));
```

#### Grouping and Partitioning

```java
Map<String, List<Locale>> countryToLocales = locales.collect(
   Collectors.groupingBy(Locale::getCountry));
List<Locale> swissLocales = countryToLocales.get("CH");
   // Yields locales de_CH, fr_CH, it_CH, and maybe more
```

```java
Map<Boolean, List<Locale>> englishAndOtherLocales = locales.collect(
   Collectors.partitioningBy(l -> l.getLanguage().equals("en")));
List<Locale> englishLocales = englishAndOtherLocales.get(true);
```

#### Downstream collectors

```java
import java.util.stream.collectors.*;
Map<String, Set<Locale>> countryToLocaleSet = locales.collect(
   groupingBy(Locale::getCountry, toSet()));

Map<String, Long> countryToLocaleCounts = locales.collect(
   groupingBy(Locale::getCountry, counting()));

public record City(String name, String state, int population) {}
Map<String, Integer> stateToCityPopulation = cities.collect(
   groupingBy(City::state, averagingInt(City::population)));
Map<String, Optional<City>> stateToLargestCity = cities.collect(
   groupingBy(City::state,
      maxBy(Comparator.comparing(City::population))));
Map<Character, Integer> stringCountsByStartingLetter = strings.collect(
   groupingBy(s -> s.charAt(0),
      collectingAndThen(toSet(), Set::size)));

Map<Character, Set<Integer>> stringLengthsByStartingLetter = strings.collect(
   groupingBy(s -> s.charAt(0),
      mapping(String::length, toSet())));

Map<String, IntSummaryStatistics> stateToCityPopulationSummary = cities.collect(
   groupingBy(City::state,
      summarizingInt(City::population)));

Map<String, Set<City>> largeCitiesByState
   = cities.collect(
      groupingBy(City::state,
         filtering(c -> c.population() > 500000,
            toSet()))); // States without large cities have empty sets

record Pair<S, T>(S first, T second) {}
Pair<List<String>, Double> result = cities.filter(c -> c.state().equals("NV"))
   .collect(teeing(
      mapping(City::name, toList()), // First downstream collector
      averagingDouble(City::population), // Second downstream collector
      (list, avg) -> new Result(list,  avg))); // Combining function
```

#### Reduce

```java
List<Integer> values = ...;
// Optional since no valid answer for empty stream
Optional<Integer> sum = values.stream().reduce((x, y) -> x + y);

Integer sum = values.stream().reduce(0, (x, y) -> x + y);
int result = words.reduce(0,
   (total, word) -> total + word.length(),
   (total1, total2) -> total1 + total2);
```

#### Primitive type streams

`IntStream`, `LongStream`, `DoubleStream`

```java
IntStream stream = IntStream.of(1, 1, 2, 3, 5);
stream = Arrays.stream(values, from, to); // values is an int[] array
IntStream zeroToNinetyNine = IntStream.range(0, 100); // Upper bound is excluded
IntStream zeroToHundred = IntStream.rangeClosed(0, 100); // Upper bound is included
```

```java
Stream<String> words = . . .;
IntStream lengths = words.mapToInt(String::length);

// convert a primitive stream to object stream
Stream<Integer> integers = IntStream.range(0, 100).boxed();
```

`toArray` to return primitive array.

#### ParallelStreams

Parallel stream from collection `words.parallelStream();`.
Sequential stream to parallel `Stream.of(wordArray).parallel();`. If the terminal method is parallel all intermediate operations run parallely.

`Stream.unordered()` to indicate ordering is not needed.

```java
Map<Integer, List<String>> result = words.parallelStream().collect(
   Collectors.groupingByConcurrent(String::length));
   // Values aren't collected in stream order
```