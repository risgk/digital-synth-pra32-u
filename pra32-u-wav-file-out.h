#pragma once

#include <stdio.h>
#include "./Digital-Synth-PRA32-U/pra32-u-common.h"

class PRA32_U_WAVFileOut {
  FILE*    m_file;
  uint32_t m_max_size;
  uint32_t m_data_size;
  boolean  m_closed;

public:
  PRA32_U_WAVFileOut()
  : m_file()
  , m_max_size()
  , m_data_size()
  , m_closed()
  {}

  INLINE void open(const char* path, uint16_t sec) {
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

  INLINE void write(int16_t left, int16_t right) {
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

  INLINE void close() {
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
