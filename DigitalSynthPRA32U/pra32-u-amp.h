#pragma once

#include "pra32-u-common.h"

template <uint8_t T>
class Amp {
  static uint8_t m_gain[2];

public:
  INLINE static void initialize() {
    set_gain<0>(127);
    set_gain<1>(127);
  }

  template <uint8_t N>
  INLINE static void set_gain(uint8_t controller_value) {
    m_gain[N] = (controller_value + 1) >> 1;
  }

  INLINE static int16_t process(int16_t audio_input, int16_t gain_mod_input) {
    int16_t audio_output = (audio_input * gain_mod_input) >> 14;
    audio_output = (audio_output * (m_gain[0] * m_gain[1])) >> 12;
    return audio_output;
  }
};

template <uint8_t T> uint8_t Amp<T>::m_gain[2];
