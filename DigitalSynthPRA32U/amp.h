#pragma once

#include "common.h"

template <uint8_t T>
class Amp {

public:
  INLINE static void initialize() {
  }

  INLINE static int16_t process(int16_t audio_input, int16_t gain_control) {
    return (audio_input * gain_control) >> 15;
  }
};
