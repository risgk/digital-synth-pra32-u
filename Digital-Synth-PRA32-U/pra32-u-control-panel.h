#pragma once

#include "pra32-u-common.h"

static uint32_t s_adc_average_value_0 = 0;
static uint32_t s_adc_average_value_1 = 0;
static uint32_t s_adc_average_value_2 = 0;

static char s_display_buffer[8][21 + 1] = {
  "                     ",
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

INLINE void PRA32_U_ControlPanel_update(uint32_t loop_counter) {
#if defined(PRA32_U_USE_CONTROL_PANEL)
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  if        ((loop_counter & 0x3F) == 0x10) {
    adc_select_input(0);
    s_adc_average_value_0 = (adc_read() + adc_read() + adc_read() + adc_read() +
                             adc_read() + adc_read() + adc_read() + adc_read()) >> 7;
  } else if ((loop_counter & 0x3F) == 0x20) {
    adc_select_input(1);
    s_adc_average_value_1 = (adc_read() + adc_read() + adc_read() + adc_read() +
                             adc_read() + adc_read() + adc_read() + adc_read()) >> 7;
  } else if ((loop_counter & 0x3F) == 0x30) {
    adc_select_input(2);
    s_adc_average_value_2 = (adc_read() + adc_read() + adc_read() + adc_read() +
                             adc_read() + adc_read() + adc_read() + adc_read()) >> 7;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_debug_print(uint32_t loop_counter) {
#if defined(PRA32_U_USE_DEBUG_PRINT)
#if defined(PRA32_U_USE_CONTROL_PANEL)
  if        (loop_counter ==  5 * 750) {
    Serial1.print("\e[7;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[0]));
  } else if (loop_counter ==  6 * 750) {
    Serial1.print("\e[8;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[1]));
  } else if (loop_counter ==  7 * 750) {
    Serial1.print("\e[9;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[2]));
  } else if (loop_counter ==  8 * 750) {
    Serial1.print("\e[10;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[3]));
  } else if (loop_counter ==  9 * 750) {
    Serial1.print("\e[11;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[4]));
  } else if (loop_counter == 10 * 750) {
    Serial1.print("\e[12;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[5]));
  } else if (loop_counter == 11 * 750) {
    Serial1.print("\e[13;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[6]));
  } else if (loop_counter == 12 * 750) {
    Serial1.print("\e[14;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[7]));
  }
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  if        (loop_counter == 13 * 750) {
    Serial1.print("\e[16;1H\e[K");
    Serial1.print(s_adc_average_value_0);
  } else if (loop_counter == 14 * 750) {
    Serial1.print("\e[17;1H\e[K");
    Serial1.print(s_adc_average_value_1);
  } else if (loop_counter == 15 * 750) {
    Serial1.print("\e[18;1H\e[K");
    Serial1.print(s_adc_average_value_2);
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}
