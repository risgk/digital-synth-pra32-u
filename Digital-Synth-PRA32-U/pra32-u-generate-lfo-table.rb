require_relative 'pra32-u-constants'

$file = File.open("pra32-u-lfo-table.h", "w")

$file.printf("#pragma once\n\n")

$file.printf("uint16_t g_lfo_rate_table[] = {\n  ")
(0..64).each do |i|
  lfo_rate = (10.0 ** ((i - 32) / 32.0)) * (2.0 * (1 << 16) * 64 / SAMPLING_RATE)

  $file.printf("%4d,", lfo_rate.floor)
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
