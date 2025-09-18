#ifndef CUSTOM_ITERATOR_H
#define CUSTOM_ITERATOR_H

template <typename T>
class custom_iterator {
public:
    virtual bool has_next() = 0;
    virtual T next() = 0;
};

#endif