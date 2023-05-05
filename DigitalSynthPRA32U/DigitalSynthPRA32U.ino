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

#define I2S_DATA_PIN  (9)
#define I2S_BCLK_PIN  (10) // I2S_LRCLK_PIN is (I2S_BCLK_PIN + 1)

void setup() {
}

void __not_in_flash_func(loop)() {
}

void setup1() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);

  Synth<0>::initialize();
  SerialIn<0>::open(SERIAL_SPEED);
//  AudioOut<0>::open();

  i2s.setDATA(I2S_DATA_PIN);
  i2s.setBCLK(I2S_BCLK_PIN);
  i2s.setBitsPerSample(16);
  i2s.setBuffers(3, 8);
  i2s.begin(SAMPLING_RATE);
}

void __not_in_flash_func(loop1)() {
  if (SerialIn<0>::available()) {
    digitalWrite(LED_BUILTIN, 0);

    uint8_t b = SerialIn<0>::read();
    Synth<0>::receive_midi_byte(b);
  }
  int16_t right_level;
  int16_t left_level = Synth<0>::process(right_level);
//  AudioOut<0>::write(left_level, right_level);

  i2s.write(left_level);
  i2s.write(right_level);
}
