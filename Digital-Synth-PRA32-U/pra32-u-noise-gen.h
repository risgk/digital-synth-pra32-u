// refs https://en.wikipedia.org/wiki/Xorshift

#pragma once

#include "pra32-u-common.h"

class PRA32_U_NoiseGen {
  uint32_t m_state_a;

public:
  PRA32_U_NoiseGen()
  : m_state_a(1)
  {
  }

  INLINE int16_t process() {
    uint32_t x = m_state_a;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    m_state_a = x;

    int16_t noise_int15 = (m_state_a >> 17) - 16384;
    return noise_int15;
  }
};
