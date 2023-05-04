#pragma once

#include "common.h"

template <uint8_t T>
class SerialIn {
public:
  INLINE static void open(uint16_t serial_speed) {
    Serial.begin(serial_speed);
  }

  INLINE static boolean available() {
    return Serial.available();
  }

  INLINE static uint8_t read() {
    return Serial.read();
  }
};
