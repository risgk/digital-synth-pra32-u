#pragma once

const uint8_t   PANEL_PITCH     = 128 + 0;
const uint8_t   PANEL_SCALE     = 128 + 1;
const uint8_t   PANEL_TRANSPOSE = 128 + 2;

const uint8_t   PC_BY_PANEL_0   = 128 + 64;
const uint8_t   PC_BY_PANEL_1   = 128 + 65;
const uint8_t   PC_BY_PANEL_2   = 128 + 66;
const uint8_t   PC_BY_PANEL_3   = 128 + 67;
const uint8_t   PC_BY_PANEL_4   = 128 + 68;
const uint8_t   PC_BY_PANEL_5   = 128 + 69;
const uint8_t   PC_BY_PANEL_6   = 128 + 70;
const uint8_t   PC_BY_PANEL_7   = 128 + 71;

const uint8_t   PC_BY_PANEL_8   = 128 + 72;
const uint8_t   PC_BY_PANEL_9   = 128 + 73;
const uint8_t   PC_BY_PANEL_10  = 128 + 74;
const uint8_t   PC_BY_PANEL_11  = 128 + 75;
const uint8_t   PC_BY_PANEL_12  = 128 + 76;
const uint8_t   PC_BY_PANEL_13  = 128 + 77;
const uint8_t   PC_BY_PANEL_14  = 128 + 78;
const uint8_t   PC_BY_PANEL_15  = 128 + 79;

const uint8_t   WR_BY_PANEL_0   = 128 + 80;  // Invalid
const uint8_t   WR_BY_PANEL_1   = 128 + 81;  // Invalid
const uint8_t   WR_BY_PANEL_2   = 128 + 82;  // Invalid
const uint8_t   WR_BY_PANEL_3   = 128 + 83;  // Invalid
const uint8_t   WR_BY_PANEL_4   = 128 + 84;  // Invalid
const uint8_t   WR_BY_PANEL_5   = 128 + 85;  // Invalid
const uint8_t   WR_BY_PANEL_6   = 128 + 86;  // Invalid
const uint8_t   WR_BY_PANEL_7   = 128 + 87;  // Invalid

const uint8_t   WR_BY_PANEL_8   = 128 + 88;
const uint8_t   WR_BY_PANEL_9   = 128 + 89;
const uint8_t   WR_BY_PANEL_10  = 128 + 90;
const uint8_t   WR_BY_PANEL_11  = 128 + 91;
const uint8_t   WR_BY_PANEL_12  = 128 + 92;
const uint8_t   WR_BY_PANEL_13  = 128 + 93;
const uint8_t   WR_BY_PANEL_14  = 128 + 94;
const uint8_t   WR_BY_PANEL_15  = 128 + 95;

const uint8_t   PAGE_INDEX_DEFAULT = 6;

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
  { "Page= 0   ", "Voice a   ",   "Voice     ", "Mode      ", VOICE_MODE     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 1   ", "Voice b   ",   "Portamento", "          ", PORTAMENTO     ,   "Pitch     ", "Bend Range", P_BEND_RANGE   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 2   ", "Osc a     ",   "Osc 1     ", "Wave      ", OSC_1_WAVE     ,   "Mixer     ", "Noise/Sub ", MIXER_SUB_OSC  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 3   ", "Osc b     ",   "Osc 1     ", "Shape     ", OSC_1_SHAPE    ,   "Osc 1     ", "Morph     ", OSC_1_MORPH    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 4   ", "Osc c     ",   "Osc 2     ", "Wave      ", OSC_2_WAVE     ,   "Mixer     ", "Osc Mix   ", MIXER_OSC_MIX  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 5   ", "Osc d     ",   "Osc 2     ", "Coarse    ", OSC_2_COARSE   ,   "Osc 2     ", "Pitch     ", OSC_2_PITCH    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 6   ", "Filter a  ",   "Filter    ", "Cutoff    ", FILTER_CUTOFF  ,   "Filter    ", "Resonance ", FILTER_RESO    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 7   ", "Filter b  ",   "Filter    ", "EG Amt    ", FILTER_EG_AMT  ,   "Filter    ", "Key Track ", FILTER_KEY_TRK ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 8   ", "Filter c  ",   "Filter    ", "Mode      ", FILTER_MODE    ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page= 9   ", "EG a      ",   "EG        ", "Attack    ", EG_ATTACK      ,   "EG        ", "Decay     ", EG_DECAY       ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=10   ", "EG b      ",   "EG        ", "Sustain   ", EG_SUSTAIN     ,   "EG        ", "Release   ", EG_RELEASE     ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=11   ", "EG c      ",   "EG        ", "Amp Mod   ", EG_AMP_MOD     ,   "Release   ", "= Decay   ", REL_EQ_DECAY   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=12   ", "EG d      ",   "EG        ", "Osc Amt   ", EG_OSC_AMT     ,   "EG        ", "Osc Dst   ", EG_OSC_DST     ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=13   ", "EG e      ",   "          ", "          ", 0xFF           ,   "EG        ", "Velo Sens ", EG_VEL_SENS    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=14   ", "Amp a     ",   "Amp       ", "Attack    ", AMP_ATTACK     ,   "Amp       ", "Decay     ", AMP_DECAY      ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=15   ", "Amp c     ",   "Amp       ", "Sustain   ", AMP_SUSTAIN    ,   "Amp       ", "Release   ", AMP_RELEASE    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=16   ", "Amp b     ",   "Amp       ", "Gain      ", AMP_GAIN       ,   "Amp       ", "Velo Sens ", AMP_VEL_SENS   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=17   ", "LFO a     ",   "LFO       ", "Wave      ", LFO_WAVE       ,   "LFO       ", "Fade Time ", LFO_FADE_TIME  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=18   ", "LFO b     ",   "LFO       ", "Rate      ", LFO_RATE       ,   "LFO       ", "Depth     ", LFO_DEPTH      ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=19   ", "LFO c     ",   "LFO       ", "Osc Amt   ", LFO_OSC_AMT    ,   "LFO       ", "Osc Dst   ", LFO_OSC_DST    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=20   ", "LFO d     ",   "LFO       ", "Filter Amt", LFO_FILTER_AMT ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=21   ", "Breath    ",   "Breath    ", "Filter Amt", BTH_FILTER_AMT ,   "Breath    ", "Amp Mod   ", BTH_AMP_MOD    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=22   ", "Chorus a  ",   "Chorus    ", "Mix       ", CHORUS_MIX     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=23   ", "Chorus b  ",   "Chorus    ", "Rate      ", CHORUS_RATE    ,   "Chorus    ", "Depth     ", CHORUS_DEPTH   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=24   ", "Delay a   ",   "Delay     ", "Feedback  ", DELAY_FEEDBACK ,   "Delay     ", "Time      ", DELAY_TIME     ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=25   ", "Delay b   ",   "Delay     ", "Mode      ", DELAY_MODE     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=26   ", "Write a   ",   "Write     ", "Program  8", WR_BY_PANEL_8  ,   "Write     ", "Program  9", WR_BY_PANEL_9  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=27   ", "Write b   ",   "Write     ", "Program 10", WR_BY_PANEL_10 ,   "Write     ", "Program 11", WR_BY_PANEL_11 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=28   ", "Write c   ",   "Write     ", "Program 12", WR_BY_PANEL_12 ,   "Write     ", "Program 13", WR_BY_PANEL_13 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=29   ", "Write d   ",   "Write     ", "Program 14", WR_BY_PANEL_14 ,   "Write     ", "Program 15", WR_BY_PANEL_15 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=30   ", "Read a    ",   "Read      ", "Program  0", PC_BY_PANEL_0  ,   "Read      ", "Program  1", PC_BY_PANEL_1  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=31   ", "Read b    ",   "Read      ", "Program  2", PC_BY_PANEL_2  ,   "Read      ", "Program  3", PC_BY_PANEL_3  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=32   ", "Read c    ",   "Read      ", "Program  4", PC_BY_PANEL_4  ,   "Read      ", "Program  5", PC_BY_PANEL_5  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=33   ", "Read d    ",   "Read      ", "Program  6", PC_BY_PANEL_6  ,   "Read      ", "Program  7", PC_BY_PANEL_7  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=34   ", "Read e    ",   "Read      ", "Program  8", PC_BY_PANEL_8  ,   "Read      ", "Program  9", PC_BY_PANEL_9  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=35   ", "Read f    ",   "Read      ", "Program 10", PC_BY_PANEL_10 ,   "Read      ", "Program 11", PC_BY_PANEL_11 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=36   ", "Read g    ",   "Read      ", "Program 12", PC_BY_PANEL_12 ,   "Read      ", "Program 13", PC_BY_PANEL_13 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=37   ", "Read h    ",   "Read      ", "Program 14", PC_BY_PANEL_14 ,   "Read      ", "Program 15", PC_BY_PANEL_15 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=38   ", "Panel a   ",   "Modulation", "          ", MODULATION     ,   "Breath    ", "Controller", BTH_CONTROLLER ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=39   ", "Panel b   ",   "          ", "          ", 0xFF           ,   "Sustain   ", "Pedal     ", SUSTAIN_PEDAL  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=40   ", "Panel c   ",   "Panel     ", "Scale     ", PANEL_SCALE    ,   "Panel     ", "Transpose ", PANEL_TRANSPOSE,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
};
