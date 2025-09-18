#include <bits/stdc++.h>
#include "custom_iterator.hpp"

using namespace std;

template <typename T>
class zigzag_iterator : public custom_iterator<T> {
private:
    vector<vector<T>> v;
    queue<pair<size_t, size_t>> q;
public:
    zigzag_iterator(vector<vector<int>>& _v): v {_v} {
        for (size_t i = 0; i < v.size(); i++) {
            if (!v[i].empty()) q.emplace(i, 0);
        }
    }

    T next() override {
        auto [vec_index, val_index] = q.front();
        q.pop();
        const T val = v[vec_index][val_index];
        if (val_index != v[vec_index].size() - 1) {
            q.emplace(vec_index, val_index + 1);
        }
        return val;
    }

    bool has_next() override {
        return !q.empty();
    }
};


int main() {
    vector<vector<int>> v {{1, 8, 2}, {}, {2, 3}, {1, 8, 9, 9}};
    zigzag_iterator<int> z(v);
    while (z.has_next()) {
        cout << z.next() << " ";
    }
    cout << endl;
}