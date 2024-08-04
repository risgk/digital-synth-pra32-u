# Digital Synth PRA32-U v2.3.1

- 2024-04-21 ISGK Instruments
- <https://github.com/risgk/digital-synth-pra32-u>


## Overview

- 4 Voice Polyphonic/Paraphonic Synthesizer for Raspberry Pi Pico/RP2040
    - Built-in Chorus and Delay FX
    - Controlled by MIDI -- PRA32-U is a MIDI sound module
    - Having the function of writing the parameters to the user programs and the flash
- Modifiable with Arduino IDE and Arduino-Pico (by Earle F. Philhower, III)
- An **I2S DAC** hardware (e.g. Pimoroni Pico Audio Pack) is required
    - PWM Audio can also be used instead of I2S (PWM Audio does not require an I2S DAC hardware)
- Prebuilt UF2 files ("bin")
    - "Digital-Synth-PRA32-U-2.3.1-Pimoroni-Pico-Audio-Pack.uf2" is for Raspberry Pi Pico and Pimoroni Pico Audio Pack


## [Change History](/PRA32-U-Change-History.md)


## Preparation for modification

- Please install **Arduino IDE**
    - NOTE: Large noise is generated during the sketch upload if other than Update Method: "Default (UF2)" is used
    - Info: <https://www.arduino.cc/en/software>
- Please install Arduino-Pico = **Raspberry Pi Pico/RP2040** (by Earle F. Philhower, III) core
    - Additional Board Manager URL: <https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json>
    - This sketch is tested with version **3.9.4**: <https://github.com/earlephilhower/arduino-pico/releases/tag/3.9.4>
        - Arduino-Pico version 3.9.4 includes Adafruit TinyUSB Library version 2.3.0
    - Info: <https://github.com/earlephilhower/arduino-pico>
- Please install Arduino **MIDI Library** (by Francois Best, lathoub)
    - This sketch is tested with version **5.0.2**: <https://github.com/FortySevenEffects/arduino_midi_library/releases/tag/5.0.2>
    - Info: <https://github.com/FortySevenEffects/arduino_midi_library>


## Features

### MIDI (Input)

#### USB MIDI Device (Default)

- MIDI Device Name: "Digital Synth PRA32-U"
- NOTE: Select USB Stack: "Adafruit TinyUSB" in the Arduino IDE "Tools" menu


#### UART MIDI (Optional)

- UART MIDI can also be used
    - Noise caused by USB communication can be avoided
- Uncomment out `//#define PRA32_U_USE_UART_MIDI` in "Digital-Synth-PRA32-U.ino"
  and modify `PRA32_U_UART_MIDI_SPEED`, `PRA32_U_UART_MIDI_TX_PIN`, and `PRA32_U_UART_MIDI_RX_PIN`
    - Speed: 31250 bps (default, for DIN/TRS MIDI) or 38400 bps (for PC)
    - GP4 and GP5 pins are used by UART1 TX and UART1 RX by default
- DIN/TRS MIDI is available by using (and modifying) Adafruit MIDI FeatherWing Kit, for example
    - Adafruit [MIDI FeatherWing Kit](https://www.adafruit.com/product/4740)
    - 木下研究所 [MIDI-UARTインターフェースさん キット](https://www.switch-science.com/products/8117) (Shipping to Japan only)
    - necobit電子 [MIDI Unit for GROVE](https://necobit.com/denshi/grove-midi-unit/) (Shipping to Japan only)
- We recommend using [Hairless MIDI<->Serial Bridge](https://projectgus.github.io/hairless-midiserial/) on PC
    - On Windows, We recommend using [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) (virtual loopback MIDI cable)
    - On Mac, a virtual MIDI bus (port) can be created by using the IAC bus


### Audio (Output)

#### I2S (Default)

- Use an I2S DAC (e.g. Texas Instruments PCM5100A and Cirrus Logic CS4344), Sampling Rate: 48 kHz, Bit Depth: 16 bit
- NOTE: The RP2040 system clock (sysclk) changes to overclocked 147.6 MHz by I2S Audio Library setSysClk()
- Modify `PRA32_U_I2S_DAC_MUTE_OFF_PIN`, `PRA32_U_I2S_DATA_PIN`, `PRA32_U_I2S_MCLK_PIN`, `PRA32_U_I2S_MCLK_MULT`,
  `PRA32_U_I2S_BCLK_PIN`, `PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS`, and `PRA32_U_I2S_SWAP_LEFT_AND_RIGHT`
  in "Digital-Synth-PRA32-U.ino" to match the hardware configuration
- The default setting is for Pimoroni [Pico Audio Pack](https://shop.pimoroni.com/products/pico-audio-pack) (PIM544)
```
#define PRA32_U_I2S_DAC_MUTE_OFF_PIN          (22)
#define PRA32_U_I2S_DATA_PIN                  (9)
//#define PRA32_U_I2S_MCLK_PIN                  (0)
//#define PRA32_U_I2S_MCLK_MULT                 (0)
#define PRA32_U_I2S_BCLK_PIN                  (10)  // LRCLK Pin is PRA32_U_I2S_BCLK_PIN + 1
#define PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS  (false)
#define PRA32_U_I2S_SWAP_LEFT_AND_RIGHT       (false)
```
- The following is setting is for [Pimoroni Pico VGA Demo Base](https://shop.pimoroni.com/products/pimoroni-pico-vga-demo-base) (PIM553)
```
//#define PRA32_U_I2S_DAC_MUTE_OFF_PIN          (0)
#define PRA32_U_I2S_DATA_PIN                  (26)
//#define PRA32_U_I2S_MCLK_PIN                  (0)
//#define PRA32_U_I2S_MCLK_MULT                 (0)
#define PRA32_U_I2S_BCLK_PIN                  (27)  // LRCLK Pin is is PRA32_U_I2S_BCLK_PIN + 1
#define PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS  (false)
#define PRA32_U_I2S_SWAP_LEFT_AND_RIGHT       (false)
```
- The following is setting is for [Waveshare Pico-Audio](https://www.waveshare.com/wiki/Pico-Audio) Rev2.1 Version (WAVESHARE-20167)
```
//#define PRA32_U_I2S_DAC_MUTE_OFF_PIN          (0)
#define PRA32_U_I2S_DATA_PIN                  (22)
#define PRA32_U_I2S_MCLK_PIN                  (26)
#define PRA32_U_I2S_MCLK_MULT                 (256)
#define PRA32_U_I2S_BCLK_PIN                  (27)  // LRCLK Pin is is PRA32_U_I2S_BCLK_PIN + 1
#define PRA32_U_I2S_SWAP_BCLK_AND_LRCLK_PINS  (true)
#define PRA32_U_I2S_SWAP_LEFT_AND_RIGHT       (true)
```


#### PWM Audio (Optional)

- PWM Audio can also be used instead of I2S
    - NOTE: To avoid noise, the parameters will not be written to the flash when using PWM audio
    - We recommend adding RC filter (post LPF) circuits to reduce PWM ripples
        - A 1st-order LPFs with a cutoff frequency 7.2 kHz (R = 220 ohm, C = 100 nF) works well
    - See "PWM audio" in [Hardware design with RP2040](https://datasheets.raspberrypi.com/rp2040/hardware-design-with-rp2040.pdf)
      for details on PWM audio
- Uncomment out `//#define PRA32_U_USE_PWM_AUDIO_INSTEAD_OF_I2S`
  in "Digital-Synth-PRA32-U.ino" and modify `PRA32_U_PWM_AUDIO_L_PIN` and `PRA32_U_PWM_AUDIO_R_PIN`
- The following is setting is for Pimoroni Pico VGA Demo Base (PIM553)
```
#define PRA32_U_PWM_AUDIO_L_PIN               (28)
#define PRA32_U_PWM_AUDIO_R_PIN               (27)
```


## Files

- "Digital-Synth-PRA32-U.ino" is a Arduino sketch for Raspberry Pi Pico/RP2040 core
    - Modify `PRA32_U_MIDI_CH` to change the MIDI Channel
- "pra32-u-make-sample-wav-file.cc" is for debugging on PC
    - GCC (g++) for PC is required
    - "pra32-u-make-sample-wav-file-cc.bat" makes a sample WAV file (working on Windows)
- "pra32-u-generate-*.rb" generates source or header files
    - A Ruby execution environment is required


## PRA32-U CTRL

- "pra32-u-ctrl.html": MIDI Controller (Editor) Application for PRA32-U, HTML App (Web App)
    - Modify `PRA32_U_MIDI_CH` to change the MIDI Channel
- We recommend using Google Chrome, which implements Web MIDI API
- Select "Digital Synth PRA32-U" in the list "MIDI Out"
- Functions
    - PRA32-U CTRL converts Program Changes (#0-7 for Preset programs, #8-15 for user programs) into Control Changes
    - When Program Change #127 is entered or Control Change #111 is changed from Off (63 or lower) to On (64 or higher), "Rand Ctrl" is processed
    - PRA32-U CTRL stores the current control values and the user programs (#8-15) in a Web browser (localStorage)
    - Current parameter values and user programs (#8-15) can be imported/exported from/to JSON files
- When not using PRA32-U CTRL
    - PRA32-U can also be controlled by MIDI without using PRA32-U CTRL
    - Refer to "PRA32-U-MIDI-Implementation-Chart.txt" for the supported functions
    - The default program is #8
    - Programs #0-15 can be modified by editing "pra32-u-program-table.h"
    - PRA32-U CTRL functions related to parameter writing
        - Write: Write the current parameters to PRA32-U (Program #8-15 and the flash)
        - Program Change: Send Program Change to PRA32-U directry
          (NOTE: The current parameters of PRA32-U will not be updated)


## Examples of Option Combinations

- PRA32-U (USB MIDI Device, I2S), Default
- PRA32-U (USB MIDI Device, UART MIDI, I2S)
- PRA32-U (USB MIDI Device, PWM Audio)
- PRA32-U with Panel (USB MIDI Device, UART MIDI, I2S, Control Panel)


## [Parameter Guide](/PRA32-U-Parameter-Guide.md)


## [MIDI Implementation Chart](/PRA32-U-MIDI-Implementation-Chart.md)


## Synthesizer Block Diagram

### Polyphonic Mode

```mermaid
graph LR
    subgraph V1[Voice 1]
        V1O1[Osc 1 w/ Sub Osc] --> V1OM[Osc Mixer]
        V1O2[Osc 2] --> V1OM
        V1OM --> V1F[Filter]
        V1F --> V1A[Amp]
        E[EG] -.-> V1O1 & V1O2 & V1F
        V1AE[Amp EG] -.-> V1A
    end
    V1A --> VM[Voice Mixer]
    V2[Voice 2] & V3[Voice 3] & V4[Voice 4] --> VM
    VM --> C[Chorus FX] --> D[Delay FX] --> AO[Audio Out]
    C --> D
    D --> AO
    N[Noise Gen]  --> V1O2 & V1OM & V2 & V3 & V4
    N -.-> L[LFO w/ S/H]
    L -.-> V1O1 & V1O2 & V1F & V2 & V3 & V4
```


### Paraphonic Mode

```mermaid
graph LR
    subgraph V1[Voice 1]
        V1O1[Osc 1 w/ Sub Osc] --> V1OM[Osc Mixer]
        V1O2[Osc 2] --> V1OM
        V1OM --> V1G[Gate]
    end
    V1G --> VM[Voice Mixer]
    V2[Voice 2] & V3[Voice 3] & V4[Voice 4] --> VM
    VM --> F[Filter] --> A[Amp] --> C[Chorus FX] --> D[Delay FX] --> AO[Audio Out]
    C --> D
    D --> AO
    N[Noise Gen]  --> V1O2 & V1OM & V2 & V3 & V4
    N -.-> L[LFO w/ S/H]
    L -.-> V1O1 & V1O2 & V2 & V3 & V4 & F
    E[EG] -.-> V1O1 & V1O2 & V2 & V3 & V4 & F
    AE[Amp EG] -.-> A
```


### Monophonic Mode

```mermaid
graph LR
    O1[Osc 1 w/ Sub Osc] --> OM[Osc Mixer]
    O2[Osc 2] --> OM
    OM --> F[Filter] --> A[Amp] --> C[Chorus FX] --> D[Delay FX] --> AO[Audio Out]
    C --> D
    D --> AO
    N[Noise Gen] --> O2 & OM
    N -.-> L[LFO w/ S/H]
    L -.-> O1 & O2 & F
    E[EG] -.-> O1 & O2 & F
    AE[Amp EG] -.-> A
```


## Simple Circuit for PWM Audio

### Circuit Diagram (Simple Circuit for PWM Audio)

![Circuit Diagram](./pra32-u-pwm-audio-circuit-diagram.png)

- This image was created with Fritzing.

- Adding 10 uF electrolytic capacitors (AC coupling capacitors) will cut
  the DC components of the audio outputs.

- NOTE: Connect an amplifier or an active speaker to the audio jack.
  Connecting a headphone or a passive speaker may cause a large current to flow and damage the devices.


### Actual Wiring Diagram (Simple Circuit for PWM Audio)

![Actual Wiring Diagram](./pra32-u-pwm-audio-bread-board.png)

- This image was created with Fritzing.


## PRA32-U with Panel, Prototype 1 (Experimental) (Optional)

![PRA32-U with Panel, Prototype 1](./pra32-u-with-panel-prototype-1.jpg)

- Uncomment out `//#define PRA32_U_USE_CONTROL_PANEL` in "Digital-Synth-PRA32-U.ino" and modify options
- This option requires 3 tactile switches, 3 VRs (ADCs), and a monochrome 128x64 OLED Display based on SSD1306 series drivers
    - Tested with Pimoroni Pico Audio Pack, necobit電子 MIDI Unit for GROVE, Long Leg 2x20 Pin Socket * 2, Seeed Studio's Grove Shield for Pi Pico, Buttons, Rotary Angle Sensors, and a OLED Display 0.96 inch
- Inputs
    - SW0: Prev Key
    - SW1: Next Key
    - SW2: Play Key (play notes)
    - VR0 (ADC0): Parameter A
    - VR1 (ADC1): Parameter B
    - VR2 (ADC2): Parameter C
- Panel Parameters
    - Panel Pitch
    - Panel Scale [Ful|Pen|Maj]
        - Full = Chromatic, 10 + 7/12 octaves
        - Major Pentatonic, 2 octaves
        - Major, 2 octaves
    - Panel Transpose [-|+]
    - Panel Velocity
- Other Operations
    - Write Program 8-15: Change the value from 0 [Rdy] to 127 [Exe]
    - Read Program 1-7, 8-15: Change the value from 0-63 [Rdy] to 64-127 [Exe]
- NOTE: Specifications may change significantly in the future


### Circuit Diagram (PRA32-U with Panel)

![Circuit Diagram](./pra32-u-with-panel-audio-circuit-diagram.png)

- This image was created with Fritzing.


## License

![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png)

**Digital Synth PRA32-U v2.3.1 by ISGK Instruments (Ryo Ishigaki)**

To the extent possible under law, ISGK Instruments (Ryo Ishigaki)
has waived all copyright and related or neighboring rights
to Digital Synth PRA32-U v2.3.1.

You should have received a copy of the CC0 legalcode along with this
work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
