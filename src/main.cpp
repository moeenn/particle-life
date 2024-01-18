#include <iostream>
#include <vector>
#include "raylib.h"

namespace game {
	struct Config {
		uint win_height;
		uint win_width;
		const char* win_title;
		uint fps;
		Color background;
		uint particle_radius;
		uint particles_per_group;
	};

	struct Particle {
		Color color;
		Vector2 position;
	};

	using ParticleGroups = std::vector<std::vector<Particle>>;
	
	Particle particle_new(const Config& config, Color color);
	void particle_render(const Particle& particle, const Config& config);
	void particle_group_new(std::vector<Particle> &particles, const Config& config, Color color);
	void particle_groups_render(ParticleGroups& groups, const Config& config);
	Vector2 get_random_position(uint height, uint width, uint radius);
};

game::Particle game::particle_new(const Config& config, Color color) {
	Particle p{
		.color = color,
		.position = game::get_random_position(config.win_height, config.win_width, config.particle_radius),	
	};
	return p;
}

void game::particle_render(const game::Particle &particle, const game::Config& config) {
	DrawCircle(particle.position.x, particle.position.y, config.particle_radius, particle.color);
}

void game::particle_group_new(std::vector<Particle> &particles, const Config& config, Color color) {	 
	particles.reserve(config.particles_per_group);
	for (uint n = 0; n < config.particles_per_group; n++) {
		game::Particle particle = game::particle_new(config, color);		
		particles.push_back(particle);
	}
}

void game::particle_groups_render(game::ParticleGroups &groups, const game::Config& config) {	
	for (std::vector<game::Particle> group : groups) {
		for (const game::Particle particle : group) {
			game::particle_render(particle, config);
		}
	}
}

Vector2 game::get_random_position(uint height, uint width, uint radius) {
	float x = GetRandomValue(radius, width -radius);
	float y = GetRandomValue(radius, height-radius);
	return Vector2 {x, y};
}

int main() {
	game::Config config {
		.win_height = 450,
		.win_width = 800,
		.win_title = "Game",
		.fps = 60,
		.background = BLACK,
		.particle_radius = 2,
		.particles_per_group = 20,
	};

	InitWindow(config.win_width, config.win_height, config.win_title);
	SetTargetFPS(config.fps);

	std::vector<game::Particle> g_one;
	game::particle_group_new(g_one, config, BLUE);	

	std::vector<game::Particle> g_two;
	game::particle_group_new(g_two, config, RED);

	game::ParticleGroups particle_groups;
	particle_groups.push_back(g_one);
	particle_groups.push_back(g_two);

	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(config.background);
			game::particle_groups_render(particle_groups, config);
		}

		EndDrawing();		
	}	

	CloseWindow();
	return 0;
}
