#include <stdio.h>
#include "display.h"


bool is_it_running = false;

void setup(void){
	color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	if (color_buffer == NULL) {
		fprintf(stderr, "Error creating color buffer.\n");
	}
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type){
		case SDL_QUIT:
			is_it_running = false;
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				is_it_running = false;
			break;

	}
}

void update(void){

}

void render(void){
	SDL_SetRenderDrawColor(renderer, 40, 48, 83, 1);
	SDL_RenderClear(renderer);

	draw_grid(grid_multiple_of);
	draw_rect(3000, 761, 55, 555, 0x004D194D);

	render_color_buffer();
	clear_color_buffer(0x00386641);

	SDL_RenderPresent(renderer);
}

int main(void) {

	is_it_running = initialize_window();
	setup();

	while(is_it_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
