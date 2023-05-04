#pragma once

#include "common.h"

template <uint8_t T>
class SerialIn {
public:
  INLINE static void open(uint16_t serial_speed) {
    UBRR0 = (1000000 / serial_speed) - 1;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  }

  INLINE static boolean available() {
    return UCSR0A & _BV(RXC0);
  }

  INLINE static int8_t read() {
    return UDR0;
  }
};
