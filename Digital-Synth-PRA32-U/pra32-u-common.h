#pragma once

#include "pra32-u-constants.h"
//#include <stdio.h>

#define INLINE inline __attribute__((always_inline))

#if !defined(ARDUINO_ARCH_AVR)
typedef int32_t __int24;
typedef uint32_t __uint24;
#endif

static INLINE uint8_t low_byte(uint16_t x) {
  return x & 0xFF;
}

static INLINE uint8_t high_byte(uint16_t x) {
  return (x >> 8) & 0xFF;
}

static INLINE int8_t high_sbyte(int16_t x) {
  return (x >> 8) & 0xFF;
}

static INLINE uint8_t hhigh_byte(__uint24 x) {
  return x >> 16;
}

static INLINE int8_t hhigh_sbyte(__int24 x) {
  return x >> 16;
}

static INLINE uint8_t ram_read_byte(const void* addr) {
  const uint8_t* p = static_cast<const uint8_t*>(addr);
  return p[0];
}

static INLINE uint16_t ram_read_word(const void* addr) {
  // for little endian cpu
  const uint8_t* p = static_cast<const uint8_t*>(addr);
  return p[0] | (p[1] << 8);
}
