#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

struct Order {
    std::string customerName;
    std::string mealName;

    Order();
    Order(std::string customerName, std::string mealName);
};

#endif

