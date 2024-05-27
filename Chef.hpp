#ifndef CHEF_HPP
#define CHEF_HPP

#include "Constants.hpp"
#include "Utilities.hpp"
#include "Party.hpp"
#include "Order.hpp"
#include <queue>
#include <mutex>
#include <thread>


class Chef {
private:
    std::queue<Order>* orderQueue;
    std::mutex* orderQueueMutex;

public:
    Chef(std::queue<Order> *orderQueue, std::mutex *orderQueueMutex);
    int calculatePreparationTime(int preparationTime);
    //void cookMeal(int preparationTime);
    void cookMealForTable(Party *party, int preparationTime);

};

#endif
