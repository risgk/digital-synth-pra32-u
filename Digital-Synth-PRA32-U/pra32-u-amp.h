#pragma once

#include "pra32-u-common.h"

class PRA32_U_Amp {
  uint8_t m_gain[2];
  int16_t m_gain_mod_input;

public:
PRA32_U_Amp()
  : m_gain()
  , m_gain_mod_input()
  {
    set_gain<0>(127);
    set_gain<1>(127);
  }

  template <uint8_t N>
  INLINE void set_gain(uint8_t controller_value) {
    m_gain[N] = (controller_value + 1) >> 1;
  }

  INLINE void process_at_low_rate(int16_t gain_mod_input) {
    m_gain_mod_input = gain_mod_input;
  }

  INLINE int16_t process(int16_t audio_input) {
    int16_t audio_output = (audio_input * m_gain_mod_input) >> 14;
    audio_output = (audio_output * (m_gain[0] * m_gain[1])) >> 12;
    return audio_output;
  }
};
