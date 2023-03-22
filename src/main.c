#include <stdio.h>
#include "display.h"
#include "vector.h"

#define points_in_array (9 * 9 * 9)
struct vec3 cube_points[points_in_array];
struct vec2 projected_points[points_in_array];

struct vec3 camera_position = {0, 0, -5};
struct vec3 cube_rotation = {0, 0, 0};

float fov_factor = 640;

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

	int point_count = 0;
	for (float x = -1; x <= 1; x += 0.25)
	{
		for (float y = -1; y <= 1; y += 0.25)
		{
			for (float z = -1; z <= 1; z += 0.25)
			{
				struct vec3 new_point =
				{
					.x = x, .y = y, .z = z
				};
				cube_points[point_count++] = new_point;
			}
			
		}
		
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

struct vec2 projection(struct vec3 point){
	struct vec2 projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};
	return projected_point;
}

void update(void){
	cube_rotation.y += 0.1;
	
	for (int i = 0; i < points_in_array; i++)
	{
		struct vec3 point = cube_points[i];

		struct vec3 transformed_point = rotate_y(point, cube_rotation.y);

		transformed_point.z -= camera_position.z;
		struct vec2 projected_point = projection(transformed_point);
		projected_points[i] = projected_point;
		
	}
	
}

void render(void){
	draw_grid(grid_multiple_of);

	for (int i = 0; i < points_in_array; i++)
	{
		struct vec2 projected_point = projected_points[i];
		draw_rect(projected_point.x + (window_width/2), projected_point.y + (window_height/2), 4, 4, 0xFFA8DADC);
	}
	

	render_color_buffer();
	clear_color_buffer(0xFF283053);

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
