// refs https://www.midi.org/specifications/item/the-midi-1-0-specification
// refs http://amei.or.jp/midistandardcommittee/MIDIspcj.html

#pragma once

#include "./DigitalSynthPRA32U/common.h"
#include "./DigitalSynthPRA32U/synth.h"

template <uint8_t T>
class MIDIIn {
  static uint8_t m_system_exclusive;
  static uint8_t m_system_data_remaining;
  static uint8_t m_running_status;
  static uint8_t m_first_data;

public:
  INLINE static void open() {
    m_running_status = STATUS_BYTE_INVALID;
    m_first_data = DATA_BYTE_INVALID;
  }

  INLINE static void receive_midi_byte(uint8_t b) {
    if (is_data_byte(b)) {
      if (m_system_exclusive) {
        // do nothing
      } else if (m_system_data_remaining != 0) {
        --m_system_data_remaining;
      } else if (m_running_status == (NOTE_ON | MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else if (b == 0) {
          Synth<0>::note_off(m_first_data);
          m_first_data = DATA_BYTE_INVALID;
        } else {
          Synth<0>::note_on(m_first_data, b);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (NOTE_OFF | MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else {
          Synth<0>::note_off(m_first_data);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (CONTROL_CHANGE | MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else {
          Synth<0>::control_change(m_first_data, b);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (PITCH_BEND | MIDI_CH)) {
        if (!is_data_byte(m_first_data)) {
          m_first_data = b;
        } else {
          Synth<0>::pitch_bend(m_first_data, b);
          m_first_data = DATA_BYTE_INVALID;
        }
      } else if (m_running_status == (PROGRAM_CHANGE | MIDI_CH)) {
        Synth<0>::program_change(b);
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
  INLINE static boolean is_real_time_message(uint8_t b) {
    return b >= REAL_TIME_MESSAGE_MIN;
  }

  INLINE static boolean is_system_message(uint8_t b) {
    return b >= SYSTEM_MESSAGE_MIN;
  }

  INLINE static boolean is_status_byte(uint8_t b) {
    return b >= STATUS_BYTE_MIN;
  }

  INLINE static boolean is_data_byte(uint8_t b) {
    return b <= DATA_BYTE_MAX;
  }

};

template <uint8_t T> uint8_t MIDIIn<T>::m_system_exclusive;
template <uint8_t T> uint8_t MIDIIn<T>::m_system_data_remaining;
template <uint8_t T> uint8_t MIDIIn<T>::m_running_status;
template <uint8_t T> uint8_t MIDIIn<T>::m_first_data;
