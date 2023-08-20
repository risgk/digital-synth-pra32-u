require_relative 'pra32-u-constants'

$file = File.open("pra32-u-eg-table.h", "w")

$file.printf("#pragma once\n\n")

$file.printf("int32_t g_eg_attack_release_coef_table[] = {\n  ")
(0..127 + 16).each do |i|
  time = i
  eg_coef = (0.5 ** (1.0 / ((0.2 / 10.0) * (SAMPLING_RATE / 4) * (10.0 ** ((time - 64.0) / 32.0)))) * 0x40000000).round

  $file.printf("%10d,", eg_coef)
  if i == 127 + 16
    $file.printf("\n")
  elsif i % 8 == (8 - 1)
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")

$file.printf("int32_t g_eg_decay_coef_table[] = {\n  ")
(0..127).each do |i|
  time = i
  eg_coef = (0.5 ** (1.0 / ((0.2 / 10.0) * (SAMPLING_RATE / 4) * (10.0 ** ((time - 64.0) / 32.0)))) * 0x40000000).round
  eg_coef = 0x40000000 if time == 127

  $file.printf("%10d,", eg_coef)
  if i == 127
    $file.printf("\n")
  elsif i % 8 == (8 - 1)
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")

$file.close
