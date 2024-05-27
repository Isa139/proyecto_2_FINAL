#ifndef PARTY_HPP
#define PARTY_HPP

#include <vector>
#include <string>
#include "Customer.hpp"

class Party {
public:
    Party(int size, int &customerCounter, const std::vector<std::string> &menu);
    ~Party();

    int getSize() const;
    std::string getDishes() const;
    int getEatingTime() const;
    int size;
    std::vector<Customer*> customers;
};

#endif // PARTY_HPP
