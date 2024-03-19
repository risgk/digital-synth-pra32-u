#pragma once

#include "pra32-u-common.h"

static volatile uint32_t s_adc_current_value_0 = 0;
static volatile uint32_t s_adc_current_value_1 = 0;
static volatile uint32_t s_adc_current_value_2 = 0;

static volatile uint32_t s_adc_control_value_0 = 0;
static volatile uint32_t s_adc_control_value_1 = 0;
static volatile uint32_t s_adc_control_value_2 = 0;

static char s_display_buffer[8][21 + 1] = {
  "                    1",
  "Filter     Filter    ",
  "Cutoff     Resonance ",
  "A 127[127] B   0[  0]",
  "                     ",
  "           Filter    ",
  "           EG Amt    ",
  "           C  64[ +0]",
};

INLINE void PRA32_U_ControlPanel_setup() {
#if defined(PRA32_U_USE_CONTROL_PANEL)
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);
  adc_gpio_init(28);
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_input(uint32_t loop_counter) {
#if defined(PRA32_U_USE_CONTROL_PANEL)
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  switch (loop_counter & 0x3F) {
  case 0x10:
    {
      adc_select_input(0);
      uint32_t adc_average_value =
        (adc_read() + adc_read() + adc_read() + adc_read() +
         adc_read() + adc_read() + adc_read() + adc_read()) >> 6;

      if        (s_adc_current_value_0     > adc_average_value + 4) {
        s_adc_current_value_0 = adc_average_value;
      } else if (s_adc_current_value_0 + 4 < adc_average_value    ) {
        s_adc_current_value_0 = adc_average_value;
      }
    }
    break;
  case 0x20:
    {
      adc_select_input(1);
      uint32_t adc_average_value =
        (adc_read() + adc_read() + adc_read() + adc_read() +
         adc_read() + adc_read() + adc_read() + adc_read()) >> 6;

      if        (s_adc_current_value_1     > adc_average_value + 4) {
        s_adc_current_value_1 = adc_average_value;
      } else if (s_adc_current_value_1 + 4 < adc_average_value    ) {
        s_adc_current_value_1 = adc_average_value;
      }
    }
    break;
  case 0x30:
    {
      adc_select_input(2);
      uint32_t adc_average_value =
        (adc_read() + adc_read() + adc_read() + adc_read() +
         adc_read() + adc_read() + adc_read() + adc_read()) >> 6;

      if        (s_adc_current_value_2     > adc_average_value + 4) {
        s_adc_current_value_2 = adc_average_value;
      } else if (s_adc_current_value_2 + 4 < adc_average_value    ) {
        s_adc_current_value_2 = adc_average_value;
      }
    }
    break;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_control() {
#if defined(PRA32_U_USE_CONTROL_PANEL)
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#if defined(PRA32_U_CONTROL_PANEL_REVERSE_ANALOG_INPUT)
  s_adc_control_value_0 = 127 - (s_adc_current_value_0 >> 2);
  s_adc_control_value_1 = 127 - (s_adc_current_value_1 >> 2);
  s_adc_control_value_2 = 127 - (s_adc_current_value_2 >> 2);
#else  // defined(PRA32_U_CONTROL_PANEL_REVERSE_ANALOG_INPUT)
  s_adc_control_value_0 = s_adc_current_value_0 >> 2;
  s_adc_control_value_1 = s_adc_current_value_1 >> 2;
  s_adc_control_value_2 = s_adc_current_value_2 >> 2;
#endif  // defined(PRA32_U_CONTROL_PANEL_REVERSE_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_debug_print(uint32_t loop_counter) {
#if defined(PRA32_U_USE_DEBUG_PRINT)
#if defined(PRA32_U_USE_CONTROL_PANEL)
  switch (loop_counter) {
  case  5 * 750:
    Serial1.print("\e[7;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[0]));
    break;
  case  6 * 750:
    Serial1.print("\e[8;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[1]));
    break;
  case  7 * 750:
    Serial1.print("\e[9;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[2]));
    break;
  case  8 * 750:
    Serial1.print("\e[10;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[3]));
    break;
  case  9 * 750:
    Serial1.print("\e[11;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[4]));
    break;
  case 10 * 750:
    Serial1.print("\e[12;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[5]));
    break;
  case 11 * 750:
    Serial1.print("\e[13;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[6]));
    break;
  case 12 * 750:
    Serial1.print("\e[14;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[7]));
    break;
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  case 13 * 750:
    Serial1.print("\e[16;1H\e[K");
    Serial1.print(s_adc_control_value_0);
    break;
  case 14 * 750:
    Serial1.print("\e[17;1H\e[K");
    Serial1.print(s_adc_control_value_1);
    break;
  case 15 * 750:
    Serial1.print("\e[18;1H\e[K");
    Serial1.print(s_adc_control_value_2);
    break;
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}
