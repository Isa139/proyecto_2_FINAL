#include "Utilities.hpp"
#include <string.h>

char* trimWhitespace(char* str) {
    char* end;

    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = 0;

    return str;
}

void toLowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int delay(int time) {
    time_t start = clock();
    while (clock() - start < time * CLOCKS_PER_SEC / 1000) {
        // Busy wait
    }
    return 0;
}

int binaryRandom() {
    return rand() % 2;
}

int randomRange(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void initializeRandomSeed() {
    srand((unsigned int)time(NULL));
}
