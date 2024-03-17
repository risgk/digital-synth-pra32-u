#pragma once

#include "pra32-u-common.h"

static uint32_t s_adc_average_value_0 = 0;
static uint32_t s_adc_average_value_1 = 0;
static uint32_t s_adc_average_value_2 = 0;

INLINE void PRA32_U_ControlPanel_setup() {
#if defined(PRA32_U_USE_CONTROL_PANEL) && defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);
  adc_gpio_init(28);
#endif  // defined(PRA32_U_USE_CONTROL_PANEL) && defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
}

INLINE void PRA32_U_ControlPanel_update(uint32_t panel_loop_counter) {
#if defined(PRA32_U_USE_CONTROL_PANEL) && defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  if        ((panel_loop_counter & 0x3F) == 0x10) {
    adc_select_input(0);
    s_adc_average_value_0 = (adc_read() + adc_read() + adc_read() + adc_read() +
                             adc_read() + adc_read() + adc_read() + adc_read()) >> 7;
  } else if ((panel_loop_counter & 0x3F) == 0x20) {
    adc_select_input(1);
    s_adc_average_value_1 = (adc_read() + adc_read() + adc_read() + adc_read() +
                             adc_read() + adc_read() + adc_read() + adc_read()) >> 7;
  } else if ((panel_loop_counter & 0x3F) == 0x30) {
    adc_select_input(2);
    s_adc_average_value_2 = (adc_read() + adc_read() + adc_read() + adc_read() +
                             adc_read() + adc_read() + adc_read() + adc_read()) >> 7;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL) && defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
}

INLINE void PRA32_U_ControlPanel_debug_print(uint32_t debug_loop_counter) {
#if defined(PRA32_U_USE_DEBUG_PRINT)
#if defined(PRA32_U_USE_CONTROL_PANEL) && defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  if        ((debug_loop_counter ==  1 * 3000) ||
             (debug_loop_counter ==  5 * 3000) ||
             (debug_loop_counter ==  9 * 3000) ||
             (debug_loop_counter == 13 * 3000)) {
    Serial1.print("\e[16;1H\e[K");
    Serial1.print(s_adc_average_value_0);
  } else if ((debug_loop_counter ==  2 * 3000) ||
             (debug_loop_counter ==  6 * 3000) ||
             (debug_loop_counter == 10 * 3000) ||
             (debug_loop_counter == 14 * 3000)) {
    Serial1.print("\e[17;1H\e[K");
    Serial1.print(s_adc_average_value_1);
  } else if ((debug_loop_counter ==  3 * 3000) ||
             (debug_loop_counter ==  7 * 3000) ||
             (debug_loop_counter == 11 * 3000) ||
             (debug_loop_counter == 15 * 3000)) {
    Serial1.print("\e[18;1H\e[K");
    Serial1.print(s_adc_average_value_2);
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL) && defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}
