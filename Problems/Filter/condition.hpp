#ifndef CONDITION_H
#define CONDITION_H

#include<bits/stdc++.h>
using namespace std;

template <typename T>
class condition {
public:
    // Lambda does not convert to function directly
    // explicit condition (function<bool(T&)>& _pred): pred {_pred} { }

    template<typename func>
    explicit condition (func _pred): pred {_pred} { }

    bool operator() (T& t) {
        return pred(t);
    }

    bool operator() (T& t) const {
        return pred(t);
    }
    
    virtual ~condition() { }
private:
    function<bool(T&)> pred;
};

#endif