File.open("../Digital-Synth-PRA32-U/pra32-u-constants.h", "r") do |input|
  File.open("./pra32-u-constants.rb", "w") do |output|
    input.each_line do |line|
      output.puts line.chomp[16..-2]
    end
  end
end
