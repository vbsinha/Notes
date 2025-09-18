#include "condition.hpp"
#include "or_condition.hpp"
#include "and_condition.hpp"
#include "record_processor.hpp"
#include "transaction.hpp"
#include <bits/stdc++.h>

int main() {
    vector<transaction> transactions = {{1, "u1", "u2", 10, 108},
                                        {2, "u2", "u3", 120, 109},
                                        {3, "u3", "u2", 30, 110},
                                        {4, "u3", "u1", 10, 111},
                                        {5, "u1", "u2", 50, 112},
                                        {6, "u1", "u3", 60, 113}};

    record_processor<transaction> rpt {transactions};

    condition<transaction> txns_from_109 { [](transaction &t) -> bool { return t.get_timestamp() >= 109; } };
    condition<transaction> txns_until_111 { [](transaction &t) -> bool { return t.get_timestamp() <= 111; } };
    condition<transaction> txns_to_u3 { [](transaction &t) -> bool { return t.get_to_id() == "u3"; } };
    condition<transaction> cond = or_condition<transaction> {txns_to_u3, and_condition<transaction> {txns_from_109, txns_until_111} };

    auto sort_fn = [](transaction &t1, transaction &t2) {return t1.get_amount() < t2.get_amount(); };
    rpt.filter_records(cond).sort(sort_fn);
    
    vector<transaction> first_page = rpt.get_page(2);
    cout << "First Page:\n";
    for (transaction txn: first_page) {
        txn.log();
    }
    cout << "Second Page:\n";
    vector<transaction> second_page = rpt.get_page(2, 2);
    for (transaction txn: second_page) {
        txn.log();
    }

}