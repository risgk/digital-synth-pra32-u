// refs http://playground.arduino.cc/Code/PCMAudio

#pragma once

#include "common.h"

template <uint8_t T>
class AudioOut {
  static uint8_t m_count;
  static uint8_t m_busy;
  static uint8_t m_busyContinuousCount;

public:
  INLINE static void open() {
#if defined(ENABLE_16_BIT_OUTPUT)
    pinMode(L_MONO_AUDIO_OUT_PIN,     OUTPUT);
    pinMode(L_MONO_LOW_AUDIO_OUT_PIN, OUTPUT);
    pinMode(R_AUDIO_OUT_PIN,          OUTPUT);
    pinMode(R_LOW_AUDIO_OUT_PIN,      OUTPUT);
    pinMode(CPU_BUSY_LED_OUT_PIN,     OUTPUT);

    // Timer/Counter0 (8-bit Fast PWM, Inverting, 62500 Hz)
    OCR0A  = 0x7F;
    TCCR0A = 0xF3;

    // Timer/Counter2 (8-bit Fast PWM, Inverting, 62500 Hz)
    OCR2A  = 0x7F;
    TCCR2A = 0xF3;
#else
    pinMode(L_MONO_AUDIO_OUT_PIN, OUTPUT);
    pinMode(R_AUDIO_OUT_PIN,      OUTPUT);
    pinMode(CPU_BUSY_LED_OUT_PIN, OUTPUT);

    // Timer/Counter0 (8-bit Fast PWM, Inverting, 62500 Hz)
  #if (L_MONO_AUDIO_OUT_PIN == 5)
    OCR0B  = 0x7F;
    TCCR0A = 0x33;
  #elif (L_MONO_AUDIO_OUT_PIN == 6)
    OCR0A  = 0x7F;
    TCCR0A = 0xC3;
  #endif

    // Timer/Counter2 (8-bit Fast PWM, Inverting, 62500 Hz)
    OCR2A  = 0x7F;
    TCCR2A = 0xC3;
#endif

    // Timer/Counter1 (9-bit Fast PWM, 31250 Hz)
    TCCR1A = 0x02;

    // Start 0 -> 2 -> 1
    TCCR0B = 0x01;
    TCCR2B = 0x01;
    TCCR1B = 0x09;

    m_count = 0;
    m_busy = 0;
    m_busyContinuousCount = 0;
  }

#if defined(ENABLE_16_BIT_OUTPUT)
  INLINE static void write(int16_t left, int16_t right) {
    uint16_t left_u16  = 0x8000 + left;
    uint16_t right_u16 = 0x8000 + right;
#else
  INLINE static void write(int8_t left, int8_t right) {
    uint8_t l = 0x7F - left;
    uint8_t r = 0x7F - right;
#endif

#if defined(DEBUG)
    // Output Elapsed Time as Channel Pressure (of Channel 16)
    ++m_count;
    if (m_count == 0x7F) {
      UDR0 = 0xDF;
    } else if (m_count == 0xFF) {
      uint8_t tcnt = TCNT1 >> 2;
      UDR0 = tcnt;
      m_count = 0;
    }
#endif

    if (TIFR1 & _BV(TOV1)) {
      // CPU BUSY
      if (m_busy) {
        ++m_busyContinuousCount;
      }
      m_busy = 1;
    } else {
      m_busy = 0;
      m_busyContinuousCount = 0;
      while ((TIFR1 & _BV(TOV1)) == 0);
    }
    TIFR1 = _BV(TOV1);

#if defined(ENABLE_16_BIT_OUTPUT)
  #if (L_MONO_AUDIO_OUT_PIN == 5)
    OCR0B = high_byte(left_u16);
    OCR0A = low_byte(left_u16);
    OCR2A = high_byte(right_u16);
    OCR2B = low_byte(right_u16);
  #elif (L_MONO_AUDIO_OUT_PIN == 6)
    OCR0A = high_byte(left_u16);
    OCR0B = low_byte(left_u16);
    OCR2A = high_byte(right_u16);
    OCR2B = low_byte(right_u16);
  #endif
#else
  #if (L_MONO_AUDIO_OUT_PIN == 5)
    OCR0B = l;
    OCR2A = r;
  #elif (L_MONO_AUDIO_OUT_PIN == 6)
    OCR0A = l;
    OCR2A = r;
  #endif
#endif

    if (m_busyContinuousCount >= 18) {
      PORTB = _BV(5);    // Turn on CPU Busy LED
    } else {
      PORTB = 0x00;      // Turn off CPU Busy LED
    }

  }
};

template <uint8_t T> uint8_t AudioOut<T>::m_count;
template <uint8_t T> uint8_t AudioOut<T>::m_busy;
template <uint8_t T> uint8_t AudioOut<T>::m_busyContinuousCount;
