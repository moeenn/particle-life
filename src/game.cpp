#include "game.hpp"

game::Particle game::particle_new(const Config& config, Color color) {
  Particle p{
      .color = color,
      .position = game::get_random_position(config.win_height, config.win_width,
                                            config.particle_radius),
      .velocity_x = 0.0,
      .velocity_y = 0.0,
  };
  return p;
}

void game::particle_render(const game::Particle& particle,
                           const game::Config& config) {
  DrawCircle(particle.position.x, particle.position.y, config.particle_radius,
             particle.color);
}

game::ParticleDistance game::particle_distance(const game::Particle& one,
                                               const game::Particle& two) {
  double dx = one.position.x - two.position.x;
  double dy = one.position.y - two.position.y;

  /** calculate distance using pythagoras theorum */
  double vector_d = std::sqrt(dx * dx + dy * dy);
  game::ParticleDistance result = {
      .x_delta = dx,
      .y_delta = dy,
      .vector = vector_d,
  };

  return result;
}

void game::particle_group_new(std::vector<Particle>& particles,
                              const Config& config,
                              Color color) {
  particles.reserve(config.particles_per_group);
  for (uint n = 0; n < config.particles_per_group; n++) {
    game::Particle particle = game::particle_new(config, color);
    particles.push_back(particle);
  }
}

void game::particle_groups_update(game::ParticleGroup& g_one,
                                  game::ParticleGroup& g_two,
                                  float gravity,
                                  const game::Config& config) {
  uint max = g_one.size();

  for (uint i = 0; i < max; i++) {
    float fx = 0.0f;
    float fy = 0.0f;

    for (uint j = 0; j < max; j++) {
      game::Particle* a = &g_one[i];
      game::Particle* b = &g_two[j];

      game::ParticleDistance distance = game::particle_distance(*a, *b);
      if (distance.vector > 0 &&
          distance.vector < config.particle_action_distance) {
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

void game::particle_groups_render(game::ParticleGroups& groups,
                                  const game::Config& config) {
  for (std::vector<game::Particle> group : groups) {
    for (const game::Particle particle : group) {
      game::particle_render(particle, config);
    }
  }
}

void game::particle_groups_apply_interactivity(
    game::ParticleGroups& groups,
    const game::InteractivityMatrix& interactivity,
    const Config& config) {
  for (uint i = 0; i < game::MAX_GROUPS; i++) {
    for (uint j = 0; j < game::MAX_GROUPS; j++) {
      game::particle_groups_update(groups[i], groups[j], interactivity[i][j],
                                   config);
    }
  }
}

Vector2 game::get_random_position(uint height, uint width, uint radius) {
  float x = GetRandomValue(radius, width - radius);
  float y = GetRandomValue(radius, height - radius);
  return Vector2{x, y};
}
