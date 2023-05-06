/*
 * Digital Synth PRA32-U
 * - We strongly recommend Raspberry Pi Pico/RP2040 (by Earle F. Philhower, III) version 3.1.1
 */

//#define DEBUG

#define SERIAL_SPEED (38400)            // For Serial MIDI
//#define SERIAL_SPEED (31250)            // For MIDI



#include "common.h"
#include "audio-out.h"
#include "synth.h"
#include "serial-in.h"

#include <I2S.h>

I2S i2s(OUTPUT);

#define I2S_DATA_PIN        (9)
#define I2S_BCLK_PIN        (10) // I2S_LRCLK_PIN is (I2S_BCLK_PIN + 1)
#define I2S_BITS_PER_SAMPLE (16)
#define I2S_BUFFERS         (3)
#define I2S_BUFFER_WORDS    (8)

void __not_in_flash_func(setup)() {
}

void __not_in_flash_func(loop)() {
}

void __not_in_flash_func(setup1)() {
  pinMode(LED_BUILTIN, OUTPUT);

  Synth<0>::initialize();

  Serial1.setTX(PIN_SERIAL1_TX);
  Serial1.setRX(PIN_SERIAL1_RX);
  Serial1.begin(SERIAL_SPEED);

//  AudioOut<0>::open();

  i2s.setDATA(I2S_DATA_PIN);
  i2s.setBCLK(I2S_BCLK_PIN);
  i2s.setBitsPerSample(I2S_BITS_PER_SAMPLE);
  i2s.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS);
  i2s.setFrequency(SAMPLING_RATE);
  i2s.begin();

  Serial.begin(0);
}

void __not_in_flash_func(loop1)() {
  uint32_t debug_loop_start_us;
  uint32_t debug_loop_end_us;
  uint32_t debug_process_start_us;
  uint32_t debug_process_end_us;

  debug_loop_start_us = micros();
  {
    int32_t b = Serial1.read();
    if (b >= 0) {
      digitalWrite(LED_BUILTIN, HIGH);

      Synth<0>::receive_midi_byte(b);

      digitalWrite(LED_BUILTIN, LOW);
    }

    debug_process_start_us = micros();

    int16_t left_buffer[I2S_BUFFER_WORDS];
    int16_t right_buffer[I2S_BUFFER_WORDS];
    for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
      left_buffer[i] = Synth<0>::process(right_buffer[i]);
    }

    debug_process_end_us = micros();

    for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
#if (I2S_BITS_PER_SAMPLE == 8)
      i2s.write8(left_buffer[i] >> 8, right_buffer[i] >> 8);
#elif  (I2S_BITS_PER_SAMPLE == 16)
      i2s.write16(left_buffer[i], right_buffer[i]);
#elif  (I2S_BITS_PER_SAMPLE == 24)
      i2s.write24(left_buffer[i] << 16, right_buffer[i] << 16);
#elif  (I2S_BITS_PER_SAMPLE == 32)
      i2s.write32(left_buffer[i] << 16, right_buffer[i] << 16);
#endif
    }
  }
  debug_loop_end_us = micros();

  uint32_t debug_loop_elapsed_us = debug_loop_end_us - debug_loop_start_us;
  static uint32_t s_debug_loop_max_us = 0;
  if (s_debug_loop_max_us < debug_loop_elapsed_us) {
    s_debug_loop_max_us = debug_loop_elapsed_us;
  }

  uint32_t debug_process_elapsed_us = debug_process_end_us - debug_process_start_us;
  static uint32_t s_debug_process_max_us = 0;
  if (s_debug_process_max_us < debug_process_elapsed_us) {
    s_debug_process_max_us = debug_process_elapsed_us;
  }

  static uint16_t s_debug_loop_counter = 0;
  if (++s_debug_loop_counter == 0) {
    Serial.println(debug_loop_elapsed_us);
    Serial.println(s_debug_loop_max_us);
    Serial.println(debug_process_elapsed_us);
    Serial.println(s_debug_process_max_us);
    Serial.println();
  }
}
