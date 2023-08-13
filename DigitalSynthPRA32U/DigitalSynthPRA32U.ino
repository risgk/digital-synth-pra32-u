/*
 * Digital Synth PRA32-U
 *
 * - An I2S DAC hardware (e.g. Pico Audio Pack) is required
 * - Raspberry Pi Pico/RP2040 core version 3.2.0 is recommended
 */

//#define DEBUG_PRINT

#define USE_USB_MIDI      // Select USB Stack: "Adafruit TinuUSB" in the Arduino IDE "Tools" menu
//#define USE_SERIAL1_MIDI

#define SERIAL1_MIDI_SPEED           (31250)
//#define SERIAL1_MIDI_SPEED           (38400)

#define MIDI_CH                      (0)  // 0-based

#define I2S_DAC_MUTE_OFF_PIN         (22)

#define I2S_DATA_PIN                 (9)
#define I2S_BCLK_PIN                 (10)  // I2S_LRCLK_PIN is I2S_BCLK_PIN + 1
#define I2S_SWAP_BCLK_AND_LRCLK_PINS (false)

#define I2S_BITS_PER_SAMPLE          (16)  // 16, 24, or 32
#define I2S_BUFFERS                  (3)
#define I2S_BUFFER_WORDS             (8)

////////////////////////////////////////////////////////////////

#include "common.h"
#include "synth.h"

#include <MIDI.h>
#if defined(USE_USB_MIDI)
#include <Adafruit_TinyUSB.h>
Adafruit_USBD_MIDI usbd_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbd_midi, MIDI);
#elif defined(USE_SERIAL1_MIDI)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);
#endif

#include <I2S.h>

I2S g_i2s_output(OUTPUT);

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte number, byte value);
void handleHandleProgramChange(byte channel, byte number);
void handleHandlePitchBend(byte channel, int bend);

void __not_in_flash_func(setup)() {
}

void __not_in_flash_func(loop)() {
}

void __not_in_flash_func(setup1)() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Synth<0>::initialize();

#if defined(USE_USB_MIDI)
  TinyUSB_Device_Init(0);
#endif
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleProgramChange(handleHandleProgramChange);
  MIDI.setHandlePitchBend(handleHandlePitchBend);
  MIDI.begin(MIDI_CHANNEL_OMNI);
#if defined(USE_SERIAL1_MIDI)
  Serial1.begin(SERIAL1_MIDI_SPEED);
#endif

  g_i2s_output.setDATA(I2S_DATA_PIN);
  g_i2s_output.setBCLK(I2S_BCLK_PIN);
  g_i2s_output.setBitsPerSample(I2S_BITS_PER_SAMPLE);
  g_i2s_output.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS);
  g_i2s_output.setFrequency(SAMPLING_RATE);
  if (I2S_SWAP_BCLK_AND_LRCLK_PINS) {
    g_i2s_output.swapClocks();
  }
  g_i2s_output.begin();

#if defined(I2S_DAC_MUTE_OFF_PIN)
  pinMode(I2S_DAC_MUTE_OFF_PIN, OUTPUT);
  digitalWrite(I2S_DAC_MUTE_OFF_PIN, HIGH);
#endif

#if defined(DEBUG_PRINT)
#if defined(USE_SERIAL1_MIDI)
  Serial.begin(0);  // Select USB Stack: "Pico SDK" in the Arduino IDE "Tools" menu
#else
  Serial1.begin(115200);
#endif
#endif
}

void __not_in_flash_func(loop1)() {

//#if defined(DEBUG_PRINT)
//  uint32_t debug_measurement_start_us = micros();
//#endif

  MIDI.read();

#if defined(DEBUG_PRINT)
  uint32_t debug_measurement_start_us = micros();
#endif

  int16_t left_buffer[I2S_BUFFER_WORDS];
  int16_t right_buffer[I2S_BUFFER_WORDS];
  for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
    left_buffer[i] = Synth<0>::process(right_buffer[i]);
  }

#if defined(DEBUG_PRINT)
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

#if defined(DEBUG_PRINT)
  static uint32_t s_debug_measurement_max_us = 0;
  uint32_t debug_measurement_elapsed_us = debug_measurement_end_us - debug_measurement_start_us;
  if (s_debug_measurement_max_us < debug_measurement_elapsed_us) {
    s_debug_measurement_max_us = debug_measurement_elapsed_us;
  }

  static uint32_t s_debug_loop_counter = 0;
  if (++s_debug_loop_counter == 4000) {
    s_debug_loop_counter = 0;
#if defined(USE_SERIAL1_MIDI)
    Serial.println(debug_measurement_elapsed_us);
    Serial.println(s_debug_measurement_max_us);
    Serial.println();
#else
    Serial1.println(debug_measurement_elapsed_us);
    Serial1.println(s_debug_measurement_max_us);
    Serial1.println();
#endif
  }
#endif
}

void __not_in_flash_func(handleNoteOn)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == MIDI_CH) {
    if (velocity > 0) {
      Synth<0>::note_on(pitch, velocity);
    } else {
      Synth<0>::note_off(pitch);
    }
  }
}

void __not_in_flash_func(handleNoteOff)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == MIDI_CH) {
    (void) velocity;
    Synth<0>::note_off(pitch);
  }
}

void __not_in_flash_func(handleControlChange)(byte channel, byte number, byte value)
{
  if ((channel - 1) == MIDI_CH) {
    Synth<0>::control_change(number, value);
  }
}

void __not_in_flash_func(handleHandleProgramChange)(byte channel, byte number)
{
  if ((channel - 1) == MIDI_CH) {
    Synth<0>::program_change(number);
  }
}

void __not_in_flash_func(handleHandlePitchBend)(byte channel, int bend)
{
  if ((channel - 1) == MIDI_CH) {
    g_synth.pitch_bend((bend + 8192) & 0x7F, (bend + 8192) >> 7);
  }
}
