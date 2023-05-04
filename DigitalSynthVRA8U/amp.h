#pragma once

#include "common.h"

template <uint8_t T>
class Amp {

public:
  INLINE static void initialize() {
  }

  INLINE static int16_t process(int16_t audio_input, uint8_t gain_control) {
    return mul_sq16_uq8(audio_input, gain_control);
  }
};
