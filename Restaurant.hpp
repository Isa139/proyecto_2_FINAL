#ifndef RESTAURANT_HPP
#define RESTAURANT_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include "Party.hpp"
#include "Order.hpp"
#include "Table.hpp"
#include "Chef.hpp"
#include "Waiter.hpp"
#include "Constants.hpp"

class Restaurant {
private:
    std::queue<Party *> customerQueue;
    std::queue<Order> orderQueue;
    Table *tables[TABLE_COUNT];
    std::mutex queueMutex;
    std::mutex orderQueueMutex;
    int customerCounter;
    Chef *chef;

public:
    Restaurant();
    void addPartyToQueue(Party *party);
    void assignTables();
    void threads();
    Party *createParty(int size, const std::vector<std::string> &menu);
};

#endif // RESTAURANT_HPP
