#pragma once

#include "pra32-u-common.h"

class PRA32_U_Amp {
  int16_t m_gain;
  int16_t m_gain_effective;
  int16_t m_gain_mod_input;

public:
PRA32_U_Amp()
  : m_gain()
  , m_gain_effective()
  , m_gain_mod_input()
  {
    set_gain(127);
  }

  INLINE void set_gain(uint8_t controller_value) {
    m_gain = ((controller_value * controller_value) * 16384) / 16129;
  }

  INLINE void process_at_low_rate(int16_t gain_mod_input) {
    update_gain_effective();
    m_gain_mod_input = gain_mod_input;
  }

  INLINE int16_t process(int16_t audio_input) {
    int16_t audio_output = (audio_input * m_gain_mod_input) >> 14;
    audio_output = (audio_output * m_gain_effective) >> 14;
    return audio_output;
  }

private:
  INLINE void update_gain_effective() {
    // effective_new = clamp(m_gain, (m_gain_effective - 256), (m_gain_effective + 256))
    volatile int32_t effective_new = m_gain - (m_gain_effective + 256);
    effective_new = (effective_new < 0) * effective_new + (m_gain_effective + 256) - (m_gain_effective - 256);
    effective_new = (effective_new > 0) * effective_new + (m_gain_effective - 256);
    m_gain_effective = effective_new;
  }
};
