import java.util.ArrayList;
import java.util.List;
import java.util.function.*;

public class LazySearch<T> {
    private List<T> values;
    protected List<UnaryOperator<T>> ops;

    public LazySearch(List<T> values) {
        this.ops = new ArrayList<>();
        this.values = values;
    }

    public LazySearch<T> map(UnaryOperator<T> op) {
        return new LazySearch<T>(this.values) {
    
            @Override
            public LazySearch<T> map(UnaryOperator<T> _op) {
                ops.add(_op);
                return this;
            }
                
        }.map(op);
    }

    public int search(T target) {
        for (int i = 0; i < values.size(); i++) {
            T v = values.get(i);
            for (var op : ops) {
                v = op.apply(v);
                if (v == null) continue;
            }
            if (v.equals(target)) return i;
        }
        return -1;
    }
}