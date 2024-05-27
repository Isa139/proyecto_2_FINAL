#include "Recipe.hpp"
#include "Utilities.hpp"
#include <stdio.h>
#include <vector>
#include <string>

struct recipe* readRecipes(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", fileName);
        return NULL;
    }

    struct recipe* head = NULL;
    struct recipe* tail = NULL;

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        struct recipe* newRecipe = (struct recipe*)malloc(sizeof(struct recipe));
        if (!newRecipe) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(file);
            freeRecipes(head);
            return NULL;
        }
        newRecipe->ingredients = NULL;
        newRecipe->next = NULL;

        char* token = strtok(line, ",");
        if (!token) {
            free(newRecipe);
            continue;
        }
        char* trimmedToken = trimWhitespace(token);
        strcpy(newRecipe->name, trimmedToken);
        toLowercase(newRecipe->name);

        token = strtok(NULL, ",");
        if (token)
            newRecipe->price = atoi(trimWhitespace(token));

        token = strtok(NULL, ",");
        if (token)
            newRecipe->preparationTime = atoi(trimWhitespace(token));

        token = strtok(NULL, ",");
        if (token)
            newRecipe->consumptionTime = atoi(trimWhitespace(token));

        struct ingredient* lastIngredient = NULL;
        while ((token = strtok(NULL, ",")) && *token != '\n') {
            struct ingredient* newIngredient = (struct ingredient*)malloc(sizeof(struct ingredient));
            if (!newIngredient) {
                fprintf(stderr, "Error: Memory allocation failed\n");
                freeIngredients(newRecipe->ingredients);
                free(newRecipe);
                fclose(file);
                freeRecipes(head);
                return NULL;
            }
            newIngredient->next = NULL;

            char* ingredientName = trimWhitespace(token);
            toLowercase(ingredientName);
            strcpy(newIngredient->name, ingredientName);

            token = strtok(NULL, ",");
            if (token)
                newIngredient->quantity = atoi(trimWhitespace(token));

            if (lastIngredient) {
                lastIngredient->next = newIngredient;
            } else {
                newRecipe->ingredients = newIngredient;
            }
            lastIngredient = newIngredient;
        }

        if (head == NULL) {
            head = newRecipe;
        } else {
            tail->next = newRecipe;
        }
        tail = newRecipe;
    }

    fclose(file);
    return head;
}

void printRecipes(struct recipe* head) {
    struct recipe* current = head;
    while (current != NULL) {
        printf("Recipe: %s\n", current->name);
        printf("Price: $%d\n", current->price);
        printf("Preparation Time: %d minutes\n", current->preparationTime);
        printf("Consumption Time: %d minutes\n", current->consumptionTime);

        printf("Ingredients:\n");
        struct ingredient* ingredientPtr = current->ingredients;
        while (ingredientPtr != NULL) {
            printf("- %s: %d\n", ingredientPtr->name, ingredientPtr->quantity);
            ingredientPtr = ingredientPtr->next;
        }
        printf("\n");

        current = current->next;
    }
}

void freeRecipes(struct recipe* head) {
    struct recipe* current = head;
    while (current != NULL) {
        struct recipe* temp = current;
        freeIngredients(temp->ingredients);
        current = current->next;
        free(temp);
    }
}

void freeIngredients(struct ingredient *head)
{
    struct ingredient *current = head;
    while (current != NULL)
    {
        struct ingredient *temp = current;
        current = current->next;
        free(temp);
    }
}

struct recipe* findRecipeByName(struct recipe* head, const char* recipeName) {
    struct recipe* current = head;
    char lowercaseName[MAX_CHARS];
    strcpy(lowercaseName, recipeName);
    toLowercase(lowercaseName);

    while (current != NULL) {
        if (strcmp(current->name, lowercaseName) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct recipe** getRecipeNames(struct recipe* head, int& count) {
    count = 0;
    struct recipe* current = head;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    struct recipe** recipeNames = (struct recipe**)malloc(count * sizeof(struct recipe*));
    if (!recipeNames) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    current = head;
    for (int i = 0; i < count; i++) {
        recipeNames[i] = current;
        current = current->next;
    }
    return recipeNames;
}
