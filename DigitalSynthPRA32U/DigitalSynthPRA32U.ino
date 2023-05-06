/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Digital Synth PRA32-U
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define DEBUG

#define SERIAL_SPEED (38400)            // For Serial MIDI
//#define SERIAL_SPEED (31250)            // For MIDI Shield (MIDI Breakout)

#define L_MONO_AUDIO_OUT_PIN (5)        // Pin D5 (or D6)
#define R_AUDIO_OUT_PIN      (11)       // Pin D11 (Fixed)
#define CPU_BUSY_LED_OUT_PIN (13)       // Pin D13 (Fixed)

#define L_MONO_LOW_AUDIO_OUT_PIN (6)    // Pin D6 (or D5): L/Mono channel, low 8-bit audio output
#define R_LOW_AUDIO_OUT_PIN      (3)    // Pin D3 (Fixed): R      channel, low 8-bit audio output

#define ENABLE_SPECIAL_PROGRAM_CHANGE   // Program Change by Control Change (112-119)
                                        // Interpret Program Change 8-15 as 0-7



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

    int32_t buffer[I2S_BUFFER_WORDS];
    for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
      int16_t right_level;
      int16_t left_level = Synth<0>::process(right_level);
      buffer[i] = (left_level << 16) + right_level;
    }

    debug_process_end_us = micros();

    for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
      i2s.write(buffer[i], true);
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
