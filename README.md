# gb17
C++ learning exercise and Gameboy Emulator

[![Build Status](https://travis-ci.org/brendonlecomte/gb17.svg?branch=master)](https://travis-ci.org/brendonlecomte/gb17)

Wins! 
- Tetris Running. Not perfectly.
- CPU Instruction Tests passing.
- Some unittest coverage

MBC Known Issues:
- No MBC3 + BATT option (pkmn red)
- Pkmn Red shows Reds hat then panics the screen

PPU Known Issues:
- X flip and Y flip for sprites not implemented
- Colours and Palette selection incorrect
- Window not implemented
- LY Coincidence interrupt not implemented
- Dr Mario weird tile issues
- Get BG / Win / Sprite Tile data by index is incorrectly done

GB Known Issues:
- Performance. Runs at normal speed but no double speed.
- holding all the buttons makes tetris panic weirdly?

Audio Known Issues:
- No APU.


The New plan...
 - SDL + imgui application for running the emulator. Should be basically the same as RetroArch.
 - First step is to compile with SDL2 so I can play tetris on it. 
 - Long term still use RetroArch for deployment. But SDL gives me 
 debugging tools to figure out my issues.
 - Add Debugging windows via Imgui. Registers. Instruction Trace. etc.
