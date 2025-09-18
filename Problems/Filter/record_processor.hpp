#ifndef RECORD_PROCESSOR_H
#define RECORD_PROCESSOR_H

#include <bits/stdc++.h>
#include "condition.hpp"

template <typename T>
class record_processor {
public:
    record_processor(vector<T>& _records): 
        records {_records} { }

    record_processor& filter_records(condition<T>& cond) {
        vector<T> filtered_records;
        for (T& record: records) {
            if (cond(record)) {
                filtered_records.push_back(record);
            }
        }
        records = filtered_records;
        return *this;
    }

    template<class compare>
    record_processor& sort(compare comp) {
        std::sort(begin(records), end(records), comp);
        return *this;
    }

    vector<T> get_page(int page_size, int index = 0) {
        vector<T> paged_records;
        for (int i = index; i < index + page_size && i < records.size(); i++) {
            paged_records.push_back(records[i]);
        }
        return paged_records;
    }
    
private:
    vector<T> records;
};

#endif