#ifndef CHIP8_H
#define CHIP8_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// bit-masking macros for decoding each opcode
#define NNN(op) (op & 0x0FFF)
#define KK(op) (op & 0x00FF)
#define X(op) ((op & 0x0F00) >> 8)
#define Y(op) ((op & 0x00F0) >> 4)
#define N(op) (op & 0x000F)

struct CHIP8 {
	uint8_t mem[4096];
	uint8_t GFX[32 * 64];
	uint8_t keys[16];
	uint8_t V[16];
	uint8_t DT;
	uint8_t ST;
	uint16_t stack[16];
	uint16_t PC;
	uint16_t SP;
	uint16_t I;
};

uint8_t CHIP8_font[80];

int CHIP8_init(struct CHIP8 *C8, const char *ROM);
void CHIP8_handle_input(struct CHIP8 *C8);
void CHIP8_exec(struct CHIP8 *C8);
void CHIP8_dec_timers(struct CHIP8 *C8);

#endif // "CHIP8.h" inluded
