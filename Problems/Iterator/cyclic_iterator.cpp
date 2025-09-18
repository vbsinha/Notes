#include "range_iterator.hpp"

template <typename T>
class cyclic_iterator : public custom_iterator<T> {
public:
    explicit cyclic_iterator(resettable_iterator<T> &_r_it): r_it {_r_it} { 
        // assert r_it.has_next() == true;
    }

    bool has_next() override {
        return true;
    }

    T next() override {
        if (!r_it.has_next()) r_it.reset();
        return r_it.next();
    }

private:
    resettable_iterator<T> &r_it;
};

int main() {
    range_iterator r_it(1, 8, 2);
    cyclic_iterator<int> c_it(r_it);
    int limit = 20;
    for (int i = 0; c_it.has_next() && i < limit; i++) {
        cout << c_it.next() << " ";
    }
    cout << endl;
}