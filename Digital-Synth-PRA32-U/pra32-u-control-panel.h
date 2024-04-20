#pragma once

#include "pra32-u-common.h"
#include "pra32-u-control-panel-font-table.h"
#include "pra32-u-control-panel-page-table.h"

#include "hardware/i2c.h"

#include <cstdio>
#include <cstring>



#define NUMBER_OF_PAGES (sizeof(g_control_panel_page_table) / sizeof(g_control_panel_page_table[0]))

static volatile uint32_t s_current_page_index = PAGE_INDEX_DEFAULT;

static volatile int32_t  s_adc_current_value[3];
static volatile uint8_t  s_adc_control_value[3];
static volatile uint8_t  s_adc_control_target[3] = { 0xFF, 0xFF, 0xFF };
static volatile boolean  s_adc_control_catched[3];

static          uint32_t s_prev_key_current_value;
static          uint32_t s_next_key_current_value;
static          uint32_t s_play_key_current_value;

static volatile uint8_t  s_panel_play_pitch_value     = 60;
static volatile uint8_t  s_panel_play_scale_value     = 0;
static volatile uint8_t  s_panel_play_transpose_value = 64;
static volatile uint8_t  s_panel_play_note_number     = 60;
static volatile uint8_t  s_panel_playing_note_number  = 0xFF;
static volatile uint8_t  s_reserved_note_off          = 0xFF;
static volatile uint8_t  s_reserved_note_on           = 0xFF;

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

static INLINE boolean PRA32_U_ControlPanel_process_reserved_note_off_on() {
  if (s_reserved_note_on <= 127) {
    g_synth.note_on(s_reserved_note_on, 100);
    s_panel_playing_note_number = s_reserved_note_on;
    s_reserved_note_on = 0xFF;
    return true;
  }

  if (s_reserved_note_off <= 127) {
    g_synth.note_off(s_reserved_note_off);
    if (s_panel_playing_note_number == s_reserved_note_off) {
      s_panel_playing_note_number = 0xFF;
    }
    s_reserved_note_off = 0xFF;
    return true;
  }

  return false;
}

static INLINE void PRA32_U_ControlPanel_update_pitch() {
  int32_t new_note_number = s_panel_play_pitch_value;

  uint32_t index_scale = ((s_panel_play_scale_value * 4) + 127) / 254;
  if        (index_scale == 2) {
    const uint8_t ary_major[53] =
      { 48, 48, 48, 48, 48, 50, 50, 50, 50, 52, 52, 52, 53, 53, 53,
                    55, 55, 55, 55, 57, 57, 57, 57, 59, 59, 59, 60,
                    60, 60, 62, 62, 62, 62, 64, 64, 64, 65, 65, 65,
                    67, 67, 67, 67, 69, 69, 69, 69, 71, 71, 71, 72, 72, 72 };
    uint32_t index_pitch = (((s_panel_play_pitch_value + 3) * 2) + 1) / 5;
    new_note_number = ary_major[index_pitch];
  } else if (index_scale == 1) {
    const uint8_t ary_pentatonic[53] =
      { 48, 48, 48, 48, 48, 50, 50, 50, 50, 52, 52, 52, 52, 52, 55,
                    55, 55, 55, 55, 57, 57, 57, 57, 57, 60, 60, 60,
                    60, 60, 62, 62, 62, 62, 64, 64, 64, 64, 64, 67,
                    67, 67, 67, 67, 69, 69, 69, 69, 69, 72, 72, 72, 72, 72 };

    uint32_t index_pitch = (((s_panel_play_pitch_value + 3) * 2) + 1) / 5;
    new_note_number = ary_pentatonic[index_pitch];
  }

  new_note_number += s_panel_play_transpose_value - 64;
  if (new_note_number < 0) {
   new_note_number = 0;
  } else if (new_note_number > 127) {
   new_note_number = 127;
  }

  bool panel_play_note_number_changed = false;
  if (s_panel_play_note_number != new_note_number) {
    s_panel_play_note_number = new_note_number;
    panel_play_note_number_changed = true;
  }

  if (s_play_key_current_value == 1) {
    if (panel_play_note_number_changed) {
      s_reserved_note_off = s_panel_playing_note_number;
      s_reserved_note_on = s_panel_play_note_number;
      PRA32_U_ControlPanel_process_reserved_note_off_on();
    }
  }
}

static INLINE boolean PRA32_U_ControlPanel_update_control_adc(uint32_t adc_number) {
  uint8_t adc_control_value_candidate = PRA32_U_ControlPanel_adc_control_value_candidate(adc_number);

  if (s_adc_control_value[adc_number] != adc_control_value_candidate) {
    uint8_t s_adc_control_value_old = s_adc_control_value[adc_number];
    s_adc_control_value[adc_number] = adc_control_value_candidate;

    uint8_t current_controller_value = s_adc_control_value[adc_number];
    if        (s_adc_control_target[adc_number] <= 0x7F) {
      current_controller_value = g_synth.current_controller_value(s_adc_control_target[adc_number]);
    } else if (s_adc_control_target[adc_number] == PANEL_PITCH) {
      current_controller_value = s_panel_play_pitch_value;
    } else if (s_adc_control_target[adc_number] == PANEL_SCALE) {
      current_controller_value = s_panel_play_scale_value;
    } else if (s_adc_control_target[adc_number] == PANEL_TRANSPOSE) {
      current_controller_value = s_panel_play_transpose_value;
    }

    if ((s_adc_control_value_old <= current_controller_value) &&
        (s_adc_control_value[adc_number] >= current_controller_value)) {
      s_adc_control_catched[adc_number] = true;
    }

    if ((s_adc_control_value_old >= current_controller_value) &&
        (s_adc_control_value[adc_number] <= current_controller_value)) {
      s_adc_control_catched[adc_number] = true;
    }

    if (s_adc_control_target[adc_number] <= 0x7F) {
      if (s_adc_control_catched[adc_number]) {
        g_synth.control_change(s_adc_control_target[adc_number], s_adc_control_value[adc_number]);
      }
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
    } else if (s_adc_control_target[adc_number] == PANEL_PITCH) {
      if (s_adc_control_catched[adc_number]) {
        s_panel_play_pitch_value = s_adc_control_value[adc_number];
        PRA32_U_ControlPanel_update_pitch();
      }
    } else if (s_adc_control_target[adc_number] == PANEL_SCALE) {
      if (s_adc_control_catched[adc_number]) {
        s_panel_play_scale_value = s_adc_control_value[adc_number];
        PRA32_U_ControlPanel_update_pitch();
      }
    } else if (s_adc_control_target[adc_number] == PANEL_TRANSPOSE) {
      if (s_adc_control_catched[adc_number]) {
        s_panel_play_transpose_value = s_adc_control_value[adc_number];
        PRA32_U_ControlPanel_update_pitch();
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
  case OSC_2_COARSE    :
  case OSC_2_PITCH     :
  case FILTER_EG_AMT   :
  case EG_OSC_AMT      :
  case LFO_OSC_AMT     :
  case LFO_FILTER_AMT  :
  case BTH_FILTER_AMT  :
  case PANEL_TRANSPOSE :
    {
      std::sprintf(value_display_text, "%+3d", static_cast<int>(controller_value) - 64);
      result = true;
    }
    break;
  case MIXER_SUB_OSC   :
    {
      std::sprintf(value_display_text, "%+3d", static_cast<int>(controller_value) - 64);

      if        (controller_value < 55) {
        value_display_text[0] = 'N';
      } else if (controller_value < 64) {
        value_display_text[1] = 'N';
      } else if (controller_value < 74) {
        value_display_text[1] = 'S';
      } else {
        value_display_text[0] = 'S';
      }

      result = true;
    }
    break;
  case OSC_1_WAVE      :
    {
      char ary[6][5] = {"Saw","Sin","  -","Tri","  -","Pls"};
      uint32_t index = ((controller_value * 10) + 127) / 254;
      if (controller_value < 6) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case OSC_2_WAVE      :
    {
      char ary[6][5] = {"Saw","Sin","  -","Tri","Nos","Sqr"};
      uint32_t index = ((controller_value * 10) + 127) / 254;
      if (controller_value < 6) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case FILTER_KEY_TRK  :
    {
      char ary[3][5] = {"0.0","0.5","1.0"};
      uint32_t index = ((controller_value * 4) + 127) / 254;
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case EG_OSC_DST      :
  case LFO_OSC_DST     :
    {
      char ary[3][5] = {"  P"," 2P"," 1S"};
      uint32_t index = ((controller_value * 4) + 127) / 254;
      if (controller_value < 3) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case VOICE_MODE      :
    {
      char ary[6][5] = {"Pol","Par","  -","Mon"," LP","Lgt"};
      uint32_t index = ((controller_value * 10) + 127) / 254;
      if (controller_value < 6) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case LFO_WAVE        :
    {
      char ary[6][5] = {"Tri","Sin","  -","Saw"," SH","Sqr"};
      uint32_t index = ((controller_value * 10) + 127) / 254;
      if (controller_value < 6) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case FILTER_MODE     :
    {
      char ary[2][5] = {" LP"," HP"};
      uint32_t index = ((controller_value * 2) + 127) / 254;
      if (controller_value < 2) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case EG_AMP_MOD      :
  case REL_EQ_DECAY    :
  case SUSTAIN_PEDAL   :
    {
      char ary[2][5] = {"Off"," On"};
      uint32_t index = ((controller_value * 2) + 127) / 254;
      if (controller_value < 2) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case BTH_AMP_MOD     :
    {
      char ary[3][5] = {"Off","Qad","Lin"};
      uint32_t index = ((controller_value * 4) + 127) / 254;
      if (controller_value < 3) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case DELAY_MODE      :
    {
      char ary[2][5] = {"  S","  P"};
      uint32_t index = ((controller_value * 2) + 127) / 254;
      if (controller_value < 2) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case  PC_BY_PANEL_0  :
  case  PC_BY_PANEL_1  :
  case  PC_BY_PANEL_2  :
  case  PC_BY_PANEL_3  :
  case  PC_BY_PANEL_4  :
  case  PC_BY_PANEL_5  :
  case  PC_BY_PANEL_6  :
  case  PC_BY_PANEL_7  :
  case  PC_BY_PANEL_8  :
  case  PC_BY_PANEL_9  :
  case  PC_BY_PANEL_10 :
  case  PC_BY_PANEL_11 :
  case  PC_BY_PANEL_12 :
  case  PC_BY_PANEL_13 :
  case  PC_BY_PANEL_14 :
  case  PC_BY_PANEL_15 :
    {
      if        (controller_value < 64) {
        value_display_text[0] = 'R';
        value_display_text[1] = 'd';
        value_display_text[2] = 'y';
      } else {
        value_display_text[0] = 'E';
        value_display_text[1] = 'x';
        value_display_text[2] = 'e';
      }

      result = true;
    }
    break;
  case  WR_BY_PANEL_0  :
  case  WR_BY_PANEL_1  :
  case  WR_BY_PANEL_2  :
  case  WR_BY_PANEL_3  :
  case  WR_BY_PANEL_4  :
  case  WR_BY_PANEL_5  :
  case  WR_BY_PANEL_6  :
  case  WR_BY_PANEL_7  :
  case  WR_BY_PANEL_8  :
  case  WR_BY_PANEL_9  :
  case  WR_BY_PANEL_10 :
  case  WR_BY_PANEL_11 :
  case  WR_BY_PANEL_12 :
  case  WR_BY_PANEL_13 :
  case  WR_BY_PANEL_14 :
  case  WR_BY_PANEL_15 :
    {
      if        (controller_value == 0) {
        value_display_text[0] = 'R';
        value_display_text[1] = 'd';
        value_display_text[2] = 'y';
      } else if (controller_value == 127) {
        value_display_text[0] = 'E';
        value_display_text[1] = 'x';
        value_display_text[2] = 'e';
      } else {
        value_display_text[0] = ' ';
        value_display_text[1] = ' ';
        value_display_text[2] = '-';
      }

      result = true;
    }
    break;
  case  PANEL_PITCH    :
    {
      uint32_t index_scale = ((s_panel_play_scale_value * 4) + 127) / 254;
      if        (index_scale == 0) {
        char ary[12][5] = { " C", "C#", " D", "D#", " E", " F", "F#", " G", "G#", " A", "A#", " B" };

        uint32_t quotient  = controller_value / 12;
        uint32_t remainder = controller_value % 12;

        value_display_text[0] = ary[remainder][0];
        value_display_text[1] = ary[remainder][1];

        if (quotient == 0) {
          value_display_text[2] = '-';
        } else {
          value_display_text[2] = '0' + quotient - 1;
        }
      } else if (index_scale == 2) {
       char ary_major[53][5] =
          { " C3", " C3", " C3", " C3", " C3", " D3", " D3", " D3", " D3", " E3", " E3", " E3", " F3", " F3", " F3",
                                 " G3", " G3", " G3", " G3", " A3", " A3", " A3", " A3", " B3", " B3", " B3", " C4",
                                 " C4", " C4", " D4", " D4", " D4", " D4", " E4", " E4", " E4", " F4", " F4", " F4",
                                 " G4", " G4", " G4", " G4", " A4", " A4", " A4", " A4", " B4", " B4", " B4", " C5", " C5", " C5" };
        uint32_t index = (((s_panel_play_pitch_value + 3) * 2) + 1) / 5;
        std::strcpy(value_display_text, ary_major[index]);
      } else if (index_scale == 1) {
       char ary_pentatonic[53][5] =
          { " C3", " C3", " C3", " C3", " C3", " D3", " D3", " D3", " D3", " E3", " E3", " E3", " E3", " E3", " G3",
                                 " G3", " G3", " G3", " G3", " A3", " A3", " A3", " A3", " A3", " C4", " C4", " C4",
                                 " C4", " C4", " D4", " D4", " D4", " D4", " E4", " E4", " E4", " E4", " E4", " G4",
                                 " G4", " G4", " G4", " G4", " A4", " A4", " A4", " A4", " A4", " C4", " C5", " C5", " C5", " C5" };
        uint32_t index = (((s_panel_play_pitch_value + 3) * 2) + 1) / 5;
        std::strcpy(value_display_text, ary_pentatonic[index]);
      }

      result = true;
    }
    break;
  case  PANEL_SCALE    :
    {
      char ary[3][5] = {"Ful","Pen","Maj"};
      uint32_t index = ((controller_value * 4) + 127) / 254;
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

  boolean processed = PRA32_U_ControlPanel_process_reserved_note_off_on();
  if (processed) {
    return;
  }

#if defined(PRA32_U_USE_CONTROL_PANEL_KEY_INPUT)
  static uint32_t s_prev_key_value_changed_time;
  static uint32_t s_next_key_value_changed_time;
  static uint32_t s_play_key_value_changed_time;

  static uint32_t s_key_inpuy_counter = 0;
  ++s_key_inpuy_counter;

#if defined(PRA32_U_KEY_INPUT_PREV_KEY_PIN)
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

        s_adc_control_catched[0] = false;
        s_adc_control_catched[1] = false;
        s_adc_control_catched[2] = false;
      }
      return;
    }
  }
#endif  // defined(PRA32_U_KEY_INPUT_PREV_KEY_PIN)

#if defined(PRA32_U_KEY_INPUT_NEXT_KEY_PIN)
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

        s_adc_control_catched[0] = false;
        s_adc_control_catched[1] = false;
        s_adc_control_catched[2] = false;
      }
      return;
    }
  }
#endif  // defined(PRA32_U_KEY_INPUT_NEXT_KEY_PIN)

#if defined(PRA32_U_KEY_INPUT_PLAY_KEY_PIN)
  if (s_key_inpuy_counter - s_play_key_value_changed_time >= PRA32_U_KEY_ANTI_CHATTERING_WAIT) {
    uint32_t value = digitalRead(PRA32_U_KEY_INPUT_PLAY_KEY_PIN) == PRA32_U_KEY_INPUT_ACTIVE_LEVEL;
    if (s_play_key_current_value != value) {
      s_play_key_current_value = value;
      s_play_key_value_changed_time = s_key_inpuy_counter;

      if (s_play_key_current_value == 1) {
        // Play key pressed
        s_reserved_note_on = s_panel_play_note_number;
      } else {
        // Play key released
        s_reserved_note_off = s_panel_playing_note_number;
      }

      PRA32_U_ControlPanel_process_reserved_note_off_on();
      return;
    }
  }
#endif  // defined(PRA32_U_KEY_INPUT_PLAY_KEY_PIN)

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
      if        (adc_control_target_0 <= 0x7F) {
        current_controller_value = g_synth.current_controller_value(adc_control_target_0);
      } else if (adc_control_target_0 == PANEL_PITCH) {
        current_controller_value = s_panel_play_pitch_value;
      } else if (adc_control_target_0 == PANEL_SCALE) {
        current_controller_value = s_panel_play_scale_value;
      } else if (adc_control_target_0 == PANEL_TRANSPOSE) {
        current_controller_value = s_panel_play_transpose_value;
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
      if        (adc_control_target_1 <= 0x7F) {
        current_controller_value = g_synth.current_controller_value(adc_control_target_1);
      } else if (adc_control_target_1 == PANEL_PITCH) {
        current_controller_value = s_panel_play_pitch_value;
      } else if (adc_control_target_1 == PANEL_SCALE) {
        current_controller_value = s_panel_play_scale_value;
      } else if (adc_control_target_1 == PANEL_TRANSPOSE) {
        current_controller_value = s_panel_play_transpose_value;
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
      if        (adc_control_target_2 <= 0x7F) {
        current_controller_value = g_synth.current_controller_value(adc_control_target_2);
      } else if (adc_control_target_2 == PANEL_PITCH) {
        current_controller_value = s_panel_play_pitch_value;
      } else if (adc_control_target_2 == PANEL_SCALE) {
        current_controller_value = s_panel_play_scale_value;
      } else if (adc_control_target_2 == PANEL_TRANSPOSE) {
        current_controller_value = s_panel_play_transpose_value;
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

void PRA32_U_ControlPanel_on_control_change(uint8_t control_number)
{
#if defined(PRA32_U_USE_CONTROL_PANEL)
  if (s_adc_control_target[0] == control_number) {
    s_adc_control_catched[0] = false;
  }

  if (s_adc_control_target[1] == control_number) {
    s_adc_control_catched[1] = false;
  }

  if (s_adc_control_target[2] == control_number) {
    s_adc_control_catched[2] = false;
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_ANALOG_INPUT)
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
