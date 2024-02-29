#include "funcs.c"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: input with the ROM file to open\n");
    return 1;
  }

  config_t config;
  init_config(&config);

  sdl_t sdl;
  if (!init_sdl(&sdl, config)) {
    printf("Failed to initialise SDL\n");
    return 1;
  }

  chip8_t chip8;
  if (!init_chip(&chip8, argv[1])) {
    printf("Failed to initialise CHIP8\n");
    return 1;
  }
  clear_screen(&chip8);

  while (chip8.state == RUNNING) {
    for (int i = 0; i < 700 / 60; i++) {
      emu_cycle(&chip8);
      handle_user_input(&sdl, &chip8);
    }
    SDL_Delay(16);
    if (chip8.delay_timer > 0) {
      chip8.delay_timer -= 1;
    }
    if (chip8.sound_timer > 0) {
      chip8.sound_timer -= 1;
    }
    update_window(&sdl, chip8);
  }

  sdl_close(&sdl);
  return 0;
}
