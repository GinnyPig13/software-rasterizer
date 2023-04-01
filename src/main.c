#include <stdio.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

struct triangle triangles_to_render[N_MESH_FACES];

struct vec3 camera_position = {0, 0, -5};
struct vec3 cube_rotation = {0, 0, 0};

float fov_factor = 640;

bool is_it_running = false;
int previous_frame_time = 0;

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

struct vec2 projection(struct vec3 point){
	struct vec2 projected_point = {
		.x = (fov_factor * point.x) / point.z,
		.y = (fov_factor * point.y) / point.z
	};
	return projected_point;
}

void update(void){
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(time_to_wait);
	}
	

	previous_frame_time = SDL_GetTicks();

	cube_rotation.y += 0.01f;
	cube_rotation.z += 0.01f;
	cube_rotation.x += 0.01f;
	
	for (int i = 0; i < N_MESH_FACES; i++) 
	{
		struct face mesh_face = mesh_faces[i];

		struct vec3 face_vertices[3];
		face_vertices[0] = mesh_vertices[mesh_face.a];
		face_vertices[1] = mesh_vertices[mesh_face.b];
		face_vertices[2] = mesh_vertices[mesh_face.c];

		struct triangle projected_triangle;

		for (int j = 0; j < 3; j++)
		{
			struct vec3 transformed_vertex = face_vertices[j];

			transformed_vertex = rotate_x(transformed_vertex, cube_rotation.x);
			transformed_vertex = rotate_y(transformed_vertex, cube_rotation.y);
			transformed_vertex = rotate_z(transformed_vertex, cube_rotation.z);

			transformed_vertex.z -= camera_position.z;
			
			struct vec2 projected_point = projection(transformed_vertex);

			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);
			
			projected_triangle.points[j] = projected_point;
		}
		
		triangles_to_render[i] = projected_triangle;
	}
}

void render(void){
	//draw_grid(grid_multiple_of);

	for (int i = 0; i < N_MESH_FACES; i++ )
	{
		struct triangle projected_triangle = triangles_to_render[i];
		draw_rect(projected_triangle.points[0].x, projected_triangle.points[0].y, 3, 3, 0xFFA8DADC);
		draw_rect(projected_triangle.points[1].x, projected_triangle.points[1].y, 3, 3, 0xFFA8DADC);
		draw_rect(projected_triangle.points[2].x, projected_triangle.points[2].y, 3, 3, 0xFFA8DADC);
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
