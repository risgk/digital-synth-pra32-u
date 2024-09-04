#define PRA32_U_MIDI_CH (0)  // 0-based


#include <stdint.h>
#include <stdio.h>

typedef signed char boolean;

#define __not_in_flash_func(func) (func)

uint8_t g_midi_ch = PRA32_U_MIDI_CH;

#include "./Digital-Synth-PRA32-U/pra32-u-common.h"
#include "./Digital-Synth-PRA32-U/pra32-u-synth.h"
#include "./pra32-u-midi-in.h"
#include "./pra32-u-wav-file-out.h"

PRA32_U_Synth      g_synth;
PRA32_U_MIDIIn     g_midi_in;
PRA32_U_WAVFileOut g_wav_file_out;

const uint16_t RECORDING_SEC = 60;
const uint16_t SERIAL_SPEED_38400 = 38400;

int main(int argc, char *argv[]) {
  // setup
  g_midi_in.open(g_synth);
  FILE* bin_file = ::fopen(argv[1], "rb");
  g_wav_file_out.open(argv[2], RECORDING_SEC);
  g_synth.initialize();

  // loop
  int c;
  while ((c = ::fgetc(bin_file)) != EOF) {
    g_midi_in.receive_midi_byte(c);
    uint16_t r = SAMPLING_RATE / (SERIAL_SPEED_38400 / 10);
    for (uint16_t i = 0; i < r; i++) {
      int16_t right_level;
      int16_t left_level = g_synth.process(right_level);
      g_wav_file_out.write(left_level, right_level);
    }
  }

  // teardown
  g_wav_file_out.close();
  ::fclose(bin_file);

  return 0;
}
