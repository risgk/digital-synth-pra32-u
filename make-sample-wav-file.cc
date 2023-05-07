#define MIDI_CH (0)


#include <stdint.h>
#include <stdio.h>

typedef signed char boolean;

#define __not_in_flash_func(func) (func)

#include "./DigitalSynthPRA32U/common.h"
#include "./DigitalSynthPRA32U/synth.h"
#include "./midi-in.h"
#include "./wav-file-out.h"

const uint16_t RECORDING_SEC = 60;
const uint16_t SERIAL_SPEED_38400 = 38400;

int main(int argc, char *argv[]) {
  // setup
  Synth<0>::initialize();
  MIDIIn<0>::open();
  FILE* bin_file = ::fopen(argv[1], "rb");
  WAVFileOut<0>::open(argv[2], RECORDING_SEC);

  // loop
  int c;
  while ((c = ::fgetc(bin_file)) != EOF) {
    MIDIIn<0>::receive_midi_byte(c);
    uint16_t r = SAMPLING_RATE / (SERIAL_SPEED_38400 / 10);
    for (uint16_t i = 0; i < r; i++) {
      int16_t right_level;
      int16_t left_level = Synth<0>::process(right_level);
      WAVFileOut<0>::write(left_level, right_level);
    }
  }

  // teardown
  WAVFileOut<0>::close();
  ::fclose(bin_file);

  return 0;
}
