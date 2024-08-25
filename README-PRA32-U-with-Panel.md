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
- Group A
    - Synth Parameters
- Group B
    - Panel Parameters
        - Panel Play Mode [Nrm|Seq]
            - Normal Mode
            - Step Sequencer Mode
        - Panel MIDI Ch
        - Panel Play Pitch
        - Panel Play Velo
        - Panel Scale [Maj|Min|Chr]
            - Major, 2 octaves
            - Minor, 2 octaves
            - Chromatic, 2 octaves
        - Panel Transpose [-|+]
    - Step Sequencer Parameters
        - Seq Pattern [Fwd|Rvs|Bnc]
            - Forward
            - Bounce
            - Reverse
        - Seq Act Steps (Active Steps)
            - bit 0: Step 1 Active, ,,,, bit 6: Step 7 Active
        - Seq Last Step
        - Seq Tempo
            - BPM 30-300
        - Seq Gate Time [1/6|2/6|3/6|4/6|5/6|6/6]
        - Seq Transpose [-|+]
        - Seq Clock Src [Int|Ext]
            - Internal
            - External (Rx MIDI Clock)
        - Seq Pitch 0-7
        - Seq Velo 0-7
    - Control Parameters
       - Modulation
       - Breath Controller
       - Sustain Pedal
- Group C
    - Write Operations
        - Write Program 8-15, Write Panel Prms (Write Panel and Step Sequencer Parameters)
          - Change the value from 0-63 [Rdy] to 64-127 [Exe] to write to the flash
- Group D
    - Read Operations
        - Read Program 1-7, 8-15, Read Panel Prms, Init Panel Prms
          - Change the value from 0-63 [Rdy] to 64-127 [Exe]
- KNOWN ISSUE: An OLED display refresh is slow when switching pages
- NOTE: UART MIDI control is recommended to prevent ADCs from being affected by USB MIDI communication noise
- NOTE: Specifications may change significantly in the future to read from the flash


### Circuit Diagram

![Circuit Diagram](./pra32-u-with-panel-circuit-diagram.png)

- This image was created with Fritzing.


### An Example of Construction by a Breadboard

- Tested with a breadboard, GY-PCM5102 (PCM5102A I2S DAC Module), 3 SWs, 3 VRs, and a OLED Display
    - An connection between RP2040's Mute Off Pin and GY-PCM5102's XSMT is omitted

![PRA32-U with Panel (Bread Board)](./pra32-u-with-panel-breadboard.jpg)
