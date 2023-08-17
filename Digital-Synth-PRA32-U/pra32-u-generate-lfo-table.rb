require_relative 'pra32-u-constants'

$file = File.open("pra32-u-lfo-table.h", "w")

$file.printf("#pragma once\n\n")

$file.printf("uint32_t g_lfo_rate_table[] = {\n  ")
(0..127).each do |i|
  lfo_rate = (2.0 ** ((i - 64) / 12.0)) *
             (A4_FREQ * (2.0 ** ((-19 - 69) / 12.0))) * (1 << 24) / (SAMPLING_RATE / 4.0)

  $file.printf("%6d,", lfo_rate.floor)
  if i == DATA_BYTE_MAX
    $file.printf("\n")
  elsif i % 16 == (16 - 1)
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")

$file.close
