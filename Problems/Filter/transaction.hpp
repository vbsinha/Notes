#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>

using namespace std;

class transaction {
public:
    transaction(int _id, string _from_id, string _to_id, int _amount, int _timestamp):
        id {_id},
        from_id {_from_id},
        to_id {_to_id},
        amount {_amount},
        timestamp {_timestamp} { }

    int get_id() { return id; }

    string get_from_id() { return from_id; }

    string get_to_id() { return to_id; }

    int get_amount() { return amount; }

    int get_timestamp() { return timestamp; }

    void log () {
        std::cout << '(' << id << ", " << from_id << ", " << to_id << ", " << amount << ", " << timestamp << ')' << std::endl;
    }

private:
    int id;
    string from_id;
    string to_id;
    int amount;
    int timestamp;
};

#endif