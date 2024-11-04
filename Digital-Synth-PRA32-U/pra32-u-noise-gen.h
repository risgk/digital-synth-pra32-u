// refs https://en.wikipedia.org/wiki/Xorshift

#pragma once

#include "pra32-u-common.h"

class PRA32_U_NoiseGen {
  uint32_t m_state_a;
  uint32_t m_state_a_prev;

public:
  PRA32_U_NoiseGen()
  : m_state_a(1)
  , m_state_a_prev(0)
  {
  }

  INLINE int16_t process() {
    m_state_a_prev = m_state_a;

    uint32_t x = m_state_a;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    m_state_a = x;

    int16_t noise_int15 = (m_state_a >> 17) - 16384;
    return noise_int15;
  }

  INLINE void get_rand_uint8_array(uint8_t array[8]) {
    array[0] = (m_state_a_prev >>  0) & 0xFFu;
    array[1] = (m_state_a_prev >>  8) & 0xFFu;
    array[2] = (m_state_a_prev >> 16) & 0xFFu;
    array[3] = (m_state_a_prev >> 24) & 0xFFu;
    array[4] = (m_state_a      >>  0) & 0xFFu;
    array[5] = (m_state_a      >>  8) & 0xFFu;
    array[6] = (m_state_a      >> 16) & 0xFFu;
    array[7] = (m_state_a      >> 24) & 0xFFu;
  }
};
