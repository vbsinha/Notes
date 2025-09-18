#ifndef RANGE_ITERATOR_H
#define RANGE_ITERATOR_H

#include "resettable_iterator.hpp"
#include <bits/stdc++.h>

using namespace std;

class range_iterator : public resettable_iterator<int> {
public:
    range_iterator(const int _start, const int _end, const int _skip);
    
    bool has_next() override;

    int next() override;

    void reset() override;

private:
    int start;
    int end;
    int skip;
    int cur;
};

#endif