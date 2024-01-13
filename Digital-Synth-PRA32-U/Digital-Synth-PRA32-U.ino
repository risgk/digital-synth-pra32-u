/*
 * Digital Synth PRA32-U
 */

//#define PRA32_U_USE_DEBUG_PRINT               // Serial1

#define PRA32_U_USE_USB_MIDI                  // Select USB Stack: "Adafruit TinyUSB" in the Arduino IDE "Tools" menu

//#define PRA32_U_USE_UART_MIDI                 // Serial2

#define PRA32_U_UART_MIDI_SPEED               (31250)
//#define PRA32_U_UART_MIDI_SPEED               (38400)

#define SERIAL1_TX_PIN                        (0)
#define SERIAL1_RX_PIN                        (1)

#define PRA32_U_MIDI_CH                       (0)  // 0-based

// for Pimoroni Pico Audio Pack [PIM544]
#define PRA32_U_I2S_DAC_MUTE_OFF_PIN          (22)
#define PRA32_U_I2S_DATA_PIN                  (9)
//#define PRA32_U_I2S_MCLK_PIN                  (0)
//#define PRA32_U_I2S_MCLK_MULT                 (0)
#define PRA32_U_I2S_BCLK_PIN                  (10)  // LRCLK Pin is PRA32_U_I2S_BCLK_PIN + 1
#define PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS  (false)

#define PRA32_U_I2S_BUFFERS                   (4)
#define PRA32_U_I2S_BUFFER_WORDS              (64)

//#define PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S

// for Pimoroni Pico VGA Demo Base (PIM553)
#define PRA32_U_PWM_AUDIO_L_PIN               (28)
#define PRA32_U_PWM_AUDIO_R_PIN               (27)

#define PRA32_U_USE_2_CORES_FOR_SIGNAL_PROCESSING

#define PRA32_U_USE_EMULATED_EEPROM

////////////////////////////////////////////////////////////////

#include "pra32-u-common.h"
#include "pra32-u-synth.h"

#define RT6150_PS_PIN                   (23)

PRA32_U_Synth g_synth;

#include <MIDI.h>
#if defined(PRA32_U_USE_USB_MIDI)
#include <Adafruit_TinyUSB.h>
Adafruit_USBD_MIDI usbd_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbd_midi, USB_MIDI);
#endif // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, SERIAL_MIDI);
#endif

#if defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
#include <PWMAudio.h>
PWMAudio g_pwm_l(PRA32_U_PWM_AUDIO_L_PIN);
PWMAudio g_pwm_r(PRA32_U_PWM_AUDIO_R_PIN);
#endif // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

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
#if defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
#if ((PRA32_U_PWM_AUDIO_L_PIN + 1) == PRA32_U_PWM_AUDIO_R_PIN) && ((PRA32_U_PWM_AUDIO_L_PIN % 2) == 0)
  g_pwm_l.setStereo(true);
  g_pwm_l.setBuffers(PRA32_U_I2S_BUFFERS, PRA32_U_I2S_BUFFER_WORDS);
  g_pwm_l.setFrequency(SAMPLING_RATE);
  g_pwm_l.begin();
#elif ((PRA32_U_PWM_AUDIO_R_PIN + 1) == PRA32_U_PWM_AUDIO_L_PIN) && ((PRA32_U_PWM_AUDIO_R_PIN % 2) == 0)
  g_pwm_r.setStereo(true);
  g_pwm_r.setBuffers(PRA32_U_I2S_BUFFERS, PRA32_U_I2S_BUFFER_WORDS);
  g_pwm_r.setFrequency(SAMPLING_RATE);
  g_pwm_r.begin();
#else
  g_pwm_l.setBuffers(PRA32_U_I2S_BUFFERS, PRA32_U_I2S_BUFFER_WORDS / 2);
  g_pwm_r.setBuffers(PRA32_U_I2S_BUFFERS, PRA32_U_I2S_BUFFER_WORDS / 2);
  g_pwm_l.setFrequency(SAMPLING_RATE);
  g_pwm_r.setFrequency(SAMPLING_RATE);
  g_pwm_l.begin();
  g_pwm_r.begin();
#endif
#else // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
  g_i2s_output.setFrequency(SAMPLING_RATE);
  g_i2s_output.setDATA(PRA32_U_I2S_DATA_PIN);
#if defined(PRA32_U_I2S_MCLK_PIN)
  g_i2s_output.setMCLK(PRA32_U_I2S_MCLK_PIN);
  g_i2s_output.setMCLKmult(PRA32_U_I2S_MCLK_MULT);
#endif // defined(PRA32_U_I2S_MCLK_PIN)
  g_i2s_output.setBCLK(PRA32_U_I2S_BCLK_PIN);
  if (PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS) {
    g_i2s_output.swapClocks();
  }
  g_i2s_output.setBitsPerSample(16);
  g_i2s_output.setBuffers(PRA32_U_I2S_BUFFERS, PRA32_U_I2S_BUFFER_WORDS);
  g_i2s_output.begin();
#endif // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

#if defined(PRA32_U_USE_USB_MIDI)
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
#endif // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
  Serial2.setTX(SERIAL1_TX_PIN);
  Serial2.setRX(SERIAL1_RX_PIN);
  SERIAL_MIDI.setHandleNoteOn(handleNoteOn);
  SERIAL_MIDI.setHandleNoteOff(handleNoteOff);
  SERIAL_MIDI.setHandleControlChange(handleControlChange);
  SERIAL_MIDI.setHandleProgramChange(handleHandleProgramChange);
  SERIAL_MIDI.setHandlePitchBend(handleHandlePitchBend);
  SERIAL_MIDI.begin(MIDI_CHANNEL_OMNI);
  SERIAL_MIDI.turnThruOff();
  Serial2.begin(PRA32_U_UART_MIDI_SPEED);
#endif // defined(PRA32_U_USE_UART_MIDI)

#if defined(PRA32_U_USE_DEBUG_PRINT)
  Serial1.setTX(4);
  Serial1.setRX(5);
  Serial1.begin(115200);
#endif // defined(PRA32_U_USE_DEBUG_PRINT)

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  pinMode(RT6150_PS_PIN, OUTPUT);
  digitalWrite(RT6150_PS_PIN, HIGH);

#if defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
#else // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
#if defined(PRA32_U_I2S_DAC_MUTE_OFF_PIN)
  pinMode(PRA32_U_I2S_DAC_MUTE_OFF_PIN, OUTPUT);
  digitalWrite(PRA32_U_I2S_DAC_MUTE_OFF_PIN, HIGH);
#endif // defined(PRA32_U_I2S_DAC_MUTE_OFF_PIN)
#endif // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

  g_synth.initialize();
}

void __not_in_flash_func(loop)() {

#if defined(PRA32_U_USE_DEBUG_PRINT)
  uint32_t debug_measurement_start0_us = micros();
#endif // defined(PRA32_U_USE_DEBUG_PRINT)

  for (uint32_t i = 0; i < (PRA32_U_I2S_BUFFER_WORDS + 15) / 16; i++) {
#if defined(PRA32_U_USE_USB_MIDI)
    USB_MIDI.read();
#endif // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
    SERIAL_MIDI.read();
#endif
  }

#if defined(PRA32_U_USE_DEBUG_PRINT)
  uint32_t debug_measurement_start1_us = micros();
#endif // defined(PRA32_U_USE_DEBUG_PRINT)

  int16_t left_buffer[PRA32_U_I2S_BUFFER_WORDS];
  int16_t right_buffer[PRA32_U_I2S_BUFFER_WORDS];
  for (uint32_t i = 0; i < PRA32_U_I2S_BUFFER_WORDS; i++) {
    left_buffer[i] = g_synth.process(right_buffer[i]);
  }

#if defined(PRA32_U_USE_DEBUG_PRINT)
  uint32_t debug_measurement_end_us = micros();
#endif // defined(PRA32_U_USE_DEBUG_PRINT)

#if defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
  for (uint32_t i = 0; i < PRA32_U_I2S_BUFFER_WORDS; i++) {
#if ((PRA32_U_PWM_AUDIO_L_PIN + 1) == PRA32_U_PWM_AUDIO_R_PIN) && ((PRA32_U_PWM_AUDIO_L_PIN % 2) == 0)
    g_pwm_l.write(left_buffer[i]);
    g_pwm_l.write(right_buffer[i]);
#elif ((PRA32_U_PWM_AUDIO_R_PIN + 1) == PRA32_U_PWM_AUDIO_L_PIN) && ((PRA32_U_PWM_AUDIO_R_PIN % 2) == 0)
    g_pwm_r.write(right_buffer[i]);
    g_pwm_r.write(left_buffer[i]);
#else
    g_pwm_l.write(left_buffer[i]);
    g_pwm_r.write(right_buffer[i]);
#endif
  }
#else // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
  for (uint32_t i = 0; i < PRA32_U_I2S_BUFFER_WORDS; i++) {
    g_i2s_output.write16(left_buffer[i], right_buffer[i]);
  }
#endif // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

#if defined(PRA32_U_USE_DEBUG_PRINT)
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

    Serial1.println(debug_measurement_elapsed1_us);
    Serial1.println(s_debug_measurement_max1_us);
    Serial1.println(debug_measurement_elapsed0_us);
    Serial1.println(s_debug_measurement_max0_us);
    Serial1.println();
  }
#endif // defined(PRA32_U_USE_DEBUG_PRINT)
}

void __not_in_flash_func(handleNoteOn)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == PRA32_U_MIDI_CH) {
    if (velocity > 0) {
      g_synth.note_on(pitch, velocity);
    } else {
      g_synth.note_off(pitch);
    }
  }
}

void __not_in_flash_func(handleNoteOff)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == PRA32_U_MIDI_CH) {
    (void) velocity;
    g_synth.note_off(pitch);
  }
}

void __not_in_flash_func(handleControlChange)(byte channel, byte number, byte value)
{
  if ((channel - 1) == PRA32_U_MIDI_CH) {
    g_synth.control_change(number, value);
  }
}

void __not_in_flash_func(handleHandleProgramChange)(byte channel, byte number)
{
  if ((channel - 1) == PRA32_U_MIDI_CH) {
    g_synth.program_change(number);
  }
}

void __not_in_flash_func(handleHandlePitchBend)(byte channel, int bend)
{
  if ((channel - 1) == PRA32_U_MIDI_CH) {
    g_synth.pitch_bend((bend + 8192) & 0x7F, (bend + 8192) >> 7);
  }
}
