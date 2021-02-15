#include "CHIP8.h"
#include "display.h"
#include <SDL2/SDL.h>
#include <stdio.h>

// frequency at which timers decrement and the screen is redrawn - default 60Hz
#define IO_DELAY (1000 / 60.0)

// frequency at which CPU instructions are executed - default 600Hz
#define OP_DELAY (1000 / 600.0)

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: ./CHIP8 <ROM>\n");
		return -1;
	}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		fprintf(stderr, "Failed to initialise SDL: %s\n", SDL_GetError());
		return -1;
	}

	struct CHIP8 C8;
	if (CHIP8_init(&C8, argv[1]) != 0) {
		fprintf(stderr, "Failed to initialise CHIP8\n");
		return -1;
	}

	struct display D;
	if (display_init(&D) != 0) {
		fprintf(stderr, "Failed to initialise display: %s\n", SDL_GetError());
		return -1;
	}

	int timer = SDL_GetTicks();
	while (!display_should_close(&D)) {
		CHIP8_handle_input(&C8);
		CHIP8_exec(&C8);

		if (SDL_GetTicks() - timer >= IO_DELAY) {
			CHIP8_dec_timers(&C8);
			display_draw(&D, C8.GFX);
			timer = SDL_GetTicks();
		}

		SDL_Delay(OP_DELAY);
	}

	display_destroy(&D);
	SDL_Quit();

	return 0;
}
