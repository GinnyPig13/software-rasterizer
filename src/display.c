#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;
int grid_multiple_of = 20;

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

void draw_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < window_width && y >= 0 && y < window_height)
    {
        color_buffer[(window_width * y) + x] = color;
    }  
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	float x_increment = delta_x / (float)side_length;
	float y_increment = delta_y / (float)side_length;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= side_length; i++)
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_increment;
		current_y += y_increment;
	}
	
}

//TODO: make draw rect work with draw pixel like he did
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

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}