#include <iostream>
#include <vector>
#include <array>
#include <cmath>
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
		double particle_velocity_factor;
		double particle_action_distance;
	};

	struct Particle {
		Color color;
		Vector2 position;
		double velocity_x;
		double velocity_y;
	};

	struct ParticleDistance {
		double x_delta;
		double y_delta;
		double vector;
	};

	using ParticleGroup = std::vector<Particle>;
	/** start off with two groups only */
	using ParticleGroups = std::array<ParticleGroup, 2>;
	
	Particle particle_new(const Config& config, Color color);
	void particle_render(const Particle& particle, const Config& config);
	ParticleDistance particle_distance(const Particle& one, const Particle& two);
	void particle_group_new(std::vector<Particle> &particles, const Config& config, Color color);
	void particle_groups_update(ParticleGroup &g_one, ParticleGroup &g_two, float gravity, const Config& config);
	void particle_groups_render(ParticleGroups& groups, const Config& config);
	Vector2 get_random_position(uint height, uint width, uint radius);
};

game::Particle game::particle_new(const Config& config, Color color) {
	Particle p{
		.color = color,
		.position = game::get_random_position(config.win_height, config.win_width, config.particle_radius),	
		.velocity_x = 0.0,
		.velocity_y = 0.0,
	};
	return p;
}

void game::particle_render(const game::Particle &particle, const game::Config& config) {
	DrawCircle(particle.position.x, particle.position.y, config.particle_radius, particle.color);
}

game::ParticleDistance game::particle_distance(const game::Particle& one, const game::Particle& two) {
	double dx = one.position.x - two.position.x;
	double dy = one.position.y - two.position.y;

	/** calculate distance using pythagoras theorum */
	double vector_d = std::sqrt(dx*dx + dy*dy);
	game::ParticleDistance result = {
		.x_delta = dx,
		.y_delta = dy,
		.vector = vector_d,
	};
	
	return result;
}

void game::particle_group_new(std::vector<Particle> &particles, const Config& config, Color color) {	 
	particles.reserve(config.particles_per_group);
	for (uint n = 0; n < config.particles_per_group; n++) {
		game::Particle particle = game::particle_new(config, color);		
		particles.push_back(particle);
	}
}

void game::particle_groups_update(game::ParticleGroup &g_one, game::ParticleGroup &g_two, float gravity, const game::Config& config) {
	uint max = g_one.size();

	for (uint i = 0; i < max; i++) {
		float fx = 0.0f;
		float fy = 0.0f;

		for (uint j = 0; j < max; j++) {
			game::Particle* a = &g_one[i];
			game::Particle* b = &g_two[j];

			game::ParticleDistance distance = game::particle_distance(*a, *b);
			if (distance.vector > 0 && distance.vector < config.particle_action_distance) {
				double force = gravity / distance.vector;
				fx += force * distance.x_delta;
				fy += force * distance.y_delta;
			}

			a->velocity_x = (a->velocity_x + fx) * config.particle_velocity_factor;			
			a->velocity_y = (a->velocity_y + fy) * config.particle_velocity_factor;

			a->position.x += a->velocity_x;
			a->position.y += a->velocity_y;

			/** flip direction when particle going off screen */
			if (a->position.x < 0 || a->position.x >= config.win_width) {
				a->velocity_x *= -1.0;
			}

			if (a->position.y < 0 || a->position.y >= config.win_height) {
				a->velocity_y *= -1.0;
			}
		}
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
		.win_height = 600,
		.win_width = 1000,
		.win_title = "Game",
		.fps = 60,
		.background = BLACK,
		.particle_radius = 2,
		.particles_per_group = 200,
		.particle_velocity_factor = 0.07,
		.particle_action_distance = 75.0,
	};

	InitWindow(config.win_width, config.win_height, config.win_title);
	SetTargetFPS(config.fps);

	game::ParticleGroups particle_groups;
	game::particle_group_new(particle_groups[0], config, YELLOW);	
	game::particle_group_new(particle_groups[1], config, RED);

	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(config.background);

			game::particle_groups_update(particle_groups[0], particle_groups[0], 0.02, config);
			game::particle_groups_update(particle_groups[0], particle_groups[1], -0.3, config);
			game::particle_groups_update(particle_groups[1], particle_groups[1], -0.02, config);

			game::particle_groups_render(particle_groups, config);
		}

		EndDrawing();		
	}	

	CloseWindow();
	return 0;
}
