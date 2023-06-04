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

  uint8_t  m_state;
  uint32_t m_level;
  int16_t  m_level_out;
  int16_t  m_attack_coef;
  int16_t  m_decay_coef;
  uint32_t m_sustain;
  int16_t  m_release_coef;

public:
  PRA32_U_EG()
  : m_state()
  , m_level()
  , m_level_out()
  , m_attack_coef()
  , m_decay_coef()
  , m_sustain()
  , m_release_coef()
  {}

  INLINE void initialize() {
    m_state = STATE_IDLE;
    set_attack(0);
    set_decay(0);
    set_sustain(127);
    set_release(0);
  }

  INLINE void set_attack(uint8_t controller_value) {
    m_attack_coef = g_eg_attack_release_coef_table[(controller_value + 1) >> 1];
  }

  INLINE void set_decay(uint8_t controller_value) {
    m_decay_coef = g_eg_decay_coef_table[(controller_value + 1) >> 1];
  }

  INLINE void set_sustain(uint8_t controller_value) {
    m_sustain = (((controller_value + 1) >> 1) << 1) << 24;
  }

  INLINE void set_release(uint8_t controller_value) {
    m_release_coef = g_eg_attack_release_coef_table[(controller_value + 1) >> 1];
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
      m_level = EG_LEVEL_MAX_X_1_5 - mul_u32_u16_h32((EG_LEVEL_MAX_X_1_5 - m_level) << 1, m_attack_coef);
      // todo
      if (m_level >= EG_LEVEL_MAX) {
        m_level = EG_LEVEL_MAX;
        m_state = STATE_SUSTAIN;
      }
      break;

    case STATE_SUSTAIN:
      if (m_level > m_sustain) {
        m_level = m_sustain + mul_u32_u16_h32((m_level - m_sustain) << 1, m_decay_coef);
        if (m_level < m_sustain) {
          m_level = m_sustain;
        }
      }
      break;

    case STATE_IDLE:
      m_level = mul_u32_u16_h32(m_level << 1, m_release_coef);
      break;
    }

    m_level_out = m_level >> 16;
#endif
  }
};
