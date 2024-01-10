/*
 * Digital Synth PRA32-U
 */

//#define DEBUG_PRINT

#define USE_USB_MIDI      // Select USB Stack: "Adafruit TinyUSB" in the Arduino IDE "Tools" menu

//#define USE_SERIAL1_MIDI

#define SERIAL1_MIDI_SPEED              (31250)
//#define SERIAL1_MIDI_SPEED              (38400)

#define SERIAL1_TX_PIN                  (0)
#define SERIAL1_RX_PIN                  (1)

#define MIDI_CH                         (0)  // 0-based

// for Pimoroni Pico Audio Pack [PIM544]
#define I2S_DAC_MUTE_OFF_PIN            (22)
#define I2S_DATA_PIN                    (9)
//#define I2S_MCLK_PIN                    (0)
//#define I2S_MCLK_MULT                   (0)
#define I2S_BCLK_PIN                    (10)  // I2S_LRCLK_PIN is I2S_BCLK_PIN + 1
#define I2S_SWAP_BCLK_AND_LRCLK_PINS    (false)

#define I2S_BUFFERS                     (4)
#define I2S_BUFFER_WORDS                (64)

//#define USE_PWM_AUDIO_INSTEAD_OF_I2S

// for Pimoroni Pico VGA Demo Base (PIM553)
#define PWM_AUDIO_L_PIN                 (28)
#define PWM_AUDIO_R_PIN                 (27)

#define USE_2_CORES_FOR_SIGNAL_PROCESSING

#define USE_EMULATED_EEPROM

////////////////////////////////////////////////////////////////

#include "pra32-u-common.h"
#include "pra32-u-synth.h"

#define RT6150_PS_PIN                   (2)

PRA32_U_Synth g_synth;

#include <MIDI.h>
#if defined(USE_USB_MIDI)
#include <Adafruit_TinyUSB.h>
Adafruit_USBD_MIDI usbd_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbd_midi, USB_MIDI);
#endif // defined(USE_USB_MIDI)

#if defined(USE_SERIAL1_MIDI)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, SERIAL1_MIDI);
#endif

#if defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
#include <PWMAudio.h>
PWMAudio g_pwm_l(PWM_AUDIO_L_PIN);
PWMAudio g_pwm_r(PWM_AUDIO_R_PIN);
#endif // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)

#include <I2S.h>
I2S g_i2s_output(OUTPUT);

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte number, byte value);
void handleHandleProgramChange(byte channel, byte number);
void handleHandlePitchBend(byte channel, int bend);

void __not_in_flash_func(setup1)() {
}

void __not_in_flash_func(loop1)() {
  g_synth.secondary_core_process();
}

void __not_in_flash_func(setup)() {
  g_i2s_output.setSysClk(SAMPLING_RATE);
#if defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
#if ((PWM_AUDIO_L_PIN + 1) == PWM_AUDIO_R_PIN) && ((PWM_AUDIO_L_PIN % 2) == 0)
  g_pwm_l.setStereo(true);
  g_pwm_l.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS);
  g_pwm_l.setFrequency(SAMPLING_RATE);
  g_pwm_l.begin();
#elif ((PWM_AUDIO_R_PIN + 1) == PWM_AUDIO_L_PIN) && ((PWM_AUDIO_R_PIN % 2) == 0)
  g_pwm_r.setStereo(true);
  g_pwm_r.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS);
  g_pwm_r.setFrequency(SAMPLING_RATE);
  g_pwm_r.begin();
#else
  g_pwm_l.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS / 2);
  g_pwm_r.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS / 2);
  g_pwm_l.setFrequency(SAMPLING_RATE);
  g_pwm_r.setFrequency(SAMPLING_RATE);
  g_pwm_l.begin();
  g_pwm_r.begin();
#endif
#else // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
  g_i2s_output.setFrequency(SAMPLING_RATE);
  g_i2s_output.setDATA(I2S_DATA_PIN);
#if defined(I2S_MCLK_PIN)
  g_i2s_output.setMCLK(I2S_MCLK_PIN);
  g_i2s_output.setMCLKmult(I2S_MCLK_MULT);
#endif // defined(I2S_MCLK_PIN)
  g_i2s_output.setBCLK(I2S_BCLK_PIN);
  if (I2S_SWAP_BCLK_AND_LRCLK_PINS) {
    g_i2s_output.swapClocks();
  }
  g_i2s_output.setBitsPerSample(16);
  g_i2s_output.setBuffers(I2S_BUFFERS, I2S_BUFFER_WORDS);
  g_i2s_output.begin();
#endif // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)

#if defined(USE_USB_MIDI)
  TinyUSB_Device_Init(0);
  USBDevice.setManufacturerDescriptor("ISGK Instruments");
  USBDevice.setProductDescriptor("Digital Synth PRA32-U");
  USB_MIDI.setHandleNoteOn(handleNoteOn);
  USB_MIDI.setHandleNoteOff(handleNoteOff);
  USB_MIDI.setHandleControlChange(handleControlChange);
  USB_MIDI.setHandleProgramChange(handleHandleProgramChange);
  USB_MIDI.setHandlePitchBend(handleHandlePitchBend);
  USB_MIDI.begin(MIDI_CHANNEL_OMNI);
  USB_MIDI.turnThruOff();
#endif // defined(USE_USB_MIDI)

#if defined(USE_SERIAL1_MIDI)
  Serial1.setTX(SERIAL1_TX_PIN);
  Serial1.setRX(SERIAL1_RX_PIN);
  SERIAL1_MIDI.setHandleNoteOn(handleNoteOn);
  SERIAL1_MIDI.setHandleNoteOff(handleNoteOff);
  SERIAL1_MIDI.setHandleControlChange(handleControlChange);
  SERIAL1_MIDI.setHandleProgramChange(handleHandleProgramChange);
  SERIAL1_MIDI.setHandlePitchBend(handleHandlePitchBend);
  SERIAL1_MIDI.begin(MIDI_CHANNEL_OMNI);
  SERIAL1_MIDI.turnThruOff();
  Serial1.begin(SERIAL1_MIDI_SPEED);
#endif // defined(USE_SERIAL1_MIDI)

#if defined(DEBUG_PRINT)
  Serial2.setTX(4);
  Serial2.setRX(5);
  Serial2.begin(115200);
#endif // defined(DEBUG_PRINT)

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(RT6150_PS_PIN, OUTPUT);
  digitalWrite(RT6150_PS_PIN, HIGH);

#if defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
#else // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
#if defined(I2S_DAC_MUTE_OFF_PIN)
  pinMode(I2S_DAC_MUTE_OFF_PIN, OUTPUT);
  digitalWrite(I2S_DAC_MUTE_OFF_PIN, HIGH);
#endif // defined(I2S_DAC_MUTE_OFF_PIN)
#endif // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)

  g_synth.initialize();
}

void __not_in_flash_func(loop)() {

#if defined(DEBUG_PRINT)
  uint32_t debug_measurement_start0_us = micros();
#endif // defined(DEBUG_PRINT)

  for (uint32_t i = 0; i < (I2S_BUFFER_WORDS + 15) / 16; i++) {
#if defined(USE_USB_MIDI)
    USB_MIDI.read();
#endif // defined(USE_USB_MIDI)

#if defined(USE_SERIAL1_MIDI)
    SERIAL1_MIDI.read();
#endif
  }

#if defined(DEBUG_PRINT)
  uint32_t debug_measurement_start1_us = micros();
#endif // defined(DEBUG_PRINT)

  int16_t left_buffer[I2S_BUFFER_WORDS];
  int16_t right_buffer[I2S_BUFFER_WORDS];
  for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
    left_buffer[i] = g_synth.process(right_buffer[i]);
  }

#if defined(DEBUG_PRINT)
  uint32_t debug_measurement_end_us = micros();
#endif // defined(DEBUG_PRINT)

#if defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
  for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
#if ((PWM_AUDIO_L_PIN + 1) == PWM_AUDIO_R_PIN) && ((PWM_AUDIO_L_PIN % 2) == 0)
    g_pwm_l.write(left_buffer[i]);
    g_pwm_l.write(right_buffer[i]);
#elif ((PWM_AUDIO_R_PIN + 1) == PWM_AUDIO_L_PIN) && ((PWM_AUDIO_R_PIN % 2) == 0)
    g_pwm_r.write(right_buffer[i]);
    g_pwm_r.write(left_buffer[i]);
#else
    g_pwm_l.write(left_buffer[i]);
    g_pwm_r.write(right_buffer[i]);
#endif
  }
#else // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
  for (uint32_t i = 0; i < I2S_BUFFER_WORDS; i++) {
    g_i2s_output.write16(left_buffer[i], right_buffer[i]);
  }
#endif // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)

#if defined(DEBUG_PRINT)
  static uint32_t s_debug_measurement_max0_us = 0;
  uint32_t debug_measurement_elapsed0_us = debug_measurement_end_us - debug_measurement_start0_us;
  s_debug_measurement_max0_us += (debug_measurement_elapsed0_us > s_debug_measurement_max0_us) *
                                 (debug_measurement_elapsed0_us - s_debug_measurement_max0_us);

  static uint32_t s_debug_measurement_max1_us = 0;
  uint32_t debug_measurement_elapsed1_us = debug_measurement_end_us - debug_measurement_start1_us;
  s_debug_measurement_max1_us += (debug_measurement_elapsed1_us > s_debug_measurement_max1_us) *
                                 (debug_measurement_elapsed1_us - s_debug_measurement_max1_us);

  static uint32_t s_debug_loop_counter = 0;
  if (++s_debug_loop_counter == 4000) {
    s_debug_loop_counter = 0;

    Serial2.println(debug_measurement_elapsed1_us);
    Serial2.println(s_debug_measurement_max1_us);
    Serial2.println(debug_measurement_elapsed0_us);
    Serial2.println(s_debug_measurement_max0_us);
    Serial2.println();
  }
#endif // defined(DEBUG_PRINT)
}

void __not_in_flash_func(handleNoteOn)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == MIDI_CH) {
    if (velocity > 0) {
      g_synth.note_on(pitch, velocity);
    } else {
      g_synth.note_off(pitch);
    }
  }
}

void __not_in_flash_func(handleNoteOff)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == MIDI_CH) {
    (void) velocity;
    g_synth.note_off(pitch);
  }
}

void __not_in_flash_func(handleControlChange)(byte channel, byte number, byte value)
{
  if ((channel - 1) == MIDI_CH) {
    g_synth.control_change(number, value);
  }
}

void __not_in_flash_func(handleHandleProgramChange)(byte channel, byte number)
{
  if ((channel - 1) == MIDI_CH) {
    g_synth.program_change(number);
  }
}

void __not_in_flash_func(handleHandlePitchBend)(byte channel, int bend)
{
  if ((channel - 1) == MIDI_CH) {
    g_synth.pitch_bend((bend + 8192) & 0x7F, (bend + 8192) >> 7);
  }
}
