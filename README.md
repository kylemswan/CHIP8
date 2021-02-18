# CHIP8

The CHIP8 is a virtual machine which was originally designed to run on 8-bit computers such as the *COSMAC VIP* and the *TELMAC 1800* in the 1970s. CHIP8 offers a basic interface for programming small games like *Pong* and *Breakout*. Since the 1970s, the CHIP8 has been implemented on many different personal computer architechtures and even some embedded systems like graphing calculators. The CHIP8 is often recommended as the ideal starting point for beginners in the virtualisation and emulations fields.

The CHIP8 has just 35 instructions - each represented by a 2 byte 'opcode' which is decoded using various bitmasks. The (virtualised) architecture is very simple:

- 4kB of memory.
- 64 x 32 monochrome pixel graphics.
- 15 x 8-bit general purpose registers plus a special flag register (`V[0]` - `V[0xF]`).
- An index register, program counter, and a stack pointer.
- A stack allowing for up to 16 levels of nested procedures.
- A delay timer and a sound timer.

See the Resources section for more information about the CHIP8.

## Examples

Playing `BREAKOUT` on the CHIP8:

![](https://raw.githubusercontent.com/kylemswan/CHIP8/main/assets/BREAKOUT.png)


Classic 2 player `PONG` clone:

![](https://raw.githubusercontent.com/kylemswan/CHIP8/main/assets/PONG.png)


## Dependencies

This emulator uses *[SDL2](https://www.libsdl.org/download-2.0.php)* to handle all of the drawing, timing, and input events - ensure that you have downloaded the development header files as well as the runtime binaries before compilation.

## Building and Running

The emulator can be built automatically using *GNU make*:
```
make
```

Then run the emulator by providing the path to the desired ROM file:
```
./CHIP8 <ROM>
```

The `makefile` also contains some helpful targets to clean up the source directory:
```
make clean	// remove build artefacts
make remove	// remove the CHIP8 executable
```

## Controls

The CHIP8's hex input pad is mapped to a block of 16 keys on a standard keyboard:

|   |   |   |   |
|---|---|---|---|
| 1 | 2 | 3 | 4 |
| Q | W | E | R |
| A | S | D | F |
| Z | X | C | V |

Each ROM uses slightly different keys for controls, but you can easily work them out with some 'button mashing'.

Press `ESC` to quit the emulator at any point.
Press `o` to reload / restart.
Press `p` to pause.

## ROMs

I have provided some ROM files for testing and playing with the emulator, all of which are in the public domain and can be freely downloaded online. 

## Some Implementation Notes

Since the CHIP8 virtual machine is designed to be implemented on many different systems, the timing has to be very flexible. The consensus online is that the delay and sound timers should be decremented at 60Hz and the CPU instructions should be run anywhere between 500 - 1000Hz. Of course, on a real system each operation would take a different amount of time, but a static timing delay suffices for a system as simple as the CHIP8. From my testing, I found 600Hz to be about the right speed for most ROMs, but you can change these timings by altering the `#define` directives at the top of the `main.c` file.

The CHIP8 should emit a beeping / buzzing sound whenever the sound timer is non-zero. Whilst I have implemented the sound timer (`ST`) that decrements as required, I haven't yet added the sound effect, mainly because playing audio with *SDL* requires a separate link library and is quite involved - I plan on adding it sometime it in the future.

## Future Improvements

- [x] Implement ability to pause and reset the CHIP8.
- [ ] Build an assembler and disassembler for writing new ROMs.
- [ ] Add audio - there should be a beep / buzz every second when the sound timer is non-zero.
- [ ] Add support for the SUPER CHIP8 system, which has a larger display and a few more instructions.

## Resources

- [The CHIP8 Wikipedia page.](https://en.wikipedia.org/wiki/CHIP-8)
- [COWGOD'S CHIP8 Reference.](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Dxyn)
- [The Reddit r/emudev community.](http://reddit.com/r/emudev)
