#pragma once

const uint8_t   PANEL_PITCH     = 128 + 0;
const uint8_t   PANEL_SCALE     = 128 + 1;
const uint8_t   PANEL_TRANSPOSE = 128 + 2;
const uint8_t   PANEL_VELOCITY  = 128 + 3;

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

const uint8_t   PAGE_GROUP_DEFAULT   = 0;
const uint8_t   PAGE_INDEX_DEFAULT_A = 7;
const uint8_t   PAGE_INDEX_DEFAULT_B = 0;
const uint8_t   PAGE_INDEX_DEFAULT_C = 4;
const uint8_t   PAGE_INDEX_DEFAULT_D = 0;

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
};

PRA32_U_ControlPanelPage g_control_panel_page_table_a[] = {
  { "Page=A00  ", "Info      ",   "PRA32-U   ", "with Panel", 0xFF           ,   "          ",PRA32_U_VERSION, 0xFF         ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A01  ", "Voice a   ",   "Voice     ", "Mode      ", VOICE_MODE     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A02  ", "Voice b   ",   "Portamento", "          ", PORTAMENTO     ,   "Pitch     ", "Bend Range", P_BEND_RANGE   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A03  ", "Osc a     ",   "Osc 1     ", "Wave      ", OSC_1_WAVE     ,   "Mixer     ", "Noise/Sub ", MIXER_SUB_OSC  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A04  ", "Osc b     ",   "Osc 1     ", "Shape     ", OSC_1_SHAPE    ,   "Osc 1     ", "Morph     ", OSC_1_MORPH    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A05  ", "Osc c     ",   "Osc 2     ", "Wave      ", OSC_2_WAVE     ,   "Mixer     ", "Osc Mix   ", MIXER_OSC_MIX  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A06  ", "Osc d     ",   "Osc 2     ", "Coarse    ", OSC_2_COARSE   ,   "Osc 2     ", "Pitch     ", OSC_2_PITCH    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A07  ", "Filter a  ",   "Filter    ", "Cutoff    ", FILTER_CUTOFF  ,   "Filter    ", "Resonance ", FILTER_RESO    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A08  ", "Filter b  ",   "Filter    ", "EG Amt    ", FILTER_EG_AMT  ,   "Filter    ", "Key Track ", FILTER_KEY_TRK ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A09  ", "Filter c  ",   "          ", "          ", 0xFF           ,   "Filter    ", "Mode      ", FILTER_MODE    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A10  ", "EG a      ",   "EG        ", "Attack    ", EG_ATTACK      ,   "EG        ", "Decay     ", EG_DECAY       ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A11  ", "EG b      ",   "EG        ", "Sustain   ", EG_SUSTAIN     ,   "EG        ", "Release   ", EG_RELEASE     ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A12  ", "EG c      ",   "EG        ", "Amp Mod   ", EG_AMP_MOD     ,   "Release   ", "= Decay   ", REL_EQ_DECAY   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A13  ", "EG d      ",   "EG        ", "Osc Amt   ", EG_OSC_AMT     ,   "EG        ", "Osc Dst   ", EG_OSC_DST     ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A14  ", "EG e      ",   "          ", "          ", 0xFF           ,   "EG        ", "Velo Sens ", EG_VEL_SENS    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A15  ", "Amp a     ",   "Amp       ", "Attack    ", AMP_ATTACK     ,   "Amp       ", "Decay     ", AMP_DECAY      ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A16  ", "Amp c     ",   "Amp       ", "Sustain   ", AMP_SUSTAIN    ,   "Amp       ", "Release   ", AMP_RELEASE    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A17  ", "Amp b     ",   "Amp       ", "Gain      ", AMP_GAIN       ,   "Amp       ", "Velo Sens ", AMP_VEL_SENS   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A18  ", "LFO a     ",   "LFO       ", "Wave      ", LFO_WAVE       ,   "LFO       ", "Fade Time ", LFO_FADE_TIME  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A19  ", "LFO b     ",   "LFO       ", "Rate      ", LFO_RATE       ,   "LFO       ", "Depth     ", LFO_DEPTH      ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A20  ", "LFO c     ",   "LFO       ", "Osc Amt   ", LFO_OSC_AMT    ,   "LFO       ", "Osc Dst   ", LFO_OSC_DST    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A21  ", "LFO d     ",   "LFO       ", "Filter Amt", LFO_FILTER_AMT ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A22  ", "Breath    ",   "Breath    ", "Filter Amt", BTH_FILTER_AMT ,   "Breath    ", "Amp Mod   ", BTH_AMP_MOD    ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A23  ", "Chorus a  ",   "Chorus    ", "Mix       ", CHORUS_MIX     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A24  ", "Chorus b  ",   "Chorus    ", "Rate      ", CHORUS_RATE    ,   "Chorus    ", "Depth     ", CHORUS_DEPTH   ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A25  ", "Delay a   ",   "Delay     ", "Feedback  ", DELAY_FEEDBACK ,   "Delay     ", "Time      ", DELAY_TIME     ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=A26  ", "Delay b   ",   "Delay     ", "Mode      ", DELAY_MODE     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
};

PRA32_U_ControlPanelPage g_control_panel_page_table_b[] = {
  { "Page=B00  ", "Panel a   ",   "Modulation", "          ", MODULATION     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=B01  ", "Panel b   ",   "Breath    ", "Controller", BTH_CONTROLLER ,   "Sustain   ", "Pedal     ", SUSTAIN_PEDAL  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=B02  ", "Panel c   ",   "Panel     ", "Scale     ", PANEL_SCALE    ,   "Panel     ", "Transpose ", PANEL_TRANSPOSE,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=B03  ", "Panel b   ",   "Panel     ", "Velocity  ", PANEL_VELOCITY ,   "          ", "          ", 0xFF           ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
};

PRA32_U_ControlPanelPage g_control_panel_page_table_c[] = {
  { "Page=C00  ", "Read a    ",   "Read      ", "Program  0", PC_BY_PANEL_0  ,   "Read      ", "Program  1", PC_BY_PANEL_1  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=C01  ", "Read b    ",   "Read      ", "Program  2", PC_BY_PANEL_2  ,   "Read      ", "Program  3", PC_BY_PANEL_3  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=C02  ", "Read c    ",   "Read      ", "Program  4", PC_BY_PANEL_4  ,   "Read      ", "Program  5", PC_BY_PANEL_5  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=C03  ", "Read d    ",   "Read      ", "Program  6", PC_BY_PANEL_6  ,   "Read      ", "Program  7", PC_BY_PANEL_7  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=C04  ", "Read e    ",   "Read      ", "Program  8", PC_BY_PANEL_8  ,   "Read      ", "Program  9", PC_BY_PANEL_9  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=C05  ", "Read f    ",   "Read      ", "Program 10", PC_BY_PANEL_10 ,   "Read      ", "Program 11", PC_BY_PANEL_11 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=C06  ", "Read g    ",   "Read      ", "Program 12", PC_BY_PANEL_12 ,   "Read      ", "Program 13", PC_BY_PANEL_13 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=C07  ", "Read h    ",   "Read      ", "Program 14", PC_BY_PANEL_14 ,   "Read      ", "Program 15", PC_BY_PANEL_15 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
};

PRA32_U_ControlPanelPage g_control_panel_page_table_d[] = {
  { "Page=D00  ", "Write a   ",   "Write     ", "Program  8", WR_BY_PANEL_8  ,   "Write     ", "Program  9", WR_BY_PANEL_9  ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=D01  ", "Write b   ",   "Write     ", "Program 10", WR_BY_PANEL_10 ,   "Write     ", "Program 11", WR_BY_PANEL_11 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=D02  ", "Write c   ",   "Write     ", "Program 12", WR_BY_PANEL_12 ,   "Write     ", "Program 13", WR_BY_PANEL_13 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
  { "Page=D03  ", "Write d   ",   "Write     ", "Program 14", WR_BY_PANEL_14 ,   "Write     ", "Program 15", WR_BY_PANEL_15 ,   "Panel     ", "Pitch     ", PANEL_PITCH    , },
};

PRA32_U_ControlPanelPage* g_control_panel_page_table[] = {
    g_control_panel_page_table_a,
    g_control_panel_page_table_b,
    g_control_panel_page_table_c,
    g_control_panel_page_table_d,
};

uint32_t g_number_of_pages[] = {
    (sizeof(g_control_panel_page_table_a) / sizeof(g_control_panel_page_table_a[0])),
    (sizeof(g_control_panel_page_table_b) / sizeof(g_control_panel_page_table_b[0])),
    (sizeof(g_control_panel_page_table_c) / sizeof(g_control_panel_page_table_c[0])),
    (sizeof(g_control_panel_page_table_d) / sizeof(g_control_panel_page_table_d[0])),
};

#define NUMBER_OF_PAGE_GROUPS (sizeof(g_control_panel_page_table) / sizeof(g_control_panel_page_table[0]))
