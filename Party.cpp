#include "Party.hpp"
#include <cstdlib>
#include <sstream>

Party::Party(int size, int &customerCounter, const std::vector<std::string> &menu) : size(size) {
    for (int i = 0; i < size; ++i) {
        int choice = rand() % menu.size();
        std::string selectedMeal = menu[choice];

        customers.push_back(new Customer("Customer" + std::to_string(customerCounter++), selectedMeal));
    }
}

Party::~Party() {
    for (auto customer : customers) {
        delete customer;
    }
}

int Party::getSize() const {
    return size;
}

std::string Party::getDishes() const {
    std::ostringstream dishesStream;
    for (const auto& customer : customers) {
        dishesStream << customer->getMeal() << ", ";
    }
    std::string dishes = dishesStream.str();
    return dishes.substr(0, dishes.size() - 2); // Elimina la última coma y el espacio
}

int Party::getEatingTime() const {
    int totalTime = 0;
    for (const auto& customer : customers) {
        totalTime += customer->calculateEatingTime(10); // Puedes ajustar el tiempo base
    }
    return totalTime;
}
