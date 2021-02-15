#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>

struct display {
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;
};

int display_init(struct display *D);
bool display_should_close(struct display *D);
void display_draw(struct display *D, uint8_t *pixels);
void display_destroy(struct display *D);

#endif // "display.h" included
