#include <string.h>

void* memcpy(void* dest, const void* src, size_t num) {
  char* d = (char*)dest;
  const char* s = (char*)src;
  while(num--) *(d++) = *(s++);
  return dest;
}

void* memset(void* dest, int value, size_t num) {
  char* d = dest;
  while (num--) *(d++) = value;
  return dest;
}

int memcmp(const void* src1, const void* src2, size_t num) {
  char* a = (char*)src1;
  char* b = (char*)src2;
  for (size_t i = 0; i < num; i++) {
    if (a[i] != b[i]) return i;
  }
  return 0;
}

char* strcpy(char* dest, const char* src) {
  char* rdest = dest;
  while (*src) *(dest++) = *(src++);
  return rdest;
}

char* strncpy(char* dest, const char* src, size_t num) {
  char* rdest = dest;
  while (num-- && *src) *(dest++) = *(src++);
  return rdest;
}

char* strcat(char* dest, const char* src) {
  char* rdest = dest;
  while (*dest) dest++;
  while ((*dest++ = *src++));
  return rdest;
}

char* strncat(char* dest, const char* src, size_t num) {
  char* rdest = dest;
  while(*dest) dest++;
  while (num-- && (*(dest++) = *(src++)));
  *dest = '\0';
  return rdest;
}

int strcmp(const char* str1, const char* str2) {
  for (int i = 0; str1 && str2; i++) {
    if (str1 != str2) return i;
  }
  return 0;
}

size_t strlen(const char* str) {
  int len = 0;
  while (*str++) len++;
  return len;
}
