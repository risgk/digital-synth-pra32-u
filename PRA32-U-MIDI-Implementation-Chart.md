```
  [Polyphonic/Paraphonic Synthesizer]                             Date: 2023-08-26                     
  Model: Digital Synth PRA32-U    MIDI Implementation Chart       Version: 0.4.0                       
+-------------------------------+---------------+---------------+-------------------------------------+
| Function...                   | Transmitted   | Recognized    | Remarks                             |
+-------------------------------+---------------+---------------+-------------------------------------+
| Basic        Default          | x             | 1             |                                     |
| Channel      Changed          | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Mode         Default          | x             | Mode 3        |                                     |
|              Messages         | x             | x             |                                     |
|              Altered          | ************* |               |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Note                          | x             | 0-127         |                                     |
| Number       : True Voice     | ************* | 0-127         |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Velocity     Note ON          | x             | x             |                                     |
|              Note OFF         | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| After        Key's            | x             | x             |                                     |
| Touch        Ch's             | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Pitch Bend                    | x             | o             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Control                     1 | x             | o             | Modulation                          |
| Change                     64 | x             | o             | Sustain Pedal [Off|On]              |
|                               |               |               |                                     |
|                            24 | x             | o             | Osc 1 Wave [Saw|-|Sin|Tri|Pls|Sqr]  |
|                           102 | x             | o             | Osc 1 Shape $$                      |
|                           103 | x             | o             | Osc 1 Morph $$                      |
|                            26 | x             | o             | Mixer Noise/Sub Osc [N|S] $$$       |
|                               |               |               |                                     |
|                            55 | x             | o             | Osc 2 Wave [Saw|O1|Sin|Tri|Nos|Sqr] |
|                            20 | x             | o             | Osc 2 Coarse [-|+]                  |
|                            21 | x             | o             | Osc 2 Pitch [-|+]                   |
|                            25 | x             | o             | Mixer Osc Mix [1|2]                 |
|                               |               |               |                                     |
|                            16 | x             | o             | Filter Cutoff                       |
|                            17 | x             | o             | Filter Resonance                    |
|                            18 | x             | o             | Filter EG Amt [-|+]                 |
|                            86 | x             | o             | Filter Key Track [0.0|0.5|1.0] $    |
|                               |               |               |                                     |
|                            23 | x             | o             | EG Attack                           |
|                            19 | x             | o             | EG Decay                            |
|                            27 | x             | o             | EG Sustain                          |
|                            28 | x             | o             | EG Release                          |
|                               |               |               |                                     |
|                           104 | x             | o             | EG Osc Amt [-|+]                    |
|                           105 | x             | o             | EG Osc Dst [P|2P|1S]                |
|                            87 | x             | o             | Voice Mode [Pol|Par|-|M|LP|Lgt]     |
|                            22 | x             | o             | Portamento                          |
|                               |               |               |                                     |
|                            14 | x             | o             | LFO Wave [T1|T2|-|Saw|SH|Sqr]       |
|                            80 | x             | o             | LFO Rate                            |
|                            81 | x             | o             | LFO Depth                           |
|                            15 | x             | o             | LFO Fade Time                       |
|                               |               |               |                                     |
|                            82 | x             | o             | LFO Osc Amt [-|+]                   |
|                             9 | x             | o             | LFO Osc Dst [P|2P|1S]               |
|                            83 | x             | o             | LFO Filter Amt [-|+]                |
|                           110 | x             | o             | Amp Level                           |
|                               |               |               |                                     |
|                            56 | x             | o             | Amp Attack                          |
|                            57 | x             | o             | Amp Decay                           |
|                            58 | x             | o             | Amp Sustain                         |
|                            59 | x             | o             | Amp Release                         |
|                               |               |               |                                     |
|                            63 | x             | o             | Chorus Mode [Off|M|-|PS|S|S2]       |
|                            61 | x             | o             | Chorus Rate                         |
|                            60 | x             | o             | Chorus Depth                        |
|                            62 | x             | o             | Chorus Delay Time                   |
|                               |               |               |                                     |
|                            85 | x             | o             | Pitch Bend Range                    |
|                           111 | x             | o             | Chorus Bypass [Off|On]              |
|                            31 | x             | o             | EG Amp Mod [Off|On]                 |
|                            89 | x             | o             | Release = Decay [Off|On]            |
|                               |               |               |                                     |
|                           109 | x             | o             | Filter Mode [LP|HP]                 |
|                               |               |               |                                     |
|                            35 | x             | o             | Pitch Bend by CC [-|+]              |
|                       112-119 |               |               | Program Change #8-15 by CC          |
|                            90 | x             | x             | [Reserved]                          |
+-------------------------------+---------------+---------------+-------------------------------------+
| Program                       | x             | o             |                                     |
| Change       : True #         | ************* | 0-15          |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| System Exclusive              | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| System       : Song Pos       | x             | x             |                                     |
| Common       : Song Sel       | x             | x             |                                     |
|              : Tune           | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| System       : Clock          | x             | x             |                                     |
| Real Time    : Commands       | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Aux          : All Sound OFF  | x             | o 120         |                                     |
| Messages     : Reset All      | x             | o 121         |                                     |
|                Controllers    |               |               |                                     |
|              : Local ON/OFF   | x             | x             |                                     |
|              : All Notes OFF  | x             | o 123-127     |                                     |
|              : Active Sense   | x             | x             |                                     |
|              : Reset          | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Notes                         | $ : Disabled in Paraphonic Mode                                     |
|                               | $$ : Disabled if Osc 1 Wave is not Pls (Pulse)                      |
|                               | $$$ : Noise is disabled if Osc 2 Wave is Nos (Noise) and            |
|                               |       Noise level is halved in Polyphonic/Paraphonic Mode           |
+-------------------------------+---------------------------------------------------------------------+
  Mode 1: Omni On,  Poly          Mode 2: Omni On,  Mono          o: Yes                               
  Mode 3: Omni Off, Poly          Mode 4: Omni Off, Mono          x: No                                
```
