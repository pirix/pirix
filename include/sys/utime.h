#pragma once

struct utimbuf {
  time_t actime;
  time_t modtime; 
};

int utime(const char* path, const struct utimbuf* times);
