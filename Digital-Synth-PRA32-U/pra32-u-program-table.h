#pragma once

const uint8_t PROGRAM_NUMBER_DEFAULT = 0;

// PRESET                                   #0   #1   #2   #3   #4   #5   #6   #7     #8   #9   #10  #11  #12  #13  #14  #15  
uint8_t g_preset_table_OSC_1_WAVE     [] = {0  , 0  , 96 , 0  , 0  , 96 , 0  , 0  ,   0  , 0  , 96 , 0  , 0  , 96 , 0  , 0  };
uint8_t g_preset_table_OSC_1_SHAPE    [] = {0  , 0  , 127, 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 127, 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_OSC_1_MORPH    [] = {0  , 0  , 127, 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 127, 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_MIXER_SUB_OSC  [] = {127, 64 , 64 , 64 , 127, 64 , 64 , 64 ,   127, 64 , 64 , 64 , 127, 64 , 64 , 64 };

uint8_t g_preset_table_OSC_2_WAVE     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_OSC_2_COARSE   [] = {71 , 76 , 64 , 64 , 64 , 64 , 64 , 64 ,   71 , 76 , 64 , 64 , 64 , 64 , 64 , 64 };
uint8_t g_preset_table_OSC_2_FINE     [] = {72 , 72 , 72 , 72 , 72 , 72 , 72 , 64 ,   72 , 72 , 72 , 72 , 72 , 72 , 72 , 64 };
uint8_t g_preset_table_MIXER_OSC_MIX  [] = {64 , 16 , 64 , 16 , 64 , 64 , 16 , 0  ,   64 , 16 , 64 , 16 , 64 , 64 , 16 , 0  };

uint8_t g_preset_table_FILTER_CUTOFF  [] = {115, 115, 67 , 121, 61 , 115, 127, 127,   115, 115, 67 , 121, 61 , 115, 127, 127};
uint8_t g_preset_table_FILTER_RESO    [] = {64 , 64 , 32 , 32 , 32 , 64 , 0  , 0  ,   64 , 64 , 32 , 32 , 32 , 64 , 0  , 0  };
uint8_t g_preset_table_FILTER_EG_AMT  [] = {64 , 64 , 124, 4  , 124, 64 , 64 , 64 ,   64 , 64 , 124, 4  , 124, 64 , 64 , 64 };
uint8_t g_preset_table_FILTER_KEY_TRK [] = {127, 127, 127, 127, 127, 127, 127, 127,   127, 127, 127, 127, 127, 127, 127, 127};

uint8_t g_preset_table_EG_ATTACK      [] = {0  , 0  , 64 , 96 , 0  , 0  , 0  , 0  ,   0  , 0  , 64 , 96 , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_EG_DECAY       [] = {127, 127, 96 , 96 , 96 , 127, 127, 127,   127, 127, 96 , 96 , 96 , 127, 127, 127};
uint8_t g_preset_table_EG_SUSTAIN     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_EG_RELEASE     [] = {0  , 0  , 0  , 0  , 96 , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 96 , 0  , 0  , 0  };

uint8_t g_preset_table_EG_OSC_AMT     [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
uint8_t g_preset_table_EG_OSC_DST     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_VOICE_MODE     [] = {127, 0  , 0  , 0  , 64 , 127, 0  , 127,   127, 0  , 0  , 0  , 64 , 127, 0  , 127};
uint8_t g_preset_table_PORTAMENTO     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };

uint8_t g_preset_table_LFO_WAVE       [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_LFO_RATE       [] = {96 , 96 , 48 , 96 , 96 , 96 , 96 , 96 ,   96 , 96 , 48 , 96 , 96 , 96 , 96 , 96 };
uint8_t g_preset_table_LFO_DEPTH      [] = {8  , 64 , 64 , 64 , 8  , 64 , 64 , 0  ,   8  , 64 , 64 , 64 , 8  , 64 , 64 , 0  };
uint8_t g_preset_table_LFO_FADE_TIME  [] = {0  , 64 , 0  , 64 , 0  , 0  , 64 , 0  ,   0  , 64 , 0  , 64 , 0  , 0  , 64 , 0  };

uint8_t g_preset_table_LFO_OSC_AMT    [] = {80 , 80 , 127, 80 , 80 , 127, 80 , 64 ,   80 , 80 , 127, 80 , 80 , 127, 80 , 64 };
uint8_t g_preset_table_LFO_OSC_DST    [] = {0  , 0  , 127, 0  , 0  , 127, 0  , 0  ,   0  , 0  , 127, 0  , 0  , 127, 0  , 0  };
uint8_t g_preset_table_LFO_FILTER_AMT [] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };
uint8_t g_preset_table_AMP_LEVEL      [] = {127, 127, 127, 127, 127, 127, 127, 127,   127, 127, 127, 127, 127, 127, 127, 127};

uint8_t g_preset_table_AMP_ATTACK     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_AMP_DECAY      [] = {127, 127, 127, 127, 96 , 127, 127, 127,   127, 127, 127, 127, 96 , 127, 127, 127};
uint8_t g_preset_table_AMP_SUSTAIN    [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_AMP_RELEASE    [] = {0  , 0  , 0  , 0  , 96 , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 96 , 0  , 0  , 0  };

uint8_t g_preset_table_CHORUS_MODE    [] = {127, 127, 127, 127, 127, 127, 127, 0  ,   127, 127, 127, 127, 127, 127, 127, 0  };
uint8_t g_preset_table_CHORUS_RATE    [] = {32 , 32 , 32 , 32 , 32 , 32 , 32 , 32 ,   32 , 32 , 32 , 32 , 32 , 32 , 32 , 32 };
uint8_t g_preset_table_CHORUS_DEPTH   [] = {32 , 32 , 32 , 32 , 32 , 32 , 32 , 32 ,   32 , 32 , 32 , 32 , 32 , 32 , 32 , 32 };
uint8_t g_preset_table_CHORUS_DLY_TIME[] = {64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 ,   64 , 64 , 64 , 64 , 64 , 64 , 64 , 64 };

uint8_t g_preset_table_P_BEND_RANGE   [] = {2  , 2  , 2  , 2  , 2  , 2  , 2  , 2  ,   2  , 2  , 2  , 2  , 2  , 2  , 2  , 2  };
uint8_t g_preset_table_CHORUS_BYPASS  [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_EG_AMP_MOD     [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
uint8_t g_preset_table_REL_EQ_DECAY   [] = {0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  ,   0  , 0  , 0  , 0  , 0  , 0  , 0  , 0  };
