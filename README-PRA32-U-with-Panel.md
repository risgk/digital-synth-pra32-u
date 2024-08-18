# PRA32-U with Panel (Experimental) (Optional)

- 2024-04-21 ISGK Instruments
- <https://github.com/risgk/digital-synth-pra32-u>


## PRA32-U with Panel, Prototype 2

![PRA32-U with Panel (Grove System)](./pra32-u-with-panel-grove-system.jpg)

- This option requires 3 SWs (tactile switches), 3 VRs (ADCs), and a monochrome 128x64 OLED Display based on SSD1306 series drivers
    - Tested with Pimoroni Pico Audio Pack, necobit電子 MIDI Unit for GROVE (optional), Long Leg 2x20 Pin Socket * 2, Seeed Studio's Grove Shield for Pi Pico, Buttons * 3, Rotary Angle Sensors * 3, and an OLED Display 0.96 inch
- Uncomment out `//#define PRA32_U_USE_CONTROL_PANEL` in "Digital-Synth-PRA32-U.ino" and modify the constants
- Inputs
    - SW0: Prev Key (Push to the previous page, Long press to the previous group)
    - SW1: Next Key (Push to the next page, Long press to the next group)
    - SW2: Play Key (Normal Mode: press to play, Sequencer Mode: push to start/stop)
    - VR0 (ADC0): Parameter A
    - VR1 (ADC1): Parameter B
    - VR2 (ADC2): Parameter C
- Panel Parameters
    - Panel Scale [Maj|Min|MaP|MiP|Chr|Ful]
        - Major, 2 octaves
        - Minor, 2 octaves
        - Major Pentatonic, 2 octaves
        - Minor Pentatonic, 2 octaves
        - Chromatic, 2 octaves
        - Full = Chromatic, 10 + 7/12 octaves
    - Panel Transpose [-|+]
    - Panel Play Mode [Nrm|Seq]
        - Normal Mode
        - Sequencer Mode
    - Panel MIDI Ch
    - Panel Play Pitch
    - Panel Play Velo
- Step Sequencer Parameters
    - Seq Pitch 0-7
    - Seq Velo 0-7
    - Seq Tempo
    - Seq Clock Src [Int|Ext]
        - Internal
        - External (MIDI Clock)
    - Seq Last Step
    - Seq Transpose [-|+]
- Other Operations
    - Write Program 8-15, Write Panel Prms (Panel and Step Sequencer Parameters), Read Program 1-7, 8-15, Read Panel Prms, Init Panel Prms
      - Change the value from 0-63 [Rdy] to 64-127 [Exe]
- KNOWN ISSUE: An OLED display refresh is slow when switching pages
- NOTE: Specifications may change significantly in the future


### Circuit Diagram

![Circuit Diagram](./pra32-u-with-panel-circuit-diagram.png)

- This image was created with Fritzing.


### An Example of Construction by a Breadboard

- Tested with a breadboard, GY-PCM5102 (PCM5102A I2S DAC Module), 3 SWs, 3 VRs, and a OLED Display
    - An connection between RP2040's Mute Off Pin and GY-PCM5102's XSMT is omitted

![PRA32-U with Panel (Bread Board)](./pra32-u-with-panel-breadboard.jpg)
