#ifndef AND_CONDITION_H
#define AND_CONDITION_H

template <typename T>
class and_condition: public condition<T> {
public:
    and_condition(const condition<T>& cond1, const condition<T>& cond2): 
        condition<T> {[&cond1, &cond2](T &t) { return cond1(t) && cond2(t); }} { }
};

#endif