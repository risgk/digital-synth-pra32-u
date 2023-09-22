

A4_FREQ = 440.0

SAMPLING_RATE       = 48000
FREQUENCY_MAX       = 23000
BIT_DEPTH           = 8
NOTE_NUMBER_MIN     = 0
NOTE_NUMBER_MAX     = 127
NOTE_NUMBER_INVALID = 255

PROGRAM_NUMBER_MAX           = 15

OSC_CONTROL_INTERVAL_BITS    = 1
OSC_CONTROL_INTERVAL         = 0x01 << OSC_CONTROL_INTERVAL_BITS
OSC_PHASE_RESOLUTION_BITS    = 24
OSC_TUNE_TABLE_STEPS_BITS    = 8
OSC_TUNE_DENOMINATOR_BITS    = 15
OSC_WAVE_TABLE_AMP_BITS      = 13
OSC_WAVE_TABLE_AMP           = 1 << OSC_WAVE_TABLE_AMP_BITS
OSC_WAVE_TABLE_SAMPLES_BITS  = 9
OSC_DETUNE_MUL_NUM_BITS      = 4
OSC_DETUNE_FREQ_MAX          = (20 * 2) * (1 << 24) / SAMPLING_RATE
FILTER_CONTROL_INTERVAL_BITS = 3
FILTER_CONTROL_INTERVAL      = 0x01 << FILTER_CONTROL_INTERVAL_BITS
FILTER_TABLE_FRACTION_BITS   = 30
EG_CONTROL_INTERVAL          = 0x10
EG_LEVEL_MAX                 = 0x40000000
EG_LEVEL_MAX_2               = 0x7FFFFFFF


DATA_BYTE_MAX         = 0x7F
STATUS_BYTE_INVALID   = 0x7F
DATA_BYTE_INVALID     = 0x80
STATUS_BYTE_MIN       = 0x80
NOTE_OFF              = 0x80
NOTE_ON               = 0x90
CONTROL_CHANGE        = 0xB0
PROGRAM_CHANGE        = 0xC0
PITCH_BEND            = 0xE0
SYSTEM_MESSAGE_MIN    = 0xF0
SYSTEM_EXCLUSIVE      = 0xF0
TIME_CODE             = 0xF1
SONG_POSITION         = 0xF2
SONG_SELECT           = 0xF3
TUNE_REQUEST          = 0xF6
EOX                   = 0xF7
REAL_TIME_MESSAGE_MIN = 0xF8
ACTIVE_SENSING        = 0xFE


MODULATION      = 1
SUSTAIN_PEDAL   = 64


OSC_1_WAVE      = 102
OSC_1_SHAPE     = 19
OSC_1_MORPH     = 20
MIXER_SUB_OSC   = 23

OSC_2_WAVE      = 104
OSC_2_COARSE    = 85
OSC_2_PITCH     = 76
MIXER_OSC_MIX   = 21

FILTER_CUTOFF   = 74
FILTER_RESO     = 71
FILTER_EG_AMT   = 24
FILTER_KEY_TRK  = 26

EG_ATTACK       = 73
EG_DECAY        = 75
EG_SUSTAIN      = 30
EG_RELEASE      = 72

EG_OSC_AMT      = 91
EG_OSC_DST      = 89
VOICE_MODE      = 27
PORTAMENTO      = 5

LFO_WAVE        = 18
LFO_RATE        = 3
LFO_DEPTH       = 17
LFO_FADE_TIME   = 56

LFO_OSC_AMT     = 13
LFO_OSC_DST     = 103
LFO_FILTER_AMT  = 25
AMP_GAIN        = 15

AMP_ATTACK      = 52
AMP_DECAY       = 53
AMP_SUSTAIN     = 54
AMP_RELEASE     = 55

FILTER_MODE     = 86
EG_AMP_MOD      = 87
REL_EQ_DECAY    = 29
P_BEND_RANGE    = 57

CHORUS_MIX      = 93
CHORUS_RATE     = 58
CHORUS_DEPTH    = 59


PC_BY_CC_8      = 112
PC_BY_CC_9      = 113
PC_BY_CC_10     = 114
PC_BY_CC_11     = 115
PC_BY_CC_12     = 116
PC_BY_CC_13     = 117
PC_BY_CC_14     = 118
PC_BY_CC_15     = 119


ALL_SOUND_OFF   = 120
RESET_ALL_CTRLS = 121
ALL_NOTES_OFF   = 123
OMNI_MODE_OFF   = 124
OMNI_MODE_ON    = 125
MONO_MODE_ON    = 126
POLY_MODE_ON    = 127

OSC_WAVE_SAW       = 0
OSC_WAVE_SINE      = 25
OSC_WAVE_TRIANGLE  = 75
OSC_WAVE_2_NOISE   = 100
OSC_WAVE_1_PULSE   = 127
OSC_WAVE_2_SQUARE  = 127

OSC_DST_PITCH      = 0
OSC_DST_PITCH_2    = 64
OSC_DST_SHAPE_1    = 127

LFO_WAVE_SINE      = 0
LFO_WAVE_SAW_DOWN  = 25
LFO_WAVE_TRIANGLE  = 75
LFO_WAVE_S_AND_H   = 100
LFO_WAVE_SQUARE    = 127

VOICE_PARAPHONIC   = 0
VOICE_POLYPHONIC   = 25
VOICE_MONOPHONIC   = 75
VOICE_LEGATO       = 100
VOICE_LEGATO_PORTA = 127
