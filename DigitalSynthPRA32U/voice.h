#pragma once

#include "common.h"
#include "program-table.h"

template <uint8_t T>
class Voice {
  static uint8_t  m_count;

  static uint8_t  m_note_queue[4];
  static uint8_t  m_note_on_number[4];
  static uint8_t  m_note_on_count[128];
  static uint8_t  m_note_on_total_count;
  static boolean  m_sustain_pedal;
  static uint8_t  m_voice_mode;

  static uint8_t  m_output_error;
  static uint8_t  m_portamento;

  static uint8_t  m_chorus_mode;
  static uint8_t  m_velocity_to_cutoff;

  static uint8_t  m_eg_osc_amt;
  static uint8_t  m_eg_osc_dst;
  static uint8_t  m_lfo_osc_amt;
  static uint8_t  m_lfo_osc_dst;

  static uint16_t m_rnd;
  static uint8_t  m_sp_prog_chg_cc_values[8];

  static uint8_t  m_param_chorus_mode;
  static boolean  m_param_chorus_bypass;

public:
  INLINE static void initialize() {
    m_count = 0;

    m_note_queue[0] = 0;
    m_note_queue[1] = 1;
    m_note_queue[2] = 2;
    m_note_queue[3] = 3;
    m_note_on_number[0] = NOTE_NUMBER_INVALID;
    m_note_on_number[1] = NOTE_NUMBER_INVALID;
    m_note_on_number[2] = NOTE_NUMBER_INVALID;
    m_note_on_number[3] = NOTE_NUMBER_INVALID;
    m_voice_mode = VOICE_PARAPHONIC;

    IOsc<0>::initialize();
    IOsc<0>::set_mono_mode(m_voice_mode);
    IFilter<0>::initialize();
    IAmp<0>::initialize();

    IEG<0>::initialize();
    IEG<1>::initialize();
    IEG<1>::set_gain<0>(90);
    IEG<1>::set_gain<1>(127);

    IDelayFx<0>::initialize();

    m_eg_osc_amt = 64;
    m_lfo_osc_amt = 64;

    m_rnd = 1;
  }

  INLINE static void note_on(uint8_t note_number, uint8_t velocity) {
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
            IOsc<0>::set_portamento<0>(0);
            IOsc<0>::set_portamento<2>(0);
          } else {
            IOsc<0>::set_portamento<0>(m_portamento);
            IOsc<0>::set_portamento<2>(m_portamento);
          }
          IOsc<0>::note_on<0>(note_number);
          IOsc<0>::note_on<2>(note_number);
          IOsc<0>::trigger_lfo();
          IEG<0>::note_on();
          IEG<1>::note_on();
#if 0
          IFilter<0>::set_cutoff_offset(cutoff_offset);
#endif
        } else {
          m_note_on_number[3] = m_note_on_number[2];
          m_note_on_number[2] = m_note_on_number[1];
          m_note_on_number[1] = m_note_on_number[0];
          m_note_on_number[0] = note_number;

          IOsc<0>::set_portamento<0>(m_portamento);
          IOsc<0>::set_portamento<2>(m_portamento);
          IOsc<0>::note_on<0>(note_number);
          IOsc<0>::note_on<2>(note_number);
        }
      } else {
        ++m_note_on_total_count;
        ++m_note_on_count[note_number];

        m_note_on_number[3] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[1];
        m_note_on_number[1] = m_note_on_number[0];
        m_note_on_number[0] = note_number;

        IOsc<0>::set_portamento<0>(m_portamento);
        IOsc<0>::set_portamento<2>(m_portamento);
        IOsc<0>::note_on<0>(note_number);
        IOsc<0>::note_on<2>(note_number);
        IOsc<0>::trigger_lfo();
        IEG<0>::note_on();
        IEG<1>::note_on();
#if 0
        IFilter<0>::set_cutoff_offset(cutoff_offset);
#endif
      }

    } else if (m_note_on_number[0] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<0>(m_portamento);
      IOsc<0>::note_on<0>(note_number);
      IEG<0>::note_on();
      IEG<1>::note_on();
#if 0
      IFilter<0>::set_cutoff_offset(cutoff_offset);
#endif
    } else if (m_note_on_number[1] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<1>(m_portamento);
      IOsc<0>::note_on<1>(note_number);
      IEG<0>::note_on();
      IEG<1>::note_on();
#if 0
      IFilter<0>::set_cutoff_offset(cutoff_offset);
#endif
    } else if (m_note_on_number[2] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<2>(m_portamento);
      IOsc<0>::note_on<2>(note_number);
      IEG<0>::note_on();
      IEG<1>::note_on();
#if 0
      IFilter<0>::set_cutoff_offset(cutoff_offset);
#endif
    } else if (m_note_on_number[3] == note_number) {
      ++m_note_on_total_count;
      ++m_note_on_count[note_number];

      IOsc<0>::set_portamento<3>(m_portamento);
      IOsc<0>::note_on<3>(note_number);
      IEG<0>::note_on();
      IEG<1>::note_on();
#if 0
      IFilter<0>::set_cutoff_offset(cutoff_offset);
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
        IOsc<0>::set_portamento<0>(m_portamento);
        IOsc<0>::note_on<0>(note_number);
        break;
      case 1:
        IOsc<0>::set_portamento<1>(m_portamento);
        IOsc<0>::note_on<1>(note_number);
        break;
      case 2:
        IOsc<0>::set_portamento<2>(m_portamento);
        IOsc<0>::note_on<2>(note_number);
        break;
      case 3:
        IOsc<0>::set_portamento<3>(m_portamento);
        IOsc<0>::note_on<3>(note_number);
        break;
      }

      if (prev_note_on_total_count == 0) {
        IOsc<0>::trigger_lfo();
      }
      IEG<0>::note_on();
      IEG<1>::note_on();
#if 0
      IFilter<0>::set_cutoff_offset(cutoff_offset);
#endif
    }
  }

  INLINE static void note_off(uint8_t note_number) {
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
        IOsc<0>::note_off<0>();
        IOsc<0>::note_off<1>();
        IOsc<0>::note_off<2>();
        IOsc<0>::note_off<3>();
      } else if (m_note_on_number[0] == note_number) {
        m_note_on_number[0] = m_note_on_number[1];
        m_note_on_number[1] = m_note_on_number[2];
        m_note_on_number[2] = m_note_on_number[3];
        m_note_on_number[3] = NOTE_NUMBER_INVALID;

        if (m_note_on_number[0] != NOTE_NUMBER_INVALID) {
          IOsc<0>::set_portamento<0>(m_portamento);
          IOsc<0>::set_portamento<2>(m_portamento);
          IOsc<0>::note_on<0>(m_note_on_number[0]);
          IOsc<0>::note_on<2>(m_note_on_number[0]);

          if (m_voice_mode == VOICE_MONOPHONIC) {
            IOsc<0>::trigger_lfo();
            IEG<0>::note_on();
            IEG<1>::note_on();
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
        IOsc<0>::note_off<0>();
      }
    } else if (m_note_on_number[1] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[1] = NOTE_NUMBER_INVALID;
        note_queue_off(1);
        IOsc<0>::note_off<1>();
      }
    } else if (m_note_on_number[2] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[2] = NOTE_NUMBER_INVALID;
        note_queue_off(2);
        IOsc<0>::note_off<2>();
      }
    } else if (m_note_on_number[3] == note_number) {
      if (m_note_on_count[note_number] == 0) {
        m_note_on_number[3] = NOTE_NUMBER_INVALID;
        note_queue_off(3);
        IOsc<0>::note_off<3>();
      }
    }

    if (m_note_on_total_count == 0) {
      IEG<0>::note_off();
      IEG<1>::note_off();
    }
  }

  static void all_sound_off() {
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
    IOsc<0>::note_off<0>();
    IOsc<0>::note_off<1>();
    IOsc<0>::note_off<2>();
    IOsc<0>::note_off<3>();
    IEG<0>::note_off();
    IEG<1>::note_off();
  }

  INLINE static void reset_all_controllers() {
    pitch_bend(0, 64);
    set_modulation(0);
#if 0
    set_expression(127);
#endif
    set_sustain_pedal(0);
  }

  INLINE static void control_change(uint8_t controller_number, uint8_t controller_value) {
    switch (controller_number) {
#if 0
    case EXPRESSION     :
      IEG<1>::set_expression(controller_value);
      break;
#endif
    case MODULATION     :
      IOsc<0>::set_lfo_depth<1>(controller_value);
      break;

    case FILTER_CUTOFF  :
      IFilter<0>::set_cutoff(controller_value);
      break;
    case FILTER_RESO    :
      IFilter<0>::set_resonance(controller_value);
      break;
    case FILTER_EG_AMT  :
      IFilter<0>::set_cutoff_eg_amt(controller_value);
      break;

    case OSC_1_WAVE     :
      IOsc<0>::set_osc_waveform<0>(controller_value);
      break;
    case OSC_2_WAVE     :
      IOsc<0>::set_osc_waveform<1>(controller_value);
      break;
    case OSC_1_SHAPE    :
      IOsc<0>::set_osc1_shape_control(controller_value);
      break;
    case OSC_1_MORPH    :
      IOsc<0>::set_osc1_morph_control(controller_value);
      break;
    case MIXER_SUB_OSC  :
      IOsc<0>::set_mixer_sub_osc_control(controller_value);
      break;

    case LFO_RATE       :
      IOsc<0>::set_lfo_rate(controller_value);
      break;
    case LFO_DEPTH      :
      IOsc<0>::set_lfo_depth<0>(controller_value);
      break;
    case LFO_OSC_AMT    :
      m_lfo_osc_amt = controller_value;
      update_lfo_osc_mod();
      break;
    case LFO_FILTER_AMT :
      IFilter<0>::set_cutoff_lfo_amt(controller_value);
      break;

    case SUSTAIN_PEDAL   :
      set_sustain_pedal(controller_value);
      break;

    case EG_ATTACK      :
      IEG<0>::set_attack(controller_value);
      break;
    case EG_DECAY       :
      IEG<0>::set_decay(controller_value);
      break;
    case EG_SUSTAIN     :
      IEG<0>::set_sustain(controller_value);
      break;
    case EG_RELEASE     :
      IEG<0>::set_release(controller_value);
      break;

    case AMP_ATTACK     :
      IEG<1>::set_attack(controller_value);
      break;
    case AMP_DECAY      :
      IEG<1>::set_decay(controller_value);
      break;
    case AMP_SUSTAIN    :
      IEG<1>::set_sustain(controller_value);
      break;
    case AMP_RELEASE    :
      IEG<1>::set_release(controller_value);
      break;

    case CHORUS_DEPTH   :
      IOsc<0>::set_chorus_depth(controller_value);
      break;
    case CHORUS_RATE    :
      IOsc<0>::set_chorus_rate(controller_value);
      break;
    case CHORUS_DLY_TIME:
      IOsc<0>::set_chorus_delay_time(controller_value);
      break;
    case CHORUS_MODE    :
      {
        uint8_t new_param_chorus_mode = CHORUS_MODE_STEREO_2;

        if        (controller_value < 16) {
          new_param_chorus_mode = CHORUS_MODE_OFF;
        } else if (controller_value < 48) {
          new_param_chorus_mode = CHORUS_MODE_MONO;
        } else if (controller_value < 80) {
          new_param_chorus_mode = CHORUS_MODE_P_STEREO;
        } else if (controller_value < 112) {
          new_param_chorus_mode = CHORUS_MODE_STEREO;
        }

        update_chorus_mode(new_param_chorus_mode, m_param_chorus_bypass);
      }
      break;

#if 0
    case OSC_LEVEL      :
      IOsc<0>::set_osc_level(controller_value);
      break;
#endif
    case AMP_LEVEL      :
      IEG<1>::set_gain<1>(controller_value);
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
      IOsc<0>::set_mono_osc2_mix(controller_value);
      break;

    case OSC_2_COARSE   :
      IOsc<0>::set_mono_osc2_pitch(controller_value);
      break;

    case OSC_2_FINE     :
      IOsc<0>::set_mono_osc2_detune(controller_value);
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
      IOsc<0>::set_lfo_waveform(controller_value);
      break;

    case LFO_FADE_TIME  :
      IOsc<0>::set_lfo_fade_time(controller_value);
      break;

    case P_BEND_RANGE   :
      IOsc<0>::set_pitch_bend_range(controller_value);
      break;

    case CHORUS_BYPASS  :
      update_chorus_mode(m_param_chorus_mode, controller_value >= 64);
      break;

#if 0
    case V_TO_CUTOFF    :
      m_velocity_to_cutoff = ((controller_value + 1) >> 1) << 1;
      break;
#endif

    case FILTER_KEY_TRK :
      IFilter<0>::set_cutoff_pitch_amt(controller_value);
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
          IOsc<0>::set_mono_mode(mono_mode);
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

    // Special Program Change
    case SP_PROG_CHG_0  :
    case SP_PROG_CHG_1  :
    case SP_PROG_CHG_2  :
    case SP_PROG_CHG_3  :
    case SP_PROG_CHG_4  :
    case SP_PROG_CHG_5  :
    case SP_PROG_CHG_6  :
    case SP_PROG_CHG_7  :
      {
        uint8_t program_number = controller_number - SP_PROG_CHG_0;
        uint8_t old_value = m_sp_prog_chg_cc_values[program_number];
        m_sp_prog_chg_cc_values[program_number] = controller_value;
        if ((old_value <= 63) && (controller_value >= 64)) {
          program_change(program_number);
        }
      }
      break;
    }
  }

  INLINE static void pitch_bend(uint8_t lsb, uint8_t msb) {
    int16_t pitch_bend = ((static_cast<uint16_t>(msb) << 8) >> 1) + lsb - 8192;
    IOsc<0>::set_pitch_bend(pitch_bend);
  }

  /* INLINE */ static void program_change(uint8_t program_number) {
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


  }

  INLINE static int16_t process(int16_t& right_level) {
    ++m_count;

    int16_t eg_output_0 = IEG<0>::process(m_count);
    int16_t osc_output = IOsc<0>::process(m_count, eg_output_0);
    int16_t lfo_output = IOsc<0>::get_lfo_level();
    int16_t filter_output = IFilter<0>::process(m_count, osc_output, eg_output_0, lfo_output);
    int16_t eg_output_1 = IEG<1>::process(m_count);
    int16_t amp_output = IAmp<0>::process(filter_output, eg_output_1);

    int16_t dir_sample = amp_output;

    int16_t eff_sample_0 = IDelayFx<0>::get(IOsc<0>::get_chorus_delay_time<0>());
    int16_t eff_sample_1 = IDelayFx<0>::get(IOsc<0>::get_chorus_delay_time<1>());
    IDelayFx<0>::push(dir_sample);

    if (m_chorus_mode >= CHORUS_MODE_MONO) {
      // For Mono Chorus and Stereo 2-phase Chorus
      right_level = dir_sample + eff_sample_1;
      return        dir_sample + eff_sample_0;
    }

    if (m_chorus_mode == CHORUS_MODE_P_STEREO) {
      // For Pseudo-Stereo Chorus
      right_level = dir_sample - eff_sample_0;
      return        dir_sample + eff_sample_0;
    }

    // For Off and Stereo Chorus
    right_level = dir_sample;
    return        eff_sample_0;
  }

private:

  INLINE static void note_queue_on(uint8_t note_on_osc_index) {
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

  INLINE static void note_queue_off(uint8_t note_off_osc_index) {
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

  static uint8_t get_rnd_7() {
    m_rnd = m_rnd ^ (m_rnd << 5);
    m_rnd = m_rnd ^ (m_rnd >> 9);
    m_rnd = m_rnd ^ (m_rnd << 8);
    return low_byte(m_rnd) >> 1;
  }

  INLINE static void set_modulation(uint8_t controller_value) {
    IOsc<0>::set_lfo_depth<1>(controller_value);
  }

#if 0
  INLINE static void set_expression(uint8_t controller_value) {
    IEG<1>::set_expression(controller_value);
  }
#endif

  INLINE static void set_sustain_pedal(uint8_t controller_value) {
    if ((m_sustain_pedal == false) && (controller_value >= 64)) {
      m_sustain_pedal = true;
    } else if (m_sustain_pedal && (controller_value < 64)) {
      m_sustain_pedal = false;

      if (m_note_on_number[0] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[0]] == 0) {
          m_note_on_number[0] = NOTE_NUMBER_INVALID;
          note_queue_off(0);
          IOsc<0>::note_off<0>();
        }
      }

      if (m_note_on_number[1] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[1]] == 0) {
          m_note_on_number[1] = NOTE_NUMBER_INVALID;
          note_queue_off(1);
          IOsc<0>::note_off<1>();
        }
      }

      if (m_note_on_number[2] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[2]] == 0) {
          m_note_on_number[2] = NOTE_NUMBER_INVALID;
          note_queue_off(2);
          IOsc<0>::note_off<2>();
        }
      }

      if (m_note_on_number[3] != NOTE_NUMBER_INVALID) {
        if (m_note_on_count[m_note_on_number[3]] == 0) {
          m_note_on_number[3] = NOTE_NUMBER_INVALID;
          note_queue_off(3);
          IOsc<0>::note_off<3>();
        }
      }

      if (m_note_on_total_count == 0) {
        IEG<0>::note_off();
        IEG<1>::note_off();
      }
    }
  }

  INLINE static void update_eg_osc_mod() {
    if        (m_eg_osc_dst < 32) {  /* OSC_DST_PITCH */
      IOsc<0>::set_pitch_eg_amt<0>(m_eg_osc_amt);
      IOsc<0>::set_pitch_eg_amt<1>(m_eg_osc_amt);
      IOsc<0>::set_shape_eg_amt(64);
    } else if (m_eg_osc_dst < 96) {  /* OSC_DST_PITCH_2 */
      IOsc<0>::set_pitch_eg_amt<0>(64);
      IOsc<0>::set_pitch_eg_amt<1>(m_eg_osc_amt);
      IOsc<0>::set_shape_eg_amt(64);
    } else {                         /* OSC_DST_SHAPE_1 */
      IOsc<0>::set_pitch_eg_amt<0>(64);
      IOsc<0>::set_pitch_eg_amt<1>(64);
      IOsc<0>::set_shape_eg_amt(m_eg_osc_amt);
    }
  }

  INLINE static void update_lfo_osc_mod() {
    if        (m_lfo_osc_dst < 32) {  /* OSC_DST_PITCH */
      IOsc<0>::set_pitch_lfo_amt<0>(m_lfo_osc_amt);
      IOsc<0>::set_pitch_lfo_amt<1>(m_lfo_osc_amt);
      IOsc<0>::set_shape_lfo_amt(64);
    } else if (m_lfo_osc_dst < 96) {  /* OSC_DST_PITCH_2 */
      IOsc<0>::set_pitch_lfo_amt<0>(64);
      IOsc<0>::set_pitch_lfo_amt<1>(m_lfo_osc_amt);
      IOsc<0>::set_shape_lfo_amt(64);
    } else {                          /* OSC_DST_SHAPE_1 */
      IOsc<0>::set_pitch_lfo_amt<0>(64);
      IOsc<0>::set_pitch_lfo_amt<1>(64);
      IOsc<0>::set_shape_lfo_amt(m_lfo_osc_amt);
    }
  }

  INLINE static void update_chorus_mode(uint8_t new_param_chorus_mode, boolean new_param_chorus_bypass) {
    if ((m_param_chorus_mode   != new_param_chorus_mode) ||
        (m_param_chorus_bypass != new_param_chorus_bypass)) {
      m_param_chorus_mode   = new_param_chorus_mode;
      m_param_chorus_bypass = new_param_chorus_bypass;
      IDelayFx<0>::attenuate();

      if (m_param_chorus_bypass) {
        m_chorus_mode = CHORUS_MODE_OFF;
        IOsc<0>::set_chorus_mode(CHORUS_MODE_OFF);
        IEG<1>::set_gain<0>(127);
      } else {
        m_chorus_mode = m_param_chorus_mode;
        switch (m_chorus_mode) {
        case CHORUS_MODE_OFF      :
          IOsc<0>::set_chorus_mode(CHORUS_MODE_OFF);
          IEG<1>::set_gain<0>(90);
          break;
        case CHORUS_MODE_STEREO   :
          IOsc<0>::set_chorus_mode(CHORUS_MODE_STEREO);
          IEG<1>::set_gain<0>(90);
          break;
        case CHORUS_MODE_P_STEREO :
          IOsc<0>::set_chorus_mode(CHORUS_MODE_P_STEREO);
          IEG<1>::set_gain<0>(64);
          break;
        case CHORUS_MODE_MONO     :
          IOsc<0>::set_chorus_mode(CHORUS_MODE_MONO);
          IEG<1>::set_gain<0>(64);
          break;
        case CHORUS_MODE_STEREO_2 :
          IOsc<0>::set_chorus_mode(CHORUS_MODE_STEREO_2);
          IEG<1>::set_gain<0>(64);
          break;
        }
      }
    }
  }
};

template <uint8_t T> uint8_t  Voice<T>::m_count;

template <uint8_t T> uint8_t  Voice<T>::m_note_queue[4];
template <uint8_t T> uint8_t  Voice<T>::m_note_on_number[4];
template <uint8_t T> uint8_t  Voice<T>::m_note_on_count[128];
template <uint8_t T> uint8_t  Voice<T>::m_note_on_total_count;
template <uint8_t T> boolean  Voice<T>::m_sustain_pedal;
template <uint8_t T> uint8_t  Voice<T>::m_voice_mode;

template <uint8_t T> uint8_t  Voice<T>::m_output_error;
template <uint8_t T> uint8_t  Voice<T>::m_portamento;

template <uint8_t T> uint8_t  Voice<T>::m_chorus_mode;
template <uint8_t T> uint8_t  Voice<T>::m_velocity_to_cutoff;

template <uint8_t T> uint8_t  Voice<T>::m_eg_osc_amt;
template <uint8_t T> uint8_t  Voice<T>::m_eg_osc_dst;
template <uint8_t T> uint8_t  Voice<T>::m_lfo_osc_amt;
template <uint8_t T> uint8_t  Voice<T>::m_lfo_osc_dst;

template <uint8_t T> uint16_t Voice<T>::m_rnd;
template <uint8_t T> uint8_t  Voice<T>::m_sp_prog_chg_cc_values[8];

template <uint8_t T> uint8_t  Voice<T>::m_param_chorus_mode;
template <uint8_t T> boolean  Voice<T>::m_param_chorus_bypass;
