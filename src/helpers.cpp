#include "../h/helpers.hpp"
#include "../h/syscall_c.h"

void print(const char* str) {
  while(*str) {
    putc(*str++);
  }
}

void print(uint64 num, uint8 base) {
  if(num == 0) {
    putc('0');
    return;
  }
  if(num < 0) {
    putc('-');
    num = num * -1;
  }
  char buffer[66];
  uint8 i = 0;

  static const char* digits = "0123456789ABCDF";

  while(num > 0 && i < 64) {
    buffer[i++] = digits[num % base];
    num /= base;
  }

  if(base == 16) {
    buffer[i++] = 'x';
    buffer[i++] = '0';
  } else if (base == 2) {
    buffer[i++] = 'b';
    buffer[i++] = '0';
  }
  
  while(i--) {
    putc(buffer[i]);
  }
}

void print(const char* str, uint64 num, char delim, uint8 base) {
  print(str);
  print(" [");
  print(num, base);
  putc(']');
  putc(delim);
}
