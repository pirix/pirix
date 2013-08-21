#pragma once

#include <pirix/types.h>

void* memcpy(void* dest, const void* src, size_t num);
void* memset(void* dest, int value, size_t num);
int memcmp(const void* src1, const void* src2, size_t num);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t num);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t num);
int strcmp(const char* str1, const char* str2);
char* strchr(const char* str, int c);
char* strtok_r(char* str, const char* delimiters, char** save);
size_t strlen(const char* str);
