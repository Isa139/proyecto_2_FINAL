#include "Table.hpp"
#include "Constants.hpp" // Ensure this is included
#include "Customer.hpp"
#include "Waiter.hpp"
#include "Chef.hpp"
#include <thread>         // Ensure this is included for std::this_thread::sleep_for
#include <chrono>
#include <iostream>

using namespace std;

Table::Table(int num, Waiter *waiter, Chef *chef)
    : number(num), capacity(MAX_CUSTOMERS_PER_TABLE), occupied(false), currentParty(nullptr), waiter(waiter), chef(chef) {}

void Table::simulateEating() {
    if (currentParty) {
        printf("Table %i is now serving a party of %i customers.\n", number, currentParty->size);
        for (int i = 0; i < currentParty->size; ++i) {
            currentParty->customers[i]->status = 'S';
            std::cout << currentParty->customers[i]->name << "'s status is " << currentParty->customers[i]->status << "\n";
            currentParty->customers[i]->orderMeal();
            waiter->takeOrder(currentParty->customers[i]);
            currentParty->customers[i]->status = 'W';
            std::cout << currentParty->customers[i]->name << "'s status is " << currentParty->customers[i]->status << "\n";
        }

        // Iniciar el hilo del chef para preparar la comida para toda la mesa
        std::thread chefTableThread(&Chef::cookMealForTable, chef, currentParty, MIN_PREP_TIME);
        chefTableThread.detach();

        for (int i = 0; i < currentParty->size; ++i) {
            std::cout << currentParty->customers[i]->name << " has begun eating.\n";
            currentParty->customers[i]->status = 'E';
            std::cout << currentParty->customers[i]->name << "'s status is " << currentParty->customers[i]->status << "\n";
            currentParty->customers[i]->eatMeal(MIN_EATING_TIME);
            std::cout << currentParty->customers[i]->name << " is done eating.\n";
            currentParty->customers[i]->status = 'D';
            std::cout << currentParty->customers[i]->name << "'s status is " << currentParty->customers[i]->status << "\n";
        }

        std::thread checkTableThread(&Table::checkAndFreeTable, this);
        checkTableThread.detach();
    }
}



void Table::assignParty(Party *party) {
    std::lock_guard<std::mutex> lock(tableMutex);
    currentParty = party;
    occupied = true;
    simulateEating();
}

void Table::checkAndFreeTable() {
    while (true) {
        bool allDone = true;
        {
            std::lock_guard<std::mutex> lock(tableMutex);
            for (int i = 0; i < currentParty->size; ++i) {
                if (currentParty->customers[i]->status != 'D') {
                    allDone = false;
                    break;
                }
            }
        }

        if (allDone) {
            freeTable();
            return;
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void Table::freeTable() {
    std::lock_guard<std::mutex> lock(tableMutex);
    occupied = false;
    currentParty = nullptr;
    std::cout << "Table " << number << " is now free.\n";
}

bool Table::isOccupied() {
    std::lock_guard<std::mutex> lock(tableMutex);
    return occupied;
}

void Table::manageTables(std::queue<Party *> &customerQueue, std::mutex &queueMutex) {
    while (true) {
        Party *party = nullptr;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!customerQueue.empty() && !isOccupied()) {
                party = customerQueue.front();
                customerQueue.pop();
            }
        }

        if (party) {
            assignParty(party);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}
