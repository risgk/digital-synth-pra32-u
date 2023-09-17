#pragma once

#include "pra32-u-common.h"

class PRA32_U_ChorusFx {
  static const uint16_t DELAY_BUFF_SIZE = 512;

  int16_t  m_delay_buff[DELAY_BUFF_SIZE];
  uint16_t m_delay_wp;

  uint16_t m_chorus_mix_control;
  uint16_t m_chorus_mix_control_effective;
  uint16_t m_chorus_depth_control;
  uint16_t m_chorus_depth_control_effective;
  uint32_t m_chorus_rate_control;
  uint16_t m_chorus_delay_time_control;
  uint16_t m_chorus_delay_time_control_effective;
  uint8_t  m_chorus_depth_control_actual;
  uint32_t m_chorus_lfo_phase;
  uint16_t m_chorus_delay_time[2];

public:
  PRA32_U_ChorusFx()
  : m_delay_buff()
  , m_delay_wp()

  , m_chorus_mix_control()
  , m_chorus_mix_control_effective()
  , m_chorus_depth_control()
  , m_chorus_depth_control_effective()
  , m_chorus_rate_control()
  , m_chorus_delay_time_control()
  , m_chorus_delay_time_control_effective()
  , m_chorus_lfo_phase()
  , m_chorus_delay_time()
  {
    m_delay_wp = DELAY_BUFF_SIZE - 1;

    set_chorus_depth     (64 );
    set_chorus_rate      (64 );
    set_chorus_delay_time(64 );

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
    m_chorus_rate_control = g_chorus_rate_table[controller_value];
  }

  INLINE void set_chorus_delay_time(uint8_t controller_value) {
    m_chorus_delay_time_control = controller_value << 6;
  }

  INLINE void set_chorus_mix(uint8_t controller_value) {
    m_chorus_mix_control = (controller_value + 1) >> 1;
  }

  template <uint8_t N>
  INLINE uint16_t get_chorus_delay_time() {
    return m_chorus_delay_time[N];
  }

  INLINE void process_at_low_rate(uint8_t count) {
#if 1
    static_cast<void>(count);

    m_chorus_mix_control_effective += (m_chorus_mix_control_effective < m_chorus_mix_control);
    m_chorus_mix_control_effective -= (m_chorus_mix_control_effective > m_chorus_mix_control);

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

    m_chorus_lfo_phase += m_chorus_rate_control;
    m_chorus_lfo_phase &= 0x00FFFFFF;

    int16_t chorus_lfo_wave_level = get_chorus_lfo_wave_level(m_chorus_lfo_phase);

    int16_t chorus_lfo_level = (chorus_lfo_wave_level * chorus_depth_control_effective_limited) >> 14;

    m_chorus_delay_time[0] = m_chorus_delay_time_control_effective - chorus_lfo_level;
    m_chorus_delay_time[1] = m_chorus_delay_time_control_effective + chorus_lfo_level;
#endif
  }

  INLINE int16_t process(int16_t dir_sample, int16_t& right_level) {
    int16_t eff_sample_0 = delay_buff_get(get_chorus_delay_time<0>());
    int16_t eff_sample_1 = delay_buff_get(get_chorus_delay_time<1>());
    delay_buff_push(dir_sample);

    right_level = (((dir_sample * (128 - m_chorus_mix_control_effective)) + (eff_sample_1 * m_chorus_mix_control_effective))) >> 7;
    return        (((dir_sample * (128 - m_chorus_mix_control_effective)) + (eff_sample_0 * m_chorus_mix_control_effective))) >> 7;
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
