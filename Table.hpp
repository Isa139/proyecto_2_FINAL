#ifndef TABLE_HPP
#define TABLE_HPP

#include <queue>
#include <mutex>
#include "Party.hpp"
#include "Waiter.hpp"
#include "Chef.hpp"

class Table {
private:
    int number;
    int capacity;
    bool occupied;
    Party *currentParty;
    std::mutex tableMutex;
    Waiter *waiter;
    Chef *chef;

public:
    Table(int num, Waiter *waiter, Chef *chef);

    void simulateEating();
    void assignParty(Party *party);
    void checkAndFreeTable();
    void freeTable();
    bool isOccupied();
    void manageTables(std::queue<Party *> &customerQueue, std::mutex &queueMutex);
};

#endif
