#ifndef STDLIB_H
#define STDLIB_H

#define NULL 0
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

typedef unsigned int size_t;

char* itoa(int value, char* buffer, int base);
void exit(int status);

#endif
