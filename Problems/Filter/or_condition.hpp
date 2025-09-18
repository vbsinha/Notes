#ifndef OR_CONDITION_H
#define OR_CONDITION_H

template <typename T>
class or_condition: public condition<T> {
public:
    or_condition(const condition<T> cond1, const condition<T> cond2): 
        condition<T> {[&cond1, &cond2](T &t) { return cond1(t) || cond2(t); }} { }
};

#endif