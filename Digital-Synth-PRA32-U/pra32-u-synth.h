#pragma once

#include "pra32-u-common.h"
#include "pra32-u-osc.h"
#include "pra32-u-filter.h"
#include "pra32-u-amp.h"
#include "pra32-u-lfo.h"
#include "pra32-u-noise-gen.h"
#include "pra32-u-eg.h"
#include "pra32-u-chorus-fx.h"
#include "pra32-u-delay-fx.h"
#include "pra32-u-program-table.h"

#if defined(ARDUINO_ARCH_RP2040)
#include <EEPROM.h>
#endif // defined(ARDUINO_ARCH_RP2040)

#include <algorithm>
#include <cstring>

class PRA32_U_Synth {
  PRA32_U_Osc       m_osc;
  PRA32_U_Filter    m_filter[4];
  PRA32_U_Amp       m_amp[4];
  PRA32_U_NoiseGen  m_noise_gen;
  PRA32_U_LFO       m_lfo;
  PRA32_U_EG        m_eg[2 * 4];
  PRA32_U_ChorusFx  m_chorus_fx;
  PRA32_U_DelayFx   m_delay_fx;

  uint32_t          m_count;

  uint8_t           m_note_queue[4];
  uint8_t           m_note_on_number[4];
  uint8_t           m_note_on_count[128];
  uint8_t           m_note_on_total_count;
  boolean           m_sustain_pedal;
  uint8_t           m_voice_mode;

  uint8_t           m_output_error;
  uint8_t           m_portamento;

  uint8_t           m_chorus_mode;

  uint8_t           m_eg_osc_amt;
  uint8_t           m_eg_osc_dst;
  uint8_t           m_lfo_osc_amt;
  uint8_t           m_lfo_osc_dst;

  uint8_t           m_controller_value_eg_attack;
  uint8_t           m_controller_value_eg_decay;
  uint8_t           m_controller_value_eg_sustain;
  uint8_t           m_controller_value_eg_release;
  uint8_t           m_controller_value_amp_attack;
  uint8_t           m_controller_value_amp_decay;
  uint8_t           m_controller_value_amp_sustain;
  uint8_t           m_controller_value_amp_release;
  uint8_t           m_controller_value_eg_amp_mod;
  uint8_t           m_controller_value_rel_eq_decay;

  uint8_t           m_program_number_to_write;
  uint8_t           m_wr_prog_to_flash_cc_value;
  uint8_t           m_sp_prog_chg_cc_values[8];
  uint8_t           m_current_controller_value_table[128];
  uint8_t           m_program_table[128][PROGRAM_NUMBER_MAX + 1];

  volatile int32_t  m_secondary_core_processing_argument;
  volatile uint32_t m_secondary_core_processing_request;
  volatile int32_t  m_secondary_core_processing_result;

public:
  PRA32_U_Synth()

  : m_osc()
  , m_filter()
  , m_amp()
  , m_noise_gen()
  , m_lfo()
  , m_eg()
  , m_chorus_fx()
  , m_delay_fx()

  , m_count()

  , m_note_queue()
  , m_note_on_number()
  , m_note_on_count()
  , m_note_on_total_count()
  , m_sustain_pedal()
  , m_voice_mode(0xFF)

  , m_output_error()
  , m_portamento()

  , m_chorus_mode()

  , m_eg_osc_amt()
  , m_eg_osc_dst()
  , m_lfo_osc_amt()
  , m_lfo_osc_dst()

  , m_controller_value_eg_attack(0)
  , m_controller_value_eg_decay(127)
  , m_controller_value_eg_sustain(0)
  , m_controller_value_eg_release(0)
  , m_controller_value_amp_attack(0)
  , m_controller_value_amp_decay(127)
  , m_controller_value_amp_sustain(0)
  , m_controller_value_amp_release(0)
  , m_controller_value_eg_amp_mod(0)
  , m_controller_value_rel_eq_decay(0)

  , m_program_number_to_write(0)
  , m_wr_prog_to_flash_cc_value(0)
  , m_sp_prog_chg_cc_values()
  , m_current_controller_value_table()
  , m_program_table()

  , m_secondary_core_processing_argument()
  , m_secondary_core_processing_request()
  , m_secondary_core_processing_result()
  {
    m_note_queue[0] = 0;
    m_note_queue[1] = 1;
    m_note_queue[2] = 2;
    m_note_queue[3] = 3;
    m_note_on_number[0] = NOTE_NUMBER_INVALID;
    m_note_on_number[1] = NOTE_NUMBER_INVALID;
    m_note_on_number[2] = NOTE_NUMBER_INVALID;
    m_note_on_number[3] = NOTE_NUMBER_INVALID;

    set_voice_mode(VOICE_PARAPHONIC);

    m_amp[0].set_gain(127);
    m_amp[1].set_gain(127);
    m_amp[2].set_gain(127);
    m_amp[3].set_gain(127);

    m_eg_osc_amt = 64;
    m_lfo_osc_amt = 64;
  }

  INLINE void initialize() {
    std::memset(m_current_controller_value_table, sizeof(m_current_controller_value_table), 255);
    std::memset(m_program_table, sizeof(m_program_table), 255);

    std::memcpy(m_program_table[OSC_1_WAVE     ], g_preset_table_OSC_1_WAVE     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[OSC_1_SHAPE    ], g_preset_table_OSC_1_SHAPE    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[OSC_1_MORPH    ], g_preset_table_OSC_1_MORPH    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[MIXER_SUB_OSC  ], g_preset_table_MIXER_SUB_OSC  , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[OSC_2_WAVE     ], g_preset_table_OSC_2_WAVE     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[OSC_2_COARSE   ], g_preset_table_OSC_2_COARSE   , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[OSC_2_PITCH    ], g_preset_table_OSC_2_PITCH    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[MIXER_OSC_MIX  ], g_preset_table_MIXER_OSC_MIX  , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[FILTER_CUTOFF  ], g_preset_table_FILTER_CUTOFF  , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[FILTER_RESO    ], g_preset_table_FILTER_RESO    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[FILTER_EG_AMT  ], g_preset_table_FILTER_EG_AMT  , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[FILTER_KEY_TRK ], g_preset_table_FILTER_KEY_TRK , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[EG_ATTACK      ], g_preset_table_EG_ATTACK      , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[EG_DECAY       ], g_preset_table_EG_DECAY       , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[EG_SUSTAIN     ], g_preset_table_EG_SUSTAIN     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[EG_RELEASE     ], g_preset_table_EG_RELEASE     , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[EG_OSC_AMT     ], g_preset_table_EG_OSC_AMT     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[EG_OSC_DST     ], g_preset_table_EG_OSC_DST     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[VOICE_MODE     ], g_preset_table_VOICE_MODE     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[PORTAMENTO     ], g_preset_table_PORTAMENTO     , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[LFO_WAVE       ], g_preset_table_LFO_WAVE       , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[LFO_RATE       ], g_preset_table_LFO_RATE       , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[LFO_DEPTH      ], g_preset_table_LFO_DEPTH      , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[LFO_FADE_TIME  ], g_preset_table_LFO_FADE_TIME  , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[LFO_OSC_AMT    ], g_preset_table_LFO_OSC_AMT    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[LFO_OSC_DST    ], g_preset_table_LFO_OSC_DST    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[LFO_FILTER_AMT ], g_preset_table_LFO_FILTER_AMT , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[AMP_GAIN       ], g_preset_table_AMP_GAIN       , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[AMP_ATTACK     ], g_preset_table_AMP_ATTACK     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[AMP_DECAY      ], g_preset_table_AMP_DECAY      , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[AMP_SUSTAIN    ], g_preset_table_AMP_SUSTAIN    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[AMP_RELEASE    ], g_preset_table_AMP_RELEASE    , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[FILTER_MODE    ], g_preset_table_FILTER_MODE    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[EG_AMP_MOD     ], g_preset_table_EG_AMP_MOD     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[REL_EQ_DECAY   ], g_preset_table_REL_EQ_DECAY   , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[P_BEND_RANGE   ], g_preset_table_P_BEND_RANGE   , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[BTH_FILTER_AMT ], g_preset_table_BTH_FILTER_AMT , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[BTH_AMP_MOD    ], g_preset_table_BTH_AMP_MOD    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[EG_VEL_SENS    ], g_preset_table_EG_VEL_SENS    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[AMP_VEL_SENS   ], g_preset_table_AMP_VEL_SENS   , sizeof(m_program_table[0]));

    std::memcpy(m_program_table[CHORUS_MIX     ], g_preset_table_CHORUS_MIX     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[CHORUS_RATE    ], g_preset_table_CHORUS_RATE    , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[CHORUS_DEPTH   ], g_preset_table_CHORUS_DEPTH   , sizeof(m_program_table[0]));


    std::memcpy(m_program_table[DELAY_FEEDBACK ], g_preset_table_DELAY_FEEDBACK , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[DELAY_TIME     ], g_preset_table_DELAY_TIME     , sizeof(m_program_table[0]));
    std::memcpy(m_program_table[DELAY_MODE     ], g_preset_table_DELAY_MODE     , sizeof(m_program_table[0]));


#if defined(ARDUINO_ARCH_RP2040)
#if 1
    EEPROM.begin(2048);

    for (uint32_t program_number = (PRESET_PROGRAM_NUMBER_MAX + 1); program_number <= PROGRAM_NUMBER_MAX; ++program_number) {
      if ((EEPROM.read(program_number * 128) == 'U') && (EEPROM.read(program_number * 128 + 1) == program_number)) {
        m_program_table[OSC_1_WAVE     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + OSC_1_WAVE     ));
        m_program_table[OSC_1_SHAPE    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + OSC_1_SHAPE    ));
        m_program_table[OSC_1_MORPH    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + OSC_1_MORPH    ));
        m_program_table[MIXER_SUB_OSC  ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + MIXER_SUB_OSC  ));

        m_program_table[OSC_2_WAVE     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + OSC_2_WAVE     ));
        m_program_table[OSC_2_COARSE   ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + OSC_2_COARSE   ));
        m_program_table[OSC_2_PITCH    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + OSC_2_PITCH    ));
        m_program_table[MIXER_OSC_MIX  ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + MIXER_OSC_MIX  ));

        m_program_table[FILTER_CUTOFF  ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + FILTER_CUTOFF  ));
        m_program_table[FILTER_RESO    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + FILTER_RESO    ));
        m_program_table[FILTER_EG_AMT  ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + FILTER_EG_AMT  ));
        m_program_table[FILTER_KEY_TRK ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + FILTER_KEY_TRK ));

        m_program_table[EG_ATTACK      ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_ATTACK      ));
        m_program_table[EG_DECAY       ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_DECAY       ));
        m_program_table[EG_SUSTAIN     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_SUSTAIN     ));
        m_program_table[EG_RELEASE     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_RELEASE     ));

        m_program_table[EG_OSC_AMT     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_OSC_AMT     ));
        m_program_table[EG_OSC_DST     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_OSC_DST     ));
        m_program_table[VOICE_MODE     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + VOICE_MODE     ));
        m_program_table[PORTAMENTO     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + PORTAMENTO     ));

        m_program_table[LFO_WAVE       ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + LFO_WAVE       ));
        m_program_table[LFO_RATE       ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + LFO_RATE       ));
        m_program_table[LFO_DEPTH      ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + LFO_DEPTH      ));
        m_program_table[LFO_FADE_TIME  ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + LFO_FADE_TIME  ));

        m_program_table[LFO_OSC_AMT    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + LFO_OSC_AMT    ));
        m_program_table[LFO_OSC_DST    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + LFO_OSC_DST    ));
        m_program_table[LFO_FILTER_AMT ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + LFO_FILTER_AMT ));
        m_program_table[AMP_GAIN       ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + AMP_GAIN       ));

        m_program_table[AMP_ATTACK     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + AMP_ATTACK     ));
        m_program_table[AMP_DECAY      ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + AMP_DECAY      ));
        m_program_table[AMP_SUSTAIN    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + AMP_SUSTAIN    ));
        m_program_table[AMP_RELEASE    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + AMP_RELEASE    ));

        m_program_table[FILTER_MODE    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + FILTER_MODE    ));
        m_program_table[EG_AMP_MOD     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_AMP_MOD     ));
        m_program_table[REL_EQ_DECAY   ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + REL_EQ_DECAY   ));
        m_program_table[P_BEND_RANGE   ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + P_BEND_RANGE   ));

        m_program_table[BTH_FILTER_AMT ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + BTH_FILTER_AMT ));
        m_program_table[BTH_AMP_MOD    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + BTH_AMP_MOD    ));
        m_program_table[EG_VEL_SENS    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + EG_VEL_SENS    ));
        m_program_table[AMP_VEL_SENS   ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + AMP_VEL_SENS   ));

        m_program_table[CHORUS_MIX     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + CHORUS_MIX     ));
        m_program_table[CHORUS_RATE    ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + CHORUS_RATE    ));
        m_program_table[CHORUS_DEPTH   ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + CHORUS_DEPTH   ));


        m_program_table[DELAY_FEEDBACK ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + DELAY_FEEDBACK ));
        m_program_table[DELAY_TIME     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + DELAY_TIME     ));
        m_program_table[DELAY_MODE     ][program_number] = std::min<uint8_t>(127, EEPROM.read(program_number * 128 + DELAY_MODE     ));

      }
    }
#endif
#endif // defined(ARDUINO_ARCH_RP2040)

    program_change(PROGRAM_NUMBER_DEFAULT);
  }

  INLINE void note_on(uint8_t note_number, uint8_t velocity) {
    if (m_note_on_total_count == 255) {
      return;
    }

    if ((m_voice_mode == VOICE_MONOPHONIC) ||
        (m_voice_mode == VOICE_LEGATO) || (m_voice_mode == VOICE_LEGATO_PORTA)) {
      if ((m_voice_mode == VOICE_LEGATO) || (m_voice_mode == VOICE_LEGATO_PORTA)) {
        ++m_note_on_total_count;
        ++m_note_on_count[note_number];

        if (m_note_on_number[0] == NOTE_NUMBER_INVALID) {
          m_note_on_number[0] = note_number;

          if (m_voice_mode == VOICE_LEGATO_PORTA) {
            m_osc.set_portamento<0>(0);
            m_osc.set_portamento<2>(0);
          } else {
            m_osc.set_portamento<0>(m_portamento);
            m_osc.set_portamento<2>(m_portamento);
          }
          m_osc.note_on<0>(note_number);
          m_osc.note_on<2>(note_number);
          m_lfo.trigger_lfo();
          m_eg[0].note_on(velocity);
          m_eg[1].note_on(velocity);
        } else {
          m_note_on_number[3] = m_note_on_number[2];
          m_note_on_number[2] = m_note_on_number[1];
          m_note_on_number[1] = m_note_on_number[0];
          m_note_on_number[0] = note_number;

          m_osc.set_portamento<0>(m_portamento);
          m_osc.set_portamento<2>(m_portamento);
          m_osc.note_on<0>(note_number);
          m_osc.note_on<2>(note_number);
        }
      } else {
        ++m_note_on_total_count;
        ++m_note_on_count[note_number];

        m_note_on_number[3] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[1];
        m_note_on_number[1] = m_note_on_number[0];
        m_note_on_number[0] = note_number;

        m_osc.set_portamento<0>(m_portamento);
        m_osc.set_portamento<2>(m_portamento);
        m_osc.note_on<0>(note_number);
        m_osc.note_on<2>(note_number);
        m_lfo.trigger_lfo();
        m_eg[0].note_on(velocity);
        m_eg[1].note_on(velocity);
      }
    } else if (m_note_on_number[0] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<0>(m_portamento);
      m_osc.note_on<0>(note_number);

      m_eg[0].note_on(velocity);
      m_eg[1].note_on(velocity);
    } else if (m_note_on_number[1] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<1>(m_portamento);
      m_osc.note_on<1>(note_number);

      if (m_voice_mode == VOICE_POLYPHONIC) {
        m_eg[2].note_on(velocity);
        m_eg[3].note_on(velocity);
      } else {
        m_eg[0].note_on(velocity);
        m_eg[1].note_on(velocity);
      }
    } else if (m_note_on_number[2] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<2>(m_portamento);
      m_osc.note_on<2>(note_number);

      if (m_voice_mode == VOICE_POLYPHONIC) {
        m_eg[4].note_on(velocity);
        m_eg[5].note_on(velocity);
      } else {
        m_eg[0].note_on(velocity);
        m_eg[1].note_on(velocity);
      }
    } else if (m_note_on_number[3] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<3>(m_portamento);
      m_osc.note_on<3>(note_number);

      if (m_voice_mode == VOICE_POLYPHONIC) {
        m_eg[6].note_on(velocity);
        m_eg[7].note_on(velocity);
      } else {
        m_eg[0].note_on(velocity);
        m_eg[1].note_on(velocity);
      }
    } else {
      uint8_t note_on_osc_index;
      if        (m_note_on_number[0] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 0;
      } else if (m_note_on_number[1] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 1;
      } else if (m_note_on_number[2] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 2;
      } else if (m_note_on_number[3] == NOTE_NUMBER_INVALID) {
        note_on_osc_index = 3;
      } else {
        note_on_osc_index = m_note_queue[0];
      }

      note_queue_on(note_on_osc_index);

      uint8_t prev_note_on_total_count = m_note_on_total_count;
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_note_on_number[note_on_osc_index] = note_number;

      switch (note_on_osc_index) {
      default:
        m_osc.set_portamento<0>(m_portamento);
        m_osc.note_on<0>(note_number);
        break;
      case 1:
        m_osc.set_portamento<1>(m_portamento);
        m_osc.note_on<1>(note_number);
        break;
      case 2:
        m_osc.set_portamento<2>(m_portamento);
        m_osc.note_on<2>(note_number);
        break;
      case 3:
        m_osc.set_portamento<3>(m_portamento);
        m_osc.note_on<3>(note_number);
        break;
      }

      if (prev_note_on_total_count == 0) {
        m_lfo.trigger_lfo();
      }

      if (m_voice_mode == VOICE_POLYPHONIC) {
        m_eg[(note_on_osc_index << 1) + 0].note_on(velocity);
        m_eg[(note_on_osc_index << 1) + 1].note_on(velocity);
      } else {
        m_eg[0].note_on(velocity);
        m_eg[1].note_on(velocity);
      }
    }
  }

  INLINE void note_off(uint8_t note_number) {
    if (m_note_on_total_count == 0) {
      return;
    }

    if (m_note_on_count[note_number] == 0) {
      return;
    }

    --m_note_on_total_count;
    --m_note_on_count[note_number];

    if (m_sustain_pedal) {
      return;
    }

    if ((m_voice_mode == VOICE_MONOPHONIC) ||
        (m_voice_mode == VOICE_LEGATO) || (m_voice_mode == VOICE_LEGATO_PORTA)) {
      if (m_note_on_total_count == 0) {
        m_note_on_number[0] = NOTE_NUMBER_INVALID;
        m_note_on_number[1] = NOTE_NUMBER_INVALID;
        m_note_on_number[2] = NOTE_NUMBER_INVALID;
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
        m_note_queue[0] = 0;
        m_note_queue[1] = 1;
        m_note_queue[2] = 2;
        m_note_queue[3] = 3;
        m_osc.note_off<0>();
        m_osc.note_off<1>();
        m_osc.note_off<2>();
        m_osc.note_off<3>();
      } else if (m_note_on_number[0] == note_number) {
        m_note_on_number[0] = m_note_on_number[1];
        m_note_on_number[1] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[3];
        m_note_on_number[3] = NOTE_NUMBER_INVALID;

        if (m_note_on_number[0] != NOTE_NUMBER_INVALID) {
          m_osc.set_portamento<0>(m_portamento);
          m_osc.set_portamento<2>(m_portamento);
          m_osc.note_on<0>(m_note_on_number[0]);
          m_osc.note_on<2>(m_note_on_number[0]);

          if (m_voice_mode == VOICE_MONOPHONIC) {
            m_lfo.trigger_lfo();
            m_eg[0].note_on(255);
            m_eg[1].note_on(255);
          }
        }
      } else if (m_note_on_number[1] == note_number) {
        m_note_on_number[1] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[3];
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
      } else if (m_note_on_number[2] == note_number) {
        m_note_on_number[2] = m_note_on_number[3];
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
      } else if (m_note_on_number[3] == note_number) {
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
      }
    } else if (m_note_on_number[0] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[0] = NOTE_NUMBER_INVALID;
        note_queue_off(0);
        m_osc.note_off<0>();

        if (m_voice_mode == VOICE_POLYPHONIC) {
          m_eg[0].note_off();
          m_eg[1].note_off();
        }
      }
    } else if (m_note_on_number[1] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[1] = NOTE_NUMBER_INVALID;
        note_queue_off(1);
        m_osc.note_off<1>();

        if (m_voice_mode == VOICE_POLYPHONIC) {
          m_eg[2].note_off();
          m_eg[3].note_off();
        }
      }
    } else if (m_note_on_number[2] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[2] = NOTE_NUMBER_INVALID;
        note_queue_off(2);
        m_osc.note_off<2>();

        if (m_voice_mode == VOICE_POLYPHONIC) {
          m_eg[4].note_off();
          m_eg[5].note_off();
        }
      }
    } else if (m_note_on_number[3] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
        note_queue_off(3);
        m_osc.note_off<3>();

        if (m_voice_mode == VOICE_POLYPHONIC) {
          m_eg[6].note_off();
          m_eg[7].note_off();
        }
      }
    }

    if (m_note_on_total_count == 0) {
      if (m_voice_mode != VOICE_POLYPHONIC) {
        m_eg[0].note_off();
        m_eg[1].note_off();
      }
    }
  }

  void all_sound_off() {
    m_sustain_pedal = false;
    m_note_on_number[0] = NOTE_NUMBER_INVALID;
    m_note_on_number[1] = NOTE_NUMBER_INVALID;
    m_note_on_number[2] = NOTE_NUMBER_INVALID;
    m_note_on_number[3] = NOTE_NUMBER_INVALID;
    for (uint8_t i = 0; i < sizeof(m_note_on_count); ++i) {
      m_note_on_count[i] = 0;
    }
    m_note_on_total_count = 0;
    m_note_queue[0] = 0;
    m_note_queue[1] = 1;
    m_note_queue[2] = 2;
    m_note_queue[3] = 3;
    m_osc.note_off<0>();
    m_osc.note_off<1>();
    m_osc.note_off<2>();
    m_osc.note_off<3>();

    m_eg[0].note_off();
    m_eg[1].note_off();
    m_eg[2].note_off();
    m_eg[3].note_off();
    m_eg[4].note_off();
    m_eg[5].note_off();
    m_eg[6].note_off();
    m_eg[7].note_off();
  }

  INLINE void reset_all_controllers() {
    pitch_bend(0, 64);
    set_modulation(0);
    set_breath_controller(0);
    set_sustain_pedal(0);
  }

  INLINE void control_change(uint8_t control_number, uint8_t controller_value) {
    m_current_controller_value_table[control_number] = controller_value;

    switch (control_number) {
    case MODULATION     :
      m_lfo.set_lfo_depth<1>(controller_value);
      break;

    case FILTER_CUTOFF  :
      m_filter[0].set_cutoff(controller_value);
      m_filter[1].set_cutoff(controller_value);
      m_filter[2].set_cutoff(controller_value);
      m_filter[3].set_cutoff(controller_value);
      break;
    case FILTER_RESO    :
      m_filter[0].set_resonance(controller_value);
      m_filter[1].set_resonance(controller_value);
      m_filter[2].set_resonance(controller_value);
      m_filter[3].set_resonance(controller_value);
      break;
    case FILTER_EG_AMT  :
      m_filter[0].set_cutoff_eg_amt(controller_value);
      m_filter[1].set_cutoff_eg_amt(controller_value);
      m_filter[2].set_cutoff_eg_amt(controller_value);
      m_filter[3].set_cutoff_eg_amt(controller_value);
      break;

    case OSC_1_WAVE     :
      m_osc.set_osc_waveform<0>(controller_value);
      break;
    case OSC_2_WAVE     :
      m_osc.set_osc_waveform<1>(controller_value);
      break;
    case OSC_1_SHAPE    :
      m_osc.set_osc1_shape_control(controller_value);
      break;
    case OSC_1_MORPH    :
      m_osc.set_osc1_morph_control(controller_value);
      break;
    case MIXER_SUB_OSC  :
      m_osc.set_mixer_sub_osc_control(controller_value);
      break;

    case LFO_RATE       :
      m_lfo.set_lfo_rate(controller_value);
      break;
    case LFO_DEPTH      :
      m_lfo.set_lfo_depth<0>(controller_value);
      break;
    case LFO_OSC_AMT    :
      m_lfo_osc_amt = controller_value;
      update_lfo_osc_mod();
      break;
    case LFO_FILTER_AMT :
      m_filter[0].set_cutoff_lfo_amt(controller_value);
      m_filter[1].set_cutoff_lfo_amt(controller_value);
      m_filter[2].set_cutoff_lfo_amt(controller_value);
      m_filter[3].set_cutoff_lfo_amt(controller_value);
      break;

    case SUSTAIN_PEDAL   :
      set_sustain_pedal(controller_value);
      break;

    case EG_ATTACK      :
      m_controller_value_eg_attack   = controller_value;
      update_eg_and_amp_eg();
      break;
    case EG_DECAY       :
      m_controller_value_eg_decay    = controller_value;
      update_eg_and_amp_eg();
      break;
    case EG_SUSTAIN     :
      m_controller_value_eg_sustain  = controller_value;
      update_eg_and_amp_eg();
      break;
    case EG_RELEASE     :
      m_controller_value_eg_release  = controller_value;
      update_eg_and_amp_eg();
      break;

    case AMP_ATTACK     :
      m_controller_value_amp_attack  = controller_value;
      update_eg_and_amp_eg();
      break;
    case AMP_DECAY      :
      m_controller_value_amp_decay   = controller_value;
      update_eg_and_amp_eg();
      break;
    case AMP_SUSTAIN    :
      m_controller_value_amp_sustain = controller_value;
      update_eg_and_amp_eg();
      break;
    case AMP_RELEASE    :
      m_controller_value_amp_release = controller_value;
      update_eg_and_amp_eg();
      break;

    case CHORUS_DEPTH   :
      m_chorus_fx.set_chorus_depth(controller_value);
      break;
    case CHORUS_RATE    :
      m_chorus_fx.set_chorus_rate(controller_value);
      break;
    case CHORUS_MIX     :
      m_chorus_fx.set_chorus_mix(controller_value);
      break;

#if 0
    case OSC_LEVEL      :
      m_osc.set_osc_level(controller_value);
      break;
#endif
    case AMP_GAIN       :
      m_amp[0].set_gain(controller_value);
      m_amp[1].set_gain(controller_value);
      m_amp[2].set_gain(controller_value);
      m_amp[3].set_gain(controller_value);
      break;

    case PORTAMENTO     :
      m_portamento = controller_value;
      break;

    case MIXER_OSC_MIX  :
      m_osc.set_mixer_osc_mix(controller_value);
      break;

    case OSC_2_COARSE   :
      m_osc.set_osc2_pitch(controller_value);
      break;

    case OSC_2_PITCH    :
      m_osc.set_osc2_detune(controller_value);
      break;

    case EG_OSC_AMT     :
      m_eg_osc_amt = controller_value;
      update_eg_osc_mod();
      break;
    case EG_OSC_DST     :
      m_eg_osc_dst = controller_value;
      update_eg_osc_mod();
      break;
    case LFO_OSC_DST    :
      m_lfo_osc_dst = controller_value;
      update_lfo_osc_mod();
      break;

    case LFO_WAVE       :
      m_lfo.set_lfo_waveform(controller_value);
      break;

    case LFO_FADE_TIME  :
      m_lfo.set_lfo_fade_time(controller_value);
      break;

    case P_BEND_RANGE   :
      m_osc.set_pitch_bend_range(controller_value);
      break;

    case EG_AMP_MOD     :
      m_controller_value_eg_amp_mod   = controller_value;
      update_eg_and_amp_eg();
      break;

    case REL_EQ_DECAY   :
      m_controller_value_rel_eq_decay = controller_value;
      update_eg_and_amp_eg();
      break;

    case FILTER_MODE    :
      m_filter[0].set_filter_mode(controller_value);
      m_filter[1].set_filter_mode(controller_value);
      m_filter[2].set_filter_mode(controller_value);
      m_filter[3].set_filter_mode(controller_value);
      break;

    case FILTER_KEY_TRK :
      m_filter[0].set_cutoff_pitch_amt(controller_value);
      m_filter[1].set_cutoff_pitch_amt(controller_value);
      m_filter[2].set_cutoff_pitch_amt(controller_value);
      m_filter[3].set_cutoff_pitch_amt(controller_value);
      break;

    case VOICE_MODE     :
      set_voice_mode(controller_value);
      break;

    case DELAY_FEEDBACK :
      m_delay_fx.set_delay_feedback(controller_value);
      break;
    case DELAY_TIME     :
      m_delay_fx.set_delay_time(controller_value);
      break;
    case DELAY_MODE     :
      m_delay_fx.set_delay_mode(controller_value);
      break;

    case BTH_FILTER_AMT    :
      m_filter[0].set_cutoff_breath_amt(controller_value);
      m_filter[1].set_cutoff_breath_amt(controller_value);
      m_filter[2].set_cutoff_breath_amt(controller_value);
      m_filter[3].set_cutoff_breath_amt(controller_value);
      break;
    case BTH_AMP_MOD    :
      m_amp[0].set_breath_mod(controller_value);
      m_amp[1].set_breath_mod(controller_value);
      m_amp[2].set_breath_mod(controller_value);
      m_amp[3].set_breath_mod(controller_value);
      break;
    case EG_VEL_SENS    :
      m_eg[0].set_velocity_sensitivity(controller_value);
      m_eg[2].set_velocity_sensitivity(controller_value);
      m_eg[4].set_velocity_sensitivity(controller_value);
      m_eg[6].set_velocity_sensitivity(controller_value);
      break;
    case AMP_VEL_SENS   :
      m_eg[1].set_velocity_sensitivity(controller_value);
      m_eg[3].set_velocity_sensitivity(controller_value);
      m_eg[5].set_velocity_sensitivity(controller_value);
      m_eg[7].set_velocity_sensitivity(controller_value);
      break;

    case BTH_CONTROLLER    :
      set_breath_controller(controller_value);
      break;

    case ALL_NOTES_OFF  :
    case OMNI_MODE_OFF  :
    case OMNI_MODE_ON   :
    case MONO_MODE_ON   :
    case POLY_MODE_ON   :
      all_sound_off();  // Strictly speaking, this is a violation of MIDI 1.0 Specification...
      break;

    case ALL_SOUND_OFF  :
      all_sound_off();
      break;

    case RESET_ALL_CTRLS:
      reset_all_controllers();
      break;

    // Write Program
    case PROG_NUM_TO_W  :
      m_program_number_to_write = controller_value % (PROGRAM_NUMBER_MAX + 1);
      break;
    case WRITE_PROG_TO_F:
      {
        uint8_t old_value = m_wr_prog_to_flash_cc_value;
        m_wr_prog_to_flash_cc_value = controller_value;

        if (m_program_number_to_write >= (PRESET_PROGRAM_NUMBER_MAX + 1)) {
          if ((old_value == 0) && (controller_value == 1)) {
            m_program_table[OSC_1_WAVE     ][m_program_number_to_write] = m_current_controller_value_table[OSC_1_WAVE     ];
            m_program_table[OSC_1_SHAPE    ][m_program_number_to_write] = m_current_controller_value_table[OSC_1_SHAPE    ];
            m_program_table[OSC_1_MORPH    ][m_program_number_to_write] = m_current_controller_value_table[OSC_1_MORPH    ];
            m_program_table[MIXER_SUB_OSC  ][m_program_number_to_write] = m_current_controller_value_table[MIXER_SUB_OSC  ];

            m_program_table[OSC_2_WAVE     ][m_program_number_to_write] = m_current_controller_value_table[OSC_2_WAVE     ];
            m_program_table[OSC_2_COARSE   ][m_program_number_to_write] = m_current_controller_value_table[OSC_2_COARSE   ];
            m_program_table[OSC_2_PITCH    ][m_program_number_to_write] = m_current_controller_value_table[OSC_2_PITCH    ];
            m_program_table[MIXER_OSC_MIX  ][m_program_number_to_write] = m_current_controller_value_table[MIXER_OSC_MIX  ];

            m_program_table[FILTER_CUTOFF  ][m_program_number_to_write] = m_current_controller_value_table[FILTER_CUTOFF  ];
            m_program_table[FILTER_RESO    ][m_program_number_to_write] = m_current_controller_value_table[FILTER_RESO    ];
            m_program_table[FILTER_EG_AMT  ][m_program_number_to_write] = m_current_controller_value_table[FILTER_EG_AMT  ];
            m_program_table[FILTER_KEY_TRK ][m_program_number_to_write] = m_current_controller_value_table[FILTER_KEY_TRK ];

            m_program_table[EG_ATTACK      ][m_program_number_to_write] = m_current_controller_value_table[EG_ATTACK      ];
            m_program_table[EG_DECAY       ][m_program_number_to_write] = m_current_controller_value_table[EG_DECAY       ];
            m_program_table[EG_SUSTAIN     ][m_program_number_to_write] = m_current_controller_value_table[EG_SUSTAIN     ];
            m_program_table[EG_RELEASE     ][m_program_number_to_write] = m_current_controller_value_table[EG_RELEASE     ];

            m_program_table[EG_OSC_AMT     ][m_program_number_to_write] = m_current_controller_value_table[EG_OSC_AMT     ];
            m_program_table[EG_OSC_DST     ][m_program_number_to_write] = m_current_controller_value_table[EG_OSC_DST     ];
            m_program_table[VOICE_MODE     ][m_program_number_to_write] = m_current_controller_value_table[VOICE_MODE     ];
            m_program_table[PORTAMENTO     ][m_program_number_to_write] = m_current_controller_value_table[PORTAMENTO     ];

            m_program_table[LFO_WAVE       ][m_program_number_to_write] = m_current_controller_value_table[LFO_WAVE       ];
            m_program_table[LFO_RATE       ][m_program_number_to_write] = m_current_controller_value_table[LFO_RATE       ];
            m_program_table[LFO_DEPTH      ][m_program_number_to_write] = m_current_controller_value_table[LFO_DEPTH      ];
            m_program_table[LFO_FADE_TIME  ][m_program_number_to_write] = m_current_controller_value_table[LFO_FADE_TIME  ];

            m_program_table[LFO_OSC_AMT    ][m_program_number_to_write] = m_current_controller_value_table[LFO_OSC_AMT    ];
            m_program_table[LFO_OSC_DST    ][m_program_number_to_write] = m_current_controller_value_table[LFO_OSC_DST    ];
            m_program_table[LFO_FILTER_AMT ][m_program_number_to_write] = m_current_controller_value_table[LFO_FILTER_AMT ];
            m_program_table[AMP_GAIN       ][m_program_number_to_write] = m_current_controller_value_table[AMP_GAIN       ];

            m_program_table[AMP_ATTACK     ][m_program_number_to_write] = m_current_controller_value_table[AMP_ATTACK     ];
            m_program_table[AMP_DECAY      ][m_program_number_to_write] = m_current_controller_value_table[AMP_DECAY      ];
            m_program_table[AMP_SUSTAIN    ][m_program_number_to_write] = m_current_controller_value_table[AMP_SUSTAIN    ];
            m_program_table[AMP_RELEASE    ][m_program_number_to_write] = m_current_controller_value_table[AMP_RELEASE    ];

            m_program_table[FILTER_MODE    ][m_program_number_to_write] = m_current_controller_value_table[FILTER_MODE    ];
            m_program_table[EG_AMP_MOD     ][m_program_number_to_write] = m_current_controller_value_table[EG_AMP_MOD     ];
            m_program_table[REL_EQ_DECAY   ][m_program_number_to_write] = m_current_controller_value_table[REL_EQ_DECAY   ];
            m_program_table[P_BEND_RANGE   ][m_program_number_to_write] = m_current_controller_value_table[P_BEND_RANGE   ];

            m_program_table[BTH_FILTER_AMT ][m_program_number_to_write] = m_current_controller_value_table[BTH_FILTER_AMT ];
            m_program_table[BTH_AMP_MOD    ][m_program_number_to_write] = m_current_controller_value_table[BTH_AMP_MOD    ];
            m_program_table[EG_VEL_SENS    ][m_program_number_to_write] = m_current_controller_value_table[EG_VEL_SENS    ];
            m_program_table[AMP_VEL_SENS   ][m_program_number_to_write] = m_current_controller_value_table[AMP_VEL_SENS   ];

            m_program_table[CHORUS_MIX     ][m_program_number_to_write] = m_current_controller_value_table[CHORUS_MIX     ];
            m_program_table[CHORUS_RATE    ][m_program_number_to_write] = m_current_controller_value_table[CHORUS_RATE    ];
            m_program_table[CHORUS_DEPTH   ][m_program_number_to_write] = m_current_controller_value_table[CHORUS_DEPTH   ];


            m_program_table[DELAY_FEEDBACK ][m_program_number_to_write] = m_current_controller_value_table[DELAY_FEEDBACK ];
            m_program_table[DELAY_TIME     ][m_program_number_to_write] = m_current_controller_value_table[DELAY_TIME     ];
            m_program_table[DELAY_MODE     ][m_program_number_to_write] = m_current_controller_value_table[DELAY_MODE     ];


#if defined(ARDUINO_ARCH_RP2040)
            EEPROM.write(m_program_number_to_write * 128 + OSC_1_WAVE     , m_current_controller_value_table[OSC_1_WAVE     ]);
            EEPROM.write(m_program_number_to_write * 128 + OSC_1_SHAPE    , m_current_controller_value_table[OSC_1_SHAPE    ]);
            EEPROM.write(m_program_number_to_write * 128 + OSC_1_MORPH    , m_current_controller_value_table[OSC_1_MORPH    ]);
            EEPROM.write(m_program_number_to_write * 128 + MIXER_SUB_OSC  , m_current_controller_value_table[MIXER_SUB_OSC  ]);

            EEPROM.write(m_program_number_to_write * 128 + OSC_2_WAVE     , m_current_controller_value_table[OSC_2_WAVE     ]);
            EEPROM.write(m_program_number_to_write * 128 + OSC_2_COARSE   , m_current_controller_value_table[OSC_2_COARSE   ]);
            EEPROM.write(m_program_number_to_write * 128 + OSC_2_PITCH    , m_current_controller_value_table[OSC_2_PITCH    ]);
            EEPROM.write(m_program_number_to_write * 128 + MIXER_OSC_MIX  , m_current_controller_value_table[MIXER_OSC_MIX  ]);

            EEPROM.write(m_program_number_to_write * 128 + FILTER_CUTOFF  , m_current_controller_value_table[FILTER_CUTOFF  ]);
            EEPROM.write(m_program_number_to_write * 128 + FILTER_RESO    , m_current_controller_value_table[FILTER_RESO    ]);
            EEPROM.write(m_program_number_to_write * 128 + FILTER_EG_AMT  , m_current_controller_value_table[FILTER_EG_AMT  ]);
            EEPROM.write(m_program_number_to_write * 128 + FILTER_KEY_TRK , m_current_controller_value_table[FILTER_KEY_TRK ]);

            EEPROM.write(m_program_number_to_write * 128 + EG_ATTACK      , m_current_controller_value_table[EG_ATTACK      ]);
            EEPROM.write(m_program_number_to_write * 128 + EG_DECAY       , m_current_controller_value_table[EG_DECAY       ]);
            EEPROM.write(m_program_number_to_write * 128 + EG_SUSTAIN     , m_current_controller_value_table[EG_SUSTAIN     ]);
            EEPROM.write(m_program_number_to_write * 128 + EG_RELEASE     , m_current_controller_value_table[EG_RELEASE     ]);

            EEPROM.write(m_program_number_to_write * 128 + EG_OSC_AMT     , m_current_controller_value_table[EG_OSC_AMT     ]);
            EEPROM.write(m_program_number_to_write * 128 + EG_OSC_DST     , m_current_controller_value_table[EG_OSC_DST     ]);
            EEPROM.write(m_program_number_to_write * 128 + VOICE_MODE     , m_current_controller_value_table[VOICE_MODE     ]);
            EEPROM.write(m_program_number_to_write * 128 + PORTAMENTO     , m_current_controller_value_table[PORTAMENTO     ]);

            EEPROM.write(m_program_number_to_write * 128 + LFO_WAVE       , m_current_controller_value_table[LFO_WAVE       ]);
            EEPROM.write(m_program_number_to_write * 128 + LFO_RATE       , m_current_controller_value_table[LFO_RATE       ]);
            EEPROM.write(m_program_number_to_write * 128 + LFO_DEPTH      , m_current_controller_value_table[LFO_DEPTH      ]);
            EEPROM.write(m_program_number_to_write * 128 + LFO_FADE_TIME  , m_current_controller_value_table[LFO_FADE_TIME  ]);

            EEPROM.write(m_program_number_to_write * 128 + LFO_OSC_AMT    , m_current_controller_value_table[LFO_OSC_AMT    ]);
            EEPROM.write(m_program_number_to_write * 128 + LFO_OSC_DST    , m_current_controller_value_table[LFO_OSC_DST    ]);
            EEPROM.write(m_program_number_to_write * 128 + LFO_FILTER_AMT , m_current_controller_value_table[LFO_FILTER_AMT ]);
            EEPROM.write(m_program_number_to_write * 128 + AMP_GAIN       , m_current_controller_value_table[AMP_GAIN       ]);

            EEPROM.write(m_program_number_to_write * 128 + AMP_ATTACK     , m_current_controller_value_table[AMP_ATTACK     ]);
            EEPROM.write(m_program_number_to_write * 128 + AMP_DECAY      , m_current_controller_value_table[AMP_DECAY      ]);
            EEPROM.write(m_program_number_to_write * 128 + AMP_SUSTAIN    , m_current_controller_value_table[AMP_SUSTAIN    ]);
            EEPROM.write(m_program_number_to_write * 128 + AMP_RELEASE    , m_current_controller_value_table[AMP_RELEASE    ]);

            EEPROM.write(m_program_number_to_write * 128 + FILTER_MODE    , m_current_controller_value_table[FILTER_MODE    ]);
            EEPROM.write(m_program_number_to_write * 128 + EG_AMP_MOD     , m_current_controller_value_table[EG_AMP_MOD     ]);
            EEPROM.write(m_program_number_to_write * 128 + REL_EQ_DECAY   , m_current_controller_value_table[REL_EQ_DECAY   ]);
            EEPROM.write(m_program_number_to_write * 128 + P_BEND_RANGE   , m_current_controller_value_table[P_BEND_RANGE   ]);

            EEPROM.write(m_program_number_to_write * 128 + BTH_FILTER_AMT , m_current_controller_value_table[BTH_FILTER_AMT ]);
            EEPROM.write(m_program_number_to_write * 128 + BTH_AMP_MOD    , m_current_controller_value_table[BTH_AMP_MOD    ]);
            EEPROM.write(m_program_number_to_write * 128 + EG_VEL_SENS    , m_current_controller_value_table[EG_VEL_SENS    ]);
            EEPROM.write(m_program_number_to_write * 128 + AMP_VEL_SENS   , m_current_controller_value_table[AMP_VEL_SENS   ]);

            EEPROM.write(m_program_number_to_write * 128 + CHORUS_MIX     , m_current_controller_value_table[CHORUS_MIX     ]);
            EEPROM.write(m_program_number_to_write * 128 + CHORUS_RATE    , m_current_controller_value_table[CHORUS_RATE    ]);
            EEPROM.write(m_program_number_to_write * 128 + CHORUS_DEPTH   , m_current_controller_value_table[CHORUS_DEPTH   ]);


            EEPROM.write(m_program_number_to_write * 128 + DELAY_FEEDBACK , m_current_controller_value_table[DELAY_FEEDBACK ]);
            EEPROM.write(m_program_number_to_write * 128 + DELAY_TIME     , m_current_controller_value_table[DELAY_TIME     ]);
            EEPROM.write(m_program_number_to_write * 128 + DELAY_MODE     , m_current_controller_value_table[DELAY_MODE     ]);


            EEPROM.write(m_program_number_to_write * 128,     'U');
            EEPROM.write(m_program_number_to_write * 128 + 1, m_program_number_to_write);

#if defined(I2S_DAC_MUTE_OFF_PIN)
            // To avoid noise, the data will not be written to the flash if I2S_DAC_MUTE_OFF_PIN is not defined
            digitalWrite(I2S_DAC_MUTE_OFF_PIN, LOW);

            EEPROM.commit();

            digitalWrite(I2S_DAC_MUTE_OFF_PIN, HIGH);
#endif // defined(I2S_DAC_MUTE_OFF_PIN)
#endif // defined(ARDUINO_ARCH_RP2040)
          }
        }
      }
      break;

    // Program Change by CC
    case PC_BY_CC_8     :
    case PC_BY_CC_9     :
    case PC_BY_CC_10    :
    case PC_BY_CC_11    :
    case PC_BY_CC_12    :
    case PC_BY_CC_13    :
    case PC_BY_CC_14    :
    case PC_BY_CC_15    :
      {
        uint8_t program_index = control_number - PC_BY_CC_8;
        uint8_t old_value = m_sp_prog_chg_cc_values[program_index];
        m_sp_prog_chg_cc_values[program_index] = controller_value;
        if ((old_value < 64) && (controller_value >= 64)) {
          program_change(program_index + 8);
        }
      }
      break;
    }
  }

  INLINE void pitch_bend(uint8_t lsb, uint8_t msb) {
    int16_t pitch_bend = ((static_cast<uint16_t>(msb) << 8) >> 1) + lsb - 8192;
    m_osc.set_pitch_bend(pitch_bend);
  }

  /* INLINE */ void program_change(uint8_t program_number) {
    if (program_number > PROGRAM_NUMBER_MAX) {
      return;
    }

    program_number = program_number & PROGRAM_NUMBER_MAX;

    control_change(OSC_1_WAVE     , m_program_table[OSC_1_WAVE     ][program_number]);
    control_change(OSC_1_SHAPE    , m_program_table[OSC_1_SHAPE    ][program_number]);
    control_change(OSC_1_MORPH    , m_program_table[OSC_1_MORPH    ][program_number]);
    control_change(MIXER_SUB_OSC  , m_program_table[MIXER_SUB_OSC  ][program_number]);

    control_change(OSC_2_WAVE     , m_program_table[OSC_2_WAVE     ][program_number]);
    control_change(OSC_2_COARSE   , m_program_table[OSC_2_COARSE   ][program_number]);
    control_change(OSC_2_PITCH    , m_program_table[OSC_2_PITCH    ][program_number]);
    control_change(MIXER_OSC_MIX  , m_program_table[MIXER_OSC_MIX  ][program_number]);

    control_change(FILTER_CUTOFF  , m_program_table[FILTER_CUTOFF  ][program_number]);
    control_change(FILTER_RESO    , m_program_table[FILTER_RESO    ][program_number]);
    control_change(FILTER_EG_AMT  , m_program_table[FILTER_EG_AMT  ][program_number]);
    control_change(FILTER_KEY_TRK , m_program_table[FILTER_KEY_TRK ][program_number]);

    control_change(EG_ATTACK      , m_program_table[EG_ATTACK      ][program_number]);
    control_change(EG_DECAY       , m_program_table[EG_DECAY       ][program_number]);
    control_change(EG_SUSTAIN     , m_program_table[EG_SUSTAIN     ][program_number]);
    control_change(EG_RELEASE     , m_program_table[EG_RELEASE     ][program_number]);

    control_change(EG_OSC_AMT     , m_program_table[EG_OSC_AMT     ][program_number]);
    control_change(EG_OSC_DST     , m_program_table[EG_OSC_DST     ][program_number]);
    control_change(VOICE_MODE     , m_program_table[VOICE_MODE     ][program_number]);
    control_change(PORTAMENTO     , m_program_table[PORTAMENTO     ][program_number]);

    control_change(LFO_WAVE       , m_program_table[LFO_WAVE       ][program_number]);
    control_change(LFO_RATE       , m_program_table[LFO_RATE       ][program_number]);
    control_change(LFO_DEPTH      , m_program_table[LFO_DEPTH      ][program_number]);
    control_change(LFO_FADE_TIME  , m_program_table[LFO_FADE_TIME  ][program_number]);

    control_change(LFO_OSC_AMT    , m_program_table[LFO_OSC_AMT    ][program_number]);
    control_change(LFO_OSC_DST    , m_program_table[LFO_OSC_DST    ][program_number]);
    control_change(LFO_FILTER_AMT , m_program_table[LFO_FILTER_AMT ][program_number]);
    control_change(AMP_GAIN       , m_program_table[AMP_GAIN       ][program_number]);

    control_change(AMP_ATTACK     , m_program_table[AMP_ATTACK     ][program_number]);
    control_change(AMP_DECAY      , m_program_table[AMP_DECAY      ][program_number]);
    control_change(AMP_SUSTAIN    , m_program_table[AMP_SUSTAIN    ][program_number]);
    control_change(AMP_RELEASE    , m_program_table[AMP_RELEASE    ][program_number]);

    control_change(FILTER_MODE    , m_program_table[FILTER_MODE    ][program_number]);
    control_change(EG_AMP_MOD     , m_program_table[EG_AMP_MOD     ][program_number]);
    control_change(REL_EQ_DECAY   , m_program_table[REL_EQ_DECAY   ][program_number]);
    control_change(P_BEND_RANGE   , m_program_table[P_BEND_RANGE   ][program_number]);

    control_change(BTH_FILTER_AMT , m_program_table[BTH_FILTER_AMT ][program_number]);
    control_change(BTH_AMP_MOD    , m_program_table[BTH_AMP_MOD    ][program_number]);
    control_change(EG_VEL_SENS    , m_program_table[EG_VEL_SENS    ][program_number]);
    control_change(AMP_VEL_SENS   , m_program_table[AMP_VEL_SENS   ][program_number]);

    control_change(CHORUS_MIX     , m_program_table[CHORUS_MIX     ][program_number]);
    control_change(CHORUS_RATE    , m_program_table[CHORUS_RATE    ][program_number]);
    control_change(CHORUS_DEPTH   , m_program_table[CHORUS_DEPTH   ][program_number]);


    control_change(DELAY_FEEDBACK , m_program_table[DELAY_FEEDBACK ][program_number]);
    control_change(DELAY_TIME     , m_program_table[DELAY_TIME     ][program_number]);
    control_change(DELAY_MODE     , m_program_table[DELAY_MODE     ][program_number]);

  }

  INLINE int16_t process(int16_t& right_level) {
    ++m_count;

    int16_t noise_int15 = m_noise_gen.process();

    switch (m_count & (0x04 - 1)) {
    case 0x00:
      m_eg[0].process_at_low_rate();
      m_eg[1].process_at_low_rate();
      break;
    case 0x01:
      m_eg[2].process_at_low_rate();
      m_eg[3].process_at_low_rate();
      break;
    case 0x02:
      m_eg[4].process_at_low_rate();
      m_eg[5].process_at_low_rate();
      break;
    case 0x03:
      m_eg[6].process_at_low_rate();
      m_eg[7].process_at_low_rate();
      m_lfo.process_at_low_rate(m_count >> 2, noise_int15);
      break;
    }

    int16_t lfo_output = m_lfo.get_output();

    switch (m_count & (0x04 - 1)) {
    case 0x00:
      {
        m_osc.process_at_low_rate_a<0>(lfo_output, m_eg[0].get_output());
        m_osc.process_at_low_rate_b(m_count >> 2, noise_int15);
        uint16_t osc_pitch_0 = (60 << 8);
        if (m_voice_mode != VOICE_PARAPHONIC) {
          osc_pitch_0 = m_osc.get_osc_pitch(0);
        }
        m_filter[0].process_at_low_rate(m_count >> 2, m_eg[0].get_output(), lfo_output, osc_pitch_0);
        m_amp[0].process_at_low_rate(m_eg[1].get_output());
      }
      break;
    case 0x01:
      m_osc.process_at_low_rate_a<1>(lfo_output, m_eg[2].get_output());
      m_filter[1].process_at_low_rate(m_count >> 2, m_eg[2].get_output(), lfo_output, m_osc.get_osc_pitch(1));
      m_amp[1].process_at_low_rate(m_eg[3].get_output());
      break;
    case 0x02:
      m_osc.process_at_low_rate_a<2>(lfo_output, m_eg[4].get_output());
      m_filter[2].process_at_low_rate(m_count >> 2, m_eg[4].get_output(), lfo_output, m_osc.get_osc_pitch(2));
      m_amp[2].process_at_low_rate(m_eg[5].get_output());
      m_delay_fx.process_at_low_rate(m_count >> 2);
      break;
    case 0x03:
      m_osc.process_at_low_rate_a<3>(lfo_output, m_eg[6].get_output());
      m_filter[3].process_at_low_rate(m_count >> 2, m_eg[6].get_output(), lfo_output, m_osc.get_osc_pitch(3));
      m_amp[3].process_at_low_rate(m_eg[7].get_output());
      m_chorus_fx.process_at_low_rate(m_count >> 2);
      break;
    }

    int16_t osc_output   [4];
    int16_t filter_output[4];
    int16_t amp_output   [4];
    int16_t voice_mixer_output;
    if (m_voice_mode == VOICE_POLYPHONIC) {
#if defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
      m_secondary_core_processing_argument = noise_int15;
      m_secondary_core_processing_request = 1;
#endif // defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)

      osc_output   [0] = m_osc      .process<0>(noise_int15);
      filter_output[0] = m_filter[0].process(osc_output   [0] << 2);
      amp_output   [0] = m_amp   [0].process(filter_output[0]);

      osc_output   [1] = m_osc      .process<1>(noise_int15);
      filter_output[1] = m_filter[1].process(osc_output   [1] << 2);
      amp_output   [1] = m_amp   [1].process(filter_output[1]);

      int32_t amp_output_sum_a = amp_output[0] + amp_output[1];

#if defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
      while (m_secondary_core_processing_request) {
        ;
      }
      int32_t amp_output_sum_b = m_secondary_core_processing_result;
#else // defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
      osc_output   [2] = m_osc      .process<2>(noise_int15);
      filter_output[2] = m_filter[2].process(osc_output   [2] << 2);
      amp_output   [2] = m_amp   [2].process(filter_output[2]);

      osc_output   [3] = m_osc      .process<3>(noise_int15);
      filter_output[3] = m_filter[3].process(osc_output   [3] << 2);
      amp_output   [3] = m_amp   [3].process(filter_output[3]);

      int32_t amp_output_sum_b = amp_output[2] + amp_output[3];
#endif // defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)

      voice_mixer_output = (amp_output_sum_a + amp_output_sum_b) >> 2;
    } else if (m_voice_mode == VOICE_PARAPHONIC) {
#if defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
      m_secondary_core_processing_argument = noise_int15;
      m_secondary_core_processing_request = 1;
#endif // defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)

      osc_output[0] = m_osc.process<0>(noise_int15);
      osc_output[1] = m_osc.process<1>(noise_int15);

      int32_t osc_output_sum_a = osc_output[0] + osc_output[1];

#if defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
      while (m_secondary_core_processing_request) {
        ;
      }
      int16_t osc_output_sum_b = m_secondary_core_processing_result;
#else // defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
      osc_output[2] = m_osc.process<2>(noise_int15);
      osc_output[3] = m_osc.process<3>(noise_int15);

      int16_t osc_output_sum_b = osc_output[2] + osc_output[3];
#endif // defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)

      int16_t osc_mixer_output = (osc_output_sum_a + osc_output_sum_b);

      filter_output[0] = m_filter[0].process(osc_mixer_output);
      amp_output   [0] = m_amp   [0].process(filter_output[0]);

      voice_mixer_output = amp_output[0];
    } else {
      osc_output[0] = m_osc.process<0>(noise_int15);
      int16_t osc_mixer_output = osc_output[0] << 1;

      filter_output[0] = m_filter[0].process(osc_mixer_output);
      amp_output   [0] = m_amp   [0].process(filter_output[0]);

      voice_mixer_output = amp_output[0];
    }

    int16_t chorus_fx_output_r;
    int16_t chorus_fx_output_l = m_chorus_fx.process(voice_mixer_output, chorus_fx_output_r);

    int16_t delay_fx_output_r;
    int16_t delay_fx_output_l = m_delay_fx.process(chorus_fx_output_l, chorus_fx_output_r, delay_fx_output_r);

    right_level = delay_fx_output_r;
    return        delay_fx_output_l;
  }

  INLINE void secondary_core_process() {
#if defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
    if (m_secondary_core_processing_request == 1) {
      int16_t noise_int15 = static_cast<int16_t>(m_secondary_core_processing_argument);

      int16_t osc_output   [4];
      int16_t filter_output[4];
      int16_t amp_output   [4];

      if (m_voice_mode == VOICE_POLYPHONIC) {
        osc_output   [2] = m_osc      .process<2>(noise_int15);
        filter_output[2] = m_filter[2].process(osc_output   [2] << 2);
        amp_output   [2] = m_amp   [2].process(filter_output[2]);

        osc_output   [3] = m_osc      .process<3>(noise_int15);
        filter_output[3] = m_filter[3].process(osc_output   [3] << 2);
        amp_output   [3] = m_amp   [3].process(filter_output[3]);

        m_secondary_core_processing_result = amp_output[2] + amp_output[3];
      } else if (m_voice_mode == VOICE_PARAPHONIC) {
        osc_output[2] = m_osc.process<2>(noise_int15);
        osc_output[3] = m_osc.process<3>(noise_int15);

        m_secondary_core_processing_result = osc_output[2] + osc_output[3];
      }

      m_secondary_core_processing_request = 0;
    }
#endif // defined(USE_2_CORES_FOR_SIGNAL_PROCESSING)
  }

private:

  INLINE void note_queue_on(uint8_t note_on_osc_index) {
    if        (m_note_queue[3] == note_on_osc_index) {
      m_note_queue[3] = note_on_osc_index;
    } else if (m_note_queue[2] == note_on_osc_index) {
      m_note_queue[2] = m_note_queue[3];
      m_note_queue[3] = note_on_osc_index;
    } else if (m_note_queue[1] == note_on_osc_index) {
      m_note_queue[1] = m_note_queue[2];
      m_note_queue[2] = m_note_queue[3];
      m_note_queue[3] = note_on_osc_index;
    } else {
      m_note_queue[0] = m_note_queue[1];
      m_note_queue[1] = m_note_queue[2];
      m_note_queue[2] = m_note_queue[3];
      m_note_queue[3] = note_on_osc_index;
    }
  }

  INLINE void note_queue_off(uint8_t note_off_osc_index) {
    if        (m_note_queue[1] == note_off_osc_index) {
      m_note_queue[1] = m_note_queue[0];
      m_note_queue[0] = note_off_osc_index;
    } else if (m_note_queue[2] == note_off_osc_index) {
      m_note_queue[2] = m_note_queue[1];
      m_note_queue[1] = m_note_queue[0];
      m_note_queue[0] = note_off_osc_index;
    } else if (m_note_queue[3] == note_off_osc_index) {
      m_note_queue[3] = m_note_queue[2];
      m_note_queue[2] = m_note_queue[1];
      m_note_queue[1] = m_note_queue[0];
      m_note_queue[0] = note_off_osc_index;
    }
  }

  INLINE void set_voice_mode(uint8_t controller_value) {
    static uint8_t voice_mode_table[6] = {
      VOICE_POLYPHONIC,
      VOICE_PARAPHONIC,
      VOICE_MONOPHONIC,
      VOICE_MONOPHONIC,
      VOICE_LEGATO_PORTA,
      VOICE_LEGATO,
    };

    volatile int32_t index = ((controller_value * 10) + 127) / 254;

    // index = min(index, 5)
    index = index - 5;
    index = (index < 0) * index + 5;

    uint8_t new_voice_mode = voice_mode_table[index];
#if 1
    if (controller_value < 6) {
      new_voice_mode = voice_mode_table[controller_value];
    }
#endif
#if defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
    // due to CPU power shortage
    if (new_voice_mode == VOICE_POLYPHONIC) {
      new_voice_mode = VOICE_PARAPHONIC;
    }
#endif // defined(USE_PWM_AUDIO_INSTEAD_OF_I2S)
    if (m_voice_mode != new_voice_mode) {
      m_voice_mode = new_voice_mode;
      all_sound_off();
      m_osc.set_gate_enabled(m_voice_mode == VOICE_PARAPHONIC);
    }
  }

  INLINE void set_modulation(uint8_t controller_value) {
    m_lfo.set_lfo_depth<1>(controller_value);
  }

  INLINE void set_breath_controller(uint8_t controller_value) {
    m_filter[0].set_breath_controller(controller_value);
    m_filter[1].set_breath_controller(controller_value);
    m_filter[2].set_breath_controller(controller_value);
    m_filter[3].set_breath_controller(controller_value);
    m_amp[0].set_breath_controller(controller_value);
    m_amp[1].set_breath_controller(controller_value);
    m_amp[2].set_breath_controller(controller_value);
    m_amp[3].set_breath_controller(controller_value);
  }

  INLINE void set_sustain_pedal(uint8_t controller_value) {
    if ((m_sustain_pedal == false) && (controller_value >= 64)) {
      m_sustain_pedal = true;
    } else if (m_sustain_pedal && (controller_value < 64)) {
      m_sustain_pedal = false;

      if (m_note_on_number[0] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[0]] == 0) {
          m_note_on_number[0] = NOTE_NUMBER_INVALID;
          note_queue_off(0);
          m_osc.note_off<0>();

          if (m_voice_mode == VOICE_POLYPHONIC) {
            m_eg[0].note_off();
            m_eg[1].note_off();
          }
        }
      }

      if (m_note_on_number[1] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[1]] == 0) {
          m_note_on_number[1] = NOTE_NUMBER_INVALID;
          note_queue_off(1);
          m_osc.note_off<1>();

          if (m_voice_mode == VOICE_POLYPHONIC) {
            m_eg[2].note_off();
            m_eg[3].note_off();
          }
        }
      }

      if (m_note_on_number[2] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[2]] == 0) {
          m_note_on_number[2] = NOTE_NUMBER_INVALID;
          note_queue_off(2);
          m_osc.note_off<2>();

          if (m_voice_mode == VOICE_POLYPHONIC) {
            m_eg[4].note_off();
            m_eg[5].note_off();
          }
        }
      }

      if (m_note_on_number[3] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[3]] == 0) {
          m_note_on_number[3] = NOTE_NUMBER_INVALID;
          note_queue_off(3);
          m_osc.note_off<3>();

          if (m_voice_mode == VOICE_POLYPHONIC) {
            m_eg[6].note_off();
            m_eg[7].note_off();
          }
        }
      }

      if (m_note_on_total_count == 0) {
        if (m_voice_mode != VOICE_POLYPHONIC) {
          m_eg[0].note_off();
          m_eg[1].note_off();
        }
      }
    }
  }

  INLINE void update_eg_osc_mod() {
#if 1
    if        (m_eg_osc_dst == 2 || m_eg_osc_dst >= 96) {  /* OSC_DST_SHAPE_1 */
#else
    if        (m_eg_osc_dst >= 96) {  /* OSC_DST_SHAPE_1 */
#endif
      m_osc.set_pitch_eg_amt<0>(64);
      m_osc.set_pitch_eg_amt<1>(64);
      m_osc.set_shape_eg_amt(m_eg_osc_amt);
#if 1
    } else if (m_eg_osc_dst == 1 || m_eg_osc_dst >= 32) {  /* OSC_DST_PITCH_2 */
#else
    } else if (m_eg_osc_dst >= 32) {  /* OSC_DST_PITCH_2 */
#endif
      m_osc.set_pitch_eg_amt<0>(64);
      m_osc.set_pitch_eg_amt<1>(m_eg_osc_amt);
      m_osc.set_shape_eg_amt(64);
    } else {                         /* OSC_DST_PITCH */
      m_osc.set_pitch_eg_amt<0>(m_eg_osc_amt);
      m_osc.set_pitch_eg_amt<1>(m_eg_osc_amt);
      m_osc.set_shape_eg_amt(64);
    }
  }

  INLINE void update_lfo_osc_mod() {
#if 1
    if        (m_lfo_osc_dst == 2 || m_lfo_osc_dst >= 96) {  /* OSC_DST_SHAPE_1 */
#else
    if        (m_lfo_osc_dst >= 96) {  /* OSC_DST_SHAPE_1 */
#endif
      m_osc.set_pitch_lfo_amt<0>(64);
      m_osc.set_pitch_lfo_amt<1>(64);
      m_osc.set_shape_lfo_amt(m_lfo_osc_amt);
#if 1
    } else if (m_lfo_osc_dst == 1 || m_lfo_osc_dst >= 32) {  /* OSC_DST_PITCH_2 */
#else
    } else if (m_lfo_osc_dst >= 32) {  /* OSC_DST_PITCH_2 */
#endif
      m_osc.set_pitch_lfo_amt<0>(64);
      m_osc.set_pitch_lfo_amt<1>(m_lfo_osc_amt);
      m_osc.set_shape_lfo_amt(64);
    } else {                         /* OSC_DST_PITCH */
      m_osc.set_pitch_lfo_amt<0>(m_lfo_osc_amt);
      m_osc.set_pitch_lfo_amt<1>(m_lfo_osc_amt);
      m_osc.set_shape_lfo_amt(64);
    }
  }

  INLINE void update_eg_and_amp_eg() {
#if 1
    if (m_controller_value_eg_amp_mod == 1 || m_controller_value_eg_amp_mod >= 64) {
#else
    if (m_controller_value_eg_amp_mod >= 64) {
#endif
      m_eg[0].set_attack  (m_controller_value_eg_attack);
      m_eg[1].set_attack  (m_controller_value_eg_attack);
      m_eg[2].set_attack  (m_controller_value_eg_attack);
      m_eg[3].set_attack  (m_controller_value_eg_attack);
      m_eg[4].set_attack  (m_controller_value_eg_attack);
      m_eg[5].set_attack  (m_controller_value_eg_attack);
      m_eg[6].set_attack  (m_controller_value_eg_attack);
      m_eg[7].set_attack  (m_controller_value_eg_attack);

      m_eg[0].set_decay   (m_controller_value_eg_decay);
      m_eg[1].set_decay   (m_controller_value_eg_decay);
      m_eg[2].set_decay   (m_controller_value_eg_decay);
      m_eg[3].set_decay   (m_controller_value_eg_decay);
      m_eg[4].set_decay   (m_controller_value_eg_decay);
      m_eg[5].set_decay   (m_controller_value_eg_decay);
      m_eg[6].set_decay   (m_controller_value_eg_decay);
      m_eg[7].set_decay   (m_controller_value_eg_decay);

      m_eg[0].set_sustain (m_controller_value_eg_sustain);
      m_eg[1].set_sustain (m_controller_value_eg_sustain);
      m_eg[2].set_sustain (m_controller_value_eg_sustain);
      m_eg[3].set_sustain (m_controller_value_eg_sustain);
      m_eg[4].set_sustain (m_controller_value_eg_sustain);
      m_eg[5].set_sustain (m_controller_value_eg_sustain);
      m_eg[6].set_sustain (m_controller_value_eg_sustain);
      m_eg[7].set_sustain (m_controller_value_eg_sustain);

#if 1
      if (m_controller_value_rel_eq_decay == 1 || m_controller_value_rel_eq_decay >= 64) {
#else
      if (m_controller_value_rel_eq_decay >= 64) {
#endif
        m_eg[0].set_release (m_controller_value_eg_decay);
        m_eg[1].set_release (m_controller_value_eg_decay);
        m_eg[2].set_release (m_controller_value_eg_decay);
        m_eg[3].set_release (m_controller_value_eg_decay);
        m_eg[4].set_release (m_controller_value_eg_decay);
        m_eg[5].set_release (m_controller_value_eg_decay);
        m_eg[6].set_release (m_controller_value_eg_decay);
        m_eg[7].set_release (m_controller_value_eg_decay);
      } else {
        m_eg[0].set_release (m_controller_value_eg_release);
        m_eg[1].set_release (m_controller_value_eg_release);
        m_eg[2].set_release (m_controller_value_eg_release);
        m_eg[3].set_release (m_controller_value_eg_release);
        m_eg[4].set_release (m_controller_value_eg_release);
        m_eg[5].set_release (m_controller_value_eg_release);
        m_eg[6].set_release (m_controller_value_eg_release);
        m_eg[7].set_release (m_controller_value_eg_release);
      }
    } else {
      m_eg[0].set_attack  (m_controller_value_eg_attack);
      m_eg[1].set_attack  (m_controller_value_amp_attack);
      m_eg[2].set_attack  (m_controller_value_eg_attack);
      m_eg[3].set_attack  (m_controller_value_amp_attack);
      m_eg[4].set_attack  (m_controller_value_eg_attack);
      m_eg[5].set_attack  (m_controller_value_amp_attack);
      m_eg[6].set_attack  (m_controller_value_eg_attack);
      m_eg[7].set_attack  (m_controller_value_amp_attack);

      m_eg[0].set_decay   (m_controller_value_eg_decay);
      m_eg[1].set_decay   (m_controller_value_amp_decay);
      m_eg[2].set_decay   (m_controller_value_eg_decay);
      m_eg[3].set_decay   (m_controller_value_amp_decay);
      m_eg[4].set_decay   (m_controller_value_eg_decay);
      m_eg[5].set_decay   (m_controller_value_amp_decay);
      m_eg[6].set_decay   (m_controller_value_eg_decay);
      m_eg[7].set_decay   (m_controller_value_amp_decay);

      m_eg[0].set_sustain (m_controller_value_eg_sustain);
      m_eg[1].set_sustain (m_controller_value_amp_sustain);
      m_eg[2].set_sustain (m_controller_value_eg_sustain);
      m_eg[3].set_sustain (m_controller_value_amp_sustain);
      m_eg[4].set_sustain (m_controller_value_eg_sustain);
      m_eg[5].set_sustain (m_controller_value_amp_sustain);
      m_eg[6].set_sustain (m_controller_value_eg_sustain);
      m_eg[7].set_sustain (m_controller_value_amp_sustain);

#if 1
      if (m_controller_value_rel_eq_decay == 1 || m_controller_value_rel_eq_decay >= 64) {
#else
      if (m_controller_value_rel_eq_decay >= 64) {
#endif
        m_eg[0].set_release (m_controller_value_eg_decay);
        m_eg[1].set_release (m_controller_value_amp_decay);
        m_eg[2].set_release (m_controller_value_eg_decay);
        m_eg[3].set_release (m_controller_value_amp_decay);
        m_eg[4].set_release (m_controller_value_eg_decay);
        m_eg[5].set_release (m_controller_value_amp_decay);
        m_eg[6].set_release (m_controller_value_eg_decay);
        m_eg[7].set_release (m_controller_value_amp_decay);
      } else {
        m_eg[0].set_release (m_controller_value_eg_release);
        m_eg[1].set_release (m_controller_value_amp_release);
        m_eg[2].set_release (m_controller_value_eg_release);
        m_eg[3].set_release (m_controller_value_amp_release);
        m_eg[4].set_release (m_controller_value_eg_release);
        m_eg[5].set_release (m_controller_value_amp_release);
        m_eg[6].set_release (m_controller_value_eg_release);
        m_eg[7].set_release (m_controller_value_amp_release);
      }
    }
  }
};
