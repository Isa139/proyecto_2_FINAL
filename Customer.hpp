#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>
#include <iostream>
#include "Utilities.hpp"

class Customer {
private:
    std::string selectedMeal;

public:
    std::string name;
    char status;

    Customer(std::string name, std::string meal);

    std::string getMeal();
    void orderMeal();
    int calculateEatingTime(int eatingTime);
    void eatMeal(int durationTime);
};

#endif
