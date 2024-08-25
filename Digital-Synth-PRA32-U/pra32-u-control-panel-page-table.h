#pragma once

const uint8_t   PAGE_GROUP_DEFAULT   = 0;
const uint8_t   PAGE_INDEX_DEFAULT_A = 0;
const uint8_t   PAGE_INDEX_DEFAULT_B = 0;
const uint8_t   PAGE_INDEX_DEFAULT_C = 0;
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
  { "Page=A-00 ", "Info      ",   "PRA32-U   ", "with Panel", 0xFF           ,   "          ",PRA32_U_VERSION, 0xFF         ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-01 ", "Voice a   ",   "Voice     ", "Mode      ", VOICE_MODE     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-02 ", "Voice b   ",   "Portamento", "          ", PORTAMENTO     ,   "Pitch     ", "Bend Range", P_BEND_RANGE   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-03 ", "Osc a     ",   "Osc 1     ", "Wave      ", OSC_1_WAVE     ,   "Mixer     ", "Noise/Sub ", MIXER_SUB_OSC  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-04 ", "Osc b     ",   "Osc 1     ", "Shape     ", OSC_1_SHAPE    ,   "Osc 1     ", "Morph     ", OSC_1_MORPH    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-05 ", "Osc c     ",   "Osc 2     ", "Wave      ", OSC_2_WAVE     ,   "Mixer     ", "Osc Mix   ", MIXER_OSC_MIX  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-06 ", "Osc d     ",   "Osc 2     ", "Coarse    ", OSC_2_COARSE   ,   "Osc 2     ", "Pitch     ", OSC_2_PITCH    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-07 ", "Filter a  ",   "Filter    ", "Cutoff    ", FILTER_CUTOFF  ,   "Filter    ", "Resonance ", FILTER_RESO    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-08 ", "Filter b  ",   "Filter    ", "EG Amt    ", FILTER_EG_AMT  ,   "Filter    ", "Key Track ", FILTER_KEY_TRK ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-09 ", "Filter c  ",   "          ", "          ", 0xFF           ,   "Filter    ", "Mode      ", FILTER_MODE    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-10 ", "EG a      ",   "EG        ", "Attack    ", EG_ATTACK      ,   "EG        ", "Decay     ", EG_DECAY       ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-11 ", "EG b      ",   "EG        ", "Sustain   ", EG_SUSTAIN     ,   "EG        ", "Release   ", EG_RELEASE     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-12 ", "EG c      ",   "EG        ", "Amp Mod   ", EG_AMP_MOD     ,   "Release   ", "= Decay   ", REL_EQ_DECAY   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-13 ", "EG d      ",   "EG        ", "Osc Amt   ", EG_OSC_AMT     ,   "EG        ", "Osc Dst   ", EG_OSC_DST     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-14 ", "EG e      ",   "          ", "          ", 0xFF           ,   "EG        ", "Velo Sens ", EG_VEL_SENS    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-15 ", "Amp a     ",   "Amp       ", "Attack    ", AMP_ATTACK     ,   "Amp       ", "Decay     ", AMP_DECAY      ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-16 ", "Amp c     ",   "Amp       ", "Sustain   ", AMP_SUSTAIN    ,   "Amp       ", "Release   ", AMP_RELEASE    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-17 ", "Amp b     ",   "Amp       ", "Gain      ", AMP_GAIN       ,   "Amp       ", "Velo Sens ", AMP_VEL_SENS   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-18 ", "LFO a     ",   "LFO       ", "Wave      ", LFO_WAVE       ,   "LFO       ", "Fade Time ", LFO_FADE_TIME  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-19 ", "LFO b     ",   "LFO       ", "Rate      ", LFO_RATE       ,   "LFO       ", "Depth     ", LFO_DEPTH      ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-20 ", "LFO c     ",   "LFO       ", "Osc Amt   ", LFO_OSC_AMT    ,   "LFO       ", "Osc Dst   ", LFO_OSC_DST    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-21 ", "LFO d     ",   "LFO       ", "Filter Amt", LFO_FILTER_AMT ,   "          ", "          ", 0xFF           ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-22 ", "Breath    ",   "Breath    ", "Filter Amt", BTH_FILTER_AMT ,   "Breath    ", "Amp Mod   ", BTH_AMP_MOD    ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-23 ", "Chorus a  ",   "Chorus    ", "Mix       ", CHORUS_MIX     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-24 ", "Chorus b  ",   "Chorus    ", "Rate      ", CHORUS_RATE    ,   "Chorus    ", "Depth     ", CHORUS_DEPTH   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-25 ", "Delay a   ",   "Delay     ", "Feedback  ", DELAY_FEEDBACK ,   "Delay     ", "Time      ", DELAY_TIME     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=A-26 ", "Delay b   ",   "Delay     ", "Mode      ", DELAY_MODE     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
};

PRA32_U_ControlPanelPage g_control_panel_page_table_b[] = {
  { "Page=B-00 ", "Panel a   ",   "Panel     ", "Play Mode ", PANEL_PLAY_MODE,   "Panel     ", "MIDI Ch   ", PANEL_MIDI_CH  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-01 ", "Panel b   ",   "Panel     ", "Play Pitch", PANEL_PLAY_PIT ,   "Panel     ", "Play Velo ", PANEL_PLAY_VELO,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-02 ", "Panel c   ",   "Panel     ", "Scale     ", PANEL_SCALE    ,   "Panel     ", "Transpose ", PANEL_TRANSPOSE,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-03 ", "Seq a     ",   "Seq       ", "Pattern   ", SEQ_PATTERN    ,   "Seq       ", "Act Steps ", SEQ_ACT_STEPS  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-05 ", "Seq b     ",   "          ", "          ", 0xFF           ,   "Seq       ", "Last Step ", SEQ_LAST_STEP  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-04 ", "Seq c     ",   "Seq       ", "Tempo     ", SEQ_TEMPO      ,   "Seq       ", "Gate Time ", SEQ_GATE_TIME  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-06 ", "Seq d     ",   "Seq       ", "Transpose ", SEQ_TRANSPOSE  ,   "Seq       ", "Clock Src ", SEQ_CLOCK_SRC  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-07 ", "Seq 0     ",   "Seq       ", "Pitch 0   ", SEQ_PITCH_0    ,   "Seq       ", "Velo 0    ", SEQ_VELO_0     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-08 ", "Seq 1     ",   "Seq       ", "Pitch 1   ", SEQ_PITCH_1    ,   "Seq       ", "Velo 1    ", SEQ_VELO_1     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-09 ", "Seq 2     ",   "Seq       ", "Pitch 2   ", SEQ_PITCH_2    ,   "Seq       ", "Velo 2    ", SEQ_VELO_2     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-10 ", "Seq 3     ",   "Seq       ", "Pitch 3   ", SEQ_PITCH_3    ,   "Seq       ", "Velo 3    ", SEQ_VELO_3     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-11 ", "Seq 4     ",   "Seq       ", "Pitch 4   ", SEQ_PITCH_4    ,   "Seq       ", "Velo 4    ", SEQ_VELO_4     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-12 ", "Seq 5     ",   "Seq       ", "Pitch 5   ", SEQ_PITCH_5    ,   "Seq       ", "Velo 5    ", SEQ_VELO_5     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-13 ", "Seq 6     ",   "Seq       ", "Pitch 6   ", SEQ_PITCH_6    ,   "Seq       ", "Velo 6    ", SEQ_VELO_6     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-14 ", "Seq 7     ",   "Seq       ", "Pitch 7   ", SEQ_PITCH_7    ,   "Seq       ", "Velo 7    ", SEQ_VELO_7     ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-15 ", "Control a ",   "Modulation", "          ", MODULATION     ,   "          ", "          ", 0xFF           ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=B-16 ", "Control b ",   "Breath    ", "Controller", BTH_CONTROLLER ,   "Sustain   ", "Pedal     ", SUSTAIN_PEDAL  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
};

PRA32_U_ControlPanelPage g_control_panel_page_table_c[] = {
  { "Page=C-00 ", "Write 8   ",   "Write     ", "Program  8", WR_PROGRAM_8   ,   "Write     ", "Program  9", WR_PROGRAM_9   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=C-01 ", "Write 10  ",   "Write     ", "Program 10", WR_PROGRAM_10  ,   "Write     ", "Program 11", WR_PROGRAM_11  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=C-02 ", "Write 12  ",   "Write     ", "Program 12", WR_PROGRAM_12  ,   "Write     ", "Program 13", WR_PROGRAM_13  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=C-03 ", "Write 14  ",   "Write     ", "Program 14", WR_PROGRAM_14  ,   "Write     ", "Program 15", WR_PROGRAM_15  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=C-04 ", "Write a   ",   "Write     ", "Panel Prms", WR_PANEL_PRMS  ,   "          ", "          ", 0xFF           ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
};

PRA32_U_ControlPanelPage g_control_panel_page_table_d[] = {
  { "Page=D-00 ", "Read 0    ",   "Read      ", "Program  0", RD_PROGRAM_0   ,   "Read      ", "Program  1", RD_PROGRAM_1   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-01 ", "Read 2    ",   "Read      ", "Program  2", RD_PROGRAM_2   ,   "Read      ", "Program  3", RD_PROGRAM_3   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-02 ", "Read 4    ",   "Read      ", "Program  4", RD_PROGRAM_4   ,   "Read      ", "Program  5", RD_PROGRAM_5   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-03 ", "Read 6    ",   "Read      ", "Program  6", RD_PROGRAM_6   ,   "Read      ", "Program  7", RD_PROGRAM_7   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-04 ", "Read 8    ",   "Read      ", "Program  8", RD_PROGRAM_8   ,   "Read      ", "Program  9", RD_PROGRAM_9   ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-05 ", "Read 10   ",   "Read      ", "Program 10", RD_PROGRAM_10  ,   "Read      ", "Program 11", RD_PROGRAM_11  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-06 ", "Read 12   ",   "Read      ", "Program 12", RD_PROGRAM_12  ,   "Read      ", "Program 13", RD_PROGRAM_13  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-07 ", "Read 14   ",   "Read      ", "Program 14", RD_PROGRAM_14  ,   "Read      ", "Program 15", RD_PROGRAM_15  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
  { "Page=D-08 ", "Read a    ",   "Read      ", "Panel Prms", RD_PANEL_PRMS  ,   "Init      ", "Panel Prms", IN_PANEL_PRMS  ,   "Panel     ", "Play Pitch", PANEL_PLAY_PIT , },
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
