// refs https://www.midi.org/specifications/item/the-midi-1-0-specification
// refs http://amei.or.jp/midistandardcommittee/MIDIspcj.html

#pragma once

#include "./Digital-Synth-PRA32-U/pra32-u-common.h"
#include "./Digital-Synth-PRA32-U/pra32-u-synth.h"

class PRA32_U_MIDIIn {
  PRA32_U_Synth* m_synth;

  uint8_t        m_system_exclusive;
  uint8_t        m_system_data_remaining;
  uint8_t        m_running_status;
  uint8_t        m_first_data;

public:
  PRA32_U_MIDIIn()
  : m_synth()
  , m_system_exclusive()
  , m_system_data_remaining()
  , m_running_status()
  , m_first_data()
  {}

  INLINE void open(PRA32_U_Synth& synth) {
    m_synth = &synth;
    m_running_status = STATUS_BYTE_INVALID;
    m_first_data = DATA_BYTE_INVALID;
  }

  INLINE void receive_midi_byte(uint8_t b) {
    if (is_data_byte(b)) {
      if (m_system_exclusive) {
        // do nothing
      } else if (m_system_data_remaining != 0) {
        --m_system_data_remaining;
      } else if (m_running_status == (NOTE_ON | PRA32_U_MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else if (b == 0) {
          m_synth->note_off(m_first_data);
          m_first_data = DATA_BYTE_INVALID;
        } else {
          m_synth->note_on(m_first_data, b);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (NOTE_OFF | PRA32_U_MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else {
          m_synth->note_off(m_first_data);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (CONTROL_CHANGE | PRA32_U_MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else {
          m_synth->control_change(m_first_data, b);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (PITCH_BEND | PRA32_U_MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else {
          m_synth->pitch_bend(m_first_data, b);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (PROGRAM_CHANGE | PRA32_U_MIDI_CH)) {
        m_synth->program_change(b);
      }
    } else if (is_system_message(b)) {
      switch (b) {
      case SYSTEM_EXCLUSIVE:
        m_system_exclusive = true;
        m_running_status = STATUS_BYTE_INVALID;
        break;
      case EOX:
      case TUNE_REQUEST:
      case 0xF4:
      case 0xF5:
        m_system_exclusive = false;
        m_system_data_remaining = 0;
        m_running_status = STATUS_BYTE_INVALID;
        break;
      case TIME_CODE:
      case SONG_SELECT:
        m_system_exclusive = false;
        m_system_data_remaining = 1;
        m_running_status = STATUS_BYTE_INVALID;
        break;
      case SONG_POSITION:
        m_system_exclusive = false;
        m_system_data_remaining = 2;
        m_running_status = STATUS_BYTE_INVALID;
        break;
      }
    } else if (is_status_byte(b)) {
      m_system_exclusive = false;
      m_running_status = b;
      m_first_data = DATA_BYTE_INVALID;
    }
  }

private:
  INLINE boolean is_real_time_message(uint8_t b) {
    return b >= REAL_TIME_MESSAGE_MIN;
  }

  INLINE boolean is_system_message(uint8_t b) {
    return b >= SYSTEM_MESSAGE_MIN;
  }

  INLINE boolean is_status_byte(uint8_t b) {
    return b >= STATUS_BYTE_MIN;
  }

  INLINE boolean is_data_byte(uint8_t b) {
    return b <= DATA_BYTE_MAX;
  }
};
