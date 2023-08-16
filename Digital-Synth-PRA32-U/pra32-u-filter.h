#pragma once

// refs https://webaudio.github.io/Audio-EQ-Cookbook/Audio-EQ-Cookbook.txt

#include "pra32-u-common.h"
#include "pra32-u-filter-table.h"

class PRA32_U_Filter {
  const int32_t*  m_lpf_table;
  int32_t         m_b_2_over_a_0;
  int32_t         m_a_1_over_a_0;
  int32_t         m_a_2_over_a_0;
  int16_t         m_x_1;
  int16_t         m_x_2;
  int32_t         m_y_1;
  int32_t         m_y_2;
  int16_t         m_cutoff_current;
  int16_t         m_cutoff_candidate;
  uint8_t         m_cutoff_control;
  uint8_t         m_cutoff_control_effective;
  int8_t          m_cutoff_eg_amt;
  int8_t          m_cutoff_lfo_amt;
  int8_t          m_cutoff_pitch_amt;
  int8_t          m_cutoff_offset;
  uint8_t         m_filter_mode;

  const uint8_t AUDIO_FRACTION_BITS = 14;
  const int16_t MAX_ABS_OUTPUT = ((124 << (AUDIO_FRACTION_BITS - 8)) >> 8) << 8;

  static const int8_t m_cutoff_mod_amt_table[128];

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
  , m_filter_mode()
  {
    m_cutoff_current = 254;

    set_cutoff(127);
    set_resonance(0);
    set_cutoff_eg_amt(64);
    set_cutoff_lfo_amt(64);
    set_cutoff_pitch_amt(0);
    set_cutoff_offset(0);

    update_coefs(0, 0, 60 << 8);
  }

  INLINE void set_cutoff(uint8_t controller_value) {
    m_cutoff_control = controller_value << 1;
  }

  INLINE void set_resonance(uint8_t controller_value) {
    m_lpf_table = g_filter_lpf_tables[(controller_value + 4) >> 4];
  }

  INLINE void set_cutoff_eg_amt(uint8_t controller_value) {
    m_cutoff_eg_amt = m_cutoff_mod_amt_table[controller_value];
  }

  INLINE void set_cutoff_lfo_amt(uint8_t controller_value) {
    m_cutoff_lfo_amt = m_cutoff_mod_amt_table[controller_value];
  }

  INLINE void set_cutoff_pitch_amt(uint8_t controller_value) {
    static int8_t cutoff_pitch_amt_table[4] = {
        +0,   +1,   +1,   +2,
    };

    m_cutoff_pitch_amt = cutoff_pitch_amt_table[controller_value >> 5];
  }

  INLINE void set_filter_mode(uint8_t controller_value) {
    m_filter_mode = controller_value;
  }

  INLINE void set_cutoff_offset(int8_t cutoff_offset) {
    m_cutoff_offset = cutoff_offset;
  }

  INLINE void process_at_low_rate(uint8_t count, int16_t eg_input, int16_t lfo_input, int16_t osc_pitch) {
    switch (count & (0x08 - 1)) {
    case 0x00:
      update_cutoff_control_effective();
      break;
    }

    update_coefs(eg_input, lfo_input, osc_pitch);
  }

  INLINE int16_t process(int16_t audio_input) {
#if 1
    int16_t x_0 = audio_input >> (16 - AUDIO_FRACTION_BITS);
    int16_t x_3 = x_0 + (m_x_1 << 1) + m_x_2;
    int32_t y_0 = mul_s32_s16_h32(m_b_2_over_a_0,   x_3) << 2;
    y_0        -= mul_s32_s32_h32(m_a_1_over_a_0, m_y_1) << 2;
    y_0        -= mul_s32_s32_h32(m_a_2_over_a_0, m_y_2) << 2;

    m_x_2 = m_x_1;
    m_y_2 = m_y_1;
    m_x_1 = x_0;
    m_y_1 = y_0;

    if (m_filter_mode >= 64) {
      // low cut
      y_0 = (audio_input << AUDIO_FRACTION_BITS) - y_0;
    }

    // y = clamp(y_0, (-MAX_ABS_OUTPUT << 16), (+MAX_ABS_OUTPUT << 16))
    volatile int32_t y = y_0 - (+MAX_ABS_OUTPUT << 16);
    y = (y < 0) * y + (+MAX_ABS_OUTPUT << 16) - (-MAX_ABS_OUTPUT << 16);
    y = (y > 0) * y + (-MAX_ABS_OUTPUT << 16);
#else
    volatile int32_t y = 0;
#endif

    return y >> AUDIO_FRACTION_BITS;
  }

private:
  INLINE void update_cutoff_control_effective() {
    m_cutoff_control_effective += (m_cutoff_control_effective < m_cutoff_control);
    m_cutoff_control_effective -= (m_cutoff_control_effective > m_cutoff_control);
  }

  INLINE void update_coefs(int16_t eg_input, int16_t lfo_input, int16_t osc_pitch) {
    m_cutoff_candidate = m_cutoff_control_effective;
    m_cutoff_candidate += (m_cutoff_eg_amt * eg_input) >> 14;
    m_cutoff_candidate += m_cutoff_offset;

    m_cutoff_candidate += (lfo_input * m_cutoff_lfo_amt) >> 14;
    m_cutoff_candidate += ((osc_pitch - (60 << 8)) * m_cutoff_pitch_amt) >> 8;

    // cutoff_current = clamp(m_cutoff_candidate, 0, 255)
    volatile int16_t cutoff_current = m_cutoff_candidate - 255;
    cutoff_current = (cutoff_current < 0) * cutoff_current + 255;
    cutoff_current = (cutoff_current > 0) * cutoff_current;
    m_cutoff_current = cutoff_current;

    size_t index = m_cutoff_current * 3;
    m_b_2_over_a_0 = m_lpf_table[index + 0];
    m_a_1_over_a_0 = m_lpf_table[index + 1];
    m_a_2_over_a_0 = m_lpf_table[index + 2];
  }
};

const int8_t PRA32_U_Filter::m_cutoff_mod_amt_table[128] = {
  -120, -120, -120, -120, -120, -118, -116, -114, -112, -110, -108, -106, -104, -102, -100,  -98,
   -96,  -94,  -92,  -90,  -88,  -86,  -84,  -82,  -80,  -78,  -76,  -74,  -72,  -70,  -68,  -66,
   -64,  -62,  -60,  -58,  -56,  -54,  -52,  -50,  -48,  -46,  -44,  -42,  -40,  -38,  -36,  -34,
   -32,  -30,  -28,  -26,  -24,  -22,  -20,  -18,  -16,  -14,  -12,  -10,   -8,   -6,   -4,   -2,
    +0,   +2,   +4,   +6,   +8,  +10,  +12,  +14,  +16,  +18,  +20,  +22,  +24,  +26,  +28,  +30,
   +32,  +34,  +36,  +38,  +40,  +42,  +44,  +46,  +48,  +50,  +52,  +54,  +56,  +58,  +60,  +62,
   +64,  +66,  +68,  +70,  +72,  +74,  +76,  +78,  +80,  +82,  +84,  +86,  +88,  +90,  +92,  +94,
   +96,  +98, +100, +102, +104, +106, +108, +110, +112, +114, +116, +118, +120, +120, +120, +120,
};
