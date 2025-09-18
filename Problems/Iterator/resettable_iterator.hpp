#ifndef RESETTABLE_ITERATOR_H
#define RESETTABLE_ITERATOR_H

#include "custom_iterator.hpp"

template <typename T>
class resettable_iterator : public custom_iterator<T> {
public:
    virtual void reset() = 0;
};

#endif