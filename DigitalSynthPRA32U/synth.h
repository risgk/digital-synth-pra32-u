#pragma once

#include "common.h"

// associations of units
#define IOsc        Osc
#define IFilter     Filter
#define IAmp        Amp
#define IEG         EG
#define IDelayFx    DelayFx
#define IVoice      Voice

#include "osc.h"
#include "filter.h"
#include "amp.h"
#include "eg.h"
#include "delay-fx.h"
#include "voice.h"

template <uint8_t T>
class Synth {
public:
  INLINE static void initialize() {
    IVoice<0>::initialize();
    program_change(PROGRAM_NUMBER_DEFAULT);
  }

  INLINE static void control_change(uint8_t controller_number, uint8_t controller_value) {
    IVoice<0>::control_change(controller_number, controller_value);
  }

  INLINE static void pitch_bend(uint8_t lsb, uint8_t msb) {
    IVoice<0>::pitch_bend(lsb, msb);
  }

  INLINE static void program_change(uint8_t program_number) {
    IVoice<0>::program_change(program_number);
  }

  INLINE static void note_on(uint8_t note_number, uint8_t velocity) {
    IVoice<0>::note_on(note_number, velocity);
  }

  INLINE static void note_off(uint8_t note_number) {
    IVoice<0>::note_off(note_number);
  }

  INLINE static int16_t process(int16_t& right_level) {
    return IVoice<0>::process(right_level);
  }
};
