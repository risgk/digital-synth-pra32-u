#pragma once

#include "pra32-u-common.h"
#include "pra32-u-lfo-table.h"

class PRA32_U_LFO {
  static const uint8_t LFO_WAVEFORM_TRI_ASYNC = 0;
  static const uint8_t LFO_WAVEFORM_TRI_SYNC  = 1;
  static const uint8_t LFO_WAVEFORM_SAW_DOWN  = 2;
  static const uint8_t LFO_WAVEFORM_RANDOM    = 3;
  static const uint8_t LFO_WAVEFORM_SQUARE    = 4;

  static const uint8_t LFO_FADE_COEF_OFF      = 1 << 4;

  static const uint8_t LFO_FADE_LEVEL_MAX     = 128;

  uint32_t m_lfo_phase;
  int16_t  m_lfo_wave_level;
  int16_t  m_lfo_level;
  uint32_t m_lfo_rate;
  uint8_t  m_lfo_depth[2];
  uint8_t  m_lfo_waveform;
  uint8_t  m_lfo_fade_coef;
  uint8_t  m_lfo_fade_cnt;
  uint8_t  m_lfo_fade_level;
  int16_t  m_noise_int15;
  int16_t  m_sampled_noise_int15;

public:
  PRA32_U_LFO()
  : m_lfo_phase()
  , m_lfo_wave_level()
  , m_lfo_level()
  , m_lfo_rate()
  , m_lfo_depth()
  , m_lfo_waveform()
  , m_lfo_fade_coef()
  , m_lfo_fade_cnt()
  , m_lfo_fade_level()
  , m_noise_int15()
  , m_sampled_noise_int15()
  {
    initialize();
  }

  INLINE void initialize() {
    m_lfo_waveform = LFO_WAVEFORM_TRI_ASYNC;
    m_lfo_fade_coef = LFO_FADE_COEF_OFF;
    m_lfo_fade_cnt = m_lfo_fade_coef;
    m_lfo_fade_level = LFO_FADE_LEVEL_MAX;
    m_noise_int15 = 0;
    m_sampled_noise_int15 = m_noise_int15;
  }

  INLINE void set_lfo_waveform(uint8_t controller_value) {
    if        (controller_value < 16) {
      m_lfo_waveform = LFO_WAVEFORM_TRI_ASYNC;
    } else if (controller_value < 48) {
      m_lfo_waveform = LFO_WAVEFORM_TRI_SYNC;
    } else if (controller_value < 80) {
      m_lfo_waveform = LFO_WAVEFORM_SAW_DOWN;
    } else if (controller_value < 112) {
      m_lfo_waveform = LFO_WAVEFORM_RANDOM;
    } else {
      m_lfo_waveform = LFO_WAVEFORM_SQUARE;
    }
  }

  INLINE void set_lfo_rate(uint8_t controller_value) {
    m_lfo_rate = g_lfo_rate_table[(controller_value + 1) >> 1];
  }

  template <uint8_t N>
  INLINE void set_lfo_depth(uint8_t controller_value) {
    m_lfo_depth[N] = (controller_value + 1) >> 1;
  }

  INLINE void set_lfo_fade_time(uint8_t controller_value) {
    m_lfo_fade_coef = ((controller_value * controller_value) >> 4) + LFO_FADE_COEF_OFF;
  }

  INLINE void trigger_lfo() {
    if (m_lfo_waveform != LFO_WAVEFORM_TRI_ASYNC) {
      m_lfo_phase = 0x00FFFFFF;
    }

    if (m_lfo_fade_coef > LFO_FADE_COEF_OFF) {
      m_lfo_fade_level = 0;
    }
  }

  INLINE int16_t get_output() {
    return m_lfo_level;
  }

  INLINE void process_at_low_rate(uint8_t count, int16_t noise_int15) {
    static_cast<void>(count);

    m_noise_int15 = noise_int15;
    update_lfo_level();
  }

private:
  INLINE int16_t get_lfo_wave_level(uint32_t phase) {
    int16_t level = 0;

    switch (m_lfo_waveform) {
    case LFO_WAVEFORM_TRI_ASYNC:
    case LFO_WAVEFORM_TRI_SYNC:
      level = static_cast<int16_t>(phase >> 8) >> 1;
      if (level < -(64 << 7)) {
        level = -(64 << 7) - (level + (64 << 7));
      } else if (level < (64 << 7)) {
        // do nothing
      } else {
        level = (64 << 7) - (level - (64 << 7));
      }
      break;
    case LFO_WAVEFORM_SAW_DOWN:
      {
        level = (64 << 7) - ((phase >> 8) >> 2);
      }
      break;
    case LFO_WAVEFORM_RANDOM:
      if (phase < m_lfo_rate) {
        m_sampled_noise_int15 = m_noise_int15;
      }
      level = m_sampled_noise_int15 >> 1;
      break;
    case LFO_WAVEFORM_SQUARE:
      if (phase < 0x008000) {
        level = 128 << 7;
      } else {
        level = 0;
      }
      break;
    }

    return level;
  }

  INLINE void update_lfo_level() {
    --m_lfo_fade_cnt;
    if (m_lfo_fade_cnt == 0) {
      m_lfo_fade_cnt = m_lfo_fade_coef;
      if (m_lfo_fade_level < LFO_FADE_LEVEL_MAX) {
        m_lfo_fade_level += 2;
      }
    }

    m_lfo_phase += m_lfo_rate;
    m_lfo_phase &= 0x00FFFFFF;
    m_lfo_wave_level = get_lfo_wave_level(m_lfo_phase);


    uint8_t lfo_depth = high_byte((m_lfo_depth[0] << 1) * m_lfo_fade_level) + m_lfo_depth[1];
    if (lfo_depth > 64) {
      lfo_depth = 64;
    }
    lfo_depth <<= 1;

    m_lfo_level = (lfo_depth * m_lfo_wave_level) >> 9;
  }
};
