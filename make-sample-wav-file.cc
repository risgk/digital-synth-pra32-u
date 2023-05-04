//#define ENABLE_16_BIT_OUTPUT

#define MAKE_SAMPLE_WAV_FILE

#define PROGMEM

#include <stdint.h>
#include <stdio.h>

typedef signed char boolean;
typedef int32_t __int24;
typedef uint32_t __uint24;

inline uint8_t pgm_read_byte(const void* addr) {
  const uint8_t* p = static_cast<const uint8_t*>(addr);
  return p[0];
}

inline uint16_t pgm_read_word(const void* addr) {
  // for little endian cpu
  const uint8_t* p = static_cast<const uint8_t*>(addr);
  return p[0] | (p[1] << 8);
}

inline uint32_t pgm_read_dword(const void* addr) {
  // for little endian cpu
  const uint8_t* p = static_cast<const uint8_t*>(addr);
  return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

#define ENABLE_OSCILLATOR_2

#include "./DigitalSynthVRA8U/common.h"
#include "./DigitalSynthVRA8U/synth.h"
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
