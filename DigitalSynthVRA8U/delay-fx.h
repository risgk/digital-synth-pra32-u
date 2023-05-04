#pragma once

#include "common.h"

template <uint8_t T>
class DelayFx {
#if defined(ENABLE_16_BIT_OUTPUT)
  static const uint16_t DELAY_BUFF_SIZE = 256;

  static int16_t  m_buff[DELAY_BUFF_SIZE];
#else
  static const uint16_t DELAY_BUFF_SIZE = 512;

  static int8_t   m_buff[DELAY_BUFF_SIZE];
#endif
  static uint16_t m_wp;

public:
  INLINE static void initialize() {
    m_wp = DELAY_BUFF_SIZE - 1;
  }

#if defined(ENABLE_16_BIT_OUTPUT)
  INLINE static void push(int16_t audio_input) {
#else
  INLINE static void push(int8_t audio_input) {
#endif
    m_wp = (m_wp + 1) & (DELAY_BUFF_SIZE - 1);
    m_buff[m_wp] = audio_input;
  }

#if defined(ENABLE_16_BIT_OUTPUT)
  INLINE static int16_t get(uint16_t sample_delay) {
#else
  INLINE static int8_t get(uint16_t sample_delay) {
#endif
    uint16_t rp = (m_wp - sample_delay) & (DELAY_BUFF_SIZE - 1);
    return m_buff[rp];
  }

  INLINE static void attenuate() {
    for (uint16_t i = 0; i < DELAY_BUFF_SIZE; ++i) {
      m_buff[i] = m_buff[i] >> 1;
    }
  }
};

#if defined(ENABLE_16_BIT_OUTPUT)
template <uint8_t T> int16_t  DelayFx<T>::m_buff[DELAY_BUFF_SIZE];
#else
template <uint8_t T> int8_t   DelayFx<T>::m_buff[DELAY_BUFF_SIZE];
#endif
template <uint8_t T> uint16_t DelayFx<T>::m_wp;
