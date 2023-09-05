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

  uint16_t m_chorus_depth_control;
  uint16_t m_chorus_depth_control_effective;
  uint32_t m_chorus_rate;
  uint16_t m_chorus_delay_time_control;
  uint16_t m_chorus_delay_time_control_effective;
  uint8_t  m_chorus_mode;
  boolean  m_chorus_bypass;
  uint8_t  m_effective_chorus_mode;
  uint8_t  m_chorus_depth_control_actual;
  uint32_t m_chorus_lfo_phase;
  uint16_t m_chorus_delay_time[2];

public:
  PRA32_U_ChorusFx()
  : m_delay_buff()
  , m_delay_wp()

  , m_gain()

  , m_chorus_depth_control()
  , m_chorus_depth_control_effective()
  , m_chorus_rate()
  , m_chorus_delay_time_control()
  , m_chorus_delay_time_control_effective()
  , m_chorus_mode()
  , m_chorus_bypass()
  , m_effective_chorus_mode()
  , m_chorus_lfo_phase()
  , m_chorus_delay_time()
  {
    m_delay_wp = DELAY_BUFF_SIZE - 1;

    set_chorus_depth     (32 );
    set_chorus_rate      (32 );
    set_chorus_delay_time(80 );

    m_chorus_mode           = CHORUS_MODE_OFF;
    m_chorus_bypass         = false;
    m_effective_chorus_mode = CHORUS_MODE_OFF;
    set_gain(90);

    m_chorus_depth_control_effective = 64 << 6;
    m_chorus_delay_time_control_effective = 64 << 6;
  }

  INLINE void set_chorus_depth(uint8_t controller_value) {
    if (controller_value < 126) {
      m_chorus_depth_control = controller_value << 6;
    } else {
      m_chorus_depth_control = 126 << 6;
    }
  }

  INLINE void set_chorus_rate(uint8_t controller_value) {
    m_chorus_rate = g_chorus_rate_table[controller_value];
  }

  INLINE void set_chorus_delay_time(uint8_t controller_value) {
    m_chorus_delay_time_control = controller_value << 6;
  }

  INLINE void set_chorus_mode(uint8_t controller_value) {
    static uint8_t chorus_mode_table[6] = {
      CHORUS_MODE_OFF,
      CHORUS_MODE_MONO,
      CHORUS_MODE_P_STEREO,
      CHORUS_MODE_P_STEREO,
      CHORUS_MODE_STEREO,
      CHORUS_MODE_STEREO_2,
    };

    volatile int32_t index = ((controller_value * 10) + 127) / 254;

    // index = max(index, 5)
    index = index - 5;
    index = (index < 0) * index + 5;

    uint8_t new_chorus_mode = chorus_mode_table[index];

    update_effective_chorus_mode(new_chorus_mode, m_chorus_bypass);
  }

  INLINE void set_chorus_bypass(uint8_t controller_value) {
    update_effective_chorus_mode(m_chorus_mode, controller_value >= 64);
  }

  INLINE void update_effective_chorus_mode(uint8_t new_chorus_mode, boolean new_chorus_bypass) {
    if ((m_chorus_mode   != new_chorus_mode) ||
        (m_chorus_bypass != new_chorus_bypass)) {
      m_chorus_mode   = new_chorus_mode;
      m_chorus_bypass = new_chorus_bypass;
      if (m_chorus_bypass) {
        m_effective_chorus_mode = CHORUS_MODE_BYPASS;
      } else {
        m_effective_chorus_mode = m_chorus_mode;
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
  INLINE uint16_t get_chorus_delay_time() {
    return m_chorus_delay_time[N];
  }

  INLINE void process_at_low_rate(uint8_t count) {
#if 1
    static_cast<void>(count);

    m_chorus_depth_control_effective += (m_chorus_depth_control_effective < m_chorus_depth_control);
    m_chorus_depth_control_effective -= (m_chorus_depth_control_effective > m_chorus_depth_control);

    m_chorus_delay_time_control_effective += (m_chorus_delay_time_control_effective < m_chorus_delay_time_control);
    m_chorus_delay_time_control_effective -= (m_chorus_delay_time_control_effective > m_chorus_delay_time_control);

    uint16_t chorus_depth_control_effective_limited;
    if (m_chorus_delay_time_control_effective < (64 << 6)) {
      if (m_chorus_depth_control_effective > (m_chorus_delay_time_control_effective << 1)) {
        chorus_depth_control_effective_limited = (m_chorus_delay_time_control_effective << 1);
      } else {
        chorus_depth_control_effective_limited = m_chorus_depth_control_effective;
      }
    } else {
      if (m_chorus_depth_control_effective > (((127 << 6) - m_chorus_delay_time_control_effective) << 1)) {
        chorus_depth_control_effective_limited = (((127 << 6) - m_chorus_delay_time_control_effective) << 1);
      } else {
        chorus_depth_control_effective_limited = m_chorus_depth_control_effective;
      }
    }

    m_chorus_lfo_phase += m_chorus_rate;
    m_chorus_lfo_phase &= 0x00FFFFFF;

    int16_t chorus_lfo_wave_level = get_chorus_lfo_wave_level(m_chorus_lfo_phase);

    int16_t chorus_lfo_level = (chorus_lfo_wave_level * chorus_depth_control_effective_limited) >> 14;

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
        m_chorus_delay_time[0] = m_chorus_delay_time_control_effective + chorus_lfo_level;
        m_chorus_delay_time[1] = m_chorus_delay_time[0];
      }
      break;
    case CHORUS_MODE_STEREO_2 :
      {
        m_chorus_delay_time[0] = m_chorus_delay_time_control_effective - chorus_lfo_level;
        m_chorus_delay_time[1] = m_chorus_delay_time_control_effective + chorus_lfo_level;
      }
      break;
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
    uint16_t curr_index  = (m_delay_wp - (sample_delay >> 4)) & (DELAY_BUFF_SIZE - 1);
    uint16_t next_index  = (curr_index - 1) & (DELAY_BUFF_SIZE - 1);
    uint16_t next_weight = (sample_delay & 0xF);
    int16_t  curr_data   = m_delay_buff[curr_index];
    int16_t  next_data   = m_delay_buff[next_index];

    // lerp
    int16_t result = curr_data + (((next_data - curr_data) * next_weight) >> 4);

    return result;
  }

  INLINE void delay_buff_attenuate() {
    for (uint16_t i = 0; i < DELAY_BUFF_SIZE; ++i) {
      m_delay_buff[i] = m_delay_buff[i] >> 1;
    }
  }

  INLINE void set_gain(uint8_t controller_value) {
    m_gain = (controller_value + 1) >> 1;
  }

  INLINE int16_t get_chorus_lfo_wave_level(uint32_t phase) {
    int16_t triangle_wave_level = 0;
    phase = (phase >> 9);

    if (phase < 0x00004000) {
      triangle_wave_level = phase - (512 << 4);
    } else {
      triangle_wave_level = (1535 << 4) - phase;
    }

    return triangle_wave_level;
  }
};
