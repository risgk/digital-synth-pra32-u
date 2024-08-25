require_relative 'Digital-Synth-PRA32-U/pra32-u-constants'

PRA32_U_MIDI_CH = 0  # 0-based

$file = File.open("pra32-u-sample-midi-stream.bin", "wb")

def control_change(control_number, value)
  $file.write([(CONTROL_CHANGE | PRA32_U_MIDI_CH), control_number, value].pack("C*"))
  wait(10)
end

def program_change(program_number)
  $file.write([(PROGRAM_CHANGE | PRA32_U_MIDI_CH), program_number].pack("C*"))
  wait(800)
end

def note_on(note_number, velocity)
  $file.write([(NOTE_ON  | PRA32_U_MIDI_CH), note_number, velocity].pack("C*"))
end

def note_off(note_number)
  $file.write([(NOTE_OFF | PRA32_U_MIDI_CH), note_number, 64].pack("C*"))
end

def wait(length)
  length.times { $file.write([ACTIVE_SENSING].pack("C")) }
end

def play_mono_a(oct)
  play_mono(12, oct, 64)
  play_mono(16, oct, 64)
  play_mono(14, oct, 64)
  play_mono(17, oct, 64)
end

def play_a(oct)
  play_chord_a(12, 16, 19, 23, oct, 64)
  play_chord_a(16, 19, 23, 26, oct, 64)
  play_chord_a(14, 17, 21, 24, oct, 64)
  play_chord_a(17, 21, 24, 28, oct, 64)
end

def play_b(oct)
  play_chord_b(12, 16, 19, 23, oct, 64)
  play_chord_b(16, 19, 23, 26, oct, 64)
  play_chord_b(14, 17, 21, 24, oct, 64)
  play_chord_b(17, 21, 24, 28, oct, 64)
end

def play_mono(x, oct, velocity)
  note_on(x + (oct * 12), velocity)
  wait(3200)
  note_off(x + (oct * 12))
  wait(800)
end

def play_chord_a(x, y, z, w, oct, velocity)
  note_on(x + (oct * 12), velocity)
  note_on(y + (oct * 12), velocity)
  note_on(z + (oct * 12), velocity)
  note_on(w + (oct * 12), velocity)
  wait(3200)
  note_off(x + (oct * 12))
  note_off(y + (oct * 12))
  note_off(z + (oct * 12))
  note_off(w + (oct * 12))
  wait(800)
end

def play_chord_b(x, y, z, w, oct, velocity)
  note_on(x + (oct * 12), velocity)
  wait(800)
  note_on(y + (oct * 12), velocity)
  wait(800)
  note_on(z + (oct * 12), velocity)
  wait(800)
  note_on(w + (oct * 12), velocity)
  wait(3200)
  note_off(w + (oct * 12))
  wait(800)
  note_off(z + (oct * 12))
  wait(800)
  note_off(y + (oct * 12))
  wait(800)
  note_off(x + (oct * 12))
  wait(800)
end

def sound_off
  control_change(ALL_SOUND_OFF, 0  )
  wait(800)
end

sound_off

program_change(0)
play_mono_a(3)
sound_off

program_change(6)
play_b(3)
sound_off

$file.close
