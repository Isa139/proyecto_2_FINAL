#ifndef WAITER_HPP
#define WAITER_HPP

#include <queue>
#include <mutex>
#include "Order.hpp"
#include "Customer.hpp"
#include "Inventory.hpp"

class Waiter {
private:
    std::queue<Order> *orderQueue;
    std::mutex *orderQueueMutex;

public:
    Waiter(std::queue<Order>* orderQueue, std::mutex* orderQueueMutex, struct recipe* recipeHead);
    struct recipe* recipeHead;
    void takeOrder(Customer* customer);
};

#endif
