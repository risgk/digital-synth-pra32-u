```
  [Polyphonic/Paraphonic Synthesizer]                             Date: 2024-09-01                     
  Model: Digital Synth PRA32-U    MIDI Implementation Chart       Version: 2.4.1                       
+-------------------------------+---------------+---------------+-------------------------------------+
| Function...                   | Transmitted   | Recognized    | Remarks                             |
+-------------------------------+---------------+---------------+-------------------------------------+
| Basic        Default          | x             | 1             |                                     |
| Channel      Changed          | x             | x             | $5                                  |
+-------------------------------+---------------+---------------+-------------------------------------+
| Mode         Default          | x             | Mode 3        |                                     |
|              Messages         | x             | x             |                                     |
|              Altered          | ************* |               |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Note                          | x             | 0-127         |                                     |
| Number       : True Voice     | ************* | 0-127         |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Velocity     Note ON          | x             | o             |                                     |
|              Note OFF         | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| After        Key's            | x             | x             |                                     |
| Touch        Ch's             | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Pitch Bend                    | x             | o             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Control                     1 | x             | o             | Modulation                          |
| Change                      2 | x             | o             | Breath Controller                   |
|                            64 | x             | o             | Sustain Pedal [Off|On]              |
|                               |               |               |                                     |
|                           102 | x             | o             | Osc 1 Wave [Saw|Sin|-|Tri|-|Pls]    |
|                            19 | x             | o             | Osc 1 Shape $2                      |
|                            20 | x             | o             | Osc 1 Morph $2                      |
|                            23 | x             | o             | Mixer Noise/Sub Osc [N|S] $3        |
|                               |               |               |                                     |
|                           104 | x             | o             | Osc 2 Wave [Saw|Sin|-|Tri|Nos|Sqr]  |
|                            85 | x             | o             | Osc 2 Coarse [-|+]                  |
|                            76 | x             | o             | Osc 2 Pitch [-|+]                   |
|                            21 | x             | o             | Mixer Osc Mix [1|2]                 |
|                               |               |               |                                     |
|                            74 | x             | o             | Filter Cutoff                       |
|                            71 | x             | o             | Filter Resonance                    |
|                            24 | x             | o             | Filter EG Amt [-|+]                 |
|                            26 | x             | o             | Filter Key Track [0.0|0.5|1.0] $1   |
|                               |               |               |                                     |
|                            73 | x             | o             | EG Attack                           |
|                            75 | x             | o             | EG Decay                            |
|                            30 | x             | o             | EG Sustain                          |
|                            72 | x             | o             | EG Release                          |
|                               |               |               |                                     |
|                            91 | x             | o             | EG Osc Amt [-|+]                    |
|                            89 | x             | o             | EG Osc Dst [P|2P|1S]                |
|                            14 | x             | o             | Voice Mode [Pol|Par|-|Mon|LP|Lgt]   |
|                             5 | x             | o             | Portamento                          |
|                               |               |               |                                     |
|                            12 | x             | o             | LFO Wave [Tri|Sin|-|Saw|SH|Sqr]     |
|                             3 | x             | o             | LFO Rate                            |
|                            17 | x             | o             | LFO Depth                           |
|                            56 | x             | o             | LFO Fade Time                       |
|                               |               |               |                                     |
|                            13 | x             | o             | LFO Osc Amt [-|+]                   |
|                           103 | x             | o             | LFO Osc Dst [P|2P|1S]               |
|                            25 | x             | o             | LFO Filter Amt [-|+]                |
|                            15 | x             | o             | Amp Gain                            |
|                               |               |               |                                     |
|                            52 | x             | o             | Amp Attack                          |
|                            53 | x             | o             | Amp Decay                           |
|                            54 | x             | o             | Amp Sustain                         |
|                            55 | x             | o             | Amp Release                         |
|                               |               |               |                                     |
|                            78 | x             | o             | Filter Mode [LP|HP]                 |
|                            28 | x             | o             | EG Amp Mod [Off|On]                 |
|                            29 | x             | o             | Release = Decay [Off|On]            |
|                            57 | x             | o             | Pitch Bend Range                    |
|                               |               |               |                                     |
|                            60 | x             | o             | Breath Filter Amt [-|+]             |
|                            61 | x             | o             | Breath Amp Mod [Off|Qad|Lin]        |
|                            62 | x             | o             | EG Velocity Sensitivity             |
|                            63 | x             | o             | Amp Velocity Sensitivity            |
|                               |               |               |                                     |
|                            27 | x             | o             | Chorus Mix [Dry|Wet]                |
|                            58 | x             | o             | Chorus Rate                         |
|                            59 | x             | o             | Chorus Depth                        |
|                               |               |               |                                     |
|                               |               |               |                                     |
|                            92 | x             | o             | Delay Feedback                      |
|                            90 | x             | o             | Delay Time                          |
|                            35 | x             | o             | Delay Mode [S|P]                    |
|                               |               |               |                                     |
|                               |               |               |                                     |
|                            87 | x             | o             | Program Number to Write to $4       |
|                           106 | x             | o             | Write Parameters to Program $4      |
|                           112 |               |               | Program Change #8 by CC             |
|                           113 |               |               | Program Change #9 by CC             |
|                           114 |               |               | Program Change #10 by CC            |
|                           115 |               |               | Program Change #11 by CC            |
|                           116 |               |               | Program Change #12 by CC            |
|                           117 |               |               | Program Change #13 by CC            |
|                           118 |               |               | Program Change #14 by CC            |
|                           119 |               |               | Program Change #15 by CC            |
|                           111 | x             | x             | [Reserved]                          |
+-------------------------------+---------------+---------------+-------------------------------------+
| Program                       | x             | o             |                                     |
| Change       : True #         | ************* | 0-15          | Default 8                           |
+-------------------------------+---------------+---------------+-------------------------------------+
| System Exclusive              | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| System       : Song Pos       | x             | x             |                                     |
| Common       : Song Sel       | x             | x             |                                     |
|              : Tune           | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| System       : Clock          | x             | x $6          |                                     |
| Real Time    : Commands       | x             | x $6          | Compatible only with Start/Stop     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Aux          : All Sound OFF  | x             | o 120         |                                     |
| Messages     : Reset All      | x             | o 121         |                                     |
|                Controllers    |               |               |                                     |
|              : Local ON/OFF   | x             | x             |                                     |
|              : All Notes OFF  | x             | o 123-127     |                                     |
|              : Active Sense   | x             | x             |                                     |
|              : Reset          | x             | x             |                                     |
+-------------------------------+---------------+---------------+-------------------------------------+
| Notes                         | $1 : Disabled in Paraphonic Mode                                    |
|                               | $2 : Disabled if Osc 1 Wave is not Pls (Pulse)                      |
|                               | $3 : Mixer Noise is disabled if Osc 2 Wave is Nos (Noise)           |
|                               | $4 : To write the current parameters to Program #8-15 and the       |
|                               |   flash, set "Program Number to Write to" (# is the value mod 16)   |
|                               |   and then change "Write Parameters to Program" from 0 to 1-127     |
|                               | $5 : Basic Channel can be changed in PRA32-U with Panel             |
|                               | $6 : o in PRA32-U with Panel if Seq Clock Src is External           |
+-------------------------------+---------------------------------------------------------------------+
  Mode 1: Omni On,  Poly          Mode 2: Omni On,  Mono          o: Yes                               
  Mode 3: Omni Off, Poly          Mode 4: Omni Off, Mono          x: No                                
```
