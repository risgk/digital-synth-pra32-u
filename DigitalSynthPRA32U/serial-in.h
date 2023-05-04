#pragma once

#include "common.h"

template <uint8_t T>
class SerialIn {
public:
  INLINE static void open(uint16_t serial_speed) {
    Serial1.setTX(PIN_SERIAL1_TX);
    Serial1.setRX(PIN_SERIAL1_RX);
    Serial1.begin(serial_speed);
  }

  INLINE static boolean available() {
    return Serial1.available();
  }

  INLINE static uint8_t read() {
    return Serial1.read();
  }
};
