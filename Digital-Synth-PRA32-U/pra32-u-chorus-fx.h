#pragma once

#include "pra32-u-common.h"

class PRA32_U_ChorusFx {
  static const uint16_t DELAY_BUFF_SIZE = 512;

  int16_t  m_buff[DELAY_BUFF_SIZE];
  uint16_t m_wp;

public:
  PRA32_U_ChorusFx()
  : m_buff()
  , m_wp()
  {}

  INLINE void initialize() {
    m_wp = DELAY_BUFF_SIZE - 1;
  }

  INLINE void control() {
  }

  INLINE int16_t process(int16_t audio_input) {
    return audio_input;
  }

//private:
  INLINE void push(int16_t audio_input) {
    m_wp = (m_wp + 1) & (DELAY_BUFF_SIZE - 1);
    m_buff[m_wp] = audio_input;
  }

  INLINE int16_t get(uint16_t sample_delay) {
    uint16_t rp = (m_wp - sample_delay) & (DELAY_BUFF_SIZE - 1);
    return m_buff[rp];
  }

  INLINE void attenuate() {
    for (uint16_t i = 0; i < DELAY_BUFF_SIZE; ++i) {
      m_buff[i] = m_buff[i] >> 1;
    }
  }
};
