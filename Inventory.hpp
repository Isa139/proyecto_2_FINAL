#ifndef INVENTORY_OPERATIONS_HPP
#define INVENTORY_OPERATIONS_HPP

#include "Data.hpp"
#include "Recipe.hpp"

struct inventory* readInventory(const char* fileName);
void printInventory(struct inventory* head);
void freeInventory(struct inventory* head);
void alterInventory(struct inventory* inventoryHead, const char* ingredientName, int quantityChange);
bool checkStock(const char* recipeName, struct inventory* inventoryHead, struct recipe* recipeHead);

#endif // INVENTORY_OPERATIONS_HPP

