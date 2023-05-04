#pragma once

#include "common.h"

template <uint8_t T>
class AudioOut {
  static uint8_t m_count;
  static uint8_t m_busy;
  static uint8_t m_busyContinuousCount;

public:
  INLINE static void open() {
  }

  INLINE static void write(int16_t left, int16_t right) {
    static_cast<void>(left);
    static_cast<void>(right);
  }
};

template <uint8_t T> uint8_t AudioOut<T>::m_count;
template <uint8_t T> uint8_t AudioOut<T>::m_busy;
template <uint8_t T> uint8_t AudioOut<T>::m_busyContinuousCount;
