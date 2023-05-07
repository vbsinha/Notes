import java.util.List;

public class LazySearchTest {
    public static void main(String[] args) {
        List<Integer> values = List.of(2,4,6,8,10);
        LazySearch l = new LazySearch(values);

        System.out.println(l.map(v -> 3*v).search(6));
        assert l.map(v -> 3*v).search(6) == 0;

        System.out.println(l.map(v -> 2*v).map(v -> v + 2).search(22));
        assert l.map(v -> 2*v).map(v -> v + 2).search(22) == 4;

        
    }
}