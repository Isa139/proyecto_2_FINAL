#include "Menu.h"
#include "Recipe.hpp"
#include "Utilities.hpp"
#include <string>

std::vector<std::string> createMenu(const char* fileNameR) {
    // Leer las recetas del archivo
    recipe* recipes = readRecipes(fileNameR);
    if (!recipes) {
        return {};
    }

    // Obtener los nombres de las recetas
    int recipeCount = 0;
    recipe** recipePtrs = getRecipeNames(recipes, recipeCount);
    if (!recipePtrs) {
        // Manejo de error de memoria
        freeRecipes(recipes);
        return {};
    }

    // Crear el menú usando std::vector
    std::vector<std::string> menu;
    for (int i = 0; i < recipeCount; ++i) {
        menu.push_back(std::string(recipePtrs[i]->name));
    }

    // Liberar memoria
    freeRecipes(recipes);
    free(recipePtrs);

    return menu;
}
