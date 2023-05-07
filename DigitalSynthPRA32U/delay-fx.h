#pragma once

#include "common.h"

template <uint8_t T>
class DelayFx {
  static const uint16_t DELAY_BUFF_SIZE = 512;

  static int16_t  m_buff[DELAY_BUFF_SIZE];
  static uint16_t m_wp;

public:
  INLINE static void initialize() {
    m_wp = DELAY_BUFF_SIZE - 1;
  }

  INLINE static void push(int16_t audio_input) {
    m_wp = (m_wp + 1) & (DELAY_BUFF_SIZE - 1);
    m_buff[m_wp] = audio_input;
  }

  INLINE static int16_t get(uint16_t sample_delay) {
    uint16_t rp = (m_wp - sample_delay) & (DELAY_BUFF_SIZE - 1);
    return m_buff[rp];
  }

  INLINE static void attenuate() {
    for (uint16_t i = 0; i < DELAY_BUFF_SIZE; ++i) {
      m_buff[i] = m_buff[i] >> 1;
    }
  }
};

template <uint8_t T> int16_t  DelayFx<T>::m_buff[DELAY_BUFF_SIZE];
template <uint8_t T> uint16_t DelayFx<T>::m_wp;
