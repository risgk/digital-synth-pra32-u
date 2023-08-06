# Digital Synth PRA32-U v0.1.1

- 2023-05-09 ISGK Instruments
- <https://github.com/risgk/digital-synth-pra32-u>


## Overview

- Monophonic/Paraphonic United Synthesizer for Raspberry Pi Pico/RP2040
    - Monophonic Mode: 1-Voice, 2-Oscillator (with Sub Oscillator)
    - Paraphonic Mode: 4-Voice, 2-Oscillator (with Sub Oscillator)
    - Built-in Chorus FX
    - Controlled by MIDI -- PRA32-U is a MIDI sound module
- The sound of **PRA32-U v0.1** is very similar to that of **VRA8-U (type-16) v2.2**
    - <https://github.com/risgk/digital-synth-vra8-u>
- An **I2S DAC** hardware (e.g. Pimoroni Pico Audio Pack and Waveshare Pico-Audio) is required
    - **NOTE**: Large noise is generated during the sketch upload!
- **Arduino IDE** and **Arduino-Pico** = **Raspberry Pi Pico/RP2040** (by Earle F. Philhower, III) core are required
    - Additional Board Manager URL: <https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json>
    - Raspberry Pi Pico/RP2040 core **version 3.3.1** is recommended


## Change History

- v0.1.1: Fix a bug where 0-velocity Note ON is not processed as Note OFF
- v0.1.0: The first release


## Features

### MIDI In

- USB MIDI is the default
    - MIDI Device Name: "Digital Synth PRA32-U"
    - **NOTE**: Select USB Stack: "Adafruit TinuUSB" in the Arduino IDE "Tools" menu
    - **KNOWN ISSUE**: When using some USB MIDI host hardware and communicate a lot, this device may miss MIDI messages
- UART MIDI can also be used instead of USB MIDI
    - Comment out `#define USE_USB_MIDI` and uncomment out `//#define USE_SERIAL1_MIDI`
      in "Digital-Synth-PRA32-U.ino" and modify `SERIAL1_MIDI_SPEED`
    - Speed: 31.25 kbps (default) or 38.4 kbps
    - UART0 TX and RX are used by default
    - DIN/TRS MIDI is available by using (and modifying) Adafruit MIDI FeatherWing Kit, for example
        - Adafruit [MIDI FeatherWing Kit](https://www.adafruit.com/product/4740)
        - necobit電子 [MIDI Unit for GROVE](https://necobit.com/denshi/grove-midi-unit/) (Shipping to Japan only)
        - 木下研究所 [MIDI-UARTインターフェースさん キット](https://www.switch-science.com/products/8117) (Shipping to Japan only)


### Audio Out

- Use an I2S DAC (e.g. Texas Instruments PCM5100A and Cirrus Logic CS4344), Sampling Rate: 48 kHz, Bit Depth: 16 bit
- **NOTE**: The RP2040 system clock (sysclk) changes to overclocked 147.6 MHz by I2S Audio Library setSysClk()
- Modify `I2S_DAC_MUTE_OFF_PIN`, `I2S_DATA_PIN`, `I2S_MCLK_PIN`, `I2S_MCLK_MULT`, `I2S_BCLK_PIN`, and `I2S_SWAP_BCLK_AND_LRCLK_PINS`
  in "Digital-Synth-PRA32-U.ino" to match the hardware configuration
- The default setting is for Pimoroni [Pico Audio Pack](https://shop.pimoroni.com/products/pico-audio-pack) [PIM544]
```
  #define I2S_DAC_MUTE_OFF_PIN            (22)
  #define I2S_DATA_PIN                    (9)
  //#define I2S_MCLK_PIN                    (0)
  //#define I2S_MCLK_MULT                   (0)
  #define I2S_BCLK_PIN                    (10)  // I2S_LRCLK_PIN is I2S_BCLK_PIN + 1
  #define I2S_SWAP_BCLK_AND_LRCLK_PINS    (false)
```
- The following is setting is for [Waveshare Pico-Audio](https://www.waveshare.com/wiki/Pico-Audio) Rev2.1 [WAVESHARE-20167]
```
  //#define I2S_DAC_MUTE_OFF_PIN            (0)
  #define I2S_DATA_PIN                    (22)
  #define I2S_MCLK_PIN                    (26)
  #define I2S_MCLK_MULT                   (256)
  #define I2S_BCLK_PIN                    (27)  // I2S_LRCLK_PIN is I2S_BCLK_PIN + 1
  #define I2S_SWAP_BCLK_AND_LRCLK_PINS    (true)
```


## Files

- "bin"
    - "Digital-Synth-PRA32-U-0.1.1-USB-MIDI-Pimoroni.uf2": USB MIDI, for Pimoroni Pico Audio Pack
    - "Digital-Synth-PRA32-U-0.1.1-UART-MIDI-Pimoroni.uf2": UART MIDI, for Pimoroni Pico Audio Pack
    - "Digital-Synth-PRA32-U-0.1.1-USB-MIDI-Waveshare.uf2": USB MIDI, for Waveshare Pico-Audio Rev2.1
    - "Digital-Synth-PRA32-U-0.1.1-UART-MIDI-Waveshare.uf2": UART MIDI, for Waveshare Pico-Audio Rev2.1
- "Digital-Synth-PRA32-U.ino" is a Arduino sketch for Raspberry Pi Pico/RP2040 core
- "pra32-u-make-sample-wav-file.cc" is for debugging on PC
    - GCC (g++) for PC is required
    - "pra32-u-make-sample-wav-file-cc.bat" makes a sample WAV file (working on Windows)
- "pra32-u-generate-*.rb" generates source or header files
    - A Ruby execution environment is required


## PRA32-U CTRL

- "pra32-u-ctrl.html": MIDI Controller (Editor) Application for PRA32-U, HTML App (Web App)
- We recommend using Google Chrome, which implements Web MIDI API
- Select "Digital Synth PRA32-U" in the list "MIDI Out"
- Functions
    - PRA32-U CTRL converts Program Changes (#0-7 for PRESET, #8-15 for user programs) into Control Changes
    - When Program Change #127 is entered or Control Change #90 is changed from Off (63 or lower) to On (64 or higher), "Rand Ctrl" is processed
    - PRA32-U CTRL stores the current control values and the user programs (#8-15) in a Web browser (localStorage)
    - Current parameter values and user programs (#8-15) can be imported/exported from/to JSON files


## [Parameter Guide](/PRA32-U-Parameter-Guide.md)


## [MIDI Implementation Chart](/PRA32-U-MIDI-Implementation-Chart.md)


## Synthesizer Block Diagram

### Monophonic Mode

```mermaid
graph LR
    subgraph V1[Voice 1]
        O1[Osc 1 w/ Sub Osc] --> OM[Osc Mixer]
        O2[Osc 2] --> OM
    end
    OM --> F[Filter] --> A[Amp] --> C[Chorus FX] --> AO[Audio Out]
    N[Noise Gen] --> O2 & OM
    N -.-> L[LFO w/ S/H]
    L -.-> O1 & O2 & F
    E[EG] -.-> O1 & O2 & F
    AE[Amp EG] -.-> A
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
    VM --> F[Filter] --> A[Amp] --> C[Chorus FX] --> AO[Audio Out]
    N[Noise Gen]  --> V1O2 & V1OM & V2 & V3 & V4
    N -.-> L[LFO w/ S/H]
    L -.-> V1O1 & V1O2 & V2 & V3 & V4 & F
    E[EG] -.-> V1O1 & V1O2 & V2 & V3 & V4 & F
    AE[Amp EG] -.-> A
```


## License

![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png)

**Digital Synth PRA32-U v0.1.1 by ISGK Instruments (Ryo Ishigaki)**

To the extent possible under law, ISGK Instruments (Ryo Ishigaki)
has waived all copyright and related or neighboring rights
to Digital Synth PRA32-U v0.1.1.

You should have received a copy of the CC0 legalcode along with this
work.  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
