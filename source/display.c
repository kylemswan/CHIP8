#include "display.h"

int display_init(struct display *D) {
	D->window = SDL_CreateWindow(
		"CHIP8",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		320,
		0
	);
	D->renderer = SDL_CreateRenderer(D->window, -1, 0);

	if (!D->window || !D->renderer) {
		return -1;
	}

	return 0;
}

bool display_should_close(struct display *D) {
	while (SDL_PollEvent(&D->event)) {
		// check for window 'X' button clicks
		if (D->event.type == SDL_QUIT) {
			return true;
		}
		// check for 'ESCAPE' key presses
		if (D->event.key.keysym.sym == SDLK_ESCAPE) {
			return true;
		}
	}
	return false;
}

void display_draw(struct display *D, uint8_t *pixels) {
	// clear the display to black
	SDL_SetRenderDrawColor(D->renderer, 0, 0, 0, 1);
	SDL_RenderClear(D->renderer);

	// draw each of the CHIP8's active pixels as a 10 x 10 white rectangle
	SDL_SetRenderDrawColor(D->renderer, 255, 255, 255, 1);
	for (int i = 0; i < 32 * 64; i++) {
		if (pixels[i]) {
			SDL_Rect pixel = {
				.x = (i % 64) * 10,
				.y = (i / 64) * 10,
				.w = 10,
				.h = 10
			};
			SDL_RenderFillRect(D->renderer, &pixel);
		}
	}

	SDL_RenderPresent(D->renderer);
}

void display_destroy(struct display *D) {
	SDL_DestroyWindow(D->window);
	SDL_DestroyRenderer(D->renderer);
}
