#pragma once

#include "pra32-u-common.h"
#include "pra32-u-osc-table.h"
#include <math.h>


class PRA32_U_Osc {
  static const uint8_t OSC_MIX_TABLE_LENGTH   = 65;

  static const uint8_t PORTAMENTO_COEF_BASE   = 190;

  static const uint8_t WAVEFORM_SAW           = 0;
  static const uint8_t WAVEFORM_SQUARE        = 1;
  static const uint8_t WAVEFORM_TRIANGLE      = 2;
  static const uint8_t WAVEFORM_SINE          = 3;
  static const uint8_t WAVEFORM_1_PULSE       = 4;
  static const uint8_t WAVEFORM_2_NOISE       = 5;

  uint32_t       m_portamento_coef[4];
  int16_t        m_pitch_eg_amt[2];
  int16_t        m_pitch_lfo_amt[2];

  uint8_t        m_waveform[2];
  int16_t        m_pitch_bend;
  uint8_t        m_pitch_bend_range;
  int16_t        m_pitch_bend_normalized;
  uint32_t       m_pitch_target[4];
  uint32_t       m_pitch_current[4];
  const int16_t* m_wave_table[4 * 4];
  const int16_t* m_wave_table_temp[4 * 3];
  uint32_t       m_freq[4 * 2];
  uint32_t       m_freq_base[4 * 2];
  int16_t        m_freq_offset[4 * 2];
  uint32_t       m_phase[4 * 2];
  boolean        m_osc_on[4];
  int8_t         m_osc_gain_effective[4];
  int8_t         m_osc_level;

  boolean        m_gate_enabled;
  uint8_t        m_mixer_osc_mix_control;
  uint8_t        m_mixer_osc_mix_control_effective;
  int8_t         m_osc2_pitch;
  int16_t        m_osc2_detune;

  uint8_t        m_phase_high;
  uint8_t        m_osc1_shape_control;
  uint8_t        m_osc1_shape_control_effective;
  uint8_t        m_osc1_morph_control;
  uint8_t        m_osc1_morph_control_effective;
  int32_t        m_osc1_shape[4];
  int32_t        m_osc1_shape_effective[4];
  uint16_t       m_osc1_phase_modulation_depth[4];
  uint16_t       m_osc1_phase_modulation_frequency_ratio[4];
  int8_t         m_mixer_noise_sub_osc_control;
  int8_t         m_mixer_noise_sub_osc_control_effective;
  int16_t        m_mix_table[OSC_MIX_TABLE_LENGTH];
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
  , m_wave_table()
  , m_wave_table_temp()
  , m_freq()
  , m_freq_base()
  , m_freq_offset()
  , m_phase()
  , m_osc_on()
  , m_osc_gain_effective()
  , m_osc_level()

  , m_gate_enabled()
  , m_mixer_osc_mix_control()
  , m_mixer_osc_mix_control_effective()
  , m_osc2_pitch()
  , m_osc2_detune()

  , m_phase_high()
  , m_osc1_shape_control()
  , m_osc1_shape_control_effective()
  , m_osc1_morph_control()
  , m_osc1_morph_control_effective()
  , m_osc1_shape()
  , m_osc1_shape_effective()
  , m_osc1_phase_modulation_depth()
  , m_osc1_phase_modulation_frequency_ratio()
  , m_mixer_noise_sub_osc_control()
  , m_mixer_noise_sub_osc_control_effective()
  , m_mix_table()
  , m_shape_eg_amt()
  , m_shape_lfo_amt()
  {
    m_portamento_coef[0] = 0;
    m_portamento_coef[1] = 0;
    m_portamento_coef[2] = 0;
    m_portamento_coef[3] = 0;

    set_gate_enabled (false);
    set_mixer_osc_mix(0);
    set_osc2_pitch   (0);
    set_osc2_detune  (0);

    m_waveform[0] = WAVEFORM_SAW;
    m_waveform[1] = WAVEFORM_SAW;
    m_pitch_target[0] = 60 << 24;
    m_pitch_target[1] = 60 << 24;
    m_pitch_target[2] = 60 << 24;
    m_pitch_target[3] = 60 << 24;
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
    m_wave_table[12] = g_osc_saw_wave_tables[0];
    m_wave_table[13] = g_osc_saw_wave_tables[0];
    m_wave_table[14] = g_osc_saw_wave_tables[0];
    m_wave_table[15] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[0] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[1] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[2] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[3] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[4] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[5] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[6] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[7] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[8] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[9] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[10] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[11] = g_osc_saw_wave_tables[0];
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

    m_osc1_shape[0]           = 0;
    m_osc1_shape[1]           = 0;
    m_osc1_shape[2]           = 0;
    m_osc1_shape[3]           = 0;
    m_osc1_shape_effective[0] = 0;
    m_osc1_shape_effective[1] = 0;
    m_osc1_shape_effective[2] = 0;
    m_osc1_shape_effective[3] = 0;

    for (uint8_t i = 0; i < OSC_MIX_TABLE_LENGTH; ++i) {
      m_mix_table[i] = static_cast<int16_t>(sqrtf(static_cast<float>(i) /
                                            (OSC_MIX_TABLE_LENGTH - 1)) * (1 << 10));
    }

    set_pitch_bend_range(2);
  }

  template <uint8_t N>
  INLINE void set_osc_waveform(uint8_t controller_value) {
    static uint8_t waveform_tables[2][6] = {
      {
        WAVEFORM_SAW,
        WAVEFORM_SINE,
        WAVEFORM_TRIANGLE,
        WAVEFORM_TRIANGLE,
        WAVEFORM_1_PULSE,
        WAVEFORM_1_PULSE,
      },
      {
        WAVEFORM_SAW,
        WAVEFORM_SINE,
        WAVEFORM_TRIANGLE,
        WAVEFORM_TRIANGLE,
        WAVEFORM_2_NOISE,
        WAVEFORM_SQUARE,
      },
    };

    volatile int32_t index = ((controller_value * 10) + 127) / 254;

    // index = min(index, 5)
    index = index - 5;
    index = (index < 0) * index + 5;

    m_waveform[N] = waveform_tables[N][index];
#if 1
    if (controller_value < 6) {
      m_waveform[N] = waveform_tables[N][controller_value];
    }
#endif
  }

  INLINE void set_osc1_shape_control(uint8_t controller_value) {
    m_osc1_shape_control = controller_value;
  }

  INLINE void set_osc1_morph_control(uint8_t controller_value) {
    m_osc1_morph_control = controller_value;
  }

  INLINE void set_mixer_sub_osc_control(uint8_t controller_value) {
    m_mixer_noise_sub_osc_control = (((controller_value - 63) >> 1) << 1);
  }

  INLINE int16_t get_pitch_mod_amt_table(uint8_t controller_value) {
    static int16_t pitch_mod_amt_table[128] = {
      -7680, -7680, -7680, -7680, -7424, -7168, -6912, -6656,
      -6400, -6144, -5888, -5632, -5376, -5120, -4864, -4608,
      -4352, -4096, -3840, -3584, -3328, -3072, -2816, -2560,
      -2304, -2048, -1792, -1536, -1280, -1024,  -768,  -512,
       -256,  -248,  -240,  -232,  -224,  -216,  -208,  -200,
       -192,  -184,  -176,  -168,  -160,  -152,  -144,  -136,
       -128,  -120,  -112,  -104,   -96,   -88,   -80,   -72,
        -64,   -56,   -48,   -40,   -32,   -24,   -16,    -8,
         +0,    +8,   +16,   +24,   +32,   +40,   +48,   +56,
        +64,   +72,   +80,   +88,   +96,  +104,  +112,  +120,
       +128,  +136,  +144,  +152,  +160,  +168,  +176,  +184,
       +192,  +200,  +208,  +216,  +224,  +232,  +240,  +248,
       +256,  +512,  +768, +1024, +1280, +1536, +1792, +2048,
      +2304, +2560, +2816, +3072, +3328, +3584, +3840, +4096,
      +4352, +4608, +4864, +5120, +5376, +5632, +5888, +6144,
      +6400, +6656, +6912, +7168, +7424, +7680, +7680, +7680,
    };

    return pitch_mod_amt_table[controller_value];
  }

  template <uint8_t N>
  INLINE void set_pitch_eg_amt(uint8_t controller_value) {
    m_pitch_eg_amt[N] = get_pitch_mod_amt_table(controller_value);
  }

  INLINE void set_shape_eg_amt(uint8_t controller_value) {
    if (controller_value == 0) {
      controller_value = 1;
    }
    m_shape_eg_amt = ((controller_value - 64) << 1);
  }

  template <uint8_t N>
  INLINE void set_pitch_lfo_amt(uint8_t controller_value) {
    m_pitch_lfo_amt[N] = get_pitch_mod_amt_table(controller_value);
  }

  INLINE void set_shape_lfo_amt(uint8_t controller_value) {
    if (controller_value == 0) {
      controller_value = 1;
    }
    m_shape_lfo_amt = -((controller_value - 64) << 1);
  }

  INLINE void set_gate_enabled(boolean gate_enabled) {
    m_gate_enabled = gate_enabled;
  }

  INLINE void set_mixer_osc_mix(uint8_t controller_value) {
    m_mixer_osc_mix_control = ((controller_value + 1) >> 1) << 1;
  }

  INLINE void set_osc2_pitch(uint8_t controller_value) {
    if (controller_value < 4) {
      m_osc2_pitch = -60;
    } else if (controller_value < 124) {
      m_osc2_pitch = controller_value - 64;
    } else {
      m_osc2_pitch = 60;
    }
  }

  INLINE void set_osc2_detune(uint8_t controller_value) {
    static int16_t m_osc2_detune_table[128] = {
      -3072, -3072, -3072, -3072, -3072, -3072, -3072, -3072,
      -3072, -3072, -2944, -2816, -2688, -2560, -2432, -2304,
      -2176, -2048, -1920, -1792, -1664, -1536, -1408, -1280,
      -1152, -1024,  -896,  -768,  -640,  -512,  -384,  -256,
       -128,  -124,  -120,  -116,  -112,  -108,  -104,  -100,
        -96,   -92,   -88,   -84,   -80,   -76,   -72,   -68,
        -64,   -60,   -56,   -52,   -48,   -44,   -40,   -36,
        -32,   -28,   -24,   -20,   -16,   -12,    -8,    -4,
         +0,    +4,    +8,   +12,   +16,   +20,   +24,   +28,
        +32,   +36,   +40,   +44,   +48,   +52,   +56,   +60,
        +64,   +68,   +72,   +76,   +80,   +84,   +88,   +92,
        +96,  +100,  +104,  +108,  +112,  +116,  +120,  +124,
       +128,  +256,  +384,  +512,  +640,  +768,  +896, +1024,
      +1152, +1280, +1408, +1536, +1664, +1792, +1920, +2048,
      +2176, +2304, +2432, +2560, +2688, +2816, +2944, +3072,
      +3072, +3072, +3072, +3072, +3072, +3072, +3072, +3072,
    };

    m_osc2_detune = m_osc2_detune_table[controller_value];
  }

  template <uint8_t N>
  INLINE void set_portamento(uint8_t controller_value) {
    m_portamento_coef[N] = g_portamento_coef_table[controller_value];
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

    m_pitch_target[N] = (n << 24);
    if (m_portamento_coef[N] == 0) {
      m_pitch_current[N] = m_pitch_target[N];
    }
    m_osc_on[N] = true;
  }

  template <uint8_t N>
  INLINE void note_off() {
    m_osc_on[N] = false;
  }

  INLINE void set_pitch_bend_range(uint8_t controller_value) {
    uint8_t range = controller_value;
    if (range > 64) {
      range = 64;
    }
    m_pitch_bend_range = range;
    update_pitch_bend();
  }

  INLINE void set_pitch_bend(int16_t pitch_bend) {
    m_pitch_bend = pitch_bend;
    update_pitch_bend();
  }

  INLINE uint16_t get_osc_pitch(uint8_t index) {
    uint16_t shifted_pitch = (64 << 8) + (m_pitch_current[index] >> 16) + m_pitch_bend_normalized;
    uint16_t osc_pitch;
    if (shifted_pitch > (64 << 8) + (NOTE_NUMBER_MAX << 8)) {
      osc_pitch = (NOTE_NUMBER_MAX << 8);
    } else if (shifted_pitch < (64 << 8) + (NOTE_NUMBER_MIN << 8)) {
      osc_pitch = (NOTE_NUMBER_MIN << 8);
    } else {
      osc_pitch = (m_pitch_current[index] >> 16) + m_pitch_bend_normalized;
    }
    return osc_pitch;
  }

  template <uint8_t N>
  INLINE void process_at_low_rate_a(int16_t lfo_level, int16_t eg_level) {
    update_pitch_current<N>();
    update_osc1_shape<N>(lfo_level, eg_level);
    update_osc1_shape_effective<N>();
    update_freq_base<N + 0>(lfo_level, eg_level);
    update_freq_base<N + 4>(lfo_level, eg_level);
  }

  INLINE void process_at_low_rate_b(uint8_t count, int16_t noise_int15) {
    switch (count & (0x08 - 1)) {
    case 0x00:
      update_freq_offset<0>(noise_int15);
      update_gate<0>();
      break;
    case 0x01:
      update_freq_offset<4>(noise_int15);
      break;
    case 0x02:
      update_freq_offset<1>(noise_int15);
      update_gate<1>();
      break;
    case 0x03:
      update_freq_offset<5>(noise_int15);
      update_mixer_control_effective();
      break;
    case 0x04:
      update_freq_offset<2>(noise_int15);
      update_gate<2>();
      break;
    case 0x05:
      update_freq_offset<6>(noise_int15);
      break;
    case 0x06:
      update_freq_offset<3>(noise_int15);
      update_gate<3>();
      break;
    case 0x07:
      update_freq_offset<7>(noise_int15);
      update_osc1_control_effective();
      break;
    }
  }

  template <uint8_t N>
  INLINE int16_t process(int16_t noise_int15) {
#if 1
    return process_osc<N>(noise_int15) >> 8;
#else
    return = 0;
#endif
  }

private:
  INLINE const int16_t* get_wave_table(uint8_t waveform, uint8_t note_number) {
    static int16_t** wave_table_table[6] = {
      g_osc_saw_wave_tables,       // WAVEFORM_SAW           = 0
      g_osc_square_wave_tables,    // WAVEFORM_SQUARE        = 1
      g_osc_triangle_wave_tables,  // WAVEFORM_TRIANGLE      = 2
      g_osc_sine_wave_tables,      // WAVEFORM_SINE          = 3
      g_osc_saw_wave_tables,       // WAVEFORM_1_PULSE       = 4
      g_osc_square_wave_tables,    // WAVEFORM_2_NOISE       = 5
    };

    return wave_table_table[waveform][note_number - NOTE_NUMBER_MIN];
  }

  INLINE int16_t get_wave_level(const int16_t* wave_table, uint32_t phase) {
    uint16_t phase16     = phase >> 8;
    uint16_t curr_index  = phase16 >> (16 - OSC_WAVE_TABLE_SAMPLES_BITS);
    uint16_t next_weight = phase16 & ((1 << (16 - OSC_WAVE_TABLE_SAMPLES_BITS)) - 1);
    int16_t  curr_data   = wave_table[curr_index + 0];
    int16_t  next_data   = wave_table[curr_index + 1];
    int16_t  level       = curr_data + (((next_data - curr_data) * next_weight) >> (16 - OSC_WAVE_TABLE_SAMPLES_BITS)); // lerp
    return level;
  }

  template <uint8_t N>
  INLINE int32_t process_osc(int16_t noise_int15) {
    int32_t result = 0;

    int16_t osc1_gain = m_mix_table[(OSC_MIX_TABLE_LENGTH - 1) - (m_mixer_osc_mix_control_effective >> 1)];
    int16_t osc2_gain = m_mix_table[                             (m_mixer_osc_mix_control_effective >> 1)];

    m_phase[N] += m_freq[N];
    boolean new_period_osc1 = (m_phase[N] & 0x00FFFFFF) < m_freq[N]; // crossing the begin of a osc 1 wave, the begin or the middle of a sub osc wave
    m_wave_table[N]      = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[N]) * (1 - new_period_osc1)));
    m_wave_table[N]      = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[N]) +
                                                            (reinterpret_cast<const uintptr_t>(m_wave_table_temp[N]) * new_period_osc1));
    m_wave_table[N + 12] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[N + 12]) * (1 - new_period_osc1)));
    m_wave_table[N + 12] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[N + 12]) +
                                                            (reinterpret_cast<const uintptr_t>(m_wave_table_temp[N + 8]) * new_period_osc1));
    if (m_waveform[0] == WAVEFORM_SINE) {
      // For Sine Wave (wave_3)

      // phase_modulation_depth_candidate = max(m_osc1_shape_effective[N] - (128 << 8), 0)
      volatile int32_t phase_modulation_depth_candidate = m_osc1_shape_effective[N] - (128 << 8);
      phase_modulation_depth_candidate = (phase_modulation_depth_candidate > 0) * phase_modulation_depth_candidate;

      m_osc1_phase_modulation_depth[N] = phase_modulation_depth_candidate;

      volatile int32_t phase_modulation_frequency_ratio_candidate = (((m_osc1_morph_control_effective + 2) >> 2) << 1) + 2;
      m_osc1_phase_modulation_frequency_ratio[N] = (m_osc1_phase_modulation_frequency_ratio[N] * (1 - new_period_osc1)) + (phase_modulation_frequency_ratio_candidate * new_period_osc1);

      uint32_t phase_3 = (((m_phase[N] >> 1) & 0x01FFFFFF) * m_osc1_phase_modulation_frequency_ratio[N]) >> 1;
      const int16_t* wave_table_sine = get_wave_table(WAVEFORM_SINE, 60);
      int16_t wave_3 = get_wave_level(wave_table_sine, phase_3);

      uint32_t phase_0 = m_phase[N] + ((wave_3 * m_osc1_phase_modulation_depth[N]) >> 4);
      int32_t wave_0 = get_wave_level(wave_table_sine, phase_0);
      result += (wave_0 * osc1_gain * m_osc_gain_effective[N]) >> 10;
    } else {
      int32_t wave_0 = get_wave_level(m_wave_table[N], m_phase[N]);
      result += (wave_0 * osc1_gain * m_osc_gain_effective[N]) >> 10;

      // For Pulse Wave (wave_3)
      uint32_t phase_3 = m_phase[N] + (m_osc1_shape_effective[N] << 8);
      boolean new_period_osc1_add = ((phase_3 + 0x00800000) & 0x00FFFFFF) < (m_freq[N] + 0x00010000); // crossing the middle of a saw wave
      m_wave_table[N + 8] = reinterpret_cast<const int16_t*>((reinterpret_cast<const uintptr_t>(m_wave_table[N + 8]) * (1 - new_period_osc1_add)));
      m_wave_table[N + 8] = reinterpret_cast<const int16_t*>( reinterpret_cast<const uint8_t*>( m_wave_table[N + 8]) +
                                                             (reinterpret_cast<const uintptr_t>(m_wave_table_temp[N]) * new_period_osc1_add));
      int16_t wave_3 = get_wave_level(m_wave_table[N + 8], phase_3);
      result += ((((wave_3 * osc1_gain * m_osc_gain_effective[N]) >> 10) * (((m_osc1_morph_control_effective - 63) >> 1) << 1)) >> 6) * (m_waveform[0] == WAVEFORM_1_PULSE);
    }

    if (m_mixer_noise_sub_osc_control_effective >= 0) {
      // Sub Osc (wave_1)
      int16_t wave_1 = get_wave_level(m_wave_table[N + 12], m_phase[N] >> 1);
      result += (wave_1 * m_mixer_noise_sub_osc_control * m_osc_gain_effective[N]) >> 6;
    } else {
      // Noise (wave_1)
      int16_t wave_1 = noise_int15 >> 1;
      result += (wave_1 * -m_mixer_noise_sub_osc_control_effective * m_osc_gain_effective[N]) >> 6;
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
      int16_t wave_2 = noise_int15 >> 1;
      result += (wave_2 * osc2_gain * m_osc_gain_effective[N]) >> 10;
    }

    return result;
  }

  template <uint8_t N>
  INLINE void update_pitch_current() {
    if (m_osc_on[N]) {
      if (m_pitch_current[N] <= m_pitch_target[N]) {
        m_pitch_current[N] = m_pitch_target[N]  - mul_s32_s32_h32((m_pitch_target[N] - m_pitch_current[N]) << 2,             m_portamento_coef[N]);
      } else {
        m_pitch_current[N] = m_pitch_current[N] + mul_s32_s32_h32((m_pitch_target[N] - m_pitch_current[N]) << 2, (1 << 30) - m_portamento_coef[N]);
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
    uint16_t pitch_temp =  (64 << 8) + (m_pitch_current[N & 0x03] >> 16) + m_pitch_bend_normalized + ((eg_level * pitch_eg_amt) >> 14);

    uint8_t coarse = high_byte(pitch_temp);
    if (coarse < (NOTE_NUMBER_MIN + 64)) {
      pitch_temp = ((NOTE_NUMBER_MIN + 64) << 8);
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      pitch_temp = ((NOTE_NUMBER_MAX + 64) << 8);
    }

    if (N >= 4) {
      pitch_temp += (lfo_level * m_pitch_lfo_amt[1]) >> 14;
      pitch_temp += (m_osc2_pitch << 8) + m_osc2_detune;
    } else {
      pitch_temp += (lfo_level * m_pitch_lfo_amt[0]) >> 14;
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
      m_wave_table_temp[N]     = get_wave_table(m_waveform[1], coarse);
    } else {
      m_wave_table_temp[N]     = get_wave_table(m_waveform[0], coarse);

      // coarse_sub = max((coarse - 12), NOTE_NUMBER_MIN)
      volatile int32_t coarse_sub = (coarse - 12) - NOTE_NUMBER_MIN;
      coarse_sub = (coarse_sub > 0) * coarse_sub + NOTE_NUMBER_MIN;

      m_wave_table_temp[N + 8] = get_wave_table(WAVEFORM_SINE, coarse_sub);
    }


    uint8_t fine = low_byte(pitch_temp);
    int32_t offset =
      ((static_cast<int32_t>((m_freq_base[N] >> 1) * g_osc_tune_table[fine >> (8 - OSC_TUNE_TABLE_STEPS_BITS)]) >>
        OSC_TUNE_DENOMINATOR_BITS) >> 0) << 1;
    m_freq_base[N] += offset;
    m_freq[N] = m_freq_base[N] + m_freq_offset[N];
  }

  template <uint8_t N>
  INLINE void update_freq_offset(int16_t noise_int15) {
    static_cast<void>(noise_int15);
    m_freq_offset[N] = (N >> 2) << 1;
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

  INLINE void update_osc1_control_effective() {
    m_osc1_shape_control_effective += (m_osc1_shape_control_effective < m_osc1_shape_control);
    m_osc1_shape_control_effective -= (m_osc1_shape_control_effective > m_osc1_shape_control);

    m_osc1_morph_control_effective += (m_osc1_morph_control_effective < m_osc1_morph_control);
    m_osc1_morph_control_effective -= (m_osc1_morph_control_effective > m_osc1_morph_control);
  }

  INLINE void update_mixer_control_effective() {
    m_mixer_osc_mix_control_effective       += (m_mixer_osc_mix_control_effective < m_mixer_osc_mix_control);
    m_mixer_osc_mix_control_effective       -= (m_mixer_osc_mix_control_effective > m_mixer_osc_mix_control);

    m_mixer_noise_sub_osc_control_effective += (m_mixer_noise_sub_osc_control_effective < m_mixer_noise_sub_osc_control);
    m_mixer_noise_sub_osc_control_effective -= (m_mixer_noise_sub_osc_control_effective > m_mixer_noise_sub_osc_control);
  }

  template <uint8_t N>
  INLINE void update_osc1_shape(int16_t lfo_level, int16_t eg_level) {
    volatile int32_t osc1_shape = (128 << 8) + (m_osc1_shape_control_effective << 8)
                                  + ((eg_level * m_shape_eg_amt) >> 5) - ((lfo_level * m_shape_lfo_amt) >> 5);

    // osc1_shape = clamp(y_0, (0 << 8), (256 << 8))
    osc1_shape = osc1_shape - (256 << 8);
    osc1_shape = (osc1_shape < 0) * osc1_shape + (256 << 8) - (0 << 8);
    osc1_shape = (osc1_shape > 0) * osc1_shape + (0 << 8);

    m_osc1_shape[N] = osc1_shape;
  }

  template <uint8_t N>
  INLINE void update_osc1_shape_effective() {
    // effective_new = clamp(m_osc1_shape[N], (m_osc1_shape_effective[N] - 0x0100), (m_osc1_shape_effective[N] + 0x0100))
    volatile int32_t effective_new = m_osc1_shape[N] - (m_osc1_shape_effective[N] + 0x0100);
    effective_new = (effective_new < 0) * effective_new + (m_osc1_shape_effective[N] + 0x0100) - (m_osc1_shape_effective[N] - 0x0100);
    effective_new = (effective_new > 0) * effective_new + (m_osc1_shape_effective[N] - 0x0100);
    m_osc1_shape_effective[N] = effective_new;
  }

  INLINE void update_pitch_bend() {
    int16_t b = m_pitch_bend + 1;
    b >>= 3;
    m_pitch_bend_normalized = (b * m_pitch_bend_range) >> 2;
  }
};
