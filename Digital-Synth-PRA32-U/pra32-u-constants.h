#pragma once

const double    A4_FREQ = 440.0;

const uint32_t  SAMPLING_RATE       = 48000;
const uint32_t  FREQUENCY_MAX       = 23000;
const uint8_t   BIT_DEPTH           = 8;
const uint8_t   NOTE_NUMBER_MIN     = 0;
const uint8_t   NOTE_NUMBER_MAX     = 127;
const uint8_t   NOTE_NUMBER_INVALID = 255;

const uint8_t   PRESET_PROGRAM_NUMBER_MAX    = 7;
const uint8_t   PROGRAM_NUMBER_MAX           = 15;

const int8_t    OSC_CONTROL_INTERVAL_BITS    = 1;
const uint8_t   OSC_CONTROL_INTERVAL         = 0x01 << OSC_CONTROL_INTERVAL_BITS;
const int8_t    OSC_PHASE_RESOLUTION_BITS    = 24;
const int8_t    OSC_TUNE_TABLE_STEPS_BITS    = 8;
const int8_t    OSC_TUNE_DENOMINATOR_BITS    = 15;
const uint8_t   OSC_WAVE_TABLE_AMP_BITS      = 13;
const int16_t   OSC_WAVE_TABLE_AMP           = 1 << OSC_WAVE_TABLE_AMP_BITS;
const int8_t    OSC_WAVE_TABLE_SAMPLES_BITS  = 9;
const int8_t    OSC_DETUNE_MUL_NUM_BITS      = 4;
const uint16_t  OSC_DETUNE_FREQ_MAX          = (20 * 2) * (1 << 24) / SAMPLING_RATE;
const int8_t    FILTER_CONTROL_INTERVAL_BITS = 3;
const uint8_t   FILTER_CONTROL_INTERVAL      = 0x01 << FILTER_CONTROL_INTERVAL_BITS;
const int8_t    FILTER_TABLE_FRACTION_BITS   = 30;
const uint8_t   EG_CONTROL_INTERVAL          = 0x10;
const int32_t   EG_LEVEL_MAX                 = 0x40000000;


const uint8_t   DATA_BYTE_MAX         = 0x7F;
const uint8_t   STATUS_BYTE_INVALID   = 0x7F;
const uint8_t   DATA_BYTE_INVALID     = 0x80;
const uint8_t   STATUS_BYTE_MIN       = 0x80;
const uint8_t   NOTE_OFF              = 0x80;
const uint8_t   NOTE_ON               = 0x90;
const uint8_t   CONTROL_CHANGE        = 0xB0;
const uint8_t   PROGRAM_CHANGE        = 0xC0;
const uint8_t   PITCH_BEND            = 0xE0;
const uint8_t   SYSTEM_MESSAGE_MIN    = 0xF0;
const uint8_t   SYSTEM_EXCLUSIVE      = 0xF0;
const uint8_t   TIME_CODE             = 0xF1;
const uint8_t   SONG_POSITION         = 0xF2;
const uint8_t   SONG_SELECT           = 0xF3;
const uint8_t   TUNE_REQUEST          = 0xF6;
const uint8_t   EOX                   = 0xF7;
const uint8_t   REAL_TIME_MESSAGE_MIN = 0xF8;
const uint8_t   ACTIVE_SENSING        = 0xFE;


const uint8_t   MODULATION      = 1;
const uint8_t   BTH_CONTROLLER  = 2;
const uint8_t   SUSTAIN_PEDAL   = 64;


const uint8_t   OSC_1_WAVE      = 102;
const uint8_t   OSC_1_SHAPE     = 19;
const uint8_t   OSC_1_MORPH     = 20;
const uint8_t   MIXER_SUB_OSC   = 23;

const uint8_t   OSC_2_WAVE      = 104;
const uint8_t   OSC_2_COARSE    = 85;
const uint8_t   OSC_2_PITCH     = 76;
const uint8_t   MIXER_OSC_MIX   = 21;

const uint8_t   FILTER_CUTOFF   = 74;
const uint8_t   FILTER_RESO     = 71;
const uint8_t   FILTER_EG_AMT   = 24;
const uint8_t   FILTER_KEY_TRK  = 26;

const uint8_t   EG_ATTACK       = 73;
const uint8_t   EG_DECAY        = 75;
const uint8_t   EG_SUSTAIN      = 30;
const uint8_t   EG_RELEASE      = 72;

const uint8_t   EG_OSC_AMT      = 91;
const uint8_t   EG_OSC_DST      = 89;
const uint8_t   VOICE_MODE      = 14;
const uint8_t   PORTAMENTO      = 5;

const uint8_t   LFO_WAVE        = 12;
const uint8_t   LFO_RATE        = 3;
const uint8_t   LFO_DEPTH       = 17;
const uint8_t   LFO_FADE_TIME   = 56;

const uint8_t   LFO_OSC_AMT     = 13;
const uint8_t   LFO_OSC_DST     = 103;
const uint8_t   LFO_FILTER_AMT  = 25;
const uint8_t   AMP_GAIN        = 15;

const uint8_t   AMP_ATTACK      = 52;
const uint8_t   AMP_DECAY       = 53;
const uint8_t   AMP_SUSTAIN     = 54;
const uint8_t   AMP_RELEASE     = 55;

const uint8_t   FILTER_MODE     = 78;
const uint8_t   EG_AMP_MOD      = 28;
const uint8_t   REL_EQ_DECAY    = 29;
const uint8_t   P_BEND_RANGE    = 57;

const uint8_t   BTH_FILTER_AMT  = 60;
const uint8_t   BTH_AMP_MOD     = 61;
const uint8_t   EG_VEL_SENS     = 62;
const uint8_t   AMP_VEL_SENS    = 63;

const uint8_t   CHORUS_MIX      = 27;
const uint8_t   CHORUS_RATE     = 58;
const uint8_t   CHORUS_DEPTH    = 59;


const uint8_t   DELAY_FEEDBACK  = 92;
const uint8_t   DELAY_TIME      = 90;
const uint8_t   DELAY_MODE      = 35;


const uint8_t   PROG_SEL_TO_W   = 93;
const uint8_t   W_PROG_TO_FLASH = 105;
const uint8_t   PC_BY_CC_8      = 112;
const uint8_t   PC_BY_CC_9      = 113;
const uint8_t   PC_BY_CC_10     = 114;
const uint8_t   PC_BY_CC_11     = 115;
const uint8_t   PC_BY_CC_12     = 116;
const uint8_t   PC_BY_CC_13     = 117;
const uint8_t   PC_BY_CC_14     = 118;
const uint8_t   PC_BY_CC_15     = 119;


const uint8_t   ALL_SOUND_OFF   = 120;
const uint8_t   RESET_ALL_CTRLS = 121;
const uint8_t   ALL_NOTES_OFF   = 123;
const uint8_t   OMNI_MODE_OFF   = 124;
const uint8_t   OMNI_MODE_ON    = 125;
const uint8_t   MONO_MODE_ON    = 126;
const uint8_t   POLY_MODE_ON    = 127;

const uint8_t   OSC_WAVE_SAW       = 0;
const uint8_t   OSC_WAVE_SINE      = 25;
const uint8_t   OSC_WAVE_TRIANGLE  = 75;
const uint8_t   OSC_WAVE_2_NOISE   = 100;
const uint8_t   OSC_WAVE_1_PULSE   = 127;
const uint8_t   OSC_WAVE_2_SQUARE  = 127;

const uint8_t   OSC_DST_PITCH      = 0;
const uint8_t   OSC_DST_PITCH_2    = 64;
const uint8_t   OSC_DST_SHAPE_1    = 127;

const uint8_t   LFO_WAVE_SINE      = 0;
const uint8_t   LFO_WAVE_SAW_DOWN  = 25;
const uint8_t   LFO_WAVE_TRIANGLE  = 75;
const uint8_t   LFO_WAVE_S_AND_H   = 100;
const uint8_t   LFO_WAVE_SQUARE    = 127;

const uint8_t   VOICE_PARAPHONIC   = 0;
const uint8_t   VOICE_POLYPHONIC   = 25;
const uint8_t   VOICE_MONOPHONIC   = 75;
const uint8_t   VOICE_LEGATO       = 100;
const uint8_t   VOICE_LEGATO_PORTA = 127;
