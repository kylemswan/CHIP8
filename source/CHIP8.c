#include "CHIP8.h"

// CHIP8 hex font set - the binary representations of each 5-byte sprite forms
// the shape of the desired character
uint8_t CHIP8_font[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// the CHIP8's hex input mapped to keyboard scancodes
uint8_t CHIP8_key_map[16] = {
	SDL_SCANCODE_X,
	SDL_SCANCODE_1,
	SDL_SCANCODE_2,
	SDL_SCANCODE_3,
	SDL_SCANCODE_Q,
	SDL_SCANCODE_W,
	SDL_SCANCODE_E,
	SDL_SCANCODE_A,
	SDL_SCANCODE_S,
	SDL_SCANCODE_D,
	SDL_SCANCODE_Z,
	SDL_SCANCODE_C,
	SDL_SCANCODE_4,
	SDL_SCANCODE_R,
	SDL_SCANCODE_F,
	SDL_SCANCODE_V,
};

int CHIP8_init(struct CHIP8 *C8, const char *ROM) {
	// initialise memory and registers
	memset(C8->mem, 0, 4096);
	memset(C8->GFX, 0, 32 * 64);
	memset(C8->keys, 0, 16);
	memset(C8->V, 0, 16);
	memset(C8->stack, 0, 16 * 2);
	C8->DT = C8->ST = 0;
	C8->SP = -1;
	C8->I = 0;
	C8->PC = 0x200;

	// read font set into memory at [0x0]
	for (int i = 0; i < 80; i++) {
		C8->mem[i] = CHIP8_font[i];
	}

	// read provided ROM file into memory at [0x200]
	FILE *fp = fopen(ROM, "rb");
	if (!fp || !fread(C8->mem + 0x200, 1, 4096 - 0x200, fp)) {
		return -1;
	}
	fclose(fp);

	// seed random number generator
	srand(time(0));

	return 0;
}

void CHIP8_handle_input(struct CHIP8 *C8) {
	const uint8_t *state = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < 16; i++) {
		C8->keys[i] = state[CHIP8_key_map[i]];
	}
}

void CHIP8_exec(struct CHIP8 *C8) {
	// fetch next (big-endian) opcode and advance the program counter
	uint16_t op = (C8->mem[C8->PC] << 8) | C8->mem[C8->PC + 1];
	C8->PC += 2;

	// decode and execute the op!
	switch (op & 0xF000) {

	case 0x0000:
		switch (op & 0x000F) {

		case 0x0: // 00E0 - CLS
			memset(C8->GFX, 0, 32 * 64);
			break;

		case 0xE: // 00EE - RET
			C8->PC = C8->stack[C8->SP];
			C8->SP--;
			break;
		
		}
		break;

	case 0x1000: // 1NNN - JMP NNN
		C8->PC = NNN(op);
		break;

	case 0x2000: // 2NNN - CALL NNN
		C8->SP++;
		C8->stack[C8->SP] = C8->PC;
		C8->PC = NNN(op);
		break;

	case 0x3000: // 3XKK - SE VX, KK
		if (C8->V[X(op)] == KK(op)) {
			C8->PC += 2;
		}
		break;

	case 0x4000: // 4XKK - SNE VX, KK
		if (C8->V[X(op)] != KK(op)) {
			C8->PC += 2;
		}
		break;

	case 0x5000: // 5XY0 - SE VX, VY
		if (C8->V[X(op)] == C8->V[Y(op)]) {
			C8->PC += 2;
		}
		break;

	case 0x6000: // 6XKK - LD VX, KK
		C8->V[X(op)] = KK(op);
		break;

	case 0x7000: // 7XKK - ADD VX, KK
		C8->V[X(op)] += KK(op);
		break;

	case 0x8000:
		switch (op & 0x000F) {

		case 0x0: // 8XY0 - LD VX, VY
			C8->V[X(op)] = C8->V[Y(op)];
			break;

		case 0x1: // 8XY1 - OR VX, VY
			C8->V[X(op)] |= C8->V[Y(op)];
			break;

		case 0x2: // 8XY2 - AND VX, VY
			C8->V[X(op)] &= C8->V[Y(op)];
			break;

		case 0x3: // 8XY3 - XOR VX, VY
			C8->V[X(op)] ^= C8->V[Y(op)];
			break;

		case 0x4: // 8XY4 - ADD VX, VY
			C8->V[0xF] = C8->V[X(op)] + C8->V[Y(op)] > 255 ? 1 : 0;
			C8->V[X(op)] = C8->V[X(op)] + C8->V[Y(op)];
			break;

		case 0x5: // 8XY5 - SUB VX, VY
			C8->V[0xF] = C8->V[X(op)] > C8->V[Y(op)] ? 1 : 0;
			C8->V[X(op)] -= C8->V[Y(op)];
			break;

		case 0x6: // 8XY6 - SHR VX
			C8->V[0xF] = C8->V[X(op)] & 1;
			C8->V[X(op)] >>= 1;
			break;

		case 0x7: // 8XY7 - SUBN VX, VY
			C8->V[0xF] = C8->V[Y(op)] > C8->V[X(op)] ? 1 : 0;
			C8->V[X(op)] = C8->V[Y(op)] - C8->V[X(op)];
			break;

		case 0xE: // 8XYE - SHL VX
			C8->V[0xF] = C8->V[X(op)] >> 7;
			C8->V[X(op)] <<= 1;
			break;

		}
		break;

	case 0x9000: // 9XY0 - SNE VX, VY
		if (C8->V[X(op)] != C8->V[Y(op)]) {
			C8->PC += 2;
		}
		break;

	case 0xA000: // ANNN - LD I, NNN
		C8->I = NNN(op);
		break;

	case 0xB000: // BNNN - JMP V0, NNN
		C8->PC = C8->V[0] + NNN(op);
		break;

	case 0xC000: // CXKK - RND VX, KK
		C8->V[X(op)] = (rand() % 255) & KK(op);
		break;

	case 0xD000: // DXYN - DRW
		C8->V[0xF] = 0;
		for (int i = 0; i < N(op); i++) {
			uint8_t sprite = C8->mem[C8->I + i];
			for (int j = 0; j < 8; j++) {
				uint8_t bit = (sprite >> (7 - j)) & 1;
				uint8_t x = C8->V[X(op)] + j;
				uint8_t y = C8->V[Y(op)] + i;
				if (C8->GFX[x + (y * 64)] && bit) {
					C8->V[0xF] = 1;
				}
				C8->GFX[x + (y * 64)] ^= bit;
			}
		}
		break;

	case 0xE000:
		switch (op & 0x00FF) {
			
		case 0x9E: // EX9E - SKP VX
			if (C8->keys[C8->V[X(op)]] == 1) {
				C8->PC += 2;
			}
			break;

		case 0xA1: // EXA1 - SKNP VX
			if (C8->keys[C8->V[X(op)]] == 0) {
				C8->PC += 2;
			}
			break;

		}
		break;

	case 0xF000:
		switch (op & 0x00FF) {

		case 0x07: // FX07 - LD VX, DT
			C8->V[X(op)] = C8->DT;
			break;

		case 0x0A: // FX0A - LD VX, KEY
			C8->PC -= 2;
			for (int i = 0; i < 16; i++) {
				if (C8->keys[i]) {
					C8->V[X(op)] = i;
					C8->PC += 2;
					break;
				}
			}
			break;

		case 0x15: // FX15 - LD DT, VX
			C8->DT = C8->V[X(op)];
			break;

		case 0x18: // FX18 - LD ST, VX
			C8->ST = C8->V[X(op)];
			break;

		case 0x1E: // FX1E - ADD I, VX
			C8->I += C8->V[X(op)];
			break;

		case 0x29: // FX29 - LD F, VX
			C8->I = 5 * C8->V[X(op)];
			break;

		case 0x33: // LD BCD, VX
			C8->mem[C8->I] = C8->V[X(op)] / 100;
			C8->mem[C8->I + 1] = (C8->V[X(op)] / 10) % 10;
			C8->mem[C8->I + 2] = C8->V[X(op)] % 10;
			break;

		case 0x55: // FX55 - LD [I], VX
			for (int j = 0; j <= X(op); j++) {
				C8->mem[C8->I + j] = C8->V[j];
			}
			break;

		case 0x65: // FX65 - LD VX, [I]
			for (int j = 0; j <= X(op); j++) {
				C8->V[j] = C8->mem[C8->I + j];
			}
			break;

		}
		break;
	}

}

void CHIP8_dec_timers(struct CHIP8 *C8) {
	if (C8->DT) {
		C8->DT--;
	}
	if (C8->ST) {
		C8->ST--;
	}
}
