#pragma once

#include "pra32-u-common.h"
#include "pra32-u-osc-table.h"
#include <math.h>


class PRA32_U_Osc {
  static const uint8_t OSC_MIX_TABLE_LENGTH   = 65;

  static const uint8_t PORTAMENTO_COEF_OFF    = 190;

  static const uint8_t WAVEFORM_SAW           = 0;
  static const uint8_t WAVEFORM_SQUARE        = 1;
  static const uint8_t WAVEFORM_TRIANGLE      = 2;
  static const uint8_t WAVEFORM_1_PULSE       = 4;
  static const uint8_t WAVEFORM_2_NOISE       = 5;

  uint8_t        m_portamento_coef[4];
  int16_t        m_pitch_eg_amt[2];
  int16_t        m_pitch_lfo_amt[2];

  uint8_t        m_waveform[2];
  int16_t        m_pitch_bend;
  uint8_t        m_pitch_bend_range;
  int16_t        m_pitch_bend_normalized;
  uint16_t       m_pitch_target[4];
  uint16_t       m_pitch_current[4];
  const int16_t* m_wave_table[4 * 3];
  const int16_t* m_wave_table_temp[4 * 2];
  uint32_t       m_freq[4 * 2];
  uint32_t       m_freq_base[4 * 2];
  uint32_t       m_freq_offset[4 * 2];
  uint32_t       m_phase[4 * 2];
  boolean        m_osc_on[4];
  int8_t         m_osc_gain_effective[4];
  int8_t         m_osc_level;

  boolean        m_mono_mode;
  boolean        m_gate_enabled;
  uint8_t        m_osc2_mix;
  int8_t         m_osc2_pitch;
  int8_t         m_osc2_detune;

  uint8_t        m_phase_high;
  int8_t         m_osc1_shape_control;
  int8_t         m_osc1_shape_control_effective;
  int8_t         m_osc1_morph_control;
  int32_t        m_osc1_shape[4];
  int32_t        m_osc1_shape_effective[4];
  int8_t         m_mixer_noise_sub_osc_control;
  int16_t        m_mix_table[OSC_MIX_TABLE_LENGTH];
  int8_t         m_shape_eg_amt;
  int8_t         m_shape_lfo_amt;

  static const int16_t m_pitch_mod_amt_table[128];

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
  , m_wave_table()
  , m_wave_table_temp()
  , m_freq()
  , m_freq_base()
  , m_freq_offset()
  , m_phase()
  , m_osc_on()
  , m_osc_gain_effective()
  , m_osc_level()

  , m_mono_mode()
  , m_gate_enabled()
  , m_osc2_mix()
  , m_osc2_pitch()
  , m_osc2_detune()

  , m_phase_high()
  , m_osc1_shape_control()
  , m_osc1_shape_control_effective()
  , m_osc1_morph_control()
  , m_osc1_shape()
  , m_osc1_shape_effective()
  , m_mixer_noise_sub_osc_control()
  , m_mix_table()
  , m_shape_eg_amt()
  , m_shape_lfo_amt()
  {
    m_portamento_coef[0] = PORTAMENTO_COEF_OFF;
    m_portamento_coef[1] = PORTAMENTO_COEF_OFF;
    m_portamento_coef[2] = PORTAMENTO_COEF_OFF;
    m_portamento_coef[3] = PORTAMENTO_COEF_OFF;

    set_mono_mode   (false);
    set_gate_enabled(false);
    set_osc2_mix    (0);
    set_osc2_pitch  (0);
    set_osc2_detune (0);

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
    m_wave_table[0] = g_osc_saw_wave_tables[0];
    m_wave_table[1] = g_osc_saw_wave_tables[0];
    m_wave_table[2] = g_osc_saw_wave_tables[0];
    m_wave_table[3] = g_osc_saw_wave_tables[0];
    m_wave_table[4] = g_osc_saw_wave_tables[0];
    m_wave_table[5] = g_osc_saw_wave_tables[0];
    m_wave_table[6] = g_osc_saw_wave_tables[0];
    m_wave_table[7] = g_osc_saw_wave_tables[0];
    m_wave_table[8] = g_osc_saw_wave_tables[0];
    m_wave_table[9] = g_osc_saw_wave_tables[0];
    m_wave_table[10] = g_osc_saw_wave_tables[0];
    m_wave_table[11] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[0] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[1] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[2] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[3] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[4] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[5] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[6] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[7] = g_osc_saw_wave_tables[0];
    m_freq[0] = g_osc_freq_table[0];
    m_freq[1] = g_osc_freq_table[0];
    m_freq[2] = g_osc_freq_table[0];
    m_freq[3] = g_osc_freq_table[0];
    m_freq[4] = g_osc_freq_table[0];
    m_freq[5] = g_osc_freq_table[0];
    m_freq[6] = g_osc_freq_table[0];
    m_freq[7] = g_osc_freq_table[0];
    m_freq_base[0] = g_osc_freq_table[0];
    m_freq_base[1] = g_osc_freq_table[0];
    m_freq_base[2] = g_osc_freq_table[0];
    m_freq_base[3] = g_osc_freq_table[0];
    m_freq_base[4] = g_osc_freq_table[0];
    m_freq_base[5] = g_osc_freq_table[0];
    m_freq_base[6] = g_osc_freq_table[0];
    m_freq_base[7] = g_osc_freq_table[0];
    m_osc_level = 72;

    m_osc1_shape[0]           = 0x8000;
    m_osc1_shape[1]           = 0x8000;
    m_osc1_shape[2]           = 0x8000;
    m_osc1_shape[3]           = 0x8000;
    m_osc1_shape_effective[0] = 0x8000;
    m_osc1_shape_effective[1] = 0x8000;
    m_osc1_shape_effective[2] = 0x8000;
    m_osc1_shape_effective[3] = 0x8000;

    for (uint8_t i = 0; i < OSC_MIX_TABLE_LENGTH; ++i) {
      m_mix_table[i] = static_cast<int16_t>(sqrtf(static_cast<float>(i) /
                                            (OSC_MIX_TABLE_LENGTH - 1)) * (1 << 10));
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
    m_osc1_shape_control = -controller_value;
  }

  INLINE void set_osc1_morph_control(uint8_t controller_value) {
    m_osc1_morph_control = -(((controller_value - 63) >> 1) << 1);
  }

  INLINE void set_mixer_sub_osc_control(uint8_t controller_value) {
    controller_value = (controller_value < 127) ? controller_value : 128;
    m_mixer_noise_sub_osc_control = controller_value - 64;
  }

  template <uint8_t N>
  INLINE void set_pitch_eg_amt(uint8_t controller_value) {
    m_pitch_eg_amt[N] = m_pitch_mod_amt_table[controller_value];
  }

  INLINE void set_shape_eg_amt(uint8_t controller_value) {
    if (controller_value == 0) {
      controller_value = 1;
    }
    m_shape_eg_amt = ((controller_value - 64) << 1);
  }

  template <uint8_t N>
  INLINE void set_pitch_lfo_amt(uint8_t controller_value) {
    m_pitch_lfo_amt[N] = m_pitch_mod_amt_table[controller_value];
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

  INLINE void set_gate_enabled(boolean gate_enabled) {
    m_gate_enabled = gate_enabled;
  }

  INLINE void set_osc2_mix(uint8_t controller_value) {
    m_osc2_mix = (controller_value + 1) >> 1;
  }

  INLINE void set_osc2_pitch(uint8_t controller_value) {
    if (controller_value < 16) {
      m_osc2_pitch = -48;
    } else if (controller_value < 112) {
      m_osc2_pitch = controller_value - 64;
    } else {
      m_osc2_pitch = 48;
    }
  }

  INLINE void set_osc2_detune(uint8_t controller_value) {
    m_osc2_detune = (controller_value - 64) << 1;
  }

  template <uint8_t N>
  INLINE void set_portamento(uint8_t controller_value) {
    m_portamento_coef[N] = ((controller_value + 1) >> 1) + PORTAMENTO_COEF_OFF;
  }

  template <uint8_t N>
  INLINE void note_on(uint8_t note_number) {
    uint8_t n;
    if (note_number < NOTE_NUMBER_MIN) {
      n = NOTE_NUMBER_MIN;
    } else if (note_number > NOTE_NUMBER_MAX) {
      n = NOTE_NUMBER_MAX;
    } else {
      n = note_number;
    }

    m_pitch_target[N] = (n << 8) + g_osc_freq_detune_table[n - NOTE_NUMBER_MIN];
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

  INLINE int16_t get_osc_pitch(uint8_t index) {
    if ((m_mono_mode == false) && m_gate_enabled) {
      // Paraphonic Mode
      return (60 << 8);
    }

    return + m_pitch_current[index] + m_pitch_bend_normalized;
  }

  template <uint8_t N>
  INLINE void process_at_low_rate_a(int16_t lfo_level, int16_t eg_level) {
    update_osc1_shape<N>(lfo_level, eg_level);
    update_osc1_shape_effective<N>();
    update_freq_base<N + 0>(lfo_level, eg_level);
    update_freq_base<N + 4>(lfo_level, eg_level);
  }

  INLINE void process_at_low_rate_b(uint8_t count, int16_t noise_int15) {
    switch (count & (0x08 - 1)) {
    case 0x00:
      update_freq_offset<0>(noise_int15);
      update_pitch_current<0>();
      update_gate<0>();
      break;
    case 0x01:
      update_freq_offset<4>(noise_int15);
      break;
    case 0x02:
      update_freq_offset<1>(noise_int15);
      update_pitch_current<1>();
      update_gate<1>();
      break;
    case 0x03:
      update_freq_offset<5>(noise_int15);
      break;
    case 0x04:
      update_freq_offset<2>(noise_int15);
      update_pitch_current<2>();
      update_gate<2>();
      break;
    case 0x05:
      update_freq_offset<6>(noise_int15);
      break;
    case 0x06:
      update_freq_offset<3>(noise_int15);
      update_pitch_current<3>();
      update_gate<3>();
      break;
    case 0x07:
      update_freq_offset<7>(noise_int15);
      update_osc1_shape_control_effective();
      break;
    }
  }

  INLINE void process(int16_t noise_int15, int16_t output[4]) {
#if 1
    if (m_mono_mode == false) {
      output[0] = process_osc<0>(noise_int15, true) >> 8;
      output[1] = process_osc<1>(noise_int15, true) >> 8;
      output[2] = process_osc<2>(noise_int15, true) >> 8;
      output[3] = process_osc<3>(noise_int15, true) >> 8;
    } else {
      output[0] = process_osc<0>(noise_int15, false) >> 7;
      output[1] = 0;
      output[2] = 0;
      output[3] = 0;
    }
#else
    output[1] = 0;
    output[2] = 0;
    output[3] = 0;
    output[4] = 0;
#endif
  }

private:
  INLINE const int16_t* get_wave_table(uint8_t waveform, uint8_t note_number) {
    const int16_t* result;
    if ((waveform == WAVEFORM_SAW) ||
        (waveform == WAVEFORM_1_PULSE)) {
      result = g_osc_saw_wave_tables[note_number - NOTE_NUMBER_MIN];
    } else if (waveform == WAVEFORM_TRIANGLE) {
      result = g_osc_triangle_wave_table;
    } else {     // WAVEFORM_SQUARE
      result = g_osc_pulse_wave_tables[note_number - NOTE_NUMBER_MIN];
    }
    return result;
  }

  INLINE int16_t get_wave_level(const int16_t* wave_table, uint32_t phase) {
    uint16_t phase16 = phase >> 8;

    uint16_t curr_index  = phase16 >> (16 - OSC_WAVE_TABLE_SAMPLES_BITS);
    uint16_t next_weight = phase16 & ((1 << (16 - OSC_WAVE_TABLE_SAMPLES_BITS)) - 1);
    int16_t curr_data   = wave_table[curr_index + 0];
    int16_t next_data   = wave_table[curr_index + 1];

    // lerp
    int16_t result = curr_data + (((next_data - curr_data) * next_weight) >> (16 - OSC_WAVE_TABLE_SAMPLES_BITS));

    return result;
  }

  template <uint8_t N>
  INLINE int32_t process_osc(int16_t noise_int15, bool halve_noise_level) {
    int32_t result = 0;

    int16_t osc1_gain = m_mix_table[(OSC_MIX_TABLE_LENGTH - 1) - m_osc2_mix];
    int16_t osc2_gain = m_mix_table[                             m_osc2_mix];

    m_phase[N] += m_freq[N];
    boolean new_period_osc1 = (m_phase[N] & 0x00FFFFFF) < m_freq[N];
    m_wave_table[N] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[N]) * (1 - new_period_osc1)));
    m_wave_table[N] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[N]) +
                                                       (reinterpret_cast<const uintptr_t>(m_wave_table_temp[N]) * new_period_osc1));
    int32_t wave_0 = get_wave_level(m_wave_table[N], m_phase[N]);
    result += (wave_0 * osc1_gain * m_osc_gain_effective[N]) >> 10;

    // For Pulse Wave (wave_3)
    uint32_t phase_3 = m_phase[N] + (m_osc1_shape_effective[N] << 8);
    boolean new_period_osc1_add = ((phase_3 + 0x00800000) & 0x00FFFFFF) < (m_freq[N] + 0x00010000); // crossing the middle of a saw wave
    m_wave_table[N + 8] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[N + 8]) * (1 - new_period_osc1_add)));
    m_wave_table[N + 8] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[N + 8]) +
                                                           (reinterpret_cast<const uintptr_t>(m_wave_table_temp[N]) * new_period_osc1_add));
    int16_t wave_3 = get_wave_level(m_wave_table[N + 8], phase_3);
    result += ((((wave_3 * osc1_gain * m_osc_gain_effective[N]) >> 10) * -m_osc1_morph_control) >> 6) * (m_waveform[0] == WAVEFORM_1_PULSE);

    if (m_mixer_noise_sub_osc_control >= 0) {
      // Sub Osc (wave_1)
      int16_t wave_1 = get_wave_level(g_osc_triangle_wave_table, m_phase[N] >> 1);
      result += (wave_1 * m_mixer_noise_sub_osc_control * m_osc_gain_effective[N]) >> 6;
    } else if (m_waveform[1] != WAVEFORM_2_NOISE) {
      // Noise (wave_1)
      int16_t wave_1 =  -OSC_WAVE_TABLE_AMPLITUDE
                       +(OSC_WAVE_TABLE_AMPLITUDE << 1) * (noise_int15 & 0x1);
      result += (wave_1 * -m_mixer_noise_sub_osc_control * m_osc_gain_effective[N]) >> (6 + halve_noise_level);
    }

    m_phase[N + 4] += m_freq[N + 4];
    boolean new_period_osc2 = (m_phase[N + 4] & 0x00FFFFFF) < m_freq[N + 4];
    m_wave_table[N + 4] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[N + 4]) * (1 - new_period_osc2)));
    m_wave_table[N + 4] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[N + 4]) +
                                                       (reinterpret_cast<const uintptr_t>(m_wave_table_temp[N + 4]) * new_period_osc2));
    if (m_waveform[1] != WAVEFORM_2_NOISE) {
      int16_t wave_2 = get_wave_level(m_wave_table[N + 4], m_phase[N + 4]);
      result += (wave_2 * osc2_gain * m_osc_gain_effective[N]) >> 10;
    } else {
      // Noise (wave_2)
      int16_t wave_2 =  -OSC_WAVE_TABLE_AMPLITUDE
                       +(OSC_WAVE_TABLE_AMPLITUDE << 1) * (noise_int15 & 0x1);
      result += (wave_2 * osc2_gain * m_osc_gain_effective[N]) >> (10 + halve_noise_level);
    }

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
    int16_t pitch_eg_amt;
    if (N >= 4) {
      pitch_eg_amt = m_pitch_eg_amt[1];
    } else {
      pitch_eg_amt = m_pitch_eg_amt[0];
    }
    uint16_t pitch_temp =  (64 << 8) + m_pitch_current[N & 0x03] + m_pitch_bend_normalized + ((pitch_eg_amt * eg_level) >> 10);

    uint8_t coarse = high_byte(pitch_temp);
    if (coarse < (NOTE_NUMBER_MIN + 64)) {
      pitch_temp = ((NOTE_NUMBER_MIN + 64) << 8);
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      pitch_temp = ((NOTE_NUMBER_MAX + 64) << 8);
    }

    if (N >= 4) {
      pitch_temp += (lfo_level * m_pitch_lfo_amt[1]) >> 9;
      pitch_temp += (m_osc2_pitch << 8) + m_osc2_detune + m_osc2_detune;
    } else {
      pitch_temp += (lfo_level * m_pitch_lfo_amt[0]) >> 9;
    }

    coarse = high_byte(pitch_temp);
    if (coarse < (NOTE_NUMBER_MIN + 64)) {
      pitch_temp = NOTE_NUMBER_MIN << 8;
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      pitch_temp = NOTE_NUMBER_MAX << 8;
    } else {
      pitch_temp -= (64 << 8);
    }

    pitch_temp += 128;  // For g_osc_tune_table[]


    coarse = high_byte(pitch_temp);
    m_freq_base[N] = g_osc_freq_table[coarse - NOTE_NUMBER_MIN];
    if (N >= 4) {
      m_wave_table_temp[N] = get_wave_table(m_waveform[1], coarse);
    } else {
      m_wave_table_temp[N] = get_wave_table(m_waveform[0], coarse);
    }


    uint8_t fine = low_byte(pitch_temp);
    int32_t offset =
      static_cast<int32_t>(m_freq_base[N] * g_osc_tune_table[fine >> (8 - OSC_TUNE_TABLE_STEPS_BITS)]) >>
      OSC_TUNE_DENOMINATOR_BITS;
    m_freq_base[N] += offset;
    m_freq[N] = m_freq_base[N] + m_freq_offset[N];
  }

  template <uint8_t N>
  INLINE void update_freq_offset(int16_t noise_int15) {
    m_freq_offset[N] = (noise_int15 >= 14336) << 7;
    m_freq[N] = m_freq_base[N] + m_freq_offset[N];
  }

  template <uint8_t N>
  INLINE void update_gate() {
    if (m_gate_enabled) {
      if (m_osc_on[N]) {
        const int8_t half_level = (m_osc_level >> 1) + 1;

        if (m_osc_gain_effective[N] >= (m_osc_level - half_level)) {
          m_osc_gain_effective[N] = m_osc_level;
        } else {
          m_osc_gain_effective[N] += half_level;
        }
      } else {
        const int8_t one_fourth_level = (m_osc_level >> 2) + 1;

        if (m_osc_gain_effective[N] <= one_fourth_level) {
          m_osc_gain_effective[N] = 0;
        } else {
          m_osc_gain_effective[N] -= one_fourth_level;
        }
      }
    } else {
      m_osc_gain_effective[N] = m_osc_level;
    }
  }

  INLINE void update_osc1_shape_control_effective() {
    m_osc1_shape_control_effective += (m_osc1_shape_control_effective < m_osc1_shape_control);
    m_osc1_shape_control_effective -= (m_osc1_shape_control_effective > m_osc1_shape_control);
  }

  template <uint8_t N>
  INLINE void update_osc1_shape(int16_t lfo_level, int16_t eg_level) {
    int32_t osc1_shape = 0x8000u - (m_osc1_shape_control_effective << 8)
                         + ((eg_level * m_shape_eg_amt) >> 5) - ((lfo_level * m_shape_lfo_amt) >> 3);

#if 0
    // osc1_shape = clamp(osc1_shape, 0x0, 0x10000)
    osc1_shape = osc1_shape - 0x10000;
    osc1_shape = (osc1_shape < 0) * osc1_shape + 0x10000;
    osc1_shape = (osc1_shape > 0) * osc1_shape;
#endif

    m_osc1_shape[N] = osc1_shape;
  }

  template <uint8_t N>
  INLINE void update_osc1_shape_effective() {
    if (m_osc1_shape_effective[N] + (1 << 8) < m_osc1_shape[N]) {
      m_osc1_shape_effective[N] += (1 << 8);
    } else if (m_osc1_shape_effective[N] > m_osc1_shape[N] + (1 << 8)) {
      m_osc1_shape_effective[N] -= (1 << 8);
    } else {
      m_osc1_shape_effective[N] = m_osc1_shape[N];
    }
  }

  INLINE void update_pitch_bend() {
    int16_t b = m_pitch_bend + 1;
    b >>= 3;
    m_pitch_bend_normalized = (b * m_pitch_bend_range) >> 2;
  }
};

const int16_t PRA32_U_Osc::m_pitch_mod_amt_table[128] = {
  -384, -384, -384, -384, -384, -384, -384, -384,
  -384, -384, -384, -368, -352, -336, -320, -304,
  -288, -272, -256, -240, -224, -208, -192, -176,
  -160, -144, -128, -112,  -96,  -80,  -64,  -48,
   -32,  -31,  -30,  -29,  -28,  -27,  -26,  -25,
   -24,  -23,  -22,  -21,  -20,  -19,  -18,  -17,
   -16,  -15,  -14,  -13,  -12,  -11,  -10,   -9,
    -8,   -7,   -6,   -5,   -4,   -3,   -2,   -1,
    +0,   +1,   +2,   +3,   +4,   +5,   +6,   +7,
    +8,   +9,  +10,  +11,  +12,  +13,  +14,  +15,
   +16,  +17,  +18,  +19,  +20,  +21,  +22,  +23,
   +24,  +25,  +26,  +27,  +28,  +29,  +30,  +31,
   +32,  +48,  +64,  +80,  +96, +112, +128, +144,
  +160, +176, +192, +208, +224, +240, +256, +272,
  +288, +304, +320, +336, +352, +368, +384, +384,
  +384, +384, +384, +384, +384, +384, +384, +384,
};
