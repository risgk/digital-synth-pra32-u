// refs https://www.g200kg.com/archives/2012/10/adsr.html
// refs https://www.g200kg.com/archives/2012/10/adsr2.html
// refs https://www.g200kg.com/archives/2020/07/adsr-1.html

#pragma once

#include "pra32-u-common.h"

class PRA32_U_EG {
  static const uint8_t STATE_ATTACK  = 0;
  static const uint8_t STATE_SUSTAIN = 1;
  static const uint8_t STATE_IDLE    = 2;

  static const uint8_t NO_DECAY_UPDATE_COEF = 255;

  uint8_t  m_state;
  uint16_t m_level;
  int16_t  m_level_out;
  uint8_t  m_attack_update_coef;
  uint8_t  m_decay_update_coef;
  uint16_t m_sustain;
  uint8_t  m_release_update_coef;
  uint8_t  m_rest;

public:
  PRA32_U_EG()
  : m_state()
  , m_level()
  , m_level_out()
  , m_attack_update_coef()
  , m_decay_update_coef()
  , m_sustain()
  , m_release_update_coef()
  , m_rest()
  {}

  INLINE void initialize() {
    m_state = STATE_IDLE;
    set_attack(0);
    set_decay(0);
    set_sustain(127);
    set_release(0);
  }

  INLINE void set_attack(uint8_t controller_value) {
    if (controller_value == 127) {
      m_attack_update_coef = 64;
    } else {
      m_attack_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE void set_decay(uint8_t controller_value) {
    if (controller_value == 127) {
      m_decay_update_coef = NO_DECAY_UPDATE_COEF;
    } else {
      m_decay_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE void set_sustain(uint8_t controller_value) {
    m_sustain = (((controller_value + 1) >> 1) << 1) << 8;
  }

  INLINE void set_release(uint8_t controller_value) {
    if (controller_value == 127) {
      m_release_update_coef = 64;
    } else {
      m_release_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE void note_on() {
    m_state = STATE_ATTACK;
    m_rest = m_attack_update_coef;
  }

  INLINE void note_off() {
    m_state = STATE_IDLE;
    m_rest = m_release_update_coef;
  }

  INLINE int16_t process(uint8_t id, uint8_t count) {
#if 1
    if ((count & (EG_CONTROL_INTERVAL - 1)) == ((id == 0) ? 3 : 11)) {
      //printf("%d PRA32_U_EG\n", count);
      switch (m_state) {
      case STATE_ATTACK:
        --m_rest;
        if (m_rest == 0) {
          m_rest = m_attack_update_coef;

          uint8_t coef;
          coef = 188 + m_attack_update_coef;

          m_level = EG_LEVEL_MAX_X_1_5 - (((EG_LEVEL_MAX_X_1_5 - m_level) * coef) >> 8);
          if (m_level >= EG_LEVEL_MAX) {
            m_level = EG_LEVEL_MAX;
            m_state = STATE_SUSTAIN;
            m_rest = m_decay_update_coef;
          }
        }
        break;

      case STATE_SUSTAIN:
        --m_rest;
        if (m_rest == 0) {
          m_rest = m_decay_update_coef;

          if ((m_level > m_sustain) && (m_decay_update_coef != NO_DECAY_UPDATE_COEF)) {
            uint8_t coef;
            coef = 188 + m_decay_update_coef;

            m_level = m_sustain + (((m_level - m_sustain) * coef) >> 8);
            if (m_level < m_sustain) {
              m_level = m_sustain;
            }
          }
        }
        break;

      case STATE_IDLE:
        --m_rest;
        if (m_rest == 0) {
          m_rest = m_release_update_coef;

          if (m_level > 0) {
            uint8_t coef;
            coef = 188 + m_release_update_coef;

            m_level = (m_level * coef) >> 8;
          }
        }
        break;
      }

      m_level_out = m_level >> 1;
    }
#endif

    return m_level_out;
  }
};
