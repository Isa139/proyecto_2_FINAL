#include "Inventory.hpp"
#include "Utilities.hpp"

struct inventory* readInventory(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", fileName);
        return NULL;
    }

    struct inventory* head = NULL;
    struct inventory* tail = NULL;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        struct inventory* newInventory = (struct inventory*)malloc(sizeof(struct inventory));
        if (!newInventory) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(file);
            return NULL;
        }
        newInventory->next = NULL;

        char* token = strtok(line, ",");
        if (!token) {
            free(newInventory);
            continue;
        }
        char* trimmedName = trimWhitespace(token);
        toLowercase(trimmedName);
        strcpy(newInventory->ingredient, trimmedName);

        token = strtok(NULL, ",");
        if (token)
            newInventory->amount = atoi(trimWhitespace(token));

        token = strtok(NULL, ",");
        if (token)
            newInventory->price = atof(trimWhitespace(token));

        if (head == NULL) {
            head = newInventory;
        } else {
            tail->next = newInventory;
        }
        tail = newInventory;
    }

    fclose(file);
    return head;
}

void printInventory(struct inventory* head) {
    struct inventory* current = head;
    while (current != NULL) {
        printf("Ingredient: %s\n", current->ingredient);
        printf("Price: $%.2f\n", current->price);
        printf("Amount: %d\n", current->amount);
        printf("\n");

        current = current->next;
    }
}

void freeInventory(struct inventory* head) {
    struct inventory* current = head;
    while (current != NULL) {
        struct inventory* temp = current;
        current = current->next;
        free(temp);
    }
}

void alterInventory(struct inventory* inventoryHead, const char* ingredientName, int quantityChange) {
    struct inventory* current = inventoryHead;
    while (current != NULL) {
        if (strcmp(current->ingredient, ingredientName) == 0) {
            current->amount += quantityChange;
            return;
        }
        current = current->next;
    }
}

bool checkStock(const char* recipeName, struct inventory* inventoryHead, struct recipe* recipeHead) {
    struct recipe* recipe = findRecipeByName(recipeHead, recipeName);
    if (!recipe) {
        fprintf(stderr, "Error: Recipe not found\n");
        return false;
    }

    struct ingredient* ingredientPtr = recipe->ingredients;
    while (ingredientPtr != NULL) {
        struct inventory* inventoryPtr = inventoryHead;
        while (inventoryPtr != NULL) {
            if (strcmp(inventoryPtr->ingredient, ingredientPtr->name) == 0) {
                if (inventoryPtr->amount < ingredientPtr->quantity) {
                    return false; // Not enough ingredients
                }
                break;
            }
            inventoryPtr = inventoryPtr->next;
        }
        if (inventoryPtr == NULL) {
            return false; // Ingredient not found
        }
        ingredientPtr = ingredientPtr->next;
    }

    return true;
}
