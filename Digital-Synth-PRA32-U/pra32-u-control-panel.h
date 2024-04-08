#pragma once

#include "pra32-u-common.h"
#include "pra32-u-control-panel-font-table.h"
#include "pra32-u-control-panel-page-table.h"

#include "hardware/i2c.h"

#include <cstdio>
#include <cstring>



#define NUMBER_OF_PAGES (sizeof(g_control_panel_page_table) / sizeof(g_control_panel_page_table[0]))

static volatile uint32_t s_current_page_index = 4;

static volatile int32_t  s_adc_current_value[3];
static volatile uint8_t  s_adc_control_value[3];
static volatile uint8_t  s_adc_control_target[3] = { 0xFF, 0xFF, 0xFF };

static volatile uint32_t s_display_draw_counter = 0;

static char s_display_buffer[8][21 + 1] = {
  "                     ",
  "                     ",
  "                     ",
  "                     ",
  "                     ",
  "                     ",
  "                     ",
  "                     ",
};



static INLINE void PRA32_U_ControlPanel_update_page() {
  PRA32_U_ControlPanelPage& current_page = g_control_panel_page_table[s_current_page_index];

  std::memset(&s_display_buffer[3], ' ', 21);
  std::memset(&s_display_buffer[7], ' ', 21);

  std::memcpy(&s_display_buffer[1][ 0], current_page.page_name_line_0            , 10);
  std::memcpy(&s_display_buffer[2][ 0], current_page.page_name_line_1            , 10);

  std::memcpy(&s_display_buffer[5][ 0], current_page.control_target_a_name_line_0, 10);
  std::memcpy(&s_display_buffer[6][ 0], current_page.control_target_a_name_line_1, 10);
  s_adc_control_target[0]             = current_page.control_target_a;

  std::memcpy(&s_display_buffer[5][11], current_page.control_target_b_name_line_0, 10);
  std::memcpy(&s_display_buffer[6][11], current_page.control_target_b_name_line_1, 10);
  s_adc_control_target[1]             = current_page.control_target_b;

  std::memcpy(&s_display_buffer[1][11], current_page.control_target_c_name_line_0, 10);
  std::memcpy(&s_display_buffer[2][11], current_page.control_target_c_name_line_1, 10);
  s_adc_control_target[2]             = current_page.control_target_c;

  s_display_draw_counter = 0;
}

static INLINE uint8_t PRA32_U_ControlPanel_adc_control_value_candidate(uint32_t adc_number) {
  volatile int32_t adc_control_value_candidate;

#if defined(PRA32_U_ANALOG_INPUT_REVERSED)
  adc_control_value_candidate = (127 - (s_adc_current_value[adc_number] / PRA32_U_ANALOG_INPUT_DENOMINATOR));
#else  // defined(PRA32_U_ANALOG_INPUT_REVERSED)
  adc_control_value_candidate = (s_adc_current_value[adc_number] / PRA32_U_ANALOG_INPUT_DENOMINATOR);
#endif  // defined(PRA32_U_ANALOG_INPUT_REVERSED)

  if (adc_control_value_candidate > 127) {
    adc_control_value_candidate = 127;
  }

  if (adc_control_value_candidate < 0) {
    adc_control_value_candidate = 0;
  }

  return adc_control_value_candidate;
}

static INLINE boolean PRA32_U_ControlPanel_update_control_adc(uint32_t adc_number) {
  uint8_t adc_control_value_candidate = PRA32_U_ControlPanel_adc_control_value_candidate(adc_number);

  if (s_adc_control_value[adc_number] != adc_control_value_candidate) {
    uint8_t s_adc_control_value_old = s_adc_control_value[adc_number];
    s_adc_control_value[adc_number] = adc_control_value_candidate;

    if (s_adc_control_target[adc_number] <= 0x7F) {
      g_synth.control_change(s_adc_control_target[adc_number], s_adc_control_value[adc_number]);
    } else if ((s_adc_control_target[adc_number] >= PC_BY_PANEL_0) && (s_adc_control_target[adc_number] <= PC_BY_PANEL_15)) {
      if ((s_adc_control_value_old < 64) && (s_adc_control_value[adc_number] >= 64)) {
        g_synth.program_change(s_adc_control_target[adc_number] - PC_BY_PANEL_0);
      }
    } else if ((s_adc_control_target[adc_number] >= WR_BY_PANEL_0) && (s_adc_control_target[adc_number] <= WR_BY_PANEL_15)) {
      static boolean s_ready_to_write[PROGRAM_NUMBER_MAX + 1] = {};

      uint8_t program_number_to_write = s_adc_control_target[adc_number] - WR_BY_PANEL_0;

      if (s_adc_control_value[adc_number] == 0) {
        s_ready_to_write[program_number_to_write] = true;
      } else if (s_ready_to_write[program_number_to_write] && (s_adc_control_value[adc_number] == 127)) {
        g_synth.write_parameters_to_program(program_number_to_write);
        s_ready_to_write[program_number_to_write] = false;
      }
    }

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
  std::memcpy(&data[1], g_control_panel_font_table[c], 6);
  i2c_write_blocking(PRA32_U_OLED_DISPLAY_I2C, PRA32_U_OLED_DISPLAY_I2C_ADDRESS, data, sizeof(data), false);
}

static INLINE boolean PRA32_U_ControlPanel_calc_value_display(uint8_t control_target, uint8_t controller_value, char value_display_text[5])
{
  boolean result = false;

  switch (control_target) {
  case MIXER_SUB_OSC   :  // TODO
  case OSC_2_COARSE    :
  case OSC_2_PITCH     :
  case FILTER_EG_AMT   :
  case EG_OSC_AMT      :
  case LFO_OSC_AMT     :
  case LFO_FILTER_AMT  :
  case BTH_FILTER_AMT  :
    {
      std::sprintf(value_display_text, "%+3d", static_cast<int>(controller_value) - 64);
      result = true;
    }
    break;
  case OSC_1_WAVE      :
    {
      char ary[6][5] = {"Saw","Sin","---","Tri","---","Pls"};
      uint32_t index = ((controller_value * 10) + 127) / 254;
      if (controller_value < 6) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case OSC_2_WAVE      :
    {
      char ary[6][5] = {"Saw","Sin","---","Tri","Nos","Sqr"};
      uint32_t index = ((controller_value * 10) + 127) / 254;
      if (controller_value < 6) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  }

  return result;
}



INLINE void PRA32_U_ControlPanel_setup() {
#if defined(PRA32_U_USE_CONTROL_PANEL)
  PRA32_U_ControlPanel_update_page();

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
  static int32_t s_adc_total_value = 0;
  switch (loop_counter & 0x3F) {
  case 0x10:
    adc_select_input(0);
    s_adc_total_value  = PRA32_U_ANALOG_INPUT_CORRECTION;
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x14:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x18:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x1C:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    if        (s_adc_current_value[0]                                  >= s_adc_total_value + PRA32_U_ANALOG_INPUT_THRESHOLD) {
      s_adc_current_value[0] = s_adc_total_value;
    } else if (s_adc_current_value[0] + PRA32_U_ANALOG_INPUT_THRESHOLD <= s_adc_total_value                                 ) {
      s_adc_current_value[0] = s_adc_total_value;
    }
    break;
  case 0x20:
    adc_select_input(1);
    s_adc_total_value  = PRA32_U_ANALOG_INPUT_CORRECTION;
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x24:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x28:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x2C:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    if        (s_adc_current_value[1]                                  >= s_adc_total_value + PRA32_U_ANALOG_INPUT_THRESHOLD) {
      s_adc_current_value[1] = s_adc_total_value;
    } else if (s_adc_current_value[1] + PRA32_U_ANALOG_INPUT_THRESHOLD <= s_adc_total_value                                 ) {
      s_adc_current_value[1] = s_adc_total_value;
    }
    break;
  case 0x30:
    adc_select_input(2);
    s_adc_total_value  = PRA32_U_ANALOG_INPUT_CORRECTION;
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x34:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x38:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    break;
  case 0x3C:
    s_adc_total_value += adc_read() + adc_read() + adc_read() + adc_read();
    if        (s_adc_current_value[2]                                  >= s_adc_total_value + PRA32_U_ANALOG_INPUT_THRESHOLD) {
      s_adc_current_value[2] = s_adc_total_value;
    } else if (s_adc_current_value[2] + PRA32_U_ANALOG_INPUT_THRESHOLD <= s_adc_total_value                                 ) {
      s_adc_current_value[2] = s_adc_total_value;
    }
    break;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)

#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_control() {
#if defined(PRA32_U_USE_CONTROL_PANEL)
  static uint32_t s_initialize_counter = 0;
  if (s_initialize_counter < 75) {
    ++s_initialize_counter;
#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
    s_adc_control_value[0] = PRA32_U_ControlPanel_adc_control_value_candidate(0);
    s_adc_control_value[1] = PRA32_U_ControlPanel_adc_control_value_candidate(1);
    s_adc_control_value[2] = PRA32_U_ControlPanel_adc_control_value_candidate(2);
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
    return;
  }

#if defined(PRA32_U_USE_CONTROL_PANEL_KEY_INPUT)
  static uint32_t s_prev_key_current_value;
  static uint32_t s_next_key_current_value;
  static uint32_t s_play_key_current_value;

  static uint32_t s_prev_key_value_changed_time;
  static uint32_t s_next_key_value_changed_time;
  static uint32_t s_play_key_value_changed_time;

  static uint32_t s_key_inpuy_counter = 0;
  ++s_key_inpuy_counter;
  if (s_key_inpuy_counter - s_prev_key_value_changed_time >= PRA32_U_KEY_ANTI_CHATTERING_WAIT) {
    uint32_t value = digitalRead(PRA32_U_KEY_INPUT_PREV_KEY_PIN) == PRA32_U_KEY_INPUT_ACTIVE_LEVEL;
    if (s_prev_key_current_value != value) {
      s_prev_key_current_value = value;
      s_prev_key_value_changed_time = s_key_inpuy_counter;

      if (s_prev_key_current_value == 0) {
        // Prev key released
        if (s_current_page_index == 0) {
          s_current_page_index = NUMBER_OF_PAGES - 1;
        } else {
          --s_current_page_index;
        }

        PRA32_U_ControlPanel_update_page();
      }
      return;
    }
  }

  if (s_key_inpuy_counter - s_next_key_value_changed_time >= PRA32_U_KEY_ANTI_CHATTERING_WAIT) {
    uint32_t value = digitalRead(PRA32_U_KEY_INPUT_NEXT_KEY_PIN) == PRA32_U_KEY_INPUT_ACTIVE_LEVEL;
    if (s_next_key_current_value != value) {
      s_next_key_current_value = value;
      s_next_key_value_changed_time = s_key_inpuy_counter;

      if (s_next_key_current_value == 0) {
        // Next key released
        if (s_current_page_index == NUMBER_OF_PAGES - 1) {
          s_current_page_index = 0;
        } else {
          ++s_current_page_index;
        }

        PRA32_U_ControlPanel_update_page();
      }
      return;
    }
  }

  if (s_key_inpuy_counter - s_play_key_value_changed_time >= PRA32_U_KEY_ANTI_CHATTERING_WAIT) {
    uint32_t value = digitalRead(PRA32_U_KEY_INPUT_PLAY_KEY_PIN) == PRA32_U_KEY_INPUT_ACTIVE_LEVEL;
    if (s_play_key_current_value != value) {
      s_play_key_current_value = value;
      s_play_key_value_changed_time = s_key_inpuy_counter;

      if (s_play_key_current_value == 1) {
        // Play key pressed
        g_synth.note_on(60, 100);
      } else {
        // Play key released
        g_synth.note_off(60);
      }
      return;
    }
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_KEY_INPUT)

#if defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  static uint32_t s_adc_number_to_check = 0;

  boolean updated = PRA32_U_ControlPanel_update_control_adc(s_adc_number_to_check);
  s_adc_number_to_check = (s_adc_number_to_check + 1) % 3;

  if (updated == false) {
    updated = PRA32_U_ControlPanel_update_control_adc(s_adc_number_to_check);
    s_adc_number_to_check = (s_adc_number_to_check + 1) % 3;

    if (updated == false) {
      updated = PRA32_U_ControlPanel_update_control_adc(s_adc_number_to_check);
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
    if (adc_control_target_0 < 0xFF) {
      uint8_t adc_control_value        = s_adc_control_value[0];
      uint8_t current_controller_value = adc_control_value;
      if (adc_control_target_0 <= 0x7F) {
        current_controller_value = g_synth.current_controller_value(adc_control_target_0);
      }

      s_display_buffer[7][ 0] = 'A';
      if        (adc_control_value < current_controller_value) {
        s_display_buffer[7][ 1] = '<';
      } else if (adc_control_value > current_controller_value) {
        s_display_buffer[7][ 1] = '>';
      } else {
        s_display_buffer[7][ 1] = '=';
      }

      std::sprintf(buff, "%3u", current_controller_value);
      s_display_buffer[7][ 2] = buff[0];
      s_display_buffer[7][ 3] = buff[1];
      s_display_buffer[7][ 4] = buff[2];

      char value_display_text[5] = {};
      boolean exists = PRA32_U_ControlPanel_calc_value_display(adc_control_target_0, current_controller_value, value_display_text);
      if (exists) {
        s_display_buffer[7][ 5] = '[';
        s_display_buffer[7][ 6] = value_display_text[0];
        s_display_buffer[7][ 7] = value_display_text[1];
        s_display_buffer[7][ 8] = value_display_text[2];
        s_display_buffer[7][ 9] = ']';
      }
    }

    uint8_t adc_control_target_1 = s_adc_control_target[1];
    if (adc_control_target_1 < 0xFF) {
      uint8_t adc_control_value        = s_adc_control_value[1];
      uint8_t current_controller_value = adc_control_value;
      if (adc_control_target_1 <= 0x7F) {
        current_controller_value = g_synth.current_controller_value(adc_control_target_1);
      }

      s_display_buffer[7][11] = 'B';
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

      char value_display_text[5] = {};
      boolean exists = PRA32_U_ControlPanel_calc_value_display(adc_control_target_1, current_controller_value, value_display_text);
      if (exists) {
        s_display_buffer[7][16] = '[';
        s_display_buffer[7][17] = value_display_text[0];
        s_display_buffer[7][18] = value_display_text[1];
        s_display_buffer[7][19] = value_display_text[2];
        s_display_buffer[7][20] = ']';
      }
    }

    uint8_t adc_control_target_2 = s_adc_control_target[2];
    if (adc_control_target_2 < 0xFF) {
      uint8_t adc_control_value        = s_adc_control_value[2];
      uint8_t current_controller_value = adc_control_value;
      if (adc_control_target_2 <= 0x7F) {
        current_controller_value = g_synth.current_controller_value(adc_control_target_2);
      }

      s_display_buffer[3][11] = 'C';
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

      char value_display_text[5] = {};
      boolean exists = PRA32_U_ControlPanel_calc_value_display(adc_control_target_2, current_controller_value, value_display_text);
      if (exists) {
        s_display_buffer[3][16] = '[';
        s_display_buffer[3][17] = value_display_text[0];
        s_display_buffer[3][18] = value_display_text[1];
        s_display_buffer[3][19] = value_display_text[2];
        s_display_buffer[3][20] = ']';
      }
    }
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

INLINE void PRA32_U_ControlPanel_update_display(uint32_t loop_counter) {
  static_cast<void>(loop_counter);

#if defined(PRA32_U_USE_CONTROL_PANEL)

#if defined(PRA32_U_USE_CONTROL_PANEL_OLED_DISPLAY)
  static uint32_t s_display_draw_position_x = 0;
  static uint32_t s_display_draw_position_y = 0;

  if ((loop_counter & 0x7F) == 0x40) {
    uint32_t display_draw_counter = s_display_draw_counter;

    s_display_draw_counter++;
    if (s_display_draw_counter == 8 * 21) {
      s_display_draw_counter = (6 * 21) + 11;
    }

    switch (display_draw_counter / 21) {
    case 0:
      s_display_draw_position_y = 1;
      break;
    case 1:
      s_display_draw_position_y = 2;
      break;
    case 2:
      s_display_draw_position_y = 3;
      break;
    case 3:
      s_display_draw_position_y = 5;
      break;
    case 4:
      s_display_draw_position_y = 6;
      break;
    case 5:
      s_display_draw_position_y = 7;
      break;
    case 6:
      s_display_draw_position_y = 3;
      break;
    case 7:
      s_display_draw_position_y = 7;
      break;
    }

    s_display_draw_position_x = display_draw_counter % 21;

    PRA32_U_ControlPanel_set_draw_position(s_display_draw_position_x, s_display_draw_position_y);
  } else if ((loop_counter & 0x7F) == 0x00) {
    PRA32_U_ControlPanel_draw_character(s_display_buffer[s_display_draw_position_y][s_display_draw_position_x]);
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
    Serial1.print(s_adc_current_value[0]);
    break;
  case 14 * 400:
    Serial1.print("\e[17;1H\e[K");
    Serial1.print(s_adc_current_value[1]);
    break;
  case 15 * 400:
    Serial1.print("\e[18;1H\e[K");
    Serial1.print(s_adc_current_value[2]);
    break;
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
#endif  // defined(PRA32_U_USE_DEBUG_PRINT)
}
