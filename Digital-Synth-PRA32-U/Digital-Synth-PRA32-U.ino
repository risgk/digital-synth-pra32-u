/*
 * Digital Synth PRA32-U
 */

#define PRA32_U_VERSION                       "v3.1.0    "

//#define PRA32_U_USE_DEBUG_PRINT               // Serial1

#define PRA32_U_USE_USB_MIDI                  // Select USB Stack: "Adafruit TinyUSB" in the Arduino IDE "Tools" menu

//#define PRA32_U_USE_UART_MIDI                 // Serial2

#define PRA32_U_UART_MIDI_SPEED               (31250)
//#define PRA32_U_UART_MIDI_SPEED               (38400)

#define PRA32_U_UART_MIDI_TX_PIN              (4)
#define PRA32_U_UART_MIDI_RX_PIN              (5)

#define PRA32_U_MIDI_CH                       (0)  // 0-based

// for Pimoroni Pico Audio Pack (PIM544)
#define PRA32_U_I2S_DAC_MUTE_OFF_PIN          (22)
#define PRA32_U_I2S_DATA_PIN                  (9)
//#define PRA32_U_I2S_MCLK_PIN                  (0)
//#define PRA32_U_I2S_MCLK_MULT                 (0)
#define PRA32_U_I2S_BCLK_PIN                  (10)  // LRCLK Pin is PRA32_U_I2S_BCLK_PIN + 1
#define PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS  (false)
#define PRA32_U_I2S_SWAP_LEFT_AND_RIGHT       (false)

#define PRA32_U_I2S_BUFFERS                   (4)
#define PRA32_U_I2S_BUFFER_WORDS              (64)

//#define PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S  // Select CPU Speed: "150 MHz (Overclock)" in the Arduino IDE "Tools" menu

// for Pimoroni Pico VGA Demo Base (PIM553)
#define PRA32_U_PWM_AUDIO_L_PIN               (28)
#define PRA32_U_PWM_AUDIO_R_PIN               (27)

#define PRA32_U_USE_2_CORES_FOR_SIGNAL_PROCESSING

#define PRA32_U_USE_EMULATED_EEPROM

////////////////////////////////////////////////////////////////

//#define PRA32_U_USE_CONTROL_PANEL               // PRA32-U with Panel

#define PRA32_U_USE_CONTROL_PANEL_KEY_INPUT     // Use tactile switches
#define PRA32_U_KEY_INPUT_ACTIVE_LEVEL          (HIGH)
#define PRA32_U_KEY_INPUT_PIN_MODE              (INPUT_PULLDOWN)
#define PRA32_U_KEY_INPUT_PREV_KEY_PIN          (16)
#define PRA32_U_KEY_INPUT_NEXT_KEY_PIN          (18)
#define PRA32_U_KEY_INPUT_PLAY_KEY_PIN          (20)
#define PRA32_U_KEY_ANTI_CHATTERING_WAIT        (15)
#define PRA32_U_KEY_LONG_PRESS_WAIT             (375)

#define PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT  // Use ADC0, ADC1, and ADC2
#define PRA32_U_ANALOG_INPUT_REVERSED           (true)
#define PRA32_U_ANALOG_INPUT_CORRECTION         (-504)
#define PRA32_U_ANALOG_INPUT_THRESHOLD          (504)
#define PRA32_U_ANALOG_INPUT_DENOMINATOR        (504)

#define PRA32_U_USE_CONTROL_PANEL_OLED_DISPLAY  // Use SSD1306 monochrome 128x64 OLED
#define PRA32_U_OLED_DISPLAY_I2C                (i2c1)
#define PRA32_U_OLED_DISPLAY_I2C_SDA_PIN        (6)
#define PRA32_U_OLED_DISPLAY_I2C_SCL_PIN        (7)
#define PRA32_U_OLED_DISPLAY_I2C_ADDRESS        (0x3C)
#define PRA32_U_OLED_DISPLAY_CONTRAST           (0xFF)
#define PRA32_U_OLED_DISPLAY_ROTATION           (true)

#define PRA32_U_DISABLE_USB_MIDI_TRANSMITTION

////////////////////////////////////////////////////////////////

uint8_t g_midi_ch = PRA32_U_MIDI_CH;

#include "hardware/adc.h"

#if defined(PRA32_U_USE_CONTROL_PANEL)
extern void PRA32_U_ControlPanel_on_control_change(uint8_t control_number);
extern void PRA32_U_ControlPanel_on_clock();
extern void PRA32_U_ControlPanel_on_start();
extern void PRA32_U_ControlPanel_on_stop();
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)

#include "pra32-u-common.h"
#include "pra32-u-synth.h"

PRA32_U_Synth g_synth;

#include <MIDI.h>
#if defined(PRA32_U_USE_USB_MIDI)
#include <Adafruit_TinyUSB.h>
Adafruit_USBD_MIDI usbd_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usbd_midi, USB_MIDI);
#endif  // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, UART_MIDI);
#endif

#include "pra32-u-control-panel.h"

#if defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
#include <PWMAudio.h>
PWMAudio g_pwm_l(PRA32_U_PWM_AUDIO_L_PIN);
PWMAudio g_pwm_r(PRA32_U_PWM_AUDIO_R_PIN);
#endif  // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

#include <I2S.h>
I2S g_i2s_output(OUTPUT);

static volatile uint32_t s_debug_measurement_elapsed0_us = 0;
static volatile uint32_t s_debug_measurement_max0_us     = 0;
static volatile uint32_t s_debug_measurement_elapsed1_us = 0;
static volatile uint32_t s_debug_measurement_max1_us     = 0;

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte number, byte value);
void handleProgramChange(byte channel, byte number);
void handlePitchBend(byte channel, int bend);
void handleClock();
void handleStart();
void handleStop();
void writeProgramsToFlashAndEndSketch();

void __not_in_flash_func(setup1)() {
  PRA32_U_ControlPanel_setup();

#if defined(PRA32_U_USE_DEBUG_PRINT)
  Serial1.setTX(0);
  Serial1.setRX(1);
  Serial1.begin(115200);
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}

void __not_in_flash_func(loop1)() {
  boolean processed = g_synth.secondary_core_process();
  if (processed) {
    static uint32_t s_loop_counter = 0;
    s_loop_counter++;
    if (s_loop_counter >= 16 * 400) {
      s_loop_counter = 0;
    }

    PRA32_U_ControlPanel_update_analog_inputs(s_loop_counter);
    PRA32_U_ControlPanel_update_display_buffer(s_loop_counter);
    PRA32_U_ControlPanel_update_display(s_loop_counter);

#if defined(PRA32_U_USE_DEBUG_PRINT)
    switch (s_loop_counter) {
    case  1 * 400:
      Serial1.print("\e[1;1H\e[K");
      Serial1.print(s_debug_measurement_elapsed1_us);
      break;
    case  2 * 400:
      Serial1.print("\e[2;1H\e[K");
      Serial1.print(s_debug_measurement_max1_us);
      break;
    case  3 * 400:
      Serial1.print("\e[4;1H\e[K");
      Serial1.print(s_debug_measurement_elapsed0_us);
      break;
    case  4 * 400:
      Serial1.print("\e[5;1H\e[K");
      Serial1.print(s_debug_measurement_max0_us);
      break;
    default:
      PRA32_U_ControlPanel_debug_print(s_loop_counter);
      break;
    }
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
  }
}

void __not_in_flash_func(setup)() {
#if defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
  pinMode(PRA32_U_PWM_AUDIO_L_PIN, OUTPUT_12MA);
  pinMode(PRA32_U_PWM_AUDIO_R_PIN, OUTPUT_12MA);
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
#else  // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
  g_i2s_output.setSysClk(SAMPLING_RATE);
  g_i2s_output.setFrequency(SAMPLING_RATE);
  g_i2s_output.setDATA(PRA32_U_I2S_DATA_PIN);
#if defined(PRA32_U_I2S_MCLK_PIN)
  g_i2s_output.setMCLK(PRA32_U_I2S_MCLK_PIN);
  g_i2s_output.setMCLKmult(PRA32_U_I2S_MCLK_MULT);
#endif  // defined(PRA32_U_I2S_MCLK_PIN)
  g_i2s_output.setBCLK(PRA32_U_I2S_BCLK_PIN);
  if (PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS) {
    g_i2s_output.swapClocks();
  }
  g_i2s_output.setBitsPerSample(16);
  g_i2s_output.setBuffers(PRA32_U_I2S_BUFFERS, PRA32_U_I2S_BUFFER_WORDS);
  g_i2s_output.begin();
#endif  // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

#if defined(PRA32_U_USE_USB_MIDI)
  TinyUSB_Device_Init(0);
  USBDevice.setManufacturerDescriptor("ISGK Instruments");
  USBDevice.setProductDescriptor("Digital Synth PRA32-U");
  USB_MIDI.setHandleNoteOn(handleNoteOn);
  USB_MIDI.setHandleNoteOff(handleNoteOff);
  USB_MIDI.setHandleControlChange(handleControlChange);
  USB_MIDI.setHandleProgramChange(handleProgramChange);
  USB_MIDI.setHandlePitchBend(handlePitchBend);
  USB_MIDI.setHandleClock(handleClock);
  USB_MIDI.setHandleStart(handleStart);
  USB_MIDI.setHandleStop(handleStop);
  USB_MIDI.begin(MIDI_CHANNEL_OMNI);
  USB_MIDI.turnThruOff();
#endif  // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
  Serial2.setTX(PRA32_U_UART_MIDI_TX_PIN);
  Serial2.setRX(PRA32_U_UART_MIDI_RX_PIN);
  UART_MIDI.setHandleNoteOn(handleNoteOn);
  UART_MIDI.setHandleNoteOff(handleNoteOff);
  UART_MIDI.setHandleControlChange(handleControlChange);
  UART_MIDI.setHandleProgramChange(handleProgramChange);
  UART_MIDI.setHandlePitchBend(handlePitchBend);
  UART_MIDI.setHandleClock(handleClock);
  UART_MIDI.setHandleStart(handleStart);
  UART_MIDI.setHandleStop(handleStop);
  UART_MIDI.begin(MIDI_CHANNEL_OMNI);
  UART_MIDI.turnThruOff();
  Serial2.begin(PRA32_U_UART_MIDI_SPEED);
#endif  // defined(PRA32_U_USE_UART_MIDI)

#if defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_2)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
#endif  // defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_2)

#if defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_RASPBERRY_PI_PICO_2) || defined(ARDUINO_RASPBERRY_PI_PICO_2W)
  pinMode(23, OUTPUT);  // RT6150 (PMIC) Power Save Pin
  digitalWrite(23, HIGH);
#endif  // defined(ARDUINO_RASPBERRY_PI_PICO) || defined(ARDUINO_RASPBERRY_PI_PICO_W) || defined(ARDUINO_RASPBERRY_PI_PICO_2) || defined(ARDUINO_RASPBERRY_PI_PICO_2W)

#if defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
#else  // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
#if defined(PRA32_U_I2S_DAC_MUTE_OFF_PIN)
  pinMode(PRA32_U_I2S_DAC_MUTE_OFF_PIN, OUTPUT);
  digitalWrite(PRA32_U_I2S_DAC_MUTE_OFF_PIN, HIGH);
#endif  // defined(PRA32_U_I2S_DAC_MUTE_OFF_PIN)
#endif  // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

  g_synth.initialize();

  PRA32_U_ControlPanel_initialize_parameters();

  delay(100);
}

void __not_in_flash_func(loop)() {

#if defined(PRA32_U_USE_DEBUG_PRINT)
  uint32_t debug_measurement_start0_us = micros();
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)

  for (uint32_t i = 0; i < ((PRA32_U_I2S_BUFFER_WORDS + 31) / 32) + 1; i++) {
#if defined(PRA32_U_USE_USB_MIDI)
    USB_MIDI.read();
#endif  // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
    UART_MIDI.read();
#endif
  }

  PRA32_U_ControlPanel_update_control();

#if defined(PRA32_U_USE_DEBUG_PRINT)
  uint32_t debug_measurement_start1_us = micros();
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)

  int16_t left_buffer[PRA32_U_I2S_BUFFER_WORDS];
  int16_t right_buffer[PRA32_U_I2S_BUFFER_WORDS];
  for (uint32_t i = 0; i < PRA32_U_I2S_BUFFER_WORDS; i++) {
    left_buffer[i] = g_synth.process(right_buffer[i]);
  }

#if defined(PRA32_U_USE_DEBUG_PRINT)
  uint32_t debug_measurement_end_us = micros();
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)

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
#else  // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)
  for (uint32_t i = 0; i < PRA32_U_I2S_BUFFER_WORDS; i++) {
    if (PRA32_U_I2S_SWAP_LEFT_AND_RIGHT) {
      g_i2s_output.write16(right_buffer[i], left_buffer[i]);
    } else {
      g_i2s_output.write16(left_buffer[i], right_buffer[i]);
    }
  }
#endif  // defined(PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S)

#if defined(PRA32_U_USE_DEBUG_PRINT)
  s_debug_measurement_elapsed0_us = debug_measurement_end_us - debug_measurement_start0_us;
  s_debug_measurement_max0_us += (s_debug_measurement_elapsed0_us > s_debug_measurement_max0_us) *
                                 (s_debug_measurement_elapsed0_us - s_debug_measurement_max0_us);

  s_debug_measurement_elapsed1_us = debug_measurement_end_us - debug_measurement_start1_us;
  s_debug_measurement_max1_us += (s_debug_measurement_elapsed1_us > s_debug_measurement_max1_us) *
                                 (s_debug_measurement_elapsed1_us - s_debug_measurement_max1_us);
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}

void __not_in_flash_func(handleNoteOn)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == g_midi_ch) {
    g_synth.note_on(pitch, velocity);
  }
}

void __not_in_flash_func(handleNoteOff)(byte channel, byte pitch, byte velocity)
{
  if ((channel - 1) == g_midi_ch) {
    (void) velocity;
    g_synth.note_off(pitch);
  }
}

void __not_in_flash_func(handleControlChange)(byte channel, byte number, byte value)
{
  if ((channel - 1) == g_midi_ch) {
    g_synth.control_change(number, value);
  }
}

void __not_in_flash_func(handleProgramChange)(byte channel, byte number)
{
  if ((channel - 1) == g_midi_ch) {
    g_synth.program_change(number);
  }
}

void __not_in_flash_func(handlePitchBend)(byte channel, int bend)
{
  if ((channel - 1) == g_midi_ch) {
    g_synth.pitch_bend((bend + 8192) & 0x7F, (bend + 8192) >> 7);
  }
}

void __not_in_flash_func(handleClock)()
{
#if defined(PRA32_U_USE_CONTROL_PANEL)
  PRA32_U_ControlPanel_on_clock();
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
}

void __not_in_flash_func(handleStart)()
{
#if defined(PRA32_U_USE_CONTROL_PANEL)
  PRA32_U_ControlPanel_on_start();
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
}

void __not_in_flash_func(handleStop)()
{
#if defined(PRA32_U_USE_CONTROL_PANEL)
  PRA32_U_ControlPanel_on_stop();
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
}
