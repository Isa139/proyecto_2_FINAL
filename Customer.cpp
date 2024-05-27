#include "Customer.hpp"
#include "Constants.hpp"  // Include the constants header
#include "Utilities.hpp"  // Ensure this is included if delay(), binaryRandom(), randomRange() are defined in Utilities
#include <thread>         // Ensure this is included for std::this_thread::sleep_for
#include <chrono>         // Ensure this is included for std::chrono::seconds

Customer::Customer(std::string name, std::string meal) : selectedMeal(meal), name(name) {}

std::string Customer::getMeal() {
    return selectedMeal;
}

void Customer::orderMeal() {
    std::cout << " -" << name << " ordered: " << selectedMeal << std::endl;
}

int Customer::calculateEatingTime(int eatingTime) {
    int randomFactor = binaryRandom();
    int eatDelay = delay(eatingTime);
    int randomRangeValue = randomRange(0, eatDelay);

    int calculatedTime = eatingTime + (eatingTime * randomFactor * randomRangeValue);
    if (calculatedTime < MIN_EATING_TIME) {
        calculatedTime = MIN_EATING_TIME;
    }
    return calculatedTime;
}

void Customer::eatMeal(int durationTime) {
    int eatingTime = calculateEatingTime(durationTime);
    std::cout << " -" << name << " is eating: " << selectedMeal << " Eating duration: " << eatingTime << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(eatingTime));
}
