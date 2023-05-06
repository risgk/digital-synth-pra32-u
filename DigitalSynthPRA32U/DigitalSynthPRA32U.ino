/*
 * Digital Synth PRA32-U
 * - We strongly recommend Raspberry Pi Pico/RP2040 (by Earle F. Philhower, III) version 3.1.1
 */

#define DEBUG

//#define USE_USB_MIDI
#define USE_SERIAL_MIDI

#define SERIAL_MIDI_SPEED   (38400)
//#define SERIAL_MIDI_SPEED   (31250)

#define MIDI_CH             (0)

#define I2S_DATA_PIN        (9)
#define I2S_BCLK_PIN        (10) // I2S_LRCLK_PIN is I2S_BCLK_PIN + 1
#define I2S_BITS_PER_SAMPLE (16) // 16, 24, or 32
#define I2S_BUFFERS         (3)
#define I2S_BUFFER_WORDS    (8)



#include "common.h"
#include "synth.h"
#include <I2S.h>

I2S g_i2s_output(OUTPUT);

void __not_in_flash_func(setup)() {
}

void __not_in_flash_func(loop)() {
}

void __not_in_flash_func(setup1)() {
  pinMode(LED_BUILTIN, OUTPUT);

  Synth<0>::initialize();

#if defined(USE_SERIAL_MIDI)
  Serial1.setTX(PIN_SERIAL1_TX);
  Serial1.setRX(PIN_SERIAL1_RX);
  Serial1.begin(SERIAL_MIDI_SPEED);
#endif

//  AudioOut<0>::open();

  g_i2s_output.setDATA(I2S_DATA_PIN);
  g_i2s_output.setBCLK(I2S_BCLK_PIN);
  g_i2s_output.setBitsPerSample(I2S_BITS_PER_SAMPLE);
  g_i2s_output.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS);
  g_i2s_output.setFrequency(SAMPLING_RATE);
  g_i2s_output.begin();

#if defined(DEBUG)
  Serial.begin(0);
#endif
}

void __not_in_flash_func(loop1)() {

#if defined(USE_SERIAL_MIDI)
  int32_t b = Serial1.read();
  if (b >= 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    Synth<0>::receive_midi_byte(b);
    digitalWrite(LED_BUILTIN, LOW);
  }
#endif

#if defined(DEBUG)
  uint32_t debug_measurement_start_us = micros();
#endif

  int16_t left_buffer[I2S_BUFFER_WORDS];
  int16_t right_buffer[I2S_BUFFER_WORDS];
  for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
    left_buffer[i] = Synth<0>::process(right_buffer[i]);
  }

#if defined(DEBUG)
  uint32_t debug_measurement_end_us = micros();
#endif

  for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
#if   (I2S_BITS_PER_SAMPLE == 16)
    g_i2s_output.write16(left_buffer[i], right_buffer[i]);
#elif (I2S_BITS_PER_SAMPLE == 24)
    g_i2s_output.write24(left_buffer[i] << 16, right_buffer[i] << 16);
#elif (I2S_BITS_PER_SAMPLE == 32)
    g_i2s_output.write32(left_buffer[i] << 16, right_buffer[i] << 16);
#endif
  }

#if defined(DEBUG)
  static uint32_t s_debug_measurement_max_us = 0;
  uint32_t debug_measurement_elapsed_us = debug_measurement_end_us - debug_measurement_start_us;
  if (s_debug_measurement_max_us < debug_measurement_elapsed_us) {
    s_debug_measurement_max_us = debug_measurement_elapsed_us;
  }

  static uint16_t s_debug_loop_counter = 0;
  if (++s_debug_loop_counter == 0) {
    Serial.println(debug_measurement_elapsed_us);
    Serial.println(s_debug_measurement_max_us);
    Serial.println();
  }
#endif
}
