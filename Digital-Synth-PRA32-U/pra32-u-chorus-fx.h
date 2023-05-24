#pragma once

#include "pra32-u-common.h"

class PRA32_U_ChorusFx {
  static const uint16_t DELAY_BUFF_SIZE = 512;

  static const uint8_t CHORUS_MODE_BYPASS     = 0;
  static const uint8_t CHORUS_MODE_OFF        = 1;
  static const uint8_t CHORUS_MODE_STEREO     = 2;
  static const uint8_t CHORUS_MODE_P_STEREO   = 3;
  static const uint8_t CHORUS_MODE_MONO       = 4;
  static const uint8_t CHORUS_MODE_STEREO_2   = 5;

  int16_t  m_delay_buff[DELAY_BUFF_SIZE];
  uint16_t m_delay_wp;

  uint8_t  m_gain;

  uint8_t  m_chorus_depth_control;
  uint8_t  m_chorus_rate_control;
  uint8_t  m_chorus_delay_time_control;
  uint8_t  m_chorus_delay_time_control_effective;
  uint8_t  m_param_chorus_mode;
  boolean  m_param_chorus_bypass;
  uint8_t  m_effective_chorus_mode;
  uint8_t  m_chorus_depth_control_actual;
  uint16_t m_chorus_lfo_phase;
  int16_t  m_chorus_lfo_wave_level;
  int16_t  m_chorus_lfo_level;
  uint16_t m_chorus_delay_time[2];

public:
  PRA32_U_ChorusFx()
  : m_delay_buff()
  , m_delay_wp()

  , m_gain()

  , m_chorus_depth_control()
  , m_chorus_rate_control()
  , m_chorus_delay_time_control()
  , m_chorus_delay_time_control_effective()
  , m_param_chorus_mode()
  , m_param_chorus_bypass()
  , m_effective_chorus_mode()
  , m_chorus_depth_control_actual()
  , m_chorus_lfo_phase()
  , m_chorus_lfo_wave_level()
  , m_chorus_lfo_level()
  , m_chorus_delay_time()
  {}

  INLINE void initialize() {
    m_delay_wp = DELAY_BUFF_SIZE - 1;

    set_chorus_depth     (32 );
    set_chorus_rate      (32 );
    set_chorus_delay_time(80 );

    m_param_chorus_mode     = CHORUS_MODE_OFF;
    m_param_chorus_bypass   = false;
    m_effective_chorus_mode = CHORUS_MODE_OFF;
    set_gain(90);

    m_chorus_depth_control_actual = 64;
  }

  INLINE void set_chorus_depth(uint8_t controller_value) {
    if (controller_value < 126) {
      m_chorus_depth_control = controller_value;
    } else {
      m_chorus_depth_control = 126;
    }
  }

  INLINE void set_chorus_rate(uint8_t controller_value) {
    if (controller_value < 2) {
      m_chorus_rate_control = 1;
    } else {
      m_chorus_rate_control = (controller_value + 2) >> 2;
    }
  }

  INLINE void set_chorus_delay_time(uint8_t controller_value) {
    m_chorus_delay_time_control = controller_value;
  }

  INLINE void set_chorus_mode(uint8_t controller_value) {
    uint8_t new_param_chorus_mode = CHORUS_MODE_STEREO_2;
    if        (controller_value < 16) {
      new_param_chorus_mode = CHORUS_MODE_OFF;
    } else if (controller_value < 48) {
      new_param_chorus_mode = CHORUS_MODE_MONO;
    } else if (controller_value < 80) {
      new_param_chorus_mode = CHORUS_MODE_P_STEREO;
    } else if (controller_value < 112) {
      new_param_chorus_mode = CHORUS_MODE_STEREO;
    }
    update_effective_chorus_mode(new_param_chorus_mode, m_param_chorus_bypass);
  }

  INLINE void set_chorus_bypass(uint8_t controller_value) {
    update_effective_chorus_mode(m_param_chorus_mode, controller_value >= 64);
  }

  INLINE void update_effective_chorus_mode(uint8_t new_param_chorus_mode, boolean new_param_chorus_bypass) {
    if ((m_param_chorus_mode   != new_param_chorus_mode) ||
        (m_param_chorus_bypass != new_param_chorus_bypass)) {
      m_param_chorus_mode   = new_param_chorus_mode;
      m_param_chorus_bypass = new_param_chorus_bypass;
      if (m_param_chorus_bypass) {
        m_effective_chorus_mode = CHORUS_MODE_BYPASS;
      } else {
        m_effective_chorus_mode = m_param_chorus_mode;
      }

      delay_buff_attenuate();

      switch (m_effective_chorus_mode) {
      case CHORUS_MODE_BYPASS   :
        set_gain(127);
        break;
      case CHORUS_MODE_OFF      :
      case CHORUS_MODE_STEREO   :
        set_gain(90);
        break;
      case CHORUS_MODE_P_STEREO :
      case CHORUS_MODE_MONO     :
      case CHORUS_MODE_STEREO_2 :
        set_gain(64);
        break;
      }
    }
  }

  template <uint8_t N>
  INLINE int16_t get_chorus_delay_time() {
    return m_chorus_delay_time[N];
  }

  INLINE void process_at_low_rate(uint8_t count) {
#if 1
    if ((count & (OSC_CONTROL_INTERVAL - 1)) == 0) {
      //printf("%d Osc\n", count);
      switch ((count >> OSC_CONTROL_INTERVAL_BITS) & 0x1F) {
      case 0x1F: update_chorus_lfo_0th();
                 update_chorus_lfo_1st();
                 update_chorus_lfo_2nd();
                 update_chorus_lfo_3rd();             break;
      }
    }
#endif
  }

  INLINE int16_t process(int16_t dir_sample, int16_t& right_level) {
    int16_t eff_sample_0 = delay_buff_get(get_chorus_delay_time<0>());
    int16_t eff_sample_1 = delay_buff_get(get_chorus_delay_time<1>());
    delay_buff_push(dir_sample);

    if (m_effective_chorus_mode >= CHORUS_MODE_MONO) {
      // For Mono Chorus and Stereo 2-phase Chorus
      right_level = ((dir_sample + eff_sample_1) * m_gain) >> 6;
      return        ((dir_sample + eff_sample_0) * m_gain) >> 6;
    }

    if (m_effective_chorus_mode == CHORUS_MODE_P_STEREO) {
      // For Pseudo-Stereo Chorus
      right_level = ((dir_sample - eff_sample_0) * m_gain) >> 6;
      return        ((dir_sample + eff_sample_0) * m_gain) >> 6;
    }

    // For Off and Stereo Chorus
    right_level = (dir_sample   * m_gain) >> 6;
    return        (eff_sample_0 * m_gain) >> 6;
  }

private:
  INLINE void delay_buff_push(int16_t audio_input) {
    m_delay_wp = (m_delay_wp + 1) & (DELAY_BUFF_SIZE - 1);
    m_delay_buff[m_delay_wp] = audio_input;
  }

  INLINE int16_t delay_buff_get(uint16_t sample_delay) {
    uint16_t rp = (m_delay_wp - sample_delay) & (DELAY_BUFF_SIZE - 1);
    return m_delay_buff[rp];
  }

  INLINE void delay_buff_attenuate() {
    for (uint16_t i = 0; i < DELAY_BUFF_SIZE; ++i) {
      m_delay_buff[i] = m_delay_buff[i] >> 1;
    }
  }

  INLINE void set_gain(uint8_t controller_value) {
    m_gain = (controller_value + 1) >> 1;
  }

  INLINE void update_chorus_lfo_0th() {
    m_chorus_delay_time_control_effective += (m_chorus_delay_time_control_effective < m_chorus_delay_time_control);
    m_chorus_delay_time_control_effective -= (m_chorus_delay_time_control_effective > m_chorus_delay_time_control);

    if (m_chorus_delay_time_control_effective < 64) {
      if (m_chorus_depth_control > (m_chorus_delay_time_control_effective << 1)) {
        m_chorus_depth_control_actual = (m_chorus_delay_time_control_effective << 1);
      } else {
        m_chorus_depth_control_actual = m_chorus_depth_control;
      }
    } else {
      if (m_chorus_depth_control > ((127 - m_chorus_delay_time_control_effective) << 1)) {
        m_chorus_depth_control_actual = ((127 - m_chorus_delay_time_control_effective) << 1);
      } else {
        m_chorus_depth_control_actual = m_chorus_depth_control;
      }
    }

    m_chorus_lfo_phase += m_chorus_rate_control;
  }

  INLINE void update_chorus_lfo_1st() {
    m_chorus_lfo_wave_level = get_chorus_lfo_wave_level(m_chorus_lfo_phase);
  }

  INLINE void update_chorus_lfo_2nd() {
    m_chorus_lfo_level = (m_chorus_lfo_wave_level * m_chorus_depth_control_actual) >> 8;
  }

  INLINE void update_chorus_lfo_3rd() {
    switch (m_effective_chorus_mode) {
    case CHORUS_MODE_BYPASS   :
    case CHORUS_MODE_OFF      :
      {
        m_chorus_delay_time[0] = 0;
        m_chorus_delay_time[1] = 0;
      }
      break;
    case CHORUS_MODE_STEREO   :
    case CHORUS_MODE_P_STEREO :
    case CHORUS_MODE_MONO     :
      {
        uint16_t chorus_delay_time_control_mul_4 = m_chorus_delay_time_control_effective * 4;
        m_chorus_delay_time[0] = chorus_delay_time_control_mul_4 + m_chorus_lfo_level;
        m_chorus_delay_time[1] = m_chorus_delay_time[0];
      }
      break;
    case CHORUS_MODE_STEREO_2 :
      {
        uint16_t chorus_delay_time_control_mul_4 = m_chorus_delay_time_control_effective * 4;
        m_chorus_delay_time[0] = chorus_delay_time_control_mul_4 - m_chorus_lfo_level;
        m_chorus_delay_time[1] = chorus_delay_time_control_mul_4 + m_chorus_lfo_level;
      }
      break;
    }
  }

  INLINE int16_t get_chorus_lfo_wave_level(uint16_t phase) {
    int16_t triangle_wave_level = 0;
    phase &= 0x1FFC;
    phase = (phase >> 2);

    if (phase < 0x0400) {
      triangle_wave_level = phase - 512;
    } else {
      triangle_wave_level = 1535 - phase;
    }

    return triangle_wave_level;
  }
};
