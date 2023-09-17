#pragma once

#include "pra32-u-common.h"

class PRA32_U_Amp {
  int16_t m_gain_control;
  int16_t m_gain_control_effective;
  int16_t m_gain_mod_input;

public:
PRA32_U_Amp()
  : m_gain_control(127)
  , m_gain_control_effective(0)
  , m_gain_mod_input(0)
  {
  }

  INLINE void set_gain(uint8_t controller_value) {
    m_gain_control = controller_value;
  }

  INLINE void process_at_low_rate(int16_t gain_mod_input) {
    update_gain_effective();
    m_gain_mod_input = gain_mod_input;
  }

  INLINE int16_t process(int16_t audio_input) {
    int16_t audio_output = (audio_input * m_gain_mod_input) >> 14;
    int16_t gain_linear = ((m_gain_control_effective * m_gain_control_effective) * 16384) / 16129;
    audio_output = (audio_output * gain_linear) >> 14;
    return audio_output;
  }

private:
  INLINE void update_gain_effective() {
    m_gain_control_effective += (m_gain_control_effective < m_gain_control);
    m_gain_control_effective -= (m_gain_control_effective > m_gain_control);
  }
};
