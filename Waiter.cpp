#include "Waiter.hpp"
#include "Recipe.hpp"
#include "Inventory.hpp"

struct inventory* originalInventory = nullptr;
struct inventory* inventoryCopy = nullptr;

Waiter::Waiter(std::queue<Order>* orderQueue, std::mutex* orderQueueMutex, struct recipe* recipeHead)
    : orderQueue(orderQueue), orderQueueMutex(orderQueueMutex), recipeHead(recipeHead) {
    if (!recipeHead) {
        std::cerr << "Error: Unable to read recipes" << std::endl;
        // Aquí puedes manejar el error de alguna manera apropiada
    }

    // Leer el inventario con la extensión del archivo y almacenarlo como inventario original
    originalInventory = readInventory("C:/Users/marij/Desktop/inventory.csv");
    if (!originalInventory) {
        std::cerr << "Error: Unable to read inventory" << std::endl;
        // Manejar el error apropiadamente
    }

    inventoryCopy = originalInventory;
}

void Waiter::takeOrder(Customer* customer) {
    std::lock_guard<std::mutex> lock(*orderQueueMutex);

    std::string selectedMeal = customer->getMeal();
    struct recipe* selectedRecipe = findRecipeByName(recipeHead, selectedMeal.c_str());
    if (!selectedRecipe) {
        std::cout << "Error: Recipe not found for " << selectedMeal << std::endl;
        return;
    }


    // Verificar el stock en la copia del inventario
    std::cout << "Waiter checking stock for " << selectedMeal << std::endl;
    if (checkStock(selectedMeal.c_str(), inventoryCopy, selectedRecipe)) {
        // Si hay suficientes ingredientes, poner la orden en la cola
        Order order(customer->name, selectedMeal);
        orderQueue->push(order);
        std::cout << "Waiter took order from " << customer->name << " for " << selectedMeal << std::endl;

        // Reducir los ingredientes en la copia del inventario
        struct ingredient* ingredientPtr = selectedRecipe->ingredients;
        while (ingredientPtr != NULL) {
            alterInventory(inventoryCopy, ingredientPtr->name, -ingredientPtr->quantity);
            ingredientPtr = ingredientPtr->next;
        }

        // Imprimir la copia del inventario después de cada orden
        std::cout << "Inventory after order:" << std::endl;
        printInventory(inventoryCopy);

    } else {
        // Si no hay suficientes ingredientes, informar al cliente
        std::cout << "Sorry, " << customer->name << ", we are out of stock for " << selectedMeal << std::endl;
    }
}
