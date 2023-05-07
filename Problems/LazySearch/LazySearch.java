import java.util.ArrayList;
import java.util.List;
import java.util.function.*;

public class LazySearch {
    private List<Integer> values;

    private LazySearch() {}

    public LazySearch(List<Integer> values) {
        this.values = values;
    }

    public LazySearch map(UnaryOperator<Integer> op) {
        return new LazySearch() {

            private List<UnaryOperator<Integer>> ops;
    
            { ops = new ArrayList<>(); 
              // ops.add(op); <- can replace the .map(op) at the end of the anon inner class
            }
    
            @Override
            public LazySearch map(UnaryOperator<Integer> op) {
                ops.add(op);
                return this;
            }
    
            @Override
            public int search(Integer target) {
                System.out.println("search called " + LazySearch.this.values + " " + ops);
                for (int i = 0; i < values.size(); i++) {
                    Integer v = values.get(i);
                    for (var op : ops) {
                        v = op.apply(v);
                    }
                    if (v == target) { return i; }
                }
                return -1;
            }
        }.map(op);
    }

    public int search(Integer target) {
        return -1;
    }
}