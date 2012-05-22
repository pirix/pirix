#include <tar.h>
#include <string.h>

extern char _binary_initrd_start;
extern char _binary_initrd_end;
extern char _binary_initrd_size;

static long read_octal(char* ptr, int length) {
  long result = 0;
  int padding = 1;

  for (int i = 0; i < length; i++) {
    if (ptr[i] == 0) break;
    if (ptr[i] == '0' || ptr[i] == ' ') {
      if (padding) continue;
      if (ptr[i] ==  ' ') break;
    }
    padding = 0;
    result = (result << 3) + (ptr[i] - '0');
  }

  return result;
}

void initrd_load() {
  char* ptr = &_binary_initrd_start;

  struct tar_header* header = (struct tar_header*)ptr;

  char filename[10];
  strncpy(filename, header->filename, 100);

  long size = read_octal(header->size, 12);

  char checksum[8];
  strncpy(checksum, header->chksum, 8);

  char magic[6];
  strncpy(magic, header->magic, 6);

  char* file = ptr+512;
}
