// refs https://www.g200kg.com/archives/2012/10/adsr.html
// refs https://www.g200kg.com/archives/2012/10/adsr2.html
// refs https://www.g200kg.com/archives/2020/07/adsr-1.html

#pragma once

#include "pra32-u-common.h"

template <uint8_t T>
class EG {
  static const uint8_t STATE_ATTACK  = 0;
  static const uint8_t STATE_SUSTAIN = 1;
  static const uint8_t STATE_IDLE    = 2;

  static const uint8_t NO_DECAY_UPDATE_COEF = 255;

  static uint8_t  m_state;
  static uint16_t m_level;
  static int16_t  m_level_out;
  static uint8_t  m_attack_update_coef;
  static uint8_t  m_decay_update_coef;
  static uint16_t m_sustain;
  static uint8_t  m_release_update_coef;
  static uint8_t  m_rest;

public:
  INLINE static void initialize() {
    m_state = STATE_IDLE;
    set_attack(0);
    set_decay(0);
    set_sustain(127);
    set_release(0);
  }

  INLINE static void set_attack(uint8_t controller_value) {
    if (controller_value == 127) {
      m_attack_update_coef = 64;
    } else {
      m_attack_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE static void set_decay(uint8_t controller_value) {
    if (controller_value == 127) {
      m_decay_update_coef = NO_DECAY_UPDATE_COEF;
    } else {
      m_decay_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE static void set_sustain(uint8_t controller_value) {
    m_sustain = (((controller_value + 1) >> 1) << 1) << 8;
  }

  INLINE static void set_release(uint8_t controller_value) {
    if (controller_value == 127) {
      m_release_update_coef = 64;
    } else {
      m_release_update_coef = (controller_value + 3) >> 1;
    }
  }

  INLINE static void note_on() {
    m_state = STATE_ATTACK;
    m_rest = m_attack_update_coef;
  }

  INLINE static void note_off() {
    m_state = STATE_IDLE;
    m_rest = m_release_update_coef;
  }

  INLINE static int16_t process(uint8_t count) {
#if 1
    if ((count & (EG_CONTROL_INTERVAL - 1)) == ((T == 0) ? 3 : 11)) {
      //printf("%d EG\n", count);
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

template <uint8_t T> uint8_t  EG<T>::m_state;
template <uint8_t T> uint16_t EG<T>::m_level;
template <uint8_t T> int16_t  EG<T>::m_level_out;
template <uint8_t T> uint8_t  EG<T>::m_attack_update_coef;
template <uint8_t T> uint8_t  EG<T>::m_decay_update_coef;
template <uint8_t T> uint16_t EG<T>::m_sustain;
template <uint8_t T> uint8_t  EG<T>::m_release_update_coef;
template <uint8_t T> uint8_t  EG<T>::m_rest;

