#pragma once

#include "pra32-u-common.h"
#include "pra32-u-osc-table.h"
#include <math.h>


class PRA32_U_Osc {
  static const uint8_t OSC_MIX_TABLE_LENGTH   = 31;

  static const uint8_t PORTAMENTO_COEF_OFF    = 190;

  static const uint8_t WAVEFORM_SAW           = 0;
  static const uint8_t WAVEFORM_SQUARE        = 1;
  static const uint8_t WAVEFORM_TRIANGLE      = 2;
  static const uint8_t WAVEFORM_1_PULSE       = 4;
  static const uint8_t WAVEFORM_2_NOISE       = 5;

  uint8_t        m_portamento_coef[4];
  int8_t         m_pitch_eg_amt[2];
  int8_t         m_pitch_lfo_amt[2];

  uint8_t        m_waveform[2];
  int16_t        m_pitch_bend;
  uint8_t        m_pitch_bend_range;
  int16_t        m_pitch_bend_normalized;
  uint16_t       m_pitch_target[4];
  uint16_t       m_pitch_current[4];
  uint16_t       m_pitch_real[4];
  const int16_t* m_wave_table[4];
  const int16_t* m_wave_table_temp[4];
  uint32_t       m_freq[4];
  uint32_t       m_freq_base[4];
  uint32_t       m_freq_offset[4];
  uint32_t       m_phase[4];
  boolean        m_osc_on[4];
  int8_t         m_osc_gain_effective[4];
  int8_t         m_osc_gain[4];
  int8_t         m_osc_level;

  boolean        m_mono_mode;
  uint8_t        m_mono_osc2_mix;
  int8_t         m_mono_osc2_pitch;
  int8_t         m_mono_osc2_detune;

  uint8_t        m_phase_high;
  int8_t         m_osc1_shape_control;
  int8_t         m_osc1_shape_control_effective;
  int8_t         m_osc1_morph_control;
  uint16_t       m_osc1_shape;
  uint8_t        m_mixer_sub_osc_control;
  uint8_t        m_mix_table[OSC_MIX_TABLE_LENGTH];
  int8_t         m_shape_eg_amt;
  int8_t         m_shape_lfo_amt;

public:
  PRA32_U_Osc()
  : m_portamento_coef()
  , m_pitch_eg_amt()
  , m_pitch_lfo_amt()

  , m_waveform()
  , m_pitch_bend()
  , m_pitch_bend_range()
  , m_pitch_bend_normalized()
  , m_pitch_target()
  , m_pitch_current()
  , m_pitch_real()
  , m_wave_table()
  , m_wave_table_temp()
  , m_freq()
  , m_freq_base()
  , m_freq_offset()
  , m_phase()
  , m_osc_on()
  , m_osc_gain_effective()
  , m_osc_gain()
  , m_osc_level()

  , m_mono_mode()
  , m_mono_osc2_mix()
  , m_mono_osc2_pitch()
  , m_mono_osc2_detune()

  , m_phase_high()
  , m_osc1_shape_control()
  , m_osc1_shape_control_effective()
  , m_osc1_morph_control()
  , m_osc1_shape()
  , m_mixer_sub_osc_control()
  , m_mix_table()
  , m_shape_eg_amt()
  , m_shape_lfo_amt()
  {}

  INLINE void initialize() {
    m_portamento_coef[0] = PORTAMENTO_COEF_OFF;
    m_portamento_coef[1] = PORTAMENTO_COEF_OFF;
    m_portamento_coef[2] = PORTAMENTO_COEF_OFF;
    m_portamento_coef[3] = PORTAMENTO_COEF_OFF;

    set_mono_mode        (false);
    set_mono_osc2_mix    (0);
    set_mono_osc2_pitch  (0);
    set_mono_osc2_detune (0);

    m_waveform[0] = WAVEFORM_SAW;
    m_waveform[1] = WAVEFORM_SAW;
    m_pitch_target[0] = 60 << 8;
    m_pitch_target[1] = 60 << 8;
    m_pitch_target[2] = 60 << 8;
    m_pitch_target[3] = 60 << 8;
    m_pitch_current[0] = m_pitch_target[0];
    m_pitch_current[1] = m_pitch_target[1];
    m_pitch_current[2] = m_pitch_target[2];
    m_pitch_current[3] = m_pitch_target[3];
    m_pitch_real[0] = m_pitch_current[0];
    m_pitch_real[1] = m_pitch_current[1];
    m_pitch_real[2] = m_pitch_current[2];
    m_pitch_real[3] = m_pitch_current[3];
    m_wave_table[0] = g_osc_saw_wave_tables[0];
    m_wave_table[1] = g_osc_saw_wave_tables[0];
    m_wave_table[2] = g_osc_saw_wave_tables[0];
    m_wave_table[3] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[0] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[1] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[2] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[3] = g_osc_saw_wave_tables[0];
    m_freq[0] = g_osc_freq_table[0];
    m_freq[1] = g_osc_freq_table[0];
    m_freq[2] = g_osc_freq_table[0];
    m_freq[3] = g_osc_freq_table[0];
    m_freq_base[0] = g_osc_freq_table[0];
    m_freq_base[1] = g_osc_freq_table[0];
    m_freq_base[2] = g_osc_freq_table[0];
    m_freq_base[3] = g_osc_freq_table[0];
    m_freq_offset[0] = 0;
    m_freq_offset[1] = 0;
    m_freq_offset[2] = 0;
    m_freq_offset[3] = 0;
    m_osc_level = 32;

    m_osc1_shape = 0x8000;
    for (uint8_t i = 0; i < OSC_MIX_TABLE_LENGTH; ++i) {
      m_mix_table[i] = static_cast<uint8_t>(sqrtf(static_cast<float>(i) /
                                                  (OSC_MIX_TABLE_LENGTH - 1)) * 255);
    }

    set_pitch_bend_range(2);
  }

  template <uint8_t N>
  INLINE void set_osc_waveform(uint8_t controller_value) {
    if (N == 0) {
      if (controller_value < 48) {
        m_waveform[0] = WAVEFORM_SAW;
      } else if (controller_value < 80) {
        m_waveform[0] = WAVEFORM_TRIANGLE;
      } else if (controller_value < 112) {
        m_waveform[0] = WAVEFORM_1_PULSE;
      } else {
        m_waveform[0] = WAVEFORM_SQUARE;
      }
    } else {
      if (controller_value < 48) {
        m_waveform[1] = WAVEFORM_SAW;
      } else if (controller_value < 80) {
        m_waveform[1] = WAVEFORM_TRIANGLE;
      } else if (controller_value < 112) {
        m_waveform[1] = WAVEFORM_2_NOISE;
      } else {
        m_waveform[1] = WAVEFORM_SQUARE;
      }
    }
  }

  INLINE void set_osc1_shape_control(uint8_t controller_value) {
    m_osc1_shape_control = -(((controller_value + 1) >> 1) << 1);
  }

  INLINE void set_osc1_morph_control(uint8_t controller_value) {
    m_osc1_morph_control = -(((controller_value - 63) >> 1) << 1);
  }

  INLINE void set_mixer_sub_osc_control(uint8_t controller_value) {
    m_mixer_sub_osc_control = (((controller_value + 1) >> 1) *
                              static_cast<uint8_t>(OSC_WAVE_TABLE_AMPLITUDE << 1)) >> 8;
  }

  template <uint8_t N>
  INLINE void set_pitch_eg_amt(uint8_t controller_value) {
    if (controller_value < 25) {
      m_pitch_eg_amt[N] = -96;
    } else if (controller_value < 48) {
      m_pitch_eg_amt[N] = (controller_value - 49) << 2;
    } else if (controller_value < 80) {
      m_pitch_eg_amt[N] = (controller_value - 62) >> 2;
    } else if (controller_value < 103) {
      m_pitch_eg_amt[N] = (controller_value - 79) << 2;
    } else {
      m_pitch_eg_amt[N] = 96;
    }
  }

  INLINE void set_shape_eg_amt(uint8_t controller_value) {
    if (controller_value == 0) {
      controller_value = 1;
    }
    m_shape_eg_amt = ((controller_value - 64) << 1);
  }

  template <uint8_t N>
  INLINE void set_pitch_lfo_amt(uint8_t controller_value) {
    if (controller_value < 25) {
      m_pitch_lfo_amt[N] = -96;
    } else if (controller_value < 48) {
      m_pitch_lfo_amt[N] = (controller_value - 49) << 2;
    } else if (controller_value < 80) {
      m_pitch_lfo_amt[N] = (controller_value - 62) >> 2;
    } else if (controller_value < 103) {
      m_pitch_lfo_amt[N] = (controller_value - 79) << 2;
    } else {
      m_pitch_lfo_amt[N] = 96;
    }
  }

  INLINE void set_shape_lfo_amt(uint8_t controller_value) {
    if (controller_value == 0) {
      controller_value = 1;
    }
    m_shape_lfo_amt = -((controller_value - 64) << 1);
  }

  INLINE void set_mono_mode(boolean mono_mode) {
    m_mono_mode = mono_mode;
  }

  INLINE void set_mono_osc2_mix(uint8_t controller_value) {
    if (controller_value >= 2) {
      controller_value -= 2;
    }

    if (controller_value > 123) {
      controller_value = 123;
    }
    m_mono_osc2_mix = controller_value >> 2;
  }

  INLINE void set_mono_osc2_pitch(uint8_t controller_value) {
    if (controller_value < 16) {
      m_mono_osc2_pitch = -48;
    } else if (controller_value < 112) {
      m_mono_osc2_pitch = controller_value - 64;
    } else {
      m_mono_osc2_pitch = 48;
    }
  }

  INLINE void set_mono_osc2_detune(uint8_t controller_value) {
    m_mono_osc2_detune = (controller_value - 64) << 1;
  }

  template <uint8_t N>
  INLINE void set_portamento(uint8_t controller_value) {
    m_portamento_coef[N] = ((controller_value + 1) >> 1) + PORTAMENTO_COEF_OFF;
  }

  template <uint8_t N>
  INLINE void note_on(uint8_t note_number) {
    m_pitch_target[N] = (note_number << 8) + g_osc_freq_detune_table[note_number - NOTE_NUMBER_MIN];
    m_osc_on[N] = true;
  }

  template <uint8_t N>
  INLINE void note_off() {
    m_osc_on[N] = false;
  }

  INLINE void set_pitch_bend_range(uint8_t controller_value) {
    uint8_t range = controller_value;
    if (range > 24) {
      range = 24;
    }
    m_pitch_bend_range = range;
    update_pitch_bend();
  }

  INLINE void set_pitch_bend(int16_t pitch_bend) {
    m_pitch_bend = pitch_bend;
    update_pitch_bend();
  }

  INLINE uint16_t get_osc_pitch() {
    if (m_mono_mode) {
      return m_pitch_current[0] + m_pitch_bend_normalized;
    }
    return (60 << 8);
  }

  INLINE void process_at_low_rate(uint8_t count, int16_t noise_int15, int16_t lfo_level, int16_t eg_level) {
    update_osc1_shape(lfo_level, eg_level);

    update_freq_base<0>(lfo_level, eg_level);
    update_freq_base<1>(lfo_level, eg_level);
    update_freq_base<2>(lfo_level, eg_level);
    update_freq_base<3>(lfo_level, eg_level);

    switch (count & (0x08 - 1)) {
    case 0x00: update_pitch_current<0>();
               update_freq_offset<0>(noise_int15);
               update_gate<0>();
               break;
    case 0x02: update_pitch_current<1>();
               update_freq_offset<1>(noise_int15);
               update_gate<1>();
               break;
    case 0x04: update_pitch_current<2>();
               update_freq_offset<2>(noise_int15);
               update_gate<2>();
               break;
    case 0x06: update_pitch_current<3>();
               update_freq_offset<3>(noise_int15);
               update_gate<3>();
               break;
    }
  }

  INLINE int16_t process(int16_t noise_int15) {
#if 1
    m_phase[0] += m_freq[0];
    boolean new_period_0 = (m_phase[0] & 0x00FFFFFF) < m_freq[0];
    m_wave_table[0] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[0]) * (1 - new_period_0)));
    m_wave_table[0] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[0]) +
                                                       (reinterpret_cast<const uintptr_t>(m_wave_table_temp[0]) * new_period_0));
    int16_t wave_0 = get_wave_level(m_wave_table[0], m_phase[0] >> 8);
    int32_t result = wave_0 * m_osc_gain_effective[0];

    if (m_mono_mode == false) {
      m_phase[1] += m_freq[1];
      boolean new_period_1 = (m_phase[1] & 0x00FFFFFF) < m_freq[1];
      m_wave_table[1] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[1]) * (1 - new_period_1)));
      m_wave_table[1] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[1]) +
                                                         (reinterpret_cast<const uintptr_t>(m_wave_table_temp[1]) * new_period_1));
      int16_t wave_1 = get_wave_level(m_wave_table[1], m_phase[1] >> 8);
      result += wave_1 * m_osc_gain_effective[1];

      m_phase[2] += m_freq[2];
      boolean new_period_2 = (m_phase[2] & 0x00FFFFFF) < m_freq[2];
      m_wave_table[2] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[2]) * (1 - new_period_2)));
      m_wave_table[2] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[2]) +
                                                         (reinterpret_cast<const uintptr_t>(m_wave_table_temp[2]) * new_period_2));
      int16_t wave_2 = get_wave_level(m_wave_table[2], m_phase[2] >> 8);
      result += wave_2 * m_osc_gain_effective[2];

      m_phase[3] += m_freq[3];
      boolean new_period_3 = (m_phase[3] & 0x00FFFFFF) < m_freq[3];
      m_wave_table[3] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[3]) * (1 - new_period_3)));
      m_wave_table[3] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[3]) +
                                                         (reinterpret_cast<const uintptr_t>(m_wave_table_temp[3]) * new_period_3));
      int16_t wave_3 = get_wave_level(m_wave_table[3], m_phase[3] >> 8);
      result += wave_3 * m_osc_gain_effective[3];
    } else {
      if (m_waveform[0] == WAVEFORM_1_PULSE) {
        // For Pulse Wave (wave_3)
        m_phase[3] = m_phase[0] + (m_osc1_shape << 8); // todo
        int16_t wave_3 = get_wave_level(m_wave_table[0], m_phase[3] >> 8);
        result += wave_3 * m_osc_gain_effective[3];
      }

      // Sub Osc (wave_1)
      int16_t wave_1 = static_cast<uint16_t>(m_phase[0] >> 9);
      if (wave_1 < -(64 << 8)) {
        wave_1 = -(64 << 8) - (wave_1 + (64 << 8));
      } else if (wave_1 < (64 << 8)) {
        // do nothing
      } else {
        wave_1 = (64 << 8) - (wave_1 - (64 << 8));
      }
      result += wave_1 * (m_osc_gain_effective[1] << 1);

      if (m_waveform[1] != WAVEFORM_2_NOISE) {
        m_phase[2] += m_freq[2];
        boolean new_period_2 = (m_phase[2] & 0x00FFFFFF) < m_freq[2];
        m_wave_table[2] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[2]) * (1 - new_period_2)));
        m_wave_table[2] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[2]) +
                                                           (reinterpret_cast<const uintptr_t>(m_wave_table_temp[2]) * new_period_2));
        int16_t wave_2 = get_wave_level(m_wave_table[2], m_phase[2] >> 8);
        result += wave_2 * m_osc_gain_effective[2];
      } else {
        // Noise (wave_2)
        int16_t wave_2 = -(OSC_WAVE_TABLE_AMPLITUDE << 8)
                         +(OSC_WAVE_TABLE_AMPLITUDE << 9) * (noise_int15 & 0x1);
        result += wave_2 * m_osc_gain_effective[2];
      }
    }
#else
    int32_t result  = 0;
#endif

    return result >> 8;
  }

private:
  INLINE const int16_t* get_wave_table(uint8_t waveform, uint8_t note_number) {
    const int16_t* result;
    if ((waveform == WAVEFORM_SAW) ||
        (m_mono_mode && (waveform == WAVEFORM_1_PULSE))) {
      result = g_osc_saw_wave_tables[note_number - NOTE_NUMBER_MIN];
    } else if (waveform == WAVEFORM_TRIANGLE) {
      result = g_osc_triangle_wave_table;
    } else {     // WAVEFORM_SQUARE
      result = g_osc_pulse_wave_tables[note_number - NOTE_NUMBER_MIN];
    }
    return result;
  }

  INLINE int16_t get_wave_level(const int16_t* wave_table, uint16_t phase) {
    uint16_t curr_index  = phase >> (16 - OSC_WAVE_TABLE_SAMPLES_BITS);
    uint16_t next_weight = phase & ((1 << (16 - OSC_WAVE_TABLE_SAMPLES_BITS)) - 1);
    int16_t curr_data   = wave_table[curr_index + 0];
    int16_t next_data   = wave_table[curr_index + 1];

    // lerp
    int16_t result = curr_data + (((next_data - curr_data) * next_weight) >> (16 - OSC_WAVE_TABLE_SAMPLES_BITS));

    return result;
  }

  template <uint8_t N>
  INLINE void update_pitch_current() {
    if (m_osc_on[N]) {
      // todo
      if ((m_portamento_coef[N] == PORTAMENTO_COEF_OFF) || (m_pitch_current[N] <= m_pitch_target[N])) {
        m_pitch_current[N] = m_pitch_target[N]  - (((m_pitch_target[N] - m_pitch_current[N]) *        m_portamento_coef[N] ) >> 8);
      } else {
        m_pitch_current[N] = m_pitch_current[N] + (((m_pitch_target[N] - m_pitch_current[N]) * (256 - m_portamento_coef[N])) >> 8);
      }
    }
  }

  template <uint8_t N>
  INLINE void update_freq_base(int16_t lfo_level, int16_t eg_level) {
    int8_t pitch_eg_amt;
    if ((N == 2) && m_mono_mode) {
      pitch_eg_amt = m_pitch_eg_amt[1];
    } else {
      pitch_eg_amt = m_pitch_eg_amt[0];
    }
    m_pitch_real[N] =  (64 << 8) + m_pitch_current[N] + m_pitch_bend_normalized + ((pitch_eg_amt * eg_level) >> 8);

    uint8_t coarse = high_byte(m_pitch_real[N]);
    if (coarse < (NOTE_NUMBER_MIN + 64)) {
      m_pitch_real[N] = ((NOTE_NUMBER_MIN + 64) << 8);
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      m_pitch_real[N] = ((NOTE_NUMBER_MAX + 64) << 8);
    }

    if (m_mono_mode && (N == 2)) {
      m_pitch_real[N] += (lfo_level * m_pitch_lfo_amt[1]) >> 7;
      m_pitch_real[N] += (m_mono_osc2_pitch << 8) + m_mono_osc2_detune + m_mono_osc2_detune;
    } else {
      m_pitch_real[N] += (lfo_level * m_pitch_lfo_amt[0]) >> 7;
    }

    coarse = high_byte(m_pitch_real[N]);
    if (coarse < (NOTE_NUMBER_MIN + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MIN << 8;
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MAX << 8;
    } else {
      m_pitch_real[N] -= (64 << 8);
    }

    m_pitch_real[N] += 128;  // For g_osc_tune_table[]


    coarse = high_byte(m_pitch_real[N]);
    m_freq_base[N] = g_osc_freq_table[coarse - NOTE_NUMBER_MIN];
    if ((N == 2) && m_mono_mode) {
      m_wave_table_temp[N] = get_wave_table(m_waveform[1], coarse);
    } else {
      m_wave_table_temp[N] = get_wave_table(m_waveform[0], coarse);
    }


    uint8_t fine = low_byte(m_pitch_real[N]);
    int32_t offset =
      static_cast<int32_t>(m_freq_base[N] * g_osc_tune_table[fine >> (8 - OSC_TUNE_TABLE_STEPS_BITS)]) >>
      OSC_TUNE_DENOMINATOR_BITS;
    m_freq_base[N] += offset;
    m_freq[N] = m_freq_base[N] + m_freq_offset[N];
  }

  template <uint8_t N>
  INLINE void update_freq_offset(int16_t noise_int15) {
    m_freq_offset[N] = (noise_int15 >= 14336) << 8;
    m_freq[N] = m_freq_base[N] + m_freq_offset[N];
  }

  template <uint8_t N>
  INLINE void update_gate() {
    if (m_mono_mode == false) {
      if (m_osc_on[N]) {
        const int8_t half_level = (m_osc_level >> 1) + 1;

        if (m_osc_gain[N] >= (m_osc_level - half_level)) {
          m_osc_gain[N] = m_osc_level;
        } else {
          m_osc_gain[N] += half_level;
        }
      } else {
        const int8_t one_fourth_level = (m_osc_level >> 2) + 1;

        if (m_osc_gain[N] <= one_fourth_level) {
          m_osc_gain[N] = 0;
        } else {
          m_osc_gain[N] -= one_fourth_level;
        }
      }
      m_osc_gain_effective[N] = m_osc_gain[N];
    } else {
      if (N == 0) {
        uint8_t base_gain = m_osc_level << 1;
        m_osc_gain_effective[0] = high_byte(m_mix_table[(OSC_MIX_TABLE_LENGTH - 1) - m_mono_osc2_mix] * base_gain);
        m_osc_gain_effective[2] = high_byte(m_mix_table[                             m_mono_osc2_mix] * base_gain);
        m_osc_gain_effective[1] = (base_gain * m_mixer_sub_osc_control) >> 6;
        m_osc_gain_effective[3] = 0;

        if (m_waveform[0] == WAVEFORM_1_PULSE) {
          int8_t temp = high_sbyte(-m_osc_gain_effective[0] * m_osc1_morph_control);
          m_osc_gain_effective[3] = temp << 2;
        }
      }
    }
  }

  INLINE void update_osc1_shape(int16_t lfo_level, int16_t eg_level) {
    m_osc1_shape_control_effective += (m_osc1_shape_control_effective < m_osc1_shape_control) << 1; // todo

    m_osc1_shape = 0x8000u - (m_osc1_shape_control_effective << 8)
                   + ((eg_level * m_shape_eg_amt) >> 5) - ((lfo_level * m_shape_lfo_amt) >> 3);
  }

  INLINE void update_pitch_bend() {
    int16_t b = m_pitch_bend + 1;
    b >>= 3;
    m_pitch_bend_normalized = (b * m_pitch_bend_range) >> 2;
  }
};
