#pragma once

struct PRA32_U_ControlPanelPage {
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
  { "Page= 1   ", "Voice     ",   "Voice     ", "Mode      ", VOICE_MODE     ,   "Portamento", "          ", PORTAMENTO     ,   "Pitch     ", "Bend Range",  P_BEND_RANGE   , },
  { "Page= 2   ", "Osc 1     ",   "Osc 1     ", "Wave      ", OSC_1_WAVE     ,   "Osc 1     ", "Shape     ", OSC_1_SHAPE    ,   "Osc 1     ", "Morph     ",  OSC_1_MORPH    , },
  { "Page= 3   ", "Osc 2     ",   "Osc 2     ", "Wave      ", OSC_2_WAVE     ,   "Osc 2     ", "Coarse    ", OSC_2_COARSE   ,   "Osc 2     ", "Pitch     ",  OSC_2_PITCH    , },
  { "Page= 4   ", "Mixer     ",   "Mixer     ", "Osc Mix   ", MIXER_OSC_MIX  ,   "Mixer     ", "Noise/Sub ", MIXER_SUB_OSC  ,   "          ", "          ",  0xFF           , },
  { "Page= 5   ", "Filter a  ",   "Filter    ", "Cutoff    ", FILTER_CUTOFF  ,   "Filter    ", "Resonance ", FILTER_RESO    ,   "Filter    ", "EG Amt    ",  FILTER_EG_AMT  , },
  { "Page= 6   ", "Filter b  ",   "Filter    ", "Key Track ", FILTER_KEY_TRK ,   "Filter    ", "Mode      ", FILTER_MODE    ,   "          ", "          ",  0xFF           , },
  { "Page= 7   ", "EG a      ",   "EG        ", "Attack    ", EG_ATTACK      ,   "EG        ", "Decay     ", EG_DECAY       ,   "EG        ", "Sustain   ",  EG_SUSTAIN     , },
  { "Page= 8   ", "EG b      ",   "EG        ", "Release   ", EG_RELEASE     ,   "EG        ", "Amp Mod   ", EG_AMP_MOD     ,   "Release   ", "= Decay   ",  REL_EQ_DECAY   , },
  { "Page= 9   ", "EG c      ",   "EG        ", "Osc Amt   ", EG_OSC_AMT     ,   "EG        ", "Osc Dst   ", EG_OSC_DST     ,   "EG        ", "Velo Sens ",  EG_VEL_SENS    , },
  { "Page=10   ", "Amp a     ",   "Amp       ", "Attack    ", AMP_ATTACK     ,   "Amp       ", "Decay     ", AMP_DECAY      ,   "Amp       ", "Sustain   ",  AMP_SUSTAIN    , },
  { "Page=11   ", "Amp b     ",   "Amp       ", "Release   ", AMP_RELEASE    ,   "Amp       ", "Gain      ", AMP_GAIN       ,   "Amp       ", "Velo Sens ",  AMP_VEL_SENS   , },
  { "Page=12   ", "LFO a     ",   "LFO       ", "Wave      ", LFO_WAVE       ,   "LFO       ", "Rate      ", LFO_RATE       ,   "LFO       ", "Depth     ",  LFO_DEPTH      , },
  { "Page=13   ", "LFO b     ",   "LFO       ", "Fade Time ", LFO_FADE_TIME  ,   "          ", "          ", 0xFF           ,   "          ", "          ",  0xFF           , },
  { "Page=14   ", "LFO c     ",   "LFO       ", "Osc Amt   ", LFO_OSC_AMT    ,   "LFO       ", "Osc Dst   ", LFO_OSC_DST    ,   "LFO       ", "Filter Amt",  LFO_FILTER_AMT , },
  { "Page=15   ", "Breath    ",   "Breath    ", "Filter Amt", BTH_FILTER_AMT ,   "Breath    ", "Amp Mod   ", BTH_AMP_MOD    ,   "          ", "          ",  0xFF           , },
  { "Page=16   ", "Chorus    ",   "Chorus    ", "Mix       ", CHORUS_MIX     ,   "Chorus    ", "Rate      ", CHORUS_RATE    ,   "Chorus    ", "Depth     ",  CHORUS_DEPTH   , },
  { "Page=17   ", "Delay     ",   "Delay     ", "Feedback  ", DELAY_FEEDBACK ,   "Delay     ", "Time      ", DELAY_TIME     ,   "Delay     ", "Mode      ",  DELAY_MODE     , },
};
