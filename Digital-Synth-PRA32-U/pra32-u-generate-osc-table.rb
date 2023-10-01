require_relative 'pra32-u-constants'

$file = File.open("pra32-u-osc-table.h", "w")

$file.printf("#pragma once\n\n")

def freq_from_note_number(note_number, pr = false)
  cent = (note_number * 100.0) - 6900.0
  hz = A4_FREQ * (2.0 ** (cent / 1200.0))
  freq = (hz * (1 << OSC_PHASE_RESOLUTION_BITS) / SAMPLING_RATE).floor.to_i
  freq = freq + 1 if freq.even?
  if pr
    printf("[0]%3d, %+f, %d\n",note_number, 1.0 - freq.to_f * SAMPLING_RATE / (hz * (1 << OSC_PHASE_RESOLUTION_BITS)), freq)
  end
  return freq
end

$file.printf("uint32_t g_osc_freq_table[] = {\n  ")
(NOTE_NUMBER_MIN..NOTE_NUMBER_MAX).each do |note_number|
  freq = freq_from_note_number(note_number, true)

  $file.printf("0x%08X,", freq)
  if note_number == DATA_BYTE_MAX
    $file.printf("\n")
  elsif note_number % 12 == (12 - 1)
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")

max_tune_rate = -Float::INFINITY
$file.printf("int16_t g_osc_tune_table[] = {\n  ")
(0..(1 << OSC_TUNE_TABLE_STEPS_BITS) - 1).each do |i|
  tune_rate = ((2.0 ** ((i - (1 << (OSC_TUNE_TABLE_STEPS_BITS - 1))) / (12.0 * (1 << OSC_TUNE_TABLE_STEPS_BITS)))) *
               (1 << OSC_TUNE_DENOMINATOR_BITS) / 1.0).round -
              (1 << OSC_TUNE_DENOMINATOR_BITS) / 1.0
  max_tune_rate = tune_rate if max_tune_rate < tune_rate

  $file.printf("%5d,", tune_rate)
  if i == (1 << OSC_TUNE_TABLE_STEPS_BITS) - 1
    $file.printf("\n")
  elsif i % 8 == 7
    $file.printf("\n  ")
  else
    $file.printf(" ")
  end
end
$file.printf("};\n\n")

def generate_osc_wave_table(name, last, amp)
  $file.printf("int16_t g_osc_#{name}_wave_table_h%d[] = {\n  ", last)
  (0..(1 << OSC_WAVE_TABLE_SAMPLES_BITS)).each do |n|
    level = 0
    nn = n
    nn = 0 if n == (1 << OSC_WAVE_TABLE_SAMPLES_BITS)
    max = last
    (1..max).each do |k|
      level += yield(nn, k)
    end
    level *= amp
    level = (level * OSC_WAVE_TABLE_AMP).round.to_i
    $file.printf("%+6d,", level)
    if n == (1 << OSC_WAVE_TABLE_SAMPLES_BITS)
      $file.printf("\n")
    elsif n % 16 == 15
      $file.printf("\n  ")
    else
      $file.printf(" ")
    end
  end
  $file.printf("};\n\n")
end

$osc_harmonics_restriction_table = []

(NOTE_NUMBER_MIN..NOTE_NUMBER_MAX).each do |note_number|
  correction = (max_tune_rate.to_f + (1 << OSC_TUNE_DENOMINATOR_BITS)) / (1 << OSC_TUNE_DENOMINATOR_BITS)
  freq = freq_from_note_number(((note_number + (3 - 1)) / 3) * 3) * correction
  freq = freq.floor
  bit = 1
  freq += bit
  $osc_harmonics_restriction_table << freq
end

OSC_DETUNE_CORRECRION = 1015  # Approx. 25 cents

def last_harmonic(freq)
  last = (freq != 0) ? ((FREQUENCY_MAX * (1 << OSC_PHASE_RESOLUTION_BITS)) /
                        (((freq * OSC_DETUNE_CORRECRION / 1000) + OSC_DETUNE_FREQ_MAX) * SAMPLING_RATE)) : 0
  last = 9 if last == 10
  last = 7 if last == 8
  last = 5 if last == 6
  last = 3 if last == 4
  last = [last, 127].min
  last
end

def generate_osc_wave_table_arrays
  $osc_harmonics_restriction_table.
    map { |freq| last_harmonic(freq) }.uniq.sort.reverse.each do |i|
    yield(i)
  end
end

generate_osc_wave_table_arrays do |last|
  generate_osc_wave_table("saw", last, 1.0) do |n, k|
    (2.0 / Math::PI) * Math.sin((2.0 * Math::PI) *
                                (n.to_f / (1 << OSC_WAVE_TABLE_SAMPLES_BITS)) * k) / k
  end
end

generate_osc_wave_table_arrays do |last|
  generate_osc_wave_table("triangle", last, 1.0) do |n, k|
    if k % 4 == 1
      +(8.0 / (Math::PI * Math::PI)) * Math.sin((2.0 * Math::PI) *
                                                (n.to_f / (1 << OSC_WAVE_TABLE_SAMPLES_BITS)) * k) / (k * k)
    elsif k % 4 == 3
      -(8.0 / (Math::PI * Math::PI)) * Math.sin((2.0 * Math::PI) *
                                                (n.to_f / (1 << OSC_WAVE_TABLE_SAMPLES_BITS)) * k) / (k * k)
    else
      0.0
    end
  end
end

generate_osc_wave_table_arrays do |last|
  generate_osc_wave_table("square", last, 1.0) do |n, k|
    if k % 2 == 1
      (4.0 / Math::PI) * Math.sin((2.0 * Math::PI) *
                                  (n.to_f / (1 << OSC_WAVE_TABLE_SAMPLES_BITS)) * k) / k
    else
      0.0
    end
  end
end

generate_osc_wave_table("sine", 1, 1.0) do |n, k|
  Math.sin((2.0 * Math::PI) * (n.to_f / (1 << OSC_WAVE_TABLE_SAMPLES_BITS)) * k)
end

def generate_osc_wave_tables_array(name, last = 127)
  $file.printf("int16_t* g_osc_#{name}_wave_tables[] = {\n  ")
  $osc_harmonics_restriction_table.each_with_index do |freq, idx|
    $file.printf("g_osc_#{name}_wave_table_h%-3d,", [last_harmonic(freq), last].min)
    if idx == DATA_BYTE_MAX
      $file.printf("\n")
    elsif idx % 3 == (3 - 1)
      $file.printf("\n  ")
    else
      $file.printf(" ")
    end
  end
  $file.printf("};\n\n")
end

generate_osc_wave_tables_array("saw")
generate_osc_wave_tables_array("triangle")
generate_osc_wave_tables_array("square")
generate_osc_wave_tables_array("sine", 1)

$file.printf("int32_t g_portamento_coef_table[] = {\n  ")
(0..127).each do |i|
  time = i
  portamento_coef = (0.5 ** (1.0 / ((0.1 / 10.0) * (SAMPLING_RATE / 4) * (10.0 ** ((time - 64.0) / 32.0)))) * 0x40000000).round
  portamento_coef = 0 if time == 0
  
  $file.printf("%10d,", portamento_coef)
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
