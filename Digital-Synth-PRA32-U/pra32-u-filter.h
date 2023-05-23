#pragma once

// refs http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

#include "pra32-u-common.h"
#include "pra32-u-filter-table.h"

class PRA32_U_Filter {
  const uint16_t* m_lpf_table;
  uint16_t        m_b_2_over_a_0;
  int16_t         m_a_1_over_a_0;
  int16_t         m_a_2_over_a_0;
  int16_t         m_x_1;
  int16_t         m_x_2;
  int16_t         m_y_1;
  int16_t         m_y_2;
  uint8_t         m_cutoff_current;
  int16_t         m_cutoff_candidate;
  uint8_t         m_cutoff_control;
  uint8_t         m_cutoff_control_effective;
  int8_t          m_cutoff_eg_amt;
  int8_t          m_cutoff_lfo_amt;
  int8_t          m_cutoff_pitch_amt;
  int8_t          m_cutoff_offset;

  const uint8_t AUDIO_FRACTION_BITS = 14;
  const int16_t MAX_ABS_OUTPUT = ((124 << (AUDIO_FRACTION_BITS - 8)) >> 8) << 8;

public:
  PRA32_U_Filter()
  : m_lpf_table()
  , m_b_2_over_a_0()
  , m_a_1_over_a_0()
  , m_a_2_over_a_0()
  , m_x_1()
  , m_x_2()
  , m_y_1()
  , m_y_2()
  , m_cutoff_current()
  , m_cutoff_candidate()
  , m_cutoff_control()
  , m_cutoff_control_effective()
  , m_cutoff_eg_amt()
  , m_cutoff_lfo_amt()
  , m_cutoff_pitch_amt()
  , m_cutoff_offset()
  {}

  INLINE void initialize() {
    m_cutoff_current = 127;

    set_cutoff(127);
    set_resonance(0);
    set_cutoff_eg_amt(64);
    set_cutoff_lfo_amt(64);
    set_cutoff_pitch_amt(0);
    set_cutoff_offset(0);

    update_coefs_0th(0);
    update_coefs_1st(0, 60 << 8);
    update_coefs_2nd();
    update_coefs_3rd();
  }

  INLINE void set_cutoff(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_control = value;
  }

  INLINE void set_resonance(uint8_t controller_value) {
    uint8_t index = (controller_value + 4) >> 4;
    m_lpf_table = g_filter_lpf_tables[index];
  }

  INLINE void set_cutoff_eg_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_eg_amt = (value - 64) << 1;
  }

  INLINE void set_cutoff_lfo_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_lfo_amt = (value - 64) << 1;
  }

  INLINE void set_cutoff_pitch_amt(uint8_t controller_value) {
    if (controller_value < 32) {
      m_cutoff_pitch_amt = 0;
    } else if (controller_value < 96) {
      m_cutoff_pitch_amt = 1;
    } else {
      m_cutoff_pitch_amt = 2;
    }
  }

  INLINE void set_cutoff_offset(int8_t cutoff_offset) {
    m_cutoff_offset = cutoff_offset;
  }

  INLINE void control(uint8_t count, int16_t eg_input, int16_t lfo_input, uint16_t osc_pitch) {
#if 1
    if ((count & (FILTER_CONTROL_INTERVAL - 1)) == 7) {
      //printf("%d PRA32_U_Filter\n", count);
      if (count & 0x10) {
        if (count & 0x08) {
          update_coefs_0th(eg_input);
          update_coefs_1st(lfo_input, osc_pitch);
          update_coefs_2nd();
          update_coefs_3rd();
        }
      }
    }
#endif
  }

  INLINE int16_t process(int16_t audio_input) {
#if 1
    int16_t x_0   = audio_input >> (16 - AUDIO_FRACTION_BITS);
    int32_t tmp   = ((x_0 + (m_x_1 << 1) + m_x_2) * m_b_2_over_a_0) >> 2;
    tmp          -= ( m_y_1                       * m_a_1_over_a_0) >> 0;
    tmp          -= ( m_y_2                       * m_a_2_over_a_0) >> 1;
    int16_t y_0   = tmp >> FILTER_TABLE_FRACTION_BITS;

    m_x_2 = m_x_1;
    m_y_2 = m_y_1;
    m_x_1 = x_0;
    m_y_1 = y_0;

    if        (high_sbyte(y_0) >= high_sbyte(+MAX_ABS_OUTPUT)) {
      y_0 = +MAX_ABS_OUTPUT;
    } else if (high_sbyte(y_0) <  high_sbyte(-MAX_ABS_OUTPUT)) {
      y_0 = -MAX_ABS_OUTPUT;
    }
#else
    int16_t y_0  = 0;
#endif

    return y_0 << (16 - AUDIO_FRACTION_BITS);
  }

private:
  INLINE void update_coefs_0th(int16_t eg_input) {
    m_cutoff_candidate = m_cutoff_control_effective;
    m_cutoff_candidate += (m_cutoff_eg_amt * eg_input) >> 14;
    m_cutoff_candidate += m_cutoff_offset;
  }

  INLINE void update_coefs_1st(int16_t lfo_input, uint16_t osc_pitch) {
    m_cutoff_candidate += (lfo_input * m_cutoff_lfo_amt) >> 14;
    if (m_cutoff_pitch_amt == 1) {
      m_cutoff_candidate += static_cast<int8_t>(high_byte(osc_pitch + 128) - 60);
    } else if (m_cutoff_pitch_amt == 2) {
      m_cutoff_candidate += static_cast<int8_t>(high_byte((osc_pitch << 1) + 128) - 120);
    }
  }

  INLINE void update_coefs_2nd() {
    if (m_cutoff_candidate > 127) {
      m_cutoff_current = 127;
    } else if (m_cutoff_candidate < 0) {
      m_cutoff_current = 0;
    } else {
      m_cutoff_current = m_cutoff_candidate;
    }

    m_cutoff_control_effective += (m_cutoff_control_effective < m_cutoff_control);
    m_cutoff_control_effective -= (m_cutoff_control_effective > m_cutoff_control);
  }

  INLINE void update_coefs_3rd() {
    size_t index = m_cutoff_current * 3;
    m_b_2_over_a_0 =                      m_lpf_table[index + 0];
    m_a_1_over_a_0 = static_cast<int16_t>(m_lpf_table[index + 1]);
    m_a_2_over_a_0 = static_cast<int16_t>(m_lpf_table[index + 2]);
  }
};
