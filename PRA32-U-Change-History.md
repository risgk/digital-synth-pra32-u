## Digital Synth PRA32-U Change History

- v3.1.0 (2025-01-19):
    - Add the Voice Assign Mode [1|2] parameter
        - Old versions is equivalent to Mode 2
    - Sine Wave (Phase Modulation): Change the unit of Frequency Ratio from 0.25 to 0.5
    - Sine Wave (Phase Modulation): Improve Frequency Ratio to change smoothly
    - Extend Osc 2 Pitch range (-5 to +7 -> -12 to +12 semitone)
    - Extend Pitch modulation range by EG Osc Amt and LFO Osc Amt (-24 to +24 -> -30 to +30 semitone)
    - PRA32-U with Panel: Change MIDI clock and commands not to be transmitted via USB MIDI
        - To avoid freeze problem when transmitting and receiving USB MIDI at the same time
    - PRA32-U with Panel: Add the Filter EG Amt parameter to the EG pages as well
    - PRA32-U with Panel: Rename the parameter name Seq Pattern to Seq Mode
    - PRA32-U with Panel: Reduce memory usage
        - Fix an issue where emulated EEPROM would not work with some compilation options
    - Other improvements
    - Tested with Arduino-Pico version 4.4.1
        - Overclock to 153.6 MHz (instead of 147.6 MHz)
- v3.0.0 (2024-11-13):
    - Change Control Numbers: Osc 1 Wave 102 to 14, Voice Mode 14 to 102
    - Mixer Noise is not disabled even if Osc 2 Wave is Nos (Noise)
    - Change Preset programs: Change Delay Mode to S (Stereo Delay)
    - Rename PRA32-U CTRL to PRA32-U Editor
    - Officially support the option PRA32-U with Panel
        - Faster OLED display updates
        - Change the page layout and the default pages
        - Change the specification of Panel Scale parameter
        - Change the specification of Seq Last Step parameter and rename Seq Last Step to Seq Num Steps (Number of Steps)
        - Add Panel Pitch Ofst (Pitch Offset) parameter
        - Add Seq On Steps parameter
        - Add Seq Rand Pitch (Randomize Pitch 0-7) function
        - Add Seq Rand Velo (Randomize Velo 0-7) function
        - Fix Seq Gate Time behavior
    - Tested with Arduino-Pico version 4.2.0
- v2.6.1 (2024-09-22):
    - Fix Bug: PWM Audio: Notes are played a whole tone higher
    - Other improvements
- v2.6.0 (2024-09-15):
    - Extend "Delay Time" range (20-300 to 1-333.3 ms)
    - Update the option PRA32-U with Panel to Prototype 3 (experimental)
        - Add the parameter "Seq Step Note" (Quarter Note, Eighth Note, Sixteenth Note)
        - "Panel Play Pitch" and "Seq Pitch 0-7" displays reflect "Panel Transpose" (but not "Seq Transpose")
        - Fix "Panel Scale", "Panel Transpose", and "Seq Transpose" update timing in "Seq Pattern" Reverse and Bounce
        - Fix a problem with "Seq Pattern" displaying incorrectly when the value is 1
    - PRA32-U CTRL, PRA32-U with Panel: Add ms display of "Delay Time"
    - Fix MIDI Implementation Chart
    - Other improvements
    - Tested with Arduino-Pico version 4.0.2
- v2.5.1 (2024-09-08):
    - Fix a problem with noise in PRA32-U CTRL Recall operation when PWM Audio is used
        - This problem occurred when 3 or 4 notes were playing in Poly Mode and PRA32_U_USE_DEBUG_PRINT was commented out (default)
    - Add description to Delay Time in Parameter Guide
- v2.5.0 (2024-09-04):
    - PRA32-U with Panel: Support MIDI Clock and Start/Stop Commands transmission
    - Fix "pra32-u-make-sample-wav-file.cc"
    - Other changes
    - Tested with Arduino-Pico version 4.0.1
- v2.4.1 (2024-09-01):
    - PRA32-U with Panel: Fix a problem with MIDI clock being processed even when sequencer is stopped
    - PRA32-U with Panel: Fix a problem where Start/Stop MIDI commands are processed even when Seq Clock Src is Internal
    - Fix MIDI Implementation Chart
    - Tested with Arduino-Pico version 4.0.1
- v2.4.0 (2024-08-25):
    - Increase the output level using Extra Amp and Limiter
    - For safety, change the mode of the PWM audio output pins to OUTPUT_12MA
    - Update the option PRA32-U with Panel to Prototype 2 (experimental)
        - Change the page structure (Add the page groups A, B, C, and D)
        - Change the specification of "Panel Scale"
        - Add the parameter "Panel MIDI Ch" (Basic Channel)
        - Add Step Sequencer Mode ("Seq") to "Panel Play Mode"
        - Add the option PRA32_U_KEY_INPUT_PIN_MODE (default: INPUT_PULLDOWN)
        - Add the functions "Write Panel Prms", "Read Panel Prms", and "Init Panel Prms"
    - PRA32-U CTRL: Change the Note ON velocity to 64
    - Other changes
    - Tested with Arduino-Pico version 3.9.5
- v2.3.1 (2024-04-21):
    - Abolish the option to write user programs to the flash when using PWM audio due to performance problem uncovered
    - Tested with Arduino-Pico version 3.7.2
- v2.3.0 (2024-04-21):
    - Allow user programs to be written to the flash when using I2S DAC without mute-off pin
    - Add the option to write user programs to the flash when using PWM audio (for Raspberry Pi Pico/H/W/WH)
    - Add the option PRA32_U_I2S_SWAP_LEFT_AND_RIGHT
    - Add the option PRA32-U with Panel, Prototype 1 (experimental)
    - Change the default program #0 to #8
    - Use core 1 for Debug Print
    - Other changes
    - Tested with Arduino-Pico version 3.7.2
- v2.2.2 (2024-02-10):
    - Fix a bug that the parameters are not written to the flash when using I2S DAC, even if PRA32_U_I2S_DAC_MUTE_OFF_PIN is defined
    - Tested with Arduino-Pico version 3.7.0
- v2.2.1 (2024-02-04):
    - No more need to install Adafruit TinyUSB Library separately from Arduino-Pico
- v2.2.0 (2024-01-26):
    - Use error diffusion for PWM audio output by default
    - Eliminate noise when PWM audio output is silent
    - Change the display of PRA32-U CTRL
    - Modify Presets;
    - Tested with Adafruit TinyUSB Library version 2.3.3
- v2.1.0 (2024-01-14):
    - Reduce noise from PMIC (Turn off RT6150 Power Save);
    - Support Osc 1 Sine Wave Phase Modulation (Osc 1 Shape and Osc 1 Morph);
    - Add the function of writing the parameters to Program #8-15 and the flash;
    - Modify Presets;
    - Add JSON files for PRA32-U CTRL;
    - Use core 0 for main processing;
    - Rename constant macros;
    - Officially support PWM audio output option;
    - Rename Serial MIDI to UART MIDI;
    - Enable UART MIDI to be used at the same time as USB MIDI;
    - Use Serial1 for Debug Print and Serial2 for UART MIDI;
    - Raspberry Pi Pico/RP2040 core version 3.6.3 is recommended
- v2.0.1 (2024-01-05):
    - Fix Ping Pong Delay to start from the left
    - Fix Parameter Guide (Filter Resonance)
- v2.0.0 (2024-01-04):
    - Add Delay Mode (Ping Pong Delay);
    - Support Breath Controller (Breath Filter Amt and Breath Amp Mod);
    - Support Note ON Velocity (EG Velocity Sensitivity and Amp Velocity Sensitivity);
    - Modify EG and Amp EG;
    - Modify Presets;
    - Change the control numbers of Control Changes
      (Voice Mode: 27 -> 14, LFO Wave: 18 -> 12, Filter Mode: 86 -> 78, EG Amp Mod: 87 -> 28, and Chorus Mix: 93 -> 27);
    - Change the meanings of the values of Control Changes
      (Osc 1 Wave, Osc 2 Wave, EG Osc Dst, Voice Mode, LFO Wave, LFO Osc Dst, Filter Mode, EG Amp Mod, Release = Decay, Breath Amp Mod, and Delay Mode);
    - Increase DMA buffer size (audio latency: 2.7 ms -> 5.3 ms);
    - Use 2 cores for signal processing in Polyphonic and Paraphonic Modes;
    - Add PWM audio output option (experimental)
- v1.2.1: Fixed an oscillation problem caused by Delay Feedback
- v1.2.0: Renew High Pass Filter; Raspberry Pi Pico/RP2040 core version 3.6.2 is recommended
- v1.1.1: Revert Filter clipping in High Pass mode
- v1.1.0: Fix Filter clipping (behavior during oscillation); Extend Pitch Bend Range;
  Delete the code for Waveshare Pico-Audio Rev2.1
- v1.0.3: Fix a problem with received MIDI messages being dropped; Improve README; Fix UF2 files
- v1.0.2: Turn MIDI Thru Off
- v1.0.1: Fix unstable PRA32-U CTRL operation when using Serial MIDI (DIN/TRS MIDI); Fix README
- v1.0.0: Change Control Numbers of Control Changes;
  Change the meanings of the values of Osc 1/2 Wave, Voice Mode, and LFO Wave; Add Sine Wave to Osc 1 and 2;
  Limit the change range and the change rate of Osc 1 Shape; Change Noise specifications;
  Change Sub Osc Triangle Wave to Sine Wave; Change LFO Triangle Wave 2 to Sine Wave;
  Improve resolution of Filter Resonance; Limit the change rate of Filter Cutoff due to modulation;
  Change Amp Level to Amp Gain; Change Chorus Mode to Chorus Mix; Add Delay Fx;
  Delete Chorus Delay Time, Chorus Bypass, and Pitch Bend by CC; Update Presets; Other changes
- v0.4.0 (Prototype): Extend Osc 2 Coarse range; Change Osc 2 Fine curve; Rename "Osc 2 Fine" to "Osc 2 Pitch";
  Change LFO Osc/Filter Amt curve; Update Presets
- v0.3.1 (Prototype): Adjust smoothing time when parameters change; Update Presets; Fix LFO Osc/Filter Amt; Other changes
- v0.3.0 (Prototype): Extend Note Number range; Remove drift of Oscillators; Improve Triangle Wave; Change Portamento curve;
  Improve Filter Low Cut Mode; Rename Filter Mode LC (Low Cut) to HP (High Pass); Modify Filter coefficients;
  Change EG curve; Extend LFO Rate curve; Change LFO Fade Time; Change Chorue Rate curve; Update Presets;
  Fix Filter Key Track (Fix problem with strange sound when pitch is outside of note number range);
  Fix behavior when Portamento is 0 (Off); Other changes
- v0.2.0 (Prototype): Change Sampling Rate to 48000 Hz; Support MCLK for I2S; Support Polyphonic Mode;
  Extend Filter Cutoff frequency change range to 10+ octaves;
  Change "Mixer Sub Osc" to "Mixer Noise/Sub Osc"; Add "EG Amp Mod", "Release = Decay", and "Filter Mode";
  Enable Osc 1 Shape, Morph, Noise/Sub Osc, and Osc 2 in Paraphonic and Polyphonic Modes; Other changes
- v0.1.0 (Prototype): The first release
