#pragma once

#include "pra32-u-common.h"

// associations of units
#define IOsc        Osc
#define IFilter     Filter
#define IAmp        Amp
#define IEG         EG
#define IDelayFx    DelayFx
#define IVoice      Voice

#include "pra32-u-osc.h"
#include "pra32-u-filter.h"
#include "pra32-u-amp.h"
#include "pra32-u-eg.h"
#include "pra32-u-delay-fx.h"
#include "pra32-u-voice.h"

class Synth {
  IVoice m_voice;

public:
  Synth()
  : m_voice()
  {}

  INLINE void initialize() {
    m_voice.initialize();
    program_change(PROGRAM_NUMBER_DEFAULT);
  }

  INLINE void control_change(uint8_t controller_number, uint8_t controller_value) {
    m_voice.control_change(controller_number, controller_value);
  }

  INLINE void pitch_bend(uint8_t lsb, uint8_t msb) {
    m_voice.pitch_bend(lsb, msb);
  }

  INLINE void program_change(uint8_t program_number) {
    m_voice.program_change(program_number);
  }

  INLINE void note_on(uint8_t note_number, uint8_t velocity) {
    m_voice.note_on(note_number, velocity);
  }

  INLINE void note_off(uint8_t note_number) {
    m_voice.note_off(note_number);
  }

  INLINE int16_t process(int16_t& right_level) {
    return m_voice.process(right_level);
  }
};
