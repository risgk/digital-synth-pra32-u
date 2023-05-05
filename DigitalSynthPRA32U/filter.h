#pragma once

// refs http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

#include "common.h"
#include "mul-q.h"
#include "filter-table.h"

template <uint8_t T>
class Filter {
  static const uint8_t*  m_lpf_table;
  static int16_t         m_b_2_over_a_0;
  static int8_t          m_a_1_over_a_0_high;
  static int16_t         m_a_2_over_a_0;
  static int16_t         m_x_1;
  static int16_t         m_x_2;
  static int16_t         m_y_1;
  static int16_t         m_y_2;
  static uint8_t         m_cutoff_current;
  static int16_t         m_cutoff_candidate;
  static uint8_t         m_cutoff_control;
  static uint8_t         m_cutoff_control_effective;
  static int8_t          m_cutoff_eg_amt;
  static int8_t          m_cutoff_lfo_amt;
  static int8_t          m_cutoff_pitch_amt;
  static int8_t          m_cutoff_offset;

  static const uint8_t AUDIO_FRACTION_BITS = 14;
  static const int16_t MAX_ABS_OUTPUT = ((124 << (AUDIO_FRACTION_BITS - 8)) >> 8) << 8;

public:
  INLINE static void initialize() {
    m_cutoff_current = 127;

    set_cutoff(127);
    set_resonance(0);
    set_cutoff_eg_amt(64);
    set_cutoff_lfo_amt(64);
    set_cutoff_pitch_amt(0);
    set_cutoff_offset(0);

    update_coefs_0th(0);
    update_coefs_1st(0);
    update_coefs_2nd();
    update_coefs_3rd();
  }

  INLINE static void set_cutoff(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_control = value;
  }

  INLINE static void set_resonance(uint8_t controller_value) {
    uint8_t index = (controller_value + 4) >> 4;
    m_lpf_table = g_filter_lpf_tables[index];
  }

  INLINE static void set_cutoff_eg_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_eg_amt = (value - 64) << 1;
  }

  INLINE static void set_cutoff_lfo_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_lfo_amt = (value - 64) << 1;
  }

  INLINE static void set_cutoff_pitch_amt(uint8_t controller_value) {
    if (controller_value < 32) {
      m_cutoff_pitch_amt = 0;
    } else if (controller_value < 96) {
      m_cutoff_pitch_amt = 1;
    } else {
      m_cutoff_pitch_amt = 2;
    }
  }

  INLINE static void set_cutoff_offset(int8_t cutoff_offset) {
    m_cutoff_offset = cutoff_offset;
  }

  INLINE static int16_t __not_in_flash_func(process)(uint8_t count, int16_t audio_input, uint8_t eg_input, int16_t lfo_input) {
#if 1
    if ((count & (FILTER_CONTROL_INTERVAL - 1)) == 7) {
      //printf("%d Filter\n", count);
      if (count & 0x10) {
        if (count & 0x08) {
          update_coefs_3rd();
        } else {
          update_coefs_2nd();
        }
      } else {
        if (count & 0x08) {
          update_coefs_1st(lfo_input);
        } else {
          update_coefs_0th(eg_input);
        }
      }
    }
#endif

#if 1
    int16_t x_0  = audio_input >> (16 - AUDIO_FRACTION_BITS);
    int16_t tmp  = mul_sq16_sq16(x_0 + (m_x_1 << 1) + m_x_2, m_b_2_over_a_0);
    tmp         -= mul_sq16_sq8( m_y_1,                      m_a_1_over_a_0_high);
    tmp         -= mul_sq16_sq16(m_y_2,                      m_a_2_over_a_0);
    int16_t y_0  = tmp << (16 - FILTER_TABLE_FRACTION_BITS);

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
  INLINE static void update_coefs_0th(uint8_t eg_input) {
    m_cutoff_candidate = m_cutoff_control_effective;
    m_cutoff_candidate += high_sbyte((m_cutoff_eg_amt * eg_input) << 1);
    m_cutoff_candidate += m_cutoff_offset;
  }

  INLINE static void update_coefs_1st(int16_t lfo_input) {
    m_cutoff_candidate -= high_sbyte(mul_sq16_sq8(lfo_input, m_cutoff_lfo_amt) << 1);

    // OSC Pitch is handled here (not in Voice) for performance reasons
    uint16_t osc_pitch = IOsc<0>::get_osc_pitch();
    if (m_cutoff_pitch_amt == 1) {
      m_cutoff_candidate += static_cast<int8_t>(high_byte(osc_pitch + 128) - 60);
    } else if (m_cutoff_pitch_amt == 2) {
      m_cutoff_candidate += static_cast<int8_t>(high_byte((osc_pitch << 1) + 128) - 120);
    }
  }

  INLINE static void update_coefs_2nd() {
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

  INLINE static void update_coefs_3rd() {
    const uint8_t* p = m_lpf_table + static_cast<uint8_t>(m_cutoff_current << 1) + static_cast<uint8_t>(m_cutoff_current << 1) + m_cutoff_current;
    m_b_2_over_a_0 = ram_read_word(p);
    p += 2;
    m_a_1_over_a_0_high = ram_read_byte(p);
    p += 1;
    m_a_2_over_a_0 = ram_read_word(p);
  }
};

template <uint8_t T> const uint8_t*  Filter<T>::m_lpf_table;
template <uint8_t T> int16_t         Filter<T>::m_b_2_over_a_0;
template <uint8_t T> int8_t          Filter<T>::m_a_1_over_a_0_high;
template <uint8_t T> int16_t         Filter<T>::m_a_2_over_a_0;
template <uint8_t T> int16_t         Filter<T>::m_x_1;
template <uint8_t T> int16_t         Filter<T>::m_x_2;
template <uint8_t T> int16_t         Filter<T>::m_y_1;
template <uint8_t T> int16_t         Filter<T>::m_y_2;
template <uint8_t T> uint8_t         Filter<T>::m_cutoff_current;
template <uint8_t T> int16_t         Filter<T>::m_cutoff_candidate;
template <uint8_t T> uint8_t         Filter<T>::m_cutoff_control;
template <uint8_t T> uint8_t         Filter<T>::m_cutoff_control_effective;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_eg_amt;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_lfo_amt;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_pitch_amt;
template <uint8_t T> int8_t          Filter<T>::m_cutoff_offset;
