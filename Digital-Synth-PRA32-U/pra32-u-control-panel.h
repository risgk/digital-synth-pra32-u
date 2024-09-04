#pragma once

#include "pra32-u-common.h"
#include "pra32-u-control-panel-font-table.h"
#include "pra32-u-control-panel-page-table.h"

#include "hardware/i2c.h"

#include <cstdio>
#include <cstring>



static volatile uint32_t s_current_page_group   = PAGE_GROUP_DEFAULT;
static volatile uint32_t s_current_page_index[] = { PAGE_INDEX_DEFAULT_A, PAGE_INDEX_DEFAULT_B, PAGE_INDEX_DEFAULT_C, PAGE_INDEX_DEFAULT_D };

static volatile int32_t  s_adc_current_value[3];
static volatile uint8_t  s_adc_control_value[3];
static volatile uint8_t  s_adc_control_target[3] = { 0xFF, 0xFF, 0xFF };
static volatile boolean  s_adc_control_catched[3];

#if defined(PRA32_U_USE_CONTROL_PANEL)
static          uint32_t s_prev_key_current_value;
static          uint32_t s_next_key_current_value;
#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
static          uint32_t s_play_key_current_value;

static          uint8_t  s_play_mode       = 0;
static          int8_t   s_panel_transpose = 0;
static          int8_t   s_seq_transpose   = 0;
static          uint8_t  s_seq_gate_time   = 6;
static          uint8_t  s_seq_last_step   = 7;
static          uint8_t  s_seq_pattern     = 0;
static          int8_t   s_seq_pattern_dir = +1;
static          uint8_t  s_seq_act_steps   = 127;

static          uint32_t s_index_scale;

enum PlayingStatus {
  PlayingStatus_Stop = 0,
  PlayingStatus_Playing,
  PlayingStatus_Seq,
};

static          uint32_t s_playing_status = PlayingStatus_Stop;

static          int32_t  s_seq_step               = 7;
static          uint32_t s_seq_sub_step           = 23;
static          uint32_t s_seq_count              = 0;
static          uint32_t s_seq_count_increment    = 0;
static          bool     s_seq_clock_src_external = false;

static volatile uint8_t  s_panel_play_note_pitch    = 60;
static volatile uint8_t  s_panel_play_note_velocity = 127;
static volatile bool     s_panel_play_note_gate     = false;
static volatile bool     s_panel_play_note_trigger  = false;

static volatile int32_t s_display_draw_counter = 0;

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


static INLINE uint32_t PRA32_U_ControlPanel_calc_bpm(uint8_t tempo_control_value) {
  uint32_t bpm = tempo_control_value + 56;

  if (bpm < 82) {
    bpm -= 82 - bpm;
  }

  if (bpm > 126) {
    bpm += bpm - 126;
  }

  if (bpm > 180) {
    bpm += bpm - 180;
  }

  return bpm;
}

static INLINE void PRA32_U_ControlPanel_update_page() {
  s_adc_control_catched[0] = false;
  s_adc_control_catched[1] = false;
  s_adc_control_catched[2] = false;

  PRA32_U_ControlPanelPage current_page = g_control_panel_page_table[s_current_page_group][s_current_page_index[s_current_page_group]];

  if (s_play_mode == 1) {  // Seq Mode
    std::memcpy(current_page.control_target_c_name_line_0, "Seq       ", 10);
    std::memcpy(current_page.control_target_c_name_line_1, "Transpose ", 10);
    current_page.control_target_c = SEQ_TRANSPOSE;
  }

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

  s_display_draw_counter = -1;
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

static INLINE boolean PRA32_U_ControlPanel_process_note_off_on() {
  static uint8_t s_panel_playing_note_pitch = 0xFF;

  if (s_panel_playing_note_pitch <= 127) {
    if (s_panel_play_note_gate == false) {
      g_synth.note_off(s_panel_playing_note_pitch);
      s_panel_playing_note_pitch = 0xFF;
    } else {
      if (s_panel_play_note_trigger) {
        s_panel_play_note_trigger = false;

        g_synth.note_on(s_panel_play_note_pitch, s_panel_play_note_velocity);
        g_synth.note_off(s_panel_playing_note_pitch);
        s_panel_playing_note_pitch = s_panel_play_note_pitch;
      }
    }
  } else {
    if (s_panel_play_note_gate) {
      s_panel_play_note_trigger = false;

      g_synth.note_on(s_panel_play_note_pitch, s_panel_play_note_velocity);
      s_panel_playing_note_pitch = s_panel_play_note_pitch;
    }
  }

  return false;
}

static INLINE void PRA32_U_ControlPanel_update_pitch(bool progress_seq_step) {
  int32_t new_pitch    = 60;
  uint8_t new_velocity = 127;

  if (s_play_mode == 0) {  // Normal Mode
    new_pitch         = g_synth.current_controller_value(PANEL_PLAY_PIT );
    new_velocity      = g_synth.current_controller_value(PANEL_PLAY_VELO);

    s_index_scale     = ((g_synth.current_controller_value(PANEL_SCALE) * 4) + 127) / 254;
    s_panel_transpose = g_synth.current_controller_value(PANEL_TRANSPOSE) - 64;
    s_seq_transpose   = 0;
  } else {  // Seq Mode
    new_pitch         = g_synth.current_controller_value(SEQ_PITCH_0 + s_seq_step);
    new_velocity      = g_synth.current_controller_value(SEQ_VELO_0  + s_seq_step);
  }

  uint32_t index_scale = s_index_scale;

  if        (index_scale == 0) {
    const uint8_t ary_major[53] =
      { 48, 48, 48, 48, 48, 50, 50, 50, 50, 52, 52, 52, 53, 53, 53,
                    55, 55, 55, 55, 57, 57, 57, 57, 59, 59, 59, 60,
                    60, 60, 62, 62, 62, 62, 64, 64, 64, 65, 65, 65,
                    67, 67, 67, 67, 69, 69, 69, 69, 71, 71, 71, 72, 72, 72 };
    uint32_t index_pitch = (((new_pitch + 3) * 2) + 1) / 5;
    new_pitch = ary_major[index_pitch];
  } else if (index_scale == 1) {
    const uint8_t ary_minor[53] =
      { 48, 48, 48, 48, 48, 50, 50, 50, 51, 51, 51, 53, 53, 53, 53,
                    55, 55, 55, 56, 56, 56, 58, 58, 58, 58, 60, 60,
                    60, 60, 62, 62, 62, 63, 63, 63, 65, 65, 65, 65,
                    67, 67, 67, 68, 68, 68, 70, 70, 70, 70, 72, 72, 72, 72 };
    uint32_t index_pitch = (((new_pitch + 3) * 2) + 1) / 5;
    new_pitch = ary_minor[index_pitch];
  } else if (index_scale == 2) {
    const uint8_t ary_chromatic[53] =
      { 48, 48, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54,
                    54, 55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 60,
                    60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66,
                    66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 72 };
    uint32_t index_pitch = (((new_pitch + 3) * 2) + 1) / 5;
    new_pitch = ary_chromatic[index_pitch];
  }

  new_pitch += s_panel_transpose + s_seq_transpose;

  if (new_pitch < 0) {
    new_pitch = 0;
  } else if (new_pitch > 127) {
    new_pitch = 127;
  }

  s_panel_play_note_velocity = new_velocity;

  bool panel_play_note_number_changed = false;

  if (s_panel_play_note_pitch != new_pitch) {
    s_panel_play_note_pitch = new_pitch;
    panel_play_note_number_changed = true;
  }

  if (((s_playing_status == PlayingStatus_Playing) && panel_play_note_number_changed) ||
      ((s_playing_status == PlayingStatus_Seq) && progress_seq_step)) {
    s_panel_play_note_gate    = true;
    s_panel_play_note_trigger = true;
  }
}

static INLINE void PRA32_U_ControlPanel_seq_clock() {
#if defined(PRA32_U_USE_USB_MIDI)
      USB_MIDI.sendRealTime(midi::Clock);
#endif  // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
      UART_MIDI.sendRealTime(midi::Clock);
#endif  // defined(PRA32_U_USE_UART_MIDI)

  ++s_seq_sub_step;

  if (s_seq_sub_step >= 12) {
    s_seq_sub_step = 0;

    do {
      if (s_seq_pattern == 0) {  // Forward
        ++s_seq_step;

        if (s_seq_step > s_seq_last_step) {
          s_seq_step = 0;
        }

        s_seq_pattern_dir = +1;
      } else if (s_seq_pattern == 1) {  // Reverse
        --s_seq_step;

        if (s_seq_step < 0) {
          s_seq_step = s_seq_last_step;
        }

        s_seq_pattern_dir = -1;
      } else {  // Bounce
        if (s_seq_pattern_dir > 0) {
          ++s_seq_step;

          if (s_seq_step > s_seq_last_step) {
            s_seq_step = s_seq_last_step;
            s_seq_pattern_dir = -1;
          } else {
            s_seq_pattern_dir = +1;
          }
        } else {
          --s_seq_step;

          if (s_seq_step < 0) {
            s_seq_step = 0;
            s_seq_pattern_dir = +1;
          } else {
            s_seq_pattern_dir = -1;
          }
        }
      }
    } while ((s_seq_step != 0) && (((1 << (s_seq_step - 1)) & s_seq_act_steps) == 0));

    s_display_buffer[0][20] = '0' + s_seq_step;

    if (s_seq_step == 0) {
      s_index_scale     = ((g_synth.current_controller_value(PANEL_SCALE) * 4) + 127) / 254;
      s_panel_transpose = g_synth.current_controller_value(PANEL_TRANSPOSE) - 64;

      int seq_transpose_value = g_synth.current_controller_value(SEQ_TRANSPOSE);
      if (seq_transpose_value < 2) {
        seq_transpose_value = 2;
      } else if (seq_transpose_value > 126) {
        seq_transpose_value = 126;
      }
      seq_transpose_value = ((seq_transpose_value - 2) / 5) - 12;
      s_seq_transpose = seq_transpose_value;
    }

    PRA32_U_ControlPanel_update_pitch(true);
  } else  if (s_seq_sub_step == (static_cast<uint32_t>(s_seq_gate_time) << 1)) {
    s_panel_play_note_gate = false;
  }
}

static INLINE void PRA32_U_ControlPanel_seq_start() {
#if defined(PRA32_U_USE_USB_MIDI)
  USB_MIDI.sendRealTime(midi::Start);
#endif  // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
  UART_MIDI.sendRealTime(midi::Start);
#endif  // defined(PRA32_U_USE_UART_MIDI)

  s_playing_status = PlayingStatus_Seq;

  if (s_seq_pattern == 0) {  // Forward
    s_seq_step = 7;
    s_seq_pattern_dir = +1;
  } else if (s_seq_pattern == 1) {  // Reverse
    s_seq_step = 0;
    s_seq_pattern_dir = -1;
  } else {  // Bounce
    s_seq_step = 0;
    s_seq_pattern_dir = -1;
  }

  s_seq_sub_step = 23;
  s_seq_count    = 0;
  s_panel_play_note_gate = false;
}

static INLINE void PRA32_U_ControlPanel_seq_stop() {
#if defined(PRA32_U_USE_USB_MIDI)
  USB_MIDI.sendRealTime(midi::Stop);
#endif  // defined(PRA32_U_USE_USB_MIDI)

#if defined(PRA32_U_USE_UART_MIDI)
  UART_MIDI.sendRealTime(midi::Stop);
#endif  // defined(PRA32_U_USE_UART_MIDI)

  s_playing_status = PlayingStatus_Stop;
  s_display_buffer[0][20] = ' ';
  s_panel_play_note_gate = false;
}

static INLINE void PRA32_U_ControlPanel_update_control_seq() {
  if ((s_playing_status == PlayingStatus_Seq) && (s_seq_clock_src_external == false)) {
    s_seq_count += s_seq_count_increment;

    if (s_seq_count >= 7680000 * 2) {
      s_seq_count -= 7680000 * 2;

      PRA32_U_ControlPanel_seq_clock();
    }
  }
}

static INLINE boolean PRA32_U_ControlPanel_update_control_adc(uint32_t adc_number) {
  uint8_t adc_control_value_candidate = PRA32_U_ControlPanel_adc_control_value_candidate(adc_number);

  if (s_adc_control_value[adc_number] != adc_control_value_candidate) {
    uint8_t s_adc_control_value_old = s_adc_control_value[adc_number];
    s_adc_control_value[adc_number] = adc_control_value_candidate;

    uint8_t current_controller_value = s_adc_control_value[adc_number];
    if        (s_adc_control_target[adc_number] < 128 + 64) {
      current_controller_value = g_synth.current_controller_value(s_adc_control_target[adc_number]);
    }

    if ((s_adc_control_value_old <= current_controller_value) &&
        (s_adc_control_value[adc_number] >= current_controller_value)) {
      s_adc_control_catched[adc_number] = true;
    }

    if ((s_adc_control_value_old >= current_controller_value) &&
        (s_adc_control_value[adc_number] <= current_controller_value)) {
      s_adc_control_catched[adc_number] = true;
    }

    if (s_adc_control_target[adc_number] < 128 + 64) {
      if (s_adc_control_catched[adc_number]) {
        g_synth.control_change(s_adc_control_target[adc_number], s_adc_control_value[adc_number]);
      }
    } else if ((s_adc_control_target[adc_number] >= RD_PROGRAM_0) && (s_adc_control_target[adc_number] <= RD_PROGRAM_15)) {
      if ((s_adc_control_value_old < 64) && (s_adc_control_value[adc_number] >= 64)) {
        g_synth.program_change(s_adc_control_target[adc_number] - RD_PROGRAM_0);
      }
    } else if ((s_adc_control_target[adc_number] >= WR_PROGRAM_0) && (s_adc_control_target[adc_number] <= WR_PROGRAM_15)) {
      if ((s_adc_control_value_old < 64) && (s_adc_control_value[adc_number] >= 64)) {
        uint8_t program_number_to_write = s_adc_control_target[adc_number] - WR_PROGRAM_0;
        g_synth.write_parameters_to_program(program_number_to_write);
      }
    } else if (s_adc_control_target[adc_number] == RD_PANEL_PRMS) {
      if ((s_adc_control_value_old < 64) && (s_adc_control_value[adc_number] >= 64)) {
        g_synth.program_change(128);
      }
    } else if (s_adc_control_target[adc_number] == IN_PANEL_PRMS) {
      if ((s_adc_control_value_old < 64) && (s_adc_control_value[adc_number] >= 64)) {
        g_synth.program_change(129);
      }
    } else if (s_adc_control_target[adc_number] == WR_PANEL_PRMS) {
      if ((s_adc_control_value_old < 64) && (s_adc_control_value[adc_number] >= 64)) {
        g_synth.write_parameters_to_program(128);
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
  case SEQ_TRANSPOSE   :
    {
      int seq_transpose_value = controller_value;
      if (seq_transpose_value < 2) {
        seq_transpose_value = 2;
      } else if (seq_transpose_value > 126) {
        seq_transpose_value = 126;
      }
      seq_transpose_value = ((seq_transpose_value - 2) / 5) - 12;
      std::sprintf(value_display_text, "%+3d", seq_transpose_value);
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
  case  RD_PROGRAM_0   :
  case  RD_PROGRAM_1   :
  case  RD_PROGRAM_2   :
  case  RD_PROGRAM_3   :
  case  RD_PROGRAM_4   :
  case  RD_PROGRAM_5   :
  case  RD_PROGRAM_6   :
  case  RD_PROGRAM_7   :
  case  RD_PROGRAM_8   :
  case  RD_PROGRAM_9   :
  case  RD_PROGRAM_10  :
  case  RD_PROGRAM_11  :
  case  RD_PROGRAM_12  :
  case  RD_PROGRAM_13  :
  case  RD_PROGRAM_14  :
  case  RD_PROGRAM_15  :
  case  WR_PROGRAM_0   :
  case  WR_PROGRAM_1   :
  case  WR_PROGRAM_2   :
  case  WR_PROGRAM_3   :
  case  WR_PROGRAM_4   :
  case  WR_PROGRAM_5   :
  case  WR_PROGRAM_6   :
  case  WR_PROGRAM_7   :
  case  WR_PROGRAM_8   :
  case  WR_PROGRAM_9   :
  case  WR_PROGRAM_10  :
  case  WR_PROGRAM_11  :
  case  WR_PROGRAM_12  :
  case  WR_PROGRAM_13  :
  case  WR_PROGRAM_14  :
  case  WR_PROGRAM_15  :
  case  RD_PANEL_PRMS  :
  case  IN_PANEL_PRMS  :
  case  WR_PANEL_PRMS  :
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
  case  PANEL_PLAY_PIT :
  case  SEQ_PITCH_0    :
  case  SEQ_PITCH_1    :
  case  SEQ_PITCH_2    :
  case  SEQ_PITCH_3    :
  case  SEQ_PITCH_4    :
  case  SEQ_PITCH_5    :
  case  SEQ_PITCH_6    :
  case  SEQ_PITCH_7    :
    {
      uint32_t index_scale = ((g_synth.current_controller_value(PANEL_SCALE) * 4) + 127) / 254;

      if (index_scale == 0) {
       char ary_major[53][5] =
          { " C3", " C3", " C3", " C3", " C3", " D3", " D3", " D3", " D3", " E3", " E3", " E3", " F3", " F3", " F3",
                                 " G3", " G3", " G3", " G3", " A3", " A3", " A3", " A3", " B3", " B3", " B3", " C4",
                                 " C4", " C4", " D4", " D4", " D4", " D4", " E4", " E4", " E4", " F4", " F4", " F4",
                                 " G4", " G4", " G4", " G4", " A4", " A4", " A4", " A4", " B4", " B4", " B4", " C5", " C5", " C5" };
        uint32_t index = (((g_synth.current_controller_value(control_target) + 3) * 2) + 1) / 5;
        std::strcpy(value_display_text, ary_major[index]);
      } else if (index_scale == 1) {
       char ary_minor[53][5] =
          { " C3", " C3", " C3", " C3", " C3", " D3", " D3", " D3", "D#3", "D#3", "D#3", " F3", " F3", " F3", " F3",
                                 " G3", " G3", " G3", "G#3", "G#3", "G#3", "A#3", "A#3", "A#3", "A#3", " C4", " C4",
                                 " C4", " C4", " D4", " D4", " D4", "D#4", "D#4", "D#4", " F4", " F4", " F4", " F4",
                                 " G4", " G4", " G4", "G#4", "G#4", "G#4", "A#4", "A#4", "A#4", "A#4", " C4", " C5", " C5", " C5" };
        uint32_t index = (((g_synth.current_controller_value(control_target) + 3) * 2) + 1) / 5;
        std::strcpy(value_display_text, ary_minor[index]);
      } else if (index_scale == 2) {
       char ary_chromatic[53][5] =
          { " C3", " C3", " C3", " C3", "C#3", "C#3", " D3", " D3", "D#3", "D#3", " E3", " E3", " F3", " F3", "F#3",
                                 "F#3", " G3", " G3", "G#3", "G#3", " A3", " A3", "A#3", "A#3", " B3", " B3", " C4",
                                 " C4", "C#4", "C#4", " D4", " D4", "D#4", "D#4", " E4", " E4", " F4", " F4", "F#4",
                                 "F#4", " G4", " G4", "G#4", "G#4", " A4", " A4", "A#4", "A#4", " B4", " B4", " C5", " C5", " C5" };
        uint32_t index = (((g_synth.current_controller_value(control_target) + 3) * 2) + 1) / 5;
        std::strcpy(value_display_text, ary_chromatic[index]);
      }

      result = true;
    }
    break;
  case  PANEL_SCALE   :
    {
      char ary[6][5] = {"Maj","Min","Chr"};
      uint32_t index = ((controller_value * 4) + 127) / 254;
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case PANEL_PLAY_MODE:
    {
      char ary[2][5] = {"Nrm","Seq"};
      uint32_t index = ((controller_value * 2) + 127) / 254;
      if (controller_value < 2) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case SEQ_TEMPO      :
    {
      uint32_t bpm = PRA32_U_ControlPanel_calc_bpm(g_synth.current_controller_value(SEQ_TEMPO      ));
      std::sprintf(value_display_text, "%3lu", bpm);
      result = true;
    }
    break;
  case SEQ_CLOCK_SRC  :
    {
      if        (controller_value < 64) {
        value_display_text[0] = 'I';
        value_display_text[1] = 'n';
        value_display_text[2] = 't';
      } else {
        value_display_text[0] = 'E';
        value_display_text[1] = 'x';
        value_display_text[2] = 't';
      }

      result = true;
    }
    break;
  case SEQ_GATE_TIME  :
    {
      char ary[6][5] = {"1/6","2/6","3/6","4/6","5/6","6/6"};
      uint32_t index = ((controller_value * 10) + 127) / 254;
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case SEQ_LAST_STEP  :
    {
      uint8_t last_step = g_synth.current_controller_value(SEQ_LAST_STEP  );
      last_step = (last_step + 8) >> 4;
      if (last_step > 7) { last_step = 7; }
      std::sprintf(value_display_text, "%3d", last_step);
      result = true;
    }
    break;
  case SEQ_PATTERN    :
    {
      char ary[3][5] = {"Fwd","Rvs","Bnc"};
      uint32_t index = ((controller_value * 4) + 127) / 254;
      if (controller_value < 2) { index = controller_value; }
      std::strcpy(value_display_text, ary[index]);
      result = true;
    }
    break;
  case SEQ_ACT_STEPS  :
    {
      uint8_t act_steps = g_synth.current_controller_value(SEQ_ACT_STEPS  );
      std::sprintf(value_display_text, "x%02X", act_steps);
      result = true;
    }
    break;
  case PANEL_MIDI_CH  :
    {
      uint8_t midi_ch = g_synth.current_controller_value(PANEL_MIDI_CH  );

      if (midi_ch > 15) {
        midi_ch = 15;
      }

      std::sprintf(value_display_text, "%3d", midi_ch + 1);
      result = true;
    }
    break;
  }

  return result;
}



INLINE void PRA32_U_ControlPanel_setup() {

#if defined(PRA32_U_KEY_INPUT_PREV_KEY_PIN)
  pinMode(PRA32_U_KEY_INPUT_PREV_KEY_PIN, PRA32_U_KEY_INPUT_PIN_MODE);
#endif  // defined(PRA32_U_KEY_INPUT_PREV_KEY_PIN)

#if defined(PRA32_U_KEY_INPUT_NEXT_KEY_PIN)
  pinMode(PRA32_U_KEY_INPUT_NEXT_KEY_PIN, PRA32_U_KEY_INPUT_PIN_MODE);
#endif  // defined(PRA32_U_KEY_INPUT_NEXT_KEY_PIN)

#if defined(PRA32_U_KEY_INPUT_PLAY_KEY_PIN)
  pinMode(PRA32_U_KEY_INPUT_PLAY_KEY_PIN, PRA32_U_KEY_INPUT_PIN_MODE);
#endif  // defined(PRA32_U_KEY_INPUT_PLAY_KEY_PIN)

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

INLINE void PRA32_U_ControlPanel_initialize_parameters() {
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

  PRA32_U_ControlPanel_update_control_seq();

  boolean processed = PRA32_U_ControlPanel_process_note_off_on();
  if (processed) {
    return;
  }

#if defined(PRA32_U_USE_CONTROL_PANEL_KEY_INPUT)
  static uint32_t s_prev_key_value_changed_time = 0;
  static uint32_t s_next_key_value_changed_time = 0;
  static uint32_t s_play_key_value_changed_time = 0;

  static uint32_t s_prev_key_long_preesed = false;
  static uint32_t s_next_key_long_preesed = false;

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
        if (s_prev_key_long_preesed == false) {
          if (s_current_page_index[s_current_page_group] == 0) {
            s_current_page_index[s_current_page_group] = g_number_of_pages[s_current_page_group] - 1;
          } else {
            --s_current_page_index[s_current_page_group];
          }

          PRA32_U_ControlPanel_update_page();
        }
        s_prev_key_long_preesed = false;
        return;
      }
    }

    if (s_prev_key_current_value == 1) {
      if (s_prev_key_long_preesed == false) {
        if (s_key_inpuy_counter - s_prev_key_value_changed_time >= PRA32_U_KEY_LONG_PRESS_WAIT) {
          s_prev_key_long_preesed = true;

          if (s_current_page_group == 0) {
            s_current_page_group = NUMBER_OF_PAGE_GROUPS - 1;
          } else {
            --s_current_page_group;
          }

          PRA32_U_ControlPanel_update_page();
          return;
        }
      }
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
        if (s_next_key_long_preesed == false) {
          if (s_current_page_index[s_current_page_group] == g_number_of_pages[s_current_page_group] - 1) {
            s_current_page_index[s_current_page_group] = 0;
          } else {
            ++s_current_page_index[s_current_page_group];
          }

          PRA32_U_ControlPanel_update_page();
        }
        s_next_key_long_preesed = false;
        return;
      }
    }

    if (s_next_key_current_value == 1) {
      if (s_next_key_long_preesed == false) {
        if (s_key_inpuy_counter - s_next_key_value_changed_time >= PRA32_U_KEY_LONG_PRESS_WAIT) {
          s_next_key_long_preesed = true;

          if (s_current_page_group == NUMBER_OF_PAGE_GROUPS - 1) {
            s_current_page_group = 0;
          } else {
            ++s_current_page_group;
          }

          PRA32_U_ControlPanel_update_page();
          return;
        }
      }
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
        if (s_play_mode == 0) {  // Normal Mode
          s_playing_status = PlayingStatus_Playing;
          s_display_buffer[0][20] = '*';
          s_panel_play_note_gate    = true;
          s_panel_play_note_trigger = true;
        }
      } else {
        // Play key released
        if (s_play_mode == 0) {  // Normal Mode
          s_playing_status = PlayingStatus_Stop;
          s_display_buffer[0][20] = ' ';
          s_panel_play_note_gate = false;
        } else {  // Seq Mode
          if (s_playing_status == PlayingStatus_Stop) {
            PRA32_U_ControlPanel_seq_start();
          } else {
            PRA32_U_ControlPanel_seq_stop();
          }
        }
      }

      PRA32_U_ControlPanel_update_page();
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


INLINE void PRA32_U_ControlPanel_on_clock()
{
  if (s_seq_clock_src_external) {
    if (s_playing_status == PlayingStatus_Seq) {
      PRA32_U_ControlPanel_seq_clock();
    }
  }
}

INLINE void PRA32_U_ControlPanel_on_start()
{
  if (s_seq_clock_src_external) {
    if (s_play_mode == 1) {  // Seq Mode
      if (s_playing_status == PlayingStatus_Stop) {
        PRA32_U_ControlPanel_seq_start();
      }
    }
  }

  PRA32_U_ControlPanel_update_page();
}

INLINE void PRA32_U_ControlPanel_on_stop()
{
  if (s_seq_clock_src_external) {
    if (s_play_mode == 1) {  // Seq Mode
      if (s_playing_status != PlayingStatus_Stop) {
        PRA32_U_ControlPanel_seq_stop();
      }
    }
  }

  PRA32_U_ControlPanel_update_page();
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
      if        (adc_control_target_0 < 128 + 64) {
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
      if        (adc_control_target_1 < 128 + 64) {
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
      if        (adc_control_target_2 < 128+64) {
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
    int32_t display_draw_counter = s_display_draw_counter;

    s_display_draw_counter++;
    if (s_display_draw_counter == (8 * 22) + 0) {
      s_display_draw_counter = (6 * 22) + 10;
    } else if (s_display_draw_counter == 0) {
      s_display_draw_position_y = 0;
      s_display_draw_position_x = 20;
    } else {
      switch (display_draw_counter / 22) {
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

      s_display_draw_position_x = display_draw_counter % 22;

      if (s_display_draw_position_x == 0) {
        s_display_draw_position_y = 0;
        s_display_draw_position_x = 20;
      } else if (s_display_draw_position_x == 11) {
        s_display_draw_position_y = 0;
        s_display_draw_position_x = 20;
      } else {
        --s_display_draw_position_x;
      }
    }

    PRA32_U_ControlPanel_set_draw_position(s_display_draw_position_x, s_display_draw_position_y);
  } else if ((loop_counter & 0x7F) == 0x00) {
    PRA32_U_ControlPanel_draw_character(s_display_buffer[s_display_draw_position_y][s_display_draw_position_x]);
  }
#endif  // defined(PRA32_U_USE_CONTROL_PANEL_OLED_DISPLAY)

#endif  // defined(PRA32_U_USE_CONTROL_PANEL)
}

void PRA32_U_ControlPanel_on_control_change(uint8_t control_number)
{
  static_cast<void>(control_number);

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

  if ((control_number == PANEL_PLAY_PIT ) ||
      (control_number == PANEL_PLAY_VELO) ||
      (control_number == PANEL_SCALE    ) ||
      (control_number == PANEL_TRANSPOSE)) {
    PRA32_U_ControlPanel_update_pitch(false);
  } else if (control_number == PANEL_PLAY_MODE) {
    uint8_t play_mode_cc_value = g_synth.current_controller_value(PANEL_PLAY_MODE);
    uint8_t new_play_mode = (play_mode_cc_value >= 64);

    if (s_play_mode != new_play_mode) {
      s_play_mode = new_play_mode;

      s_playing_status = PlayingStatus_Stop;
      s_display_buffer[0][20] = ' ';
      s_panel_play_note_gate = false;

      PRA32_U_ControlPanel_update_pitch(false);
      PRA32_U_ControlPanel_update_page();
    }
  } else if (control_number == SEQ_TEMPO) {
    uint32_t bpm = PRA32_U_ControlPanel_calc_bpm(g_synth.current_controller_value(SEQ_TEMPO));
    s_seq_count_increment = bpm * 8192;
  } else if (control_number == SEQ_CLOCK_SRC) {
    uint32_t seq_clock_src = g_synth.current_controller_value(SEQ_CLOCK_SRC  );
    s_seq_clock_src_external = (seq_clock_src >= 64);
  } else if (control_number == SEQ_GATE_TIME) {
    uint8_t controller_value = g_synth.current_controller_value(SEQ_GATE_TIME  );
    s_seq_gate_time = (((controller_value * 10) + 127) / 254) + 1;
  } else if (control_number == SEQ_LAST_STEP) {
    uint8_t last_step = g_synth.current_controller_value(SEQ_LAST_STEP  );
    last_step = (last_step + 8) >> 4;
    if (last_step > 7) { last_step = 7; }
    s_seq_last_step = last_step;
  } else if (control_number == SEQ_PATTERN    ) {
    uint8_t pattern = g_synth.current_controller_value(SEQ_PATTERN    );
    s_seq_pattern = ((pattern * 4) + 127) / 254;
  } else if (control_number == SEQ_ACT_STEPS  ) {
    s_seq_act_steps = g_synth.current_controller_value(SEQ_ACT_STEPS  );
  } else if (control_number == PANEL_MIDI_CH) {
    uint8_t midi_ch = g_synth.current_controller_value(PANEL_MIDI_CH);

    if (midi_ch > 15) {
      midi_ch = 15;
    }

    g_midi_ch = midi_ch;
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
