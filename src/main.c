#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_it_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;
int grid_multiple_of = 10;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	//Create a SDL Window
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	//Create a SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}

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

void draw_grid(int multiple_of) {
	//Vertical white lines
	for(int index = 0; index < (window_width * window_height) && index < (window_width * window_height); index += window_width){
		for(int currentIndex = index; currentIndex < index + window_width && currentIndex < (window_width * window_height); currentIndex += multiple_of){
			color_buffer[currentIndex] = 0xFFFFFFFC;
		}
	}

	//Horizontal white lines
	for(int index = 0; index < (window_width * window_height); index += (multiple_of * window_width)){
		for(int currentIndex = index; currentIndex < index + window_width && currentIndex < (window_width * window_height); currentIndex++){
			color_buffer[currentIndex] = 0xFFFFFFFC;
		}
	}
}

void draw_rect(int top_left_x, int top_left_y, int width, int height, uint32_t color){
	int index = (top_left_y * window_width) + top_left_x;
	for(int timesLooped = 0; timesLooped < height; timesLooped++){
		for(int currentIndex = index; currentIndex < index + width; currentIndex++){
			color_buffer[currentIndex] = color;
		}
		index += window_width;
	}
}

void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)(window_width) * sizeof(uint32_t)
	);
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
	for (int y = 0; y < window_height; y++) {
		for (int x = 0; x < window_width; x++) {
			color_buffer[(window_width * y) + x] = color;
		}
	}
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

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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
