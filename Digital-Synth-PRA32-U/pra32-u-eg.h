// refs https://www.g200kg.com/archives/2012/10/adsr.html
// refs https://www.g200kg.com/archives/2012/10/adsr2.html
// refs https://www.g200kg.com/archives/2020/07/adsr-1.html

#pragma once

#include "pra32-u-common.h"
#include "pra32-u-eg-table.h"

class PRA32_U_EG {
  static const uint8_t STATE_ATTACK  = 0;
  static const uint8_t STATE_SUSTAIN = 1;
  static const uint8_t STATE_IDLE    = 2;

  uint8_t m_state;
  int32_t m_level;
  int16_t m_level_out;
  int32_t m_attack_coef;
  int32_t m_decay_coef;
  int32_t m_sustain;
  int32_t m_release_coef;

public:
  PRA32_U_EG()
  : m_state()
  , m_level()
  , m_level_out()
  , m_attack_coef()
  , m_decay_coef()
  , m_sustain()
  , m_release_coef()
  {
    m_state = STATE_IDLE;
    set_attack(0);
    set_decay(0);
    set_sustain(127);
    set_release(0);
  }

  INLINE void set_attack(uint8_t controller_value) {
    m_attack_coef = g_eg_attack_release_coef_table[controller_value + 16];
  }

  INLINE void set_decay(uint8_t controller_value) {
    m_decay_coef = g_eg_decay_coef_table[controller_value];
  }

  INLINE void set_sustain(uint8_t controller_value) {
    m_sustain = ((controller_value + 1) >> 1) << 24;
  }

  INLINE void set_release(uint8_t controller_value) {
    m_release_coef = g_eg_attack_release_coef_table[controller_value];
  }

  INLINE void set_velocity_sensitivity(uint8_t controller_value) {
    // todo
  }

  INLINE void note_on() {
    m_state = STATE_ATTACK;
  }

  INLINE void note_off() {
    m_state = STATE_IDLE;
  }

  INLINE int16_t get_output() {
    return m_level_out;
  }

  INLINE void process_at_low_rate() {
#if 1
    switch (m_state) {
    case STATE_ATTACK:
      m_level = EG_LEVEL_MAX_2 - (mul_s32_s32_h32((EG_LEVEL_MAX_2 - m_level), m_attack_coef) << 2);
      if (m_level >= EG_LEVEL_MAX) {
        m_level = EG_LEVEL_MAX;
        m_state = STATE_SUSTAIN;
      }
      break;

    case STATE_SUSTAIN:
      {
        // effective_sustain = min(effective_sustain, m_level)
        int32_t effective_sustain = m_sustain - m_level;
        effective_sustain = (effective_sustain < 0) * effective_sustain + m_level;

        m_level = effective_sustain + (mul_s32_s32_h32((m_level - effective_sustain), m_decay_coef) << 2);
      }
      break;

    case STATE_IDLE:
      m_level = mul_s32_s32_h32(m_level, m_release_coef) << 2;
      break;
    }

    m_level_out = m_level >> 16;
#endif
  }
};
