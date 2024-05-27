#include "Order.hpp"

Order::Order() : customerName(""), mealName("") {}

Order::Order(std::string customerName, std::string mealName) : customerName(customerName), mealName(mealName) {}
