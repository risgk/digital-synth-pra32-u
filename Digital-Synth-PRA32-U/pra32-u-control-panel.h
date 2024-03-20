#pragma once

#include "pra32-u-common.h"

#include <cstdio>

static volatile uint32_t s_adc_current_value[3];
static volatile uint32_t s_adc_control_value[3];
static volatile uint8_t  s_adc_control_target[3] = {FILTER_CUTOFF  , FILTER_RESO    , FILTER_EG_AMT  };

static char s_display_buffer[8][21 + 1] = {
  "                    5",
  "Filter     Filter    ",
  "Cutoff     Resonance ",
  "A          B         ",
  "                     ",
  "           Filter    ",
  "           EG Amt    ",
  "           C    [   ]",
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

      if        (s_adc_current_value[0]     >= adc_average_value + 4) {
        s_adc_current_value[0] = adc_average_value;
      } else if (s_adc_current_value[0] + 4 <= adc_average_value    ) {
        s_adc_current_value[0] = adc_average_value;
      }
    }
    break;
  case 0x20:
    {
      adc_select_input(1);
      uint32_t adc_average_value =
        (adc_read() + adc_read() + adc_read() + adc_read() +
         adc_read() + adc_read() + adc_read() + adc_read()) >> 6;

      if        (s_adc_current_value[1]     > adc_average_value + 4) {
        s_adc_current_value[1] = adc_average_value;
      } else if (s_adc_current_value[1] + 4 < adc_average_value    ) {
        s_adc_current_value[1] = adc_average_value;
      }
    }
    break;
  case 0x30:
    {
      adc_select_input(2);
      uint32_t adc_average_value =
        (adc_read() + adc_read() + adc_read() + adc_read() +
         adc_read() + adc_read() + adc_read() + adc_read()) >> 6;

      if        (s_adc_current_value[2]     > adc_average_value + 4) {
        s_adc_current_value[2] = adc_average_value;
      } else if (s_adc_current_value[2] + 4 < adc_average_value    ) {
        s_adc_current_value[2] = adc_average_value;
      }
    }
    break;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

static INLINE uint8_t PRA32_U_ControlPanel_adc_control_value_candidate(int adc_number) {
#if defined(PRA32_U_CONTROL_PANEL_REVERSE_ANALOG_INPUT)
  return (127 - (s_adc_current_value[adc_number] >> 2));
#else  // defined(PRA32_U_CONTROL_PANEL_REVERSE_ANALOG_INPUT)
  return (s_adc_current_value[adc_number] >> 2);
#endif  // defined(PRA32_U_CONTROL_PANEL_REVERSE_ANALOG_INPUT)
}

static INLINE void PRA32_U_ControlPanel_update_adc_control(int adc_number) {
  uint32_t adc_control_value_candidate = PRA32_U_ControlPanel_adc_control_value_candidate(adc_number);
  if (s_adc_control_value[adc_number] != adc_control_value_candidate) {
    s_adc_control_value[adc_number] = adc_control_value_candidate;
    g_synth.control_change(s_adc_control_target[adc_number], s_adc_control_value[adc_number]);
  }
}

INLINE void PRA32_U_ControlPanel_update_control() {
#if defined(PRA32_U_USE_CONTROL_PANEL)
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  static uint32_t s_initialize_counter = 0;
  if (s_initialize_counter < 2400) {
    ++s_initialize_counter;
    s_adc_control_value[0] = PRA32_U_ControlPanel_adc_control_value_candidate(0);
    s_adc_control_value[1] = PRA32_U_ControlPanel_adc_control_value_candidate(1);
    s_adc_control_value[2] = PRA32_U_ControlPanel_adc_control_value_candidate(2);
    return;
  }

  PRA32_U_ControlPanel_update_adc_control(0);
  PRA32_U_ControlPanel_update_adc_control(1);
  PRA32_U_ControlPanel_update_adc_control(2);
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_display_buffer(uint32_t loop_counter) {
#if defined(PRA32_U_USE_CONTROL_PANEL)
  if (loop_counter == 0) {
    char buff[6];

    uint8_t adc_control_target_0 = s_adc_control_target[0];
    if (adc_control_target_0 <= 127) {
      std::sprintf(buff, "%3u", g_synth.current_controller_value(adc_control_target_0));
      s_display_buffer[3][ 2] = buff[0];
      s_display_buffer[3][ 3] = buff[1];
      s_display_buffer[3][ 4] = buff[2];
    }

    uint8_t adc_control_target_1 = s_adc_control_target[1];
    if (adc_control_target_1 <= 127) {
      std::sprintf(buff, "%3u", g_synth.current_controller_value(adc_control_target_1));
      s_display_buffer[3][13] = buff[0];
      s_display_buffer[3][14] = buff[1];
      s_display_buffer[3][15] = buff[2];
    }

    uint8_t adc_control_target_2 = s_adc_control_target[2];
    if (adc_control_target_2 <= 127) {
      std::sprintf(buff, "%3u", g_synth.current_controller_value(adc_control_target_2));
      s_display_buffer[7][13] = buff[0];
      s_display_buffer[7][14] = buff[1];
      s_display_buffer[7][15] = buff[2];

      std::sprintf(buff, "%+3d", static_cast<int>(g_synth.current_controller_value(adc_control_target_2)) - 64);
      s_display_buffer[7][17] = buff[0];
      s_display_buffer[7][18] = buff[1];
      s_display_buffer[7][19] = buff[2];
    }
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_debug_print(uint32_t loop_counter) {
#if defined(PRA32_U_USE_DEBUG_PRINT)
#if defined(PRA32_U_USE_CONTROL_PANEL)
  switch (loop_counter) {
  case  5 * 375:
    Serial1.print("\e[7;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[0]));
    break;
  case  6 * 375:
    Serial1.print("\e[8;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[1]));
    break;
  case  7 * 375:
    Serial1.print("\e[9;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[2]));
    break;
  case  8 * 375:
    Serial1.print("\e[10;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[3]));
    break;
  case  9 * 375:
    Serial1.print("\e[11;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[4]));
    break;
  case 10 * 375:
    Serial1.print("\e[12;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[5]));
    break;
  case 11 * 375:
    Serial1.print("\e[13;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[6]));
    break;
  case 12 * 375:
    Serial1.print("\e[14;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[7]));
    break;
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  case 13 * 375:
    Serial1.print("\e[16;1H\e[K");
    Serial1.print(s_adc_control_value[0]);
    break;
  case 14 * 375:
    Serial1.print("\e[17;1H\e[K");
    Serial1.print(s_adc_control_value[1]);
    break;
  case 15 * 375:
    Serial1.print("\e[18;1H\e[K");
    Serial1.print(s_adc_control_value[2]);
    break;
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}
