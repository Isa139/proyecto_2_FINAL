#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cctype>

char* trimWhitespace(char* str);
void toLowercase(char* str);
int delay(int time);
int binaryRandom();
int randomRange(int min, int max);
void initializeRandomSeed();

#endif // UTILITIES_HPP
