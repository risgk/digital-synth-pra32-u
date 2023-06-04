#pragma once

#include "pra32-u-constants.h"

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

static INLINE int32_t mul_s32_s16_h32(int32_t x, int16_t y) {
  int32_t  x1 = x >> 16;
  uint32_t x0 = x & 0xFFFF;
  return (static_cast<int32_t>(x0 * y) >> 16) + (x1 * y);
}

static INLINE int32_t mul_u32_s16_h32(uint32_t x, int16_t y) {
  uint32_t x1 = x >> 16;
  uint32_t x0 = x & 0xFFFF;
  return (static_cast<int32_t>(x0 * y) >> 16) + (x1 * y);
}

static INLINE int32_t mul_u32_u16_h32(uint32_t x, uint16_t y) {
  uint32_t x1 = x >> 16;
  uint32_t x0 = x & 0xFFFF;
  return (static_cast<uint32_t>(x0 * y) >> 16) + (x1 * y);
}
