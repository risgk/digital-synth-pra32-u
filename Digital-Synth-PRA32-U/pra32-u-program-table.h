#pragma once

const uint8_t PROGRAM_NUMBER_DEFAULT = 8;

// Preset                                         #0   #1   #2   #3   #4   #5   #6   #7     #8   #9   #10  #11  #12  #13  #14  #15  
const uint8_t g_preset_table_OSC_1_WAVE     [] = {127, 127, 127, 127, 127, 127, 127, 0  ,   127, 127, 127, 127, 127, 127, 127, 0  };
const uint8_t g_preset_table_OSC_1_SHAPE    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_OSC_1_MORPH    [] = {64 , 64 , 64 , 64 , 64 , 0  , 0  , 0  ,   64 , 64 , 64 , 64 , 64 , 0  , 0  , 0  };
const uint8_t g_preset_table_MIXER_SUB_OSC  [] = {127, 64 , 64 , 64 , 127, 127, 64 , 64 ,   127, 64 , 64 , 64 , 127, 127, 64 , 64 };

const uint8_t g_preset_table_OSC_2_WAVE     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_OSC_2_COARSE   [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_OSC_2_PITCH    [] = {109, 72 , 72 , 72 , 72 , 72 , 72 , 72 ,   109, 72 , 72 , 72 , 72 , 72 , 72 , 72 };
const uint8_t g_preset_table_MIXER_OSC_MIX  [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 0  ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 0  };

const uint8_t g_preset_table_FILTER_CUTOFF  [] = {115, 115, 91 , 103, 55 , 115, 67 , 127,   115, 115, 91 , 103, 55 , 115, 67 , 127};
const uint8_t g_preset_table_FILTER_RESO    [] = {64 , 0  , 32 , 32 , 64 , 64 , 32 , 0  ,   64 , 0  , 32 , 32 , 64 , 64 , 32 , 0  };
const uint8_t g_preset_table_FILTER_EG_AMT  [] = {64 , 64 , 88 , 16 , 112, 64 , 64 , 64 ,   64 , 64 , 88 , 16 , 112, 64 , 64 , 64 };
const uint8_t g_preset_table_FILTER_KEY_TRK [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 0  ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 0  };

const uint8_t g_preset_table_EG_ATTACK      [] = {0  , 64 , 48 , 96 , 0  , 0  , 0  , 0  ,   0  , 64 , 48 , 96 , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_DECAY       [] = {0  , 0  , 80 , 96 , 96 , 0  , 100, 0  ,   0  , 0  , 80 , 96 , 96 , 0  , 100, 0  };
const uint8_t g_preset_table_EG_SUSTAIN     [] = {127, 127, 0  , 0  , 0  , 127, 0  , 127,   127, 127, 0  , 0  , 0  , 127, 0  , 127};
const uint8_t g_preset_table_EG_RELEASE     [] = {0  , 64 , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 64 , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_EG_OSC_AMT     [] = {64 , 64 , 72 , 64 , 64 , 64 , 96 , 64 ,   64 , 64 , 72 , 64 , 64 , 64 , 96 , 64 };
const uint8_t g_preset_table_EG_OSC_DST     [] = {0  , 0  , 64 , 0  , 0  , 0  , 127, 0  ,   0  , 0  , 64 , 0  , 0  , 0  , 127, 0  };
const uint8_t g_preset_table_VOICE_MODE     [] = {127, 0  , 0  , 0  , 75 , 127, 0  , 0  ,   127, 0  , 0  , 0  , 75 , 127, 0  , 0  };
const uint8_t g_preset_table_PORTAMENTO     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_LFO_WAVE       [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_LFO_RATE       [] = {80 , 80 , 80 , 80 , 80 , 80 , 80 , 80 ,   80 , 80 , 80 , 80 , 80 , 80 , 80 , 80 };
const uint8_t g_preset_table_LFO_DEPTH      [] = {0  , 0  , 0  , 0  , 0  , 127, 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 127, 0  , 0  };
const uint8_t g_preset_table_LFO_FADE_TIME  [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_LFO_OSC_AMT    [] = {96 , 96 , 64 , 64 , 64 , 96 , 64 , 64 ,   96 , 96 , 64 , 64 , 64 , 96 , 64 , 64 };
const uint8_t g_preset_table_LFO_OSC_DST    [] = {0  , 0  , 0  , 0  , 0  , 127, 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 127, 0  , 0  };
const uint8_t g_preset_table_LFO_FILTER_AMT [] = {64 , 64 , 88 , 88 , 88 , 64 , 88 , 64 ,   64 , 64 , 88 , 88 , 88 , 64 , 88 , 64 };
const uint8_t g_preset_table_AMP_GAIN       [] = {90 , 90 , 90 , 90 , 90 , 90 , 127, 90 ,   90 , 90 , 90 , 90 , 90 , 90 , 127, 90 };

const uint8_t g_preset_table_AMP_ATTACK     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_AMP_DECAY      [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_AMP_SUSTAIN    [] = {127, 127, 127, 127, 127, 127, 127, 127,   127, 127, 127, 127, 127, 127, 127, 127};
const uint8_t g_preset_table_AMP_RELEASE    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_FILTER_MODE    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_AMP_MOD     [] = {127, 127, 0  , 0  , 127, 127, 127, 0  ,   127, 127, 0  , 0  , 127, 127, 127, 0  };
const uint8_t g_preset_table_REL_EQ_DECAY   [] = {0  , 0  , 0  , 0  , 127, 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 127, 0  , 0  , 0  };
const uint8_t g_preset_table_P_BEND_RANGE   [] = {12 , 12 , 12 , 12 , 12 , 12 , 12 , 12 ,   12 , 12 , 12 , 12 , 12 , 12 , 12 , 12 };

const uint8_t g_preset_table_BTH_FILTER_AMT [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_BTH_AMP_MOD    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_EG_VEL_SENS    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
const uint8_t g_preset_table_AMP_VEL_SENS   [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

const uint8_t g_preset_table_VOICE_ASGN_MODE[] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };




const uint8_t g_preset_table_CHORUS_MIX     [] = {64 , 127, 127, 127, 64 , 64 , 127, 0  ,   64 , 127, 127, 127, 64 , 64 , 127, 0  };
const uint8_t g_preset_table_CHORUS_RATE    [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
const uint8_t g_preset_table_CHORUS_DEPTH   [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };


const uint8_t g_preset_table_DELAY_FEEDBACK [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 0  ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 0  };
const uint8_t g_preset_table_DELAY_TIME     [] = {93 , 93 , 93 , 93 , 93 , 93 , 93 , 93 ,   93 , 93 , 93 , 93 , 93 , 93 , 93 , 93 };
const uint8_t g_preset_table_DELAY_MODE     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

