require_relative 'pra32-u-constants'

$file = File.open("pra32-u-eg-table.h", "w")

$file.printf("#pragma once\n\n")

$file.printf("uint16_t g_eg_attack_release_coef_table[] = {\n  ")
(0..127).each do |i|
  eg_coef = (((189 + (i / 2.0)) / 256.0) ** (1.0 / (((i / 2.0) + 1) * 3)) * 32768.0).round

  $file.printf("%5d,", eg_coef)
  if i == DATA_BYTE_MAX
    $file.printf("\n")
  elsif i % 8 == (8 - 1)
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")

$file.printf("uint16_t g_eg_decay_coef_table[] = {\n  ")
(0..127).each do |i|
  eg_coef = (((189 + (i / 2.0)) / 256.0) ** (1.0 / (((i / 2.0) + 1) * 3)) * 32768.0).round
  eg_coef = 32768 if i == 127

  $file.printf("%5d,", eg_coef)
  if i == DATA_BYTE_MAX
    $file.printf("\n")
  elsif i % 8 == (8 - 1)
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")

$file.close
