#include "game.hpp"

int main() {
  game::Config config{
      .win_height = 450,
      .win_width = 800,
      .win_title = "Game",
      .fps = 60,
      .background = BLACK,
      .particle_radius = 2,
      .particles_per_group = 300,
      .particle_velocity_factor = 0.01,
      .particle_action_distance = 75.0,
  };

  InitWindow(config.win_width, config.win_height, config.win_title);
  SetTargetFPS(config.fps);

  game::ParticleGroups particle_groups;
  game::particle_group_new(particle_groups[0], config, RED);
  game::particle_group_new(particle_groups[1], config, YELLOW);
  game::particle_group_new(particle_groups[2], config, BLUE);

  const game::InteractivityMatrix matrix{
      /* red   yellow   blue */
      {0.02, -0.5, -0.02}, /* red */
      {0.02, -0.04, -0.7}, /* yellow */
      {0.02, 0.02, 0.02},  /* blue */
  };

  while (!WindowShouldClose()) {
    BeginDrawing();
    {
      ClearBackground(config.background);
      game::particle_groups_apply_interactivity(particle_groups, matrix,
                                                config);

      game::particle_groups_render(particle_groups, config);
    }
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
