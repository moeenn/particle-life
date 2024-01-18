#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#include "raylib.h"

namespace game {
const uint MAX_GROUPS = 3;

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

using InteractivityMatrix = double[MAX_GROUPS][MAX_GROUPS];
using ParticleGroup = std::vector<Particle>;
using ParticleGroups = std::array<ParticleGroup, MAX_GROUPS>;

Particle particle_new(const Config& config, Color color);
void particle_render(const Particle& particle, const Config& config);
ParticleDistance particle_distance(const Particle& one, const Particle& two);
void particle_group_new(std::vector<Particle>& particles,
                        const Config& config,
                        Color color);

void particle_groups_update(ParticleGroup& g_one,
                            ParticleGroup& g_two,
                            float gravity,
                            const Config& config);

void particle_groups_render(ParticleGroups& groups, const Config& config);
void particle_groups_apply_interactivity(
    ParticleGroups& groups,
    const InteractivityMatrix& interactivity,
    const Config& config);

Vector2 get_random_position(uint height, uint width, uint radius);
};  // namespace game
