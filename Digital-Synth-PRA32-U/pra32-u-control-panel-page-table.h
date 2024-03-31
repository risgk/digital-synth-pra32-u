#pragma once

struct {
  char    page_name_line_0            [10 + 1];
  char    page_name_line_1            [10 + 1];
  char    control_target_a_name_line_0[10 + 1];
  char    control_target_a_name_line_1[10 + 1];
  uint8_t control_target_a;
  char    control_target_b_name_line_0[10 + 1];
  char    control_target_b_name_line_1[10 + 1];
  uint8_t control_target_b;
  char    control_target_c_name_line_0[10 + 1];
  char    control_target_c_name_line_1[10 + 1];
  uint8_t control_target_c;
} g_control_panel_page_table[] = {
  { "Voice     ", "          ",   "Voice     ", "Mode      ", VOICE_MODE     ,   "Portamento", "          ", PORTAMENTO     ,   "Pitch     ", "Bend Range",  P_BEND_RANGE   , },
  { "Osc 1     ", "          ",   "Osc 1     ", "Wave      ", OSC_1_WAVE     ,   "Osc 1     ", "Shape     ", OSC_1_SHAPE    ,   "Osc 1     ", "Morph     ",  OSC_1_MORPH    , },
  { "Osc 2     ", "          ",   "Osc 2     ", "Wave      ", OSC_2_WAVE     ,   "Osc 2     ", "Coarse    ", OSC_2_COARSE   ,   "Osc 2     ", "Pitch     ",  OSC_2_PITCH    , },
  { "Mixer     ", "          ",   "Mixer     ", "Osc Mix   ", MIXER_OSC_MIX  ,   "Mixer     ", "Noise/Sub ", MIXER_SUB_OSC  ,   "          ", "          ",  0xFF           , },
  { "Filter a  ", "          ",   "Filter    ", "Cutoff    ", FILTER_CUTOFF  ,   "Filter    ", "Resonance ", FILTER_RESO    ,   "Filter    ", "EG Amt    ",  FILTER_EG_AMT  , },
  { "Filter b  ", "          ",   "Filter    ", "Key Track ", FILTER_KEY_TRK ,   "Filter    ", "Mode      ", FILTER_MODE    ,   "          ", "          ",  0xFF           , },
  { "EG a      ", "          ",   "EG        ", "Attack    ", EG_ATTACK      ,   "EG        ", "Decay     ", EG_DECAY       ,   "EG        ", "Sustain   ",  EG_SUSTAIN     , },
  { "EG b      ", "          ",   "EG        ", "Release   ", EG_RELEASE     ,   "EG        ", "Amp Mod   ", EG_AMP_MOD     ,   "Release   ", "= Decay   ",  REL_EQ_DECAY   , },
  { "EG c      ", "          ",   "EG        ", "Osc Amt   ", EG_OSC_AMT     ,   "EG        ", "Osc Dst   ", EG_OSC_DST     ,   "EG        ", "Velo Sens ",  EG_VEL_SENS    , },
  { "Amp a     ", "          ",   "Amp       ", "Attack    ", AMP_ATTACK     ,   "Amp       ", "Decay     ", AMP_DECAY      ,   "Amp       ", "Sustain   ",  AMP_SUSTAIN    , },
  { "Amp b     ", "          ",   "Amp       ", "Release   ", AMP_RELEASE    ,   "Amp       ", "Gain      ", AMP_GAIN       ,   "Amp       ", "Velo Sens ",  AMP_VEL_SENS   , },
  { "LFO a     ", "          ",   "LFO       ", "Wave      ", LFO_WAVE       ,   "LFO       ", "Rate      ", LFO_RATE       ,   "LFO       ", "Depth     ",  LFO_DEPTH      , },
  { "LFO b     ", "          ",   "LFO       ", "Fade Time ", LFO_FADE_TIME  ,   "          ", "          ", 0xFF           ,   "          ", "          ",  0xFF           , },
  { "LFO c     ", "          ",   "LFO       ", "Osc Amt   ", LFO_OSC_AMT    ,   "LFO       ", "Osc Dst   ", LFO_OSC_DST    ,   "LFO       ", "Filter Amt",  LFO_FILTER_AMT , },
  { "Breath    ", "          ",   "Breath    ", "Filter Amt", BTH_FILTER_AMT ,   "Breath    ", "Amp Mod   ", BTH_AMP_MOD    ,   "          ", "          ",  0xFF           , },
  { "Chorus    ", "          ",   "Chorus    ", "Mix       ", CHORUS_MIX     ,   "Chorus    ", "Rate      ", CHORUS_RATE    ,   "Chorus    ", "Depth     ",  CHORUS_DEPTH   , },
  { "Delay     ", "          ",   "Delay     ", "Feedback  ", DELAY_FEEDBACK ,   "Delay     ", "Time      ", DELAY_TIME     ,   "Delay     ", "Mode      ",  DELAY_MODE     , },
};
