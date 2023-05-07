#pragma once

#include <stdio.h>
#include "./DigitalSynthPRA32U/common.h"

template <uint8_t T>
class WAVFileOut {
  static FILE*    m_file;
  static uint32_t m_max_size;
  static uint32_t m_data_size;
  static boolean  m_closed;

public:
  INLINE static void open(const char* path, uint16_t sec) {
    m_file = fopen(path, "wb");
    fwrite("RIFF", 1, 4, m_file);
    fwrite("\x00\x00\x00\x00", 1, 4, m_file);
    fwrite("WAVE", 1, 4, m_file);
    fwrite("fmt ", 1, 4, m_file);
    fwrite("\x10\x00\x00\x00", 1, 4, m_file);
    fwrite("\x01\x00\x02\x00", 1, 4, m_file);
    uint32_t a[1] = {SAMPLING_RATE};
    fwrite(a, 4, 1, m_file);
    a[0] = {SAMPLING_RATE * 4};
    fwrite(a, 4, 1, m_file);
    fwrite("\x04\x00\x10\x00", 1, 4, m_file);
    fwrite("data", 1, 4, m_file);
    fwrite("\x00\x00\x00\x00", 1, 4, m_file);
    m_max_size = (SAMPLING_RATE) * 2 * sec;
    m_data_size = 0;
    m_closed = false;
  }

  INLINE static void write(int16_t left, int16_t right) {
    if (m_data_size < m_max_size) {
      int16_t l[1] = {left};
      int16_t r[1] = {right};
      fwrite(l, 2, 1, m_file);
      fwrite(r, 2, 1, m_file);
      m_data_size += 2;
      m_data_size += 2;
    } else {
      close();
      m_closed = true;
    }
  }

  INLINE static void close() {
    if (!m_closed) {
      fseek(m_file, 0, SEEK_END);
      long file_size = ftell(m_file);
      fseek(m_file, 4, SEEK_SET);
      uint32_t a[1] = {static_cast<uint32_t>(file_size) - 8};
      fwrite(a, 4, 1, m_file);
      fseek(m_file, 40, SEEK_SET);
      uint32_t a2[1] = {static_cast<uint32_t>(file_size) - 36};
      fwrite(a2, 4, 1, m_file);
      fclose(m_file);
      printf("End Of Recording\n");
    }
  }
};

template <uint8_t T> FILE*    WAVFileOut<T>::m_file;
template <uint8_t T> uint32_t WAVFileOut<T>::m_max_size;
template <uint8_t T> uint32_t WAVFileOut<T>::m_data_size;
template <uint8_t T> boolean  WAVFileOut<T>::m_closed;
