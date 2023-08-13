#pragma once

#include "pra32-u-common.h"
#include "pra32-u-osc.h"
#include "pra32-u-filter.h"
#include "pra32-u-amp.h"
#include "pra32-u-lfo.h"
#include "pra32-u-noise-gen.h"
#include "pra32-u-eg.h"
#include "pra32-u-chorus-fx.h"
#include "pra32-u-program-table.h"

class PRA32_U_Synth {
  PRA32_U_Osc      m_osc;
  PRA32_U_Filter   m_filter;
  PRA32_U_Amp      m_amp;
  PRA32_U_NoiseGen m_noise_gen;
  PRA32_U_LFO      m_lfo;
  PRA32_U_EG       m_eg[2];
  PRA32_U_ChorusFx m_chorus_fx;

  uint32_t         m_count;

  uint8_t          m_note_queue[4];
  uint8_t          m_note_on_number[4];
  uint8_t          m_note_on_count[128];
  uint8_t          m_note_on_total_count;
  boolean          m_sustain_pedal;
  uint8_t          m_voice_mode;

  uint8_t          m_output_error;
  uint8_t          m_portamento;

  uint8_t          m_chorus_mode;
  uint8_t          m_velocity_to_cutoff;

  uint8_t          m_eg_osc_amt;
  uint8_t          m_eg_osc_dst;
  uint8_t          m_lfo_osc_amt;
  uint8_t          m_lfo_osc_dst;

  uint8_t          m_controller_value_eg_attack;
  uint8_t          m_controller_value_eg_decay;
  uint8_t          m_controller_value_eg_sustain;
  uint8_t          m_controller_value_eg_release;
  uint8_t          m_controller_value_amp_attack;
  uint8_t          m_controller_value_amp_decay;
  uint8_t          m_controller_value_amp_sustain;
  uint8_t          m_controller_value_amp_release;
  uint8_t          m_controller_value_eg_amp_mod;
  uint8_t          m_controller_value_rel_eq_decay;

  uint8_t          m_sp_prog_chg_cc_values[8];

public:
  PRA32_U_Synth()

  : m_osc()
  , m_filter()
  , m_amp()
  , m_noise_gen()
  , m_lfo()
  , m_eg()
  , m_chorus_fx()

  , m_count()

  , m_note_queue()
  , m_note_on_number()
  , m_note_on_count()
  , m_note_on_total_count()
  , m_sustain_pedal()
  , m_voice_mode()

  , m_output_error()
  , m_portamento()

  , m_chorus_mode()
  , m_velocity_to_cutoff()

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

  , m_sp_prog_chg_cc_values()
  {
    m_note_queue[0] = 0;
    m_note_queue[1] = 1;
    m_note_queue[2] = 2;
    m_note_queue[3] = 3;
    m_note_on_number[0] = NOTE_NUMBER_INVALID;
    m_note_on_number[1] = NOTE_NUMBER_INVALID;
    m_note_on_number[2] = NOTE_NUMBER_INVALID;
    m_note_on_number[3] = NOTE_NUMBER_INVALID;
    m_voice_mode = VOICE_PARAPHONIC;

    m_osc.set_mono_mode(m_voice_mode);

    m_amp.set_gain<1>(127);

    m_eg_osc_amt = 64;
    m_lfo_osc_amt = 64;

    program_change(PROGRAM_NUMBER_DEFAULT);
  }

  INLINE void note_on(uint8_t note_number, uint8_t velocity) {
    if (m_note_on_total_count == 255) {
      return;
    }

#if 0
    int8_t cutoff_offset = 0;
    if (m_velocity_to_cutoff == 128) {
      cutoff_offset = velocity - 100;
    } else {
      cutoff_offset = high_sbyte(static_cast<int8_t>(velocity - 100) * (m_velocity_to_cutoff << 1));
    }
#else
    (void) velocity;
#endif

    if (m_voice_mode != VOICE_PARAPHONIC) {
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
          m_eg[0].note_on();
          m_eg[1].note_on();
#if 0
          m_filter.set_cutoff_offset(cutoff_offset);
#endif
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
        m_eg[0].note_on();
        m_eg[1].note_on();
#if 0
        m_filter.set_cutoff_offset(cutoff_offset);
#endif
      }

    } else if (m_note_on_number[0] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<0>(m_portamento);
      m_osc.note_on<0>(note_number);
      m_eg[0].note_on();
      m_eg[1].note_on();
#if 0
      m_filter.set_cutoff_offset(cutoff_offset);
#endif
    } else if (m_note_on_number[1] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<1>(m_portamento);
      m_osc.note_on<1>(note_number);
      m_eg[0].note_on();
      m_eg[1].note_on();
#if 0
      m_filter.set_cutoff_offset(cutoff_offset);
#endif
    } else if (m_note_on_number[2] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<2>(m_portamento);
      m_osc.note_on<2>(note_number);
      m_eg[0].note_on();
      m_eg[1].note_on();
#if 0
      m_filter.set_cutoff_offset(cutoff_offset);
#endif
    } else if (m_note_on_number[3] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      m_osc.set_portamento<3>(m_portamento);
      m_osc.note_on<3>(note_number);
      m_eg[0].note_on();
      m_eg[1].note_on();
#if 0
      m_filter.set_cutoff_offset(cutoff_offset);
#endif
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
      m_eg[0].note_on();
      m_eg[1].note_on();
#if 0
      m_filter.set_cutoff_offset(cutoff_offset);
#endif
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

    if (m_voice_mode != VOICE_PARAPHONIC) {

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
            m_eg[0].note_on();
            m_eg[1].note_on();
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
      }
    } else if (m_note_on_number[1] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[1] = NOTE_NUMBER_INVALID;
        note_queue_off(1);
        m_osc.note_off<1>();
      }
    } else if (m_note_on_number[2] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[2] = NOTE_NUMBER_INVALID;
        note_queue_off(2);
        m_osc.note_off<2>();
      }
    } else if (m_note_on_number[3] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
        note_queue_off(3);
        m_osc.note_off<3>();
      }
    }

    if (m_note_on_total_count == 0) {
      m_eg[0].note_off();
      m_eg[1].note_off();
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
  }

  INLINE void reset_all_controllers() {
    pitch_bend(0, 64);
    set_modulation(0);
#if 0
    set_expression(127);
#endif
    set_sustain_pedal(0);
  }

  INLINE void control_change(uint8_t controller_number, uint8_t controller_value) {
    switch (controller_number) {
#if 0
    case EXPRESSION     :
      m_eg[1].set_expression(controller_value);
      break;
#endif
    case MODULATION     :
      m_lfo.set_lfo_depth<1>(controller_value);
      break;

    case FILTER_CUTOFF  :
      m_filter.set_cutoff(controller_value);
      break;
    case FILTER_RESO    :
      m_filter.set_resonance(controller_value);
      break;
    case FILTER_EG_AMT  :
      m_filter.set_cutoff_eg_amt(controller_value);
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
      m_filter.set_cutoff_lfo_amt(controller_value);
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
    case CHORUS_DLY_TIME:
      m_chorus_fx.set_chorus_delay_time(controller_value);
      break;
    case CHORUS_MODE    :
      m_chorus_fx.set_chorus_mode(controller_value);
      break;

#if 0
    case OSC_LEVEL      :
      m_osc.set_osc_level(controller_value);
      break;
#endif
    case AMP_LEVEL      :
      m_amp.set_gain<1>(controller_value);
      break;

    case PORTAMENTO     :
      m_portamento = controller_value;
      break;

    case P_BEND_BY_CC   :
      {
        uint8_t lsb = 0;
        uint8_t msb = controller_value;
        if (msb == 0x7F) {
          lsb = 0x7F;
        }
        pitch_bend(lsb, msb);
      }
      break;

    case MIXER_OSC_MIX  :
      m_osc.set_osc2_mix(controller_value);
      break;

    case OSC_2_COARSE   :
      m_osc.set_osc2_pitch(controller_value);
      break;

    case OSC_2_FINE     :
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

    case CHORUS_BYPASS  :
      m_chorus_fx.set_chorus_bypass(controller_value);
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
      m_filter.set_filter_mode(controller_value);
      break;

#if 0
    case V_TO_CUTOFF    :
      m_velocity_to_cutoff = ((controller_value + 1) >> 1) << 1;
      break;
#endif

    case FILTER_KEY_TRK :
      m_filter.set_cutoff_pitch_amt(controller_value);
      break;

    case VOICE_MODE     :
      {
        uint8_t new_voice_mode = VOICE_PARAPHONIC;
        if (controller_value >= 112) {
          new_voice_mode = VOICE_LEGATO_PORTA;
        } else if (controller_value >= 80) {
          new_voice_mode = VOICE_LEGATO;
        } else if (controller_value >= 32) {
          new_voice_mode = VOICE_MONOPHONIC;
        }

        if (m_voice_mode != new_voice_mode) {
          m_voice_mode = new_voice_mode;
          all_sound_off();
          boolean mono_mode = (m_voice_mode != VOICE_PARAPHONIC);
          m_osc.set_mono_mode(mono_mode);
        }
      }
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
        uint8_t program_index = controller_number - PC_BY_CC_8;
        uint8_t old_value = m_sp_prog_chg_cc_values[program_index];
        m_sp_prog_chg_cc_values[program_index] = controller_value;
        if ((old_value <= 63) && (controller_value >= 64)) {
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

    control_change(OSC_1_WAVE     , g_preset_table_OSC_1_WAVE     [program_number]);
    control_change(OSC_1_SHAPE    , g_preset_table_OSC_1_SHAPE    [program_number]);
    control_change(OSC_1_MORPH    , g_preset_table_OSC_1_MORPH    [program_number]);
    control_change(MIXER_SUB_OSC  , g_preset_table_MIXER_SUB_OSC  [program_number]);

    control_change(OSC_2_WAVE     , g_preset_table_OSC_2_WAVE     [program_number]);
    control_change(OSC_2_COARSE   , g_preset_table_OSC_2_COARSE   [program_number]);
    control_change(OSC_2_FINE     , g_preset_table_OSC_2_FINE     [program_number]);
    control_change(MIXER_OSC_MIX  , g_preset_table_MIXER_OSC_MIX  [program_number]);

    control_change(FILTER_CUTOFF  , g_preset_table_FILTER_CUTOFF  [program_number]);
    control_change(FILTER_RESO    , g_preset_table_FILTER_RESO    [program_number]);
    control_change(FILTER_EG_AMT  , g_preset_table_FILTER_EG_AMT  [program_number]);
    control_change(FILTER_KEY_TRK , g_preset_table_FILTER_KEY_TRK [program_number]);

    control_change(EG_ATTACK      , g_preset_table_EG_ATTACK      [program_number]);
    control_change(EG_DECAY       , g_preset_table_EG_DECAY       [program_number]);
    control_change(EG_SUSTAIN     , g_preset_table_EG_SUSTAIN     [program_number]);
    control_change(EG_RELEASE     , g_preset_table_EG_RELEASE     [program_number]);

    control_change(EG_OSC_AMT     , g_preset_table_EG_OSC_AMT     [program_number]);
    control_change(EG_OSC_DST     , g_preset_table_EG_OSC_DST     [program_number]);
    control_change(VOICE_MODE     , g_preset_table_VOICE_MODE     [program_number]);
    control_change(PORTAMENTO     , g_preset_table_PORTAMENTO     [program_number]);

    control_change(LFO_WAVE       , g_preset_table_LFO_WAVE       [program_number]);
    control_change(LFO_RATE       , g_preset_table_LFO_RATE       [program_number]);
    control_change(LFO_DEPTH      , g_preset_table_LFO_DEPTH      [program_number]);
    control_change(LFO_FADE_TIME  , g_preset_table_LFO_FADE_TIME  [program_number]);

    control_change(LFO_OSC_AMT    , g_preset_table_LFO_OSC_AMT    [program_number]);
    control_change(LFO_OSC_DST    , g_preset_table_LFO_OSC_DST    [program_number]);
    control_change(LFO_FILTER_AMT , g_preset_table_LFO_FILTER_AMT [program_number]);
    control_change(AMP_LEVEL      , g_preset_table_AMP_LEVEL      [program_number]);

    control_change(AMP_ATTACK     , g_preset_table_AMP_ATTACK     [program_number]);
    control_change(AMP_DECAY      , g_preset_table_AMP_DECAY      [program_number]);
    control_change(AMP_SUSTAIN    , g_preset_table_AMP_SUSTAIN    [program_number]);
    control_change(AMP_RELEASE    , g_preset_table_AMP_RELEASE    [program_number]);

    control_change(CHORUS_MODE    , g_preset_table_CHORUS_MODE    [program_number]);
    control_change(CHORUS_RATE    , g_preset_table_CHORUS_RATE    [program_number]);
    control_change(CHORUS_DEPTH   , g_preset_table_CHORUS_DEPTH   [program_number]);
    control_change(CHORUS_DLY_TIME, g_preset_table_CHORUS_DLY_TIME[program_number]);

    control_change(P_BEND_RANGE   , g_preset_table_P_BEND_RANGE   [program_number]);
    control_change(CHORUS_BYPASS  , g_preset_table_CHORUS_BYPASS  [program_number]);
    control_change(EG_AMP_MOD     , g_preset_table_EG_AMP_MOD     [program_number]);
    control_change(REL_EQ_DECAY   , g_preset_table_REL_EQ_DECAY   [program_number]);

    control_change(FILTER_MODE    , g_preset_table_FILTER_MODE    [program_number]);
  }

  INLINE int16_t process(int16_t& right_level) {
    ++m_count;

    int16_t noise_int15 = m_noise_gen.process();

    switch (m_count & (0x04 - 1)) {
    case 0x00:
      break;
    case 0x01:
      m_eg[0].process_at_low_rate();
      break;
    case 0x02:
      m_eg[1].process_at_low_rate();
      break;
    case 0x03:
      m_lfo.process_at_low_rate(m_count >> 2, noise_int15);
      break;
    }

    int16_t lfo_output = m_lfo.get_output();
    int16_t eg_output_0 = m_eg[0].get_output();
    int16_t eg_output_1 = m_eg[1].get_output();

    switch (m_count & (0x04 - 1)) {
    case 0x00:
      m_osc.process_at_low_rate(m_count >> 2, noise_int15, lfo_output, eg_output_0);
      break;
    case 0x01:
      m_filter.process_at_low_rate(m_count >> 2, eg_output_0, lfo_output, m_osc.get_osc_pitch());
      break;
    case 0x02:
      m_amp.process_at_low_rate(eg_output_1);
      break;
    case 0x03:
      m_chorus_fx.process_at_low_rate();
      break;
    }

    int16_t osc_output = m_osc.process(noise_int15);
    int16_t filter_output = m_filter.process(osc_output);
    int16_t amp_output = m_amp.process(filter_output);

    int16_t left_level = m_chorus_fx.process(amp_output, right_level);
    return left_level;
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

  INLINE void set_modulation(uint8_t controller_value) {
    m_lfo.set_lfo_depth<1>(controller_value);
  }

#if 0
  INLINE void set_expression(uint8_t controller_value) {
    m_eg[1].set_expression(controller_value);
  }
#endif

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
        }
      }

      if (m_note_on_number[1] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[1]] == 0) {
          m_note_on_number[1] = NOTE_NUMBER_INVALID;
          note_queue_off(1);
          m_osc.note_off<1>();
        }
      }

      if (m_note_on_number[2] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[2]] == 0) {
          m_note_on_number[2] = NOTE_NUMBER_INVALID;
          note_queue_off(2);
          m_osc.note_off<2>();
        }
      }

      if (m_note_on_number[3] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[3]] == 0) {
          m_note_on_number[3] = NOTE_NUMBER_INVALID;
          note_queue_off(3);
          m_osc.note_off<3>();
        }
      }

      if (m_note_on_total_count == 0) {
        m_eg[0].note_off();
        m_eg[1].note_off();
      }
    }
  }

  INLINE void update_eg_osc_mod() {
    if        (m_eg_osc_dst < 32) {  /* OSC_DST_PITCH */
      m_osc.set_pitch_eg_amt<0>(m_eg_osc_amt);
      m_osc.set_pitch_eg_amt<1>(m_eg_osc_amt);
      m_osc.set_shape_eg_amt(64);
    } else if (m_eg_osc_dst < 96) {  /* OSC_DST_PITCH_2 */
      m_osc.set_pitch_eg_amt<0>(64);
      m_osc.set_pitch_eg_amt<1>(m_eg_osc_amt);
      m_osc.set_shape_eg_amt(64);
    } else {                         /* OSC_DST_SHAPE_1 */
      m_osc.set_pitch_eg_amt<0>(64);
      m_osc.set_pitch_eg_amt<1>(64);
      m_osc.set_shape_eg_amt(m_eg_osc_amt);
    }
  }

  INLINE void update_lfo_osc_mod() {
    if        (m_lfo_osc_dst < 32) {  /* OSC_DST_PITCH */
      m_osc.set_pitch_lfo_amt<0>(m_lfo_osc_amt);
      m_osc.set_pitch_lfo_amt<1>(m_lfo_osc_amt);
      m_osc.set_shape_lfo_amt(64);
    } else if (m_lfo_osc_dst < 96) {  /* OSC_DST_PITCH_2 */
      m_osc.set_pitch_lfo_amt<0>(64);
      m_osc.set_pitch_lfo_amt<1>(m_lfo_osc_amt);
      m_osc.set_shape_lfo_amt(64);
    } else {                          /* OSC_DST_SHAPE_1 */
      m_osc.set_pitch_lfo_amt<0>(64);
      m_osc.set_pitch_lfo_amt<1>(64);
      m_osc.set_shape_lfo_amt(m_lfo_osc_amt);
    }
  }

  INLINE void update_eg_and_amp_eg() {
    if (m_controller_value_eg_amp_mod < 64) {
      m_eg[0].set_attack  (m_controller_value_eg_attack);
      m_eg[1].set_attack  (m_controller_value_amp_attack);

      m_eg[0].set_decay   (m_controller_value_eg_decay);
      m_eg[1].set_decay   (m_controller_value_amp_decay);

      m_eg[0].set_sustain (m_controller_value_eg_sustain);
      m_eg[1].set_sustain (m_controller_value_amp_sustain);

      if (m_controller_value_rel_eq_decay < 64) {
        m_eg[0].set_release (m_controller_value_eg_release);
        m_eg[1].set_release (m_controller_value_amp_release);
      } else {
        m_eg[0].set_release (m_controller_value_eg_decay);
        m_eg[1].set_release (m_controller_value_amp_decay);
      }

    } else {
      m_eg[0].set_attack  (m_controller_value_eg_attack);
      m_eg[1].set_attack  (m_controller_value_eg_attack);

      m_eg[0].set_decay   (m_controller_value_eg_decay);
      m_eg[1].set_decay   (m_controller_value_eg_decay);

      m_eg[0].set_sustain (m_controller_value_eg_sustain);
      m_eg[1].set_sustain (m_controller_value_eg_sustain);

      if (m_controller_value_rel_eq_decay < 64) {
        m_eg[0].set_release (m_controller_value_eg_release);
        m_eg[1].set_release (m_controller_value_eg_release);
      } else {
        m_eg[0].set_release (m_controller_value_eg_decay);
        m_eg[1].set_release (m_controller_value_eg_decay);
      }
    }
  }
};
