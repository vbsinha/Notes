import java.util.List;

// Run: javac LazySearchTest.java && java LazySearchTest
public class LazySearchTest {
    public static void main(String[] args) {
        List<Integer> values = List.of(2,4,6,8,10);
        LazySearch<Integer> l = new LazySearch<>(values);

        assert l.map(v -> 3*v).search(6) == 0;
        assert l.map(v -> 2*v).map(v -> v + 2).search(22) == 4;
    }
}