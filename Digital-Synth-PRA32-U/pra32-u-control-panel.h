#pragma once

#include "pra32-u-common.h"
#include "pra32-u-control-panel-font-table.h"

#include "hardware/i2c.h"

#include <cstdio>
#include <cstring>



static volatile uint32_t s_prev_key_current_value;
static volatile uint32_t s_next_key_current_value;
static volatile uint32_t s_play_key_current_value;

static volatile uint32_t s_prev_key_value_changed_time;
static volatile uint32_t s_next_key_value_changed_time;
static volatile uint32_t s_play_key_value_changed_time;

static volatile uint32_t s_adc_current_value[3];
static volatile uint32_t s_adc_control_value[3];
static volatile uint8_t  s_adc_control_target[3] = {FILTER_CUTOFF  , FILTER_RESO    , FILTER_EG_AMT  };

static char s_display_buffer[8][21 + 1] = {
  "                     ",
  "Filter     Filter    ",
  "Cutoff     Resonance ",
  "A          B         ",
  "                     ",
  "Filter A   Filter    ",
  "           EG Amt    ",
  "Page= 6    C    [   ]",
};



static INLINE uint8_t PRA32_U_ControlPanel_adc_control_value_candidate(uint32_t adc_number) {
#if defined(PRA32_U_ANALOG_INPUT_REVERSED)
  return (127 - (s_adc_current_value[adc_number] >> 9));
#else  // defined(PRA32_U_ANALOG_INPUT_REVERSED)
  return (s_adc_current_value[adc_number] >> 9);
#endif  // defined(PRA32_U_ANALOG_INPUT_REVERSED)
}

static INLINE boolean PRA32_U_ControlPanel_update_adc_control(uint32_t adc_number) {
  uint32_t adc_control_value_candidate = PRA32_U_ControlPanel_adc_control_value_candidate(adc_number);
  if (s_adc_control_value[adc_number] != adc_control_value_candidate) {
    s_adc_control_value[adc_number] = adc_control_value_candidate;
    g_synth.control_change(s_adc_control_target[adc_number], s_adc_control_value[adc_number]);
    return true;
  }

  return false;
}

static INLINE void PRA32_U_ControlPanel_set_draw_position(uint8_t x, uint8_t y) {
  uint8_t commands[] = {0x00,  static_cast<uint8_t>(0xB0 + y), 
                               static_cast<uint8_t>(0x10 + ((x * 6) >> 4)),
                               static_cast<uint8_t>(0x00 + ((x * 6) & 0x0F))};
  i2c_write_blocking(PRA32_U_OLED_DISPLAY_I2C, PRA32_U_OLED_DISPLAY_I2C_ADDRESS, commands, sizeof(commands), false);
}

static INLINE void PRA32_U_ControlPanel_draw_character(uint8_t c) {
  uint8_t data[] = {0x40,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  std::memcpy(&data[1], g_font_table[c], 6);
  i2c_write_blocking(PRA32_U_OLED_DISPLAY_I2C, PRA32_U_OLED_DISPLAY_I2C_ADDRESS, data, sizeof(data), false);
}



INLINE void PRA32_U_ControlPanel_setup() {
#if defined(PRA32_U_USE_CONTROL_PANEL)

#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  adc_init();
  adc_gpio_init(26);
  adc_gpio_init(27);
  adc_gpio_init(28);
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)

#if defined(PRA32_U_USE_CONTROL_PANEL_OLED_DISPLAY)
  i2c_init(PRA32_U_OLED_DISPLAY_I2C, 400 * 1000);
  i2c_set_slave_mode(PRA32_U_OLED_DISPLAY_I2C, false, 0);
  gpio_set_function(PRA32_U_OLED_DISPLAY_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PRA32_U_OLED_DISPLAY_I2C_SDA_PIN);
  gpio_set_function(PRA32_U_OLED_DISPLAY_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PRA32_U_OLED_DISPLAY_I2C_SCL_PIN);

  uint8_t commands_init_0[] = {0x00,  0x81, PRA32_U_OLED_DISPLAY_CONTRAST,  0xA0, 0xC0,  0x8D, 0x14};
  if (PRA32_U_OLED_DISPLAY_ROTATION) {
    commands_init_0[3] = 0xA1;
    commands_init_0[4] = 0xC8;
  }

  i2c_write_blocking(PRA32_U_OLED_DISPLAY_I2C, PRA32_U_OLED_DISPLAY_I2C_ADDRESS, commands_init_0, sizeof(commands_init_0), false);

  for (uint8_t y = 0; y <= 7; ++y) {
    for (uint8_t x = 0; x <= 20; ++x) {
      PRA32_U_ControlPanel_set_draw_position(x, y);
      PRA32_U_ControlPanel_draw_character(s_display_buffer[y][x]);
    }

    uint8_t commands[] = {0x00,  static_cast<uint8_t>(0xB0 + y), 0x17, 0x0E};
    i2c_write_blocking(PRA32_U_OLED_DISPLAY_I2C, PRA32_U_OLED_DISPLAY_I2C_ADDRESS, commands, sizeof(commands), false);

    uint8_t data[] = {0x40,  0x00, 0x00};
    i2c_write_blocking(PRA32_U_OLED_DISPLAY_I2C, PRA32_U_OLED_DISPLAY_I2C_ADDRESS, data, sizeof(data), false);
  }

  uint8_t commands_init_1[] = {0x00,  0xAF};
  i2c_write_blocking(PRA32_U_OLED_DISPLAY_I2C, PRA32_U_OLED_DISPLAY_I2C_ADDRESS, commands_init_1, sizeof(commands_init_1), false);
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_OLED_DISPLAY)

#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_analog_inputs(uint32_t loop_counter) {
  static_cast<void>(loop_counter);

#if defined(PRA32_U_USE_CONTROL_PANEL)

#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  static uint32_t s_adc_average_value = 0;
  switch (loop_counter & 0x3F) {
  case 0x10:
    adc_select_input(0);
    s_adc_average_value  = adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x14:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x18:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x1C:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    if        (s_adc_current_value[0]       >= s_adc_average_value + 256) {
      s_adc_current_value[0] = s_adc_average_value;
    } else if (s_adc_current_value[0] + 256 <= s_adc_average_value      ) {
      s_adc_current_value[0] = s_adc_average_value;
    }
    break;
  case 0x20:
    adc_select_input(1);
    s_adc_average_value  = adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x24:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x28:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x2C:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    if        (s_adc_current_value[1]       >= s_adc_average_value + 256) {
      s_adc_current_value[1] = s_adc_average_value;
    } else if (s_adc_current_value[1] + 256 <= s_adc_average_value      ) {
      s_adc_current_value[1] = s_adc_average_value;
    }
    break;
  case 0x30:
    adc_select_input(2);
    s_adc_average_value  = adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x34:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x38:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x3C:
    s_adc_average_value += adc_read() + adc_read() + adc_read() + adc_read();
    if        (s_adc_current_value[2]       >= s_adc_average_value + 256) {
      s_adc_current_value[2] = s_adc_average_value;
    } else if (s_adc_current_value[2] + 256 <= s_adc_average_value      ) {
      s_adc_current_value[2] = s_adc_average_value;
    }
    break;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)

#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_control() {
#if defined(PRA32_U_USE_CONTROL_PANEL)

#if defined(PRA32_U_USE_CONTROL_PANEL_KEY_INPUT)
  static uint32_t s_key_inpuy_counter = 0;
  s_key_inpuy_counter++;
  if (s_key_inpuy_counter - s_prev_key_value_changed_time >= PRA32_U_KEY_ANTI_CHATTERING_WAIT) {
    uint32_t value = digitalRead(PRA32_U_KEY_INPUT_PREV_KEY_PIN) == PRA32_U_KEY_INPUT_ACTIVE_LEVEL;
    s_prev_key_current_value = value;
  }

  if (s_key_inpuy_counter - s_prev_key_value_changed_time >= PRA32_U_KEY_ANTI_CHATTERING_WAIT) {
    uint32_t value = digitalRead(PRA32_U_KEY_INPUT_NEXT_KEY_PIN) == PRA32_U_KEY_INPUT_ACTIVE_LEVEL;
    s_next_key_current_value = value;
  }

  if (s_key_inpuy_counter - s_prev_key_value_changed_time >= PRA32_U_KEY_ANTI_CHATTERING_WAIT) {
    uint32_t value = digitalRead(PRA32_U_KEY_INPUT_PLAY_KEY_PIN) == PRA32_U_KEY_INPUT_ACTIVE_LEVEL;
    s_play_key_current_value = value;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_KEY_INPUT)

#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  static uint32_t s_initialize_counter = 0;
  if (s_initialize_counter < 75) {
    ++s_initialize_counter;
    s_adc_control_value[0] = PRA32_U_ControlPanel_adc_control_value_candidate(0);
    s_adc_control_value[1] = PRA32_U_ControlPanel_adc_control_value_candidate(1);
    s_adc_control_value[2] = PRA32_U_ControlPanel_adc_control_value_candidate(2);
    return;
  }

  static uint32_t s_adc_number_to_check = 0;

  boolean updated = PRA32_U_ControlPanel_update_adc_control(s_adc_number_to_check);
  s_adc_number_to_check = (s_adc_number_to_check + 1) % 3;

  if (updated == false) {
    updated = PRA32_U_ControlPanel_update_adc_control(s_adc_number_to_check);
    s_adc_number_to_check = (s_adc_number_to_check + 1) % 3;

    if (updated == false) {
      updated = PRA32_U_ControlPanel_update_adc_control(s_adc_number_to_check);
      s_adc_number_to_check = (s_adc_number_to_check + 1) % 3;
    }
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)

#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_display_buffer(uint32_t loop_counter) {
  static_cast<void>(loop_counter);

#if defined(PRA32_U_USE_CONTROL_PANEL)
  if ((loop_counter & 0x7F) == 0x00) {
    char buff[6];

    uint8_t adc_control_target_0 = s_adc_control_target[0];
    if (adc_control_target_0 <= 127) {
      uint8_t adc_control_value        = s_adc_control_value[0];
      uint8_t current_controller_value = g_synth.current_controller_value(adc_control_target_0);

      if        (adc_control_value < current_controller_value) {
        s_display_buffer[3][ 1] = '<';
      } else if (adc_control_value > current_controller_value) {
        s_display_buffer[3][ 1] = '>';
      } else {
        s_display_buffer[3][ 1] = '=';
      }

      std::sprintf(buff, "%3u", current_controller_value);
      s_display_buffer[3][ 2] = buff[0];
      s_display_buffer[3][ 3] = buff[1];
      s_display_buffer[3][ 4] = buff[2];
    }

    uint8_t adc_control_target_1 = s_adc_control_target[1];
    if (adc_control_target_1 <= 127) {
      uint8_t adc_control_value        = s_adc_control_value[1];
      uint8_t current_controller_value = g_synth.current_controller_value(adc_control_target_1);

      if        (adc_control_value < current_controller_value) {
        s_display_buffer[3][12] = '<';
      } else if (adc_control_value > current_controller_value) {
        s_display_buffer[3][12] = '>';
      } else {
        s_display_buffer[3][12] = '=';
      }

      std::sprintf(buff, "%3u", current_controller_value);
      s_display_buffer[3][13] = buff[0];
      s_display_buffer[3][14] = buff[1];
      s_display_buffer[3][15] = buff[2];
    }

    uint8_t adc_control_target_2 = s_adc_control_target[2];
    if (adc_control_target_2 <= 127) {
      uint8_t adc_control_value        = s_adc_control_value[2];
      uint8_t current_controller_value = g_synth.current_controller_value(adc_control_target_2);

      if        (adc_control_value < current_controller_value) {
        s_display_buffer[7][12] = '<';
      } else if (adc_control_value > current_controller_value) {
        s_display_buffer[7][12] = '>';
      } else {
        s_display_buffer[7][12] = '=';
      }

      std::sprintf(buff, "%3u", current_controller_value);
      s_display_buffer[7][13] = buff[0];
      s_display_buffer[7][14] = buff[1];
      s_display_buffer[7][15] = buff[2];

      std::sprintf(buff, "%+3d", static_cast<int>(current_controller_value) - 64);
      s_display_buffer[7][17] = buff[0];
      s_display_buffer[7][18] = buff[1];
      s_display_buffer[7][19] = buff[2];
    }
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_display(uint32_t loop_counter) {
  static_cast<void>(loop_counter);

#if defined(PRA32_U_USE_CONTROL_PANEL)

#if defined(PRA32_U_USE_CONTROL_PANEL_OLED_DISPLAY)
  static uint32_t s_display_draw_counter = 0;
  if ((loop_counter & 0x7F) == 0x40) {
    ++s_display_draw_counter;
    if (s_display_draw_counter >= 31) {
      s_display_draw_counter = 0;
    }

    uint8_t x = (s_display_draw_counter % 21) + ((s_display_draw_counter >= 21) * 11);
    uint8_t y = ((s_display_draw_counter >= 21) * 4) + 3;
    PRA32_U_ControlPanel_set_draw_position(x, y);
  } else if ((loop_counter & 0x7F) == 0x00) {
    uint8_t x = (s_display_draw_counter % 21) + ((s_display_draw_counter >= 21) * 11);
    uint8_t y = ((s_display_draw_counter >= 21) * 4) + 3;
    PRA32_U_ControlPanel_draw_character(s_display_buffer[y][x]);
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_OLED_DISPLAY)

#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_debug_print(uint32_t loop_counter) {
  static_cast<void>(loop_counter);

#if defined(PRA32_U_USE_DEBUG_PRINT)
#if defined(PRA32_U_USE_CONTROL_PANEL)
  switch (loop_counter) {
  case  5 * 400:
    Serial1.print("\e[7;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[0]));
    break;
  case  6 * 400:
    Serial1.print("\e[8;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[1]));
    break;
  case  7 * 400:
    Serial1.print("\e[9;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[2]));
    break;
  case  8 * 400:
    Serial1.print("\e[10;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[3]));
    break;
  case  9 * 400:
    Serial1.print("\e[11;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[4]));
    break;
  case 10 * 400:
    Serial1.print("\e[12;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[5]));
    break;
  case 11 * 400:
    Serial1.print("\e[13;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[6]));
    break;
  case 12 * 400:
    Serial1.print("\e[14;1H\e[K");
    Serial1.print(static_cast<char*>(s_display_buffer[7]));
    break;
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  case 13 * 400:
    Serial1.print("\e[16;1H\e[K");
    Serial1.print(s_adc_control_value[0]);
    break;
  case 14 * 400:
    Serial1.print("\e[17;1H\e[K");
    Serial1.print(s_adc_control_value[1]);
    break;
  case 15 * 400:
    Serial1.print("\e[18;1H\e[K");
    Serial1.print(s_adc_control_value[2]);
    break;
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}
