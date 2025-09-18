#include "range_iterator.hpp"
#include <bits/stdc++.h>

using namespace std;

range_iterator::range_iterator(const int _start, const int _end, const int _skip) :
        start {_start},
        end {_end},
        skip {_skip},
        cur {_start} { }
    
bool range_iterator::has_next() {
    return cur < end;
}

int range_iterator::next() {
    int val = cur;
    cur += skip;
    return val;
}

void range_iterator::reset() {
    cur = start;
}

// int main() {
//     range_iterator r(1, 8, 2);
//     while (r.has_next()) {
//         cout << r.next() << " ";
//     }
//     cout << endl;
// }