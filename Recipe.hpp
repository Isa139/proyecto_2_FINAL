#ifndef RECIPE_HPP
#define RECIPE_HPP

#include "Data.hpp"
#include <vector>

struct recipe* readRecipes(const char* fileName);
void printRecipes(struct recipe* head);

void freeRecipes(struct recipe* head);
void freeIngredients(ingredient* head);
struct recipe* findRecipeByName(struct recipe* head, const char* recipeName);
struct recipe** getRecipeNames(struct recipe* head, int& count);

#endif // RECIPE_HPP
