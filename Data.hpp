#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_CHARS 100

struct ingredient {
    char name[MAX_CHARS];
    int quantity;
    struct ingredient* next;
};

struct recipe {
    char name[MAX_CHARS];
    int price;
    int preparationTime;
    int consumptionTime;
    struct ingredient* ingredients;
    struct recipe* next;
};

struct inventory {
    char ingredient[MAX_CHARS];
    int amount;
    float price;
    struct inventory* next;
};

#endif // DATA_STRUCTURES_HPP
