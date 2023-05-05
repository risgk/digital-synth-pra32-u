#define ENABLE_16_BIT_OUTPUT

#define MAKE_SAMPLE_WAV_FILE

#include <stdint.h>
#include <stdio.h>

typedef signed char boolean;

#define ENABLE_OSCILLATOR_2

#include "./DigitalSynthPRA32U/common.h"
#include "./DigitalSynthPRA32U/synth.h"
#include "./wav-file-out.h"

const uint16_t RECORDING_SEC = 60;
const uint16_t SERIAL_SPEED_38400 = 38400;

int main(int argc, char *argv[]) {
  // setup
  Synth<0>::initialize();
  FILE* bin_file = ::fopen(argv[1], "rb");
  WAVFileOut<0>::open(argv[2], RECORDING_SEC);

  // loop
  int c;
  while ((c = ::fgetc(bin_file)) != EOF) {
    Synth<0>::receive_midi_byte(c);
    uint16_t r = SAMPLING_RATE / (SERIAL_SPEED_38400 / 10);
    for (uint16_t i = 0; i < r; i++) {
#if defined(ENABLE_16_BIT_OUTPUT)
      int16_t right_level;
      int16_t left_level = Synth<0>::process(right_level);
      WAVFileOut<0>::write(left_level, right_level);
#else
      int8_t right_level;
      int8_t left_level = Synth<0>::process(right_level);
      WAVFileOut<0>::write(left_level, right_level);
#endif
    }
  }

  // teardown
  WAVFileOut<0>::close();
  ::fclose(bin_file);

  return 0;
}
