#include "Restaurant.hpp"
#include "Constants.hpp" // Ensure this is included
#include "Table.hpp"
#include "Party.hpp"
#include "Waiter.hpp"
#include "Chef.hpp"
#include "Recipe.hpp"

Restaurant::Restaurant() : customerCounter(1) {
    struct recipe* recipeHead = readRecipes("C:/Users/marij/Desktop/recipes.csv");

    chef = new Chef(&orderQueue, &orderQueueMutex);
    for (int i = 0; i < TABLE_COUNT; ++i) {
        tables[i] = new Table(i + 1, new Waiter(&orderQueue, &orderQueueMutex, recipeHead), chef);
    }
}

void Restaurant::addPartyToQueue(Party *party) {
    std::lock_guard<std::mutex> lock(queueMutex);
    customerQueue.push(party);
}

void Restaurant::assignTables() {
    while (true) {
        for (int i = 0; i < TABLE_COUNT; ++i) {
            if (!tables[i]->isOccupied()) {
                Party *party = nullptr;
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    if (!customerQueue.empty()) {
                        party = customerQueue.front();
                        customerQueue.pop();
                    }
                }
                if (party) {
                    tables[i]->assignParty(party);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Restaurant::threads() {
    std::thread tableThreads[TABLE_COUNT];
    for (int i = 0; i < TABLE_COUNT; ++i) {
        tableThreads[i] = std::thread(&Table::manageTables, tables[i], std::ref(customerQueue), std::ref(queueMutex));
    }

    for (int i = 0; i < TABLE_COUNT; ++i) {
        tableThreads[i].detach();
    }

    assignTables();
}

Party *Restaurant::createParty(int size, const std::vector<std::string> &menu) {
    return new Party(size, customerCounter, menu);
}
