#include "defs.h"

// SDL functions
bool init_sdl(sdl_t *sdl, const config_t config) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("SDL could not be initialised. Error: %s\n", SDL_GetError());
    return false;
  }

  sdl->window = SDL_CreateWindow(
      "CHIP8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      config.screen_w * config.scale_factor,
      config.screen_h * config.scale_factor, SDL_WINDOW_SHOWN);

  if (sdl->window == NULL) {
    printf("Window could not be created. Error: %s\n", SDL_GetError());
    return false;
  }

  sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);

  if (sdl->renderer == NULL) {
    printf("Renderer could not be initialised. Error: %s\n", SDL_GetError());
    return false;
  }

  SDL_RenderSetScale(sdl->renderer, config.scale_factor, config.scale_factor);

  return true;
}

bool init_chip(chip8_t *chip8, const char rom_name[]) {
  const unsigned short entry_point = 0x200;
  unsigned char font[] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };

  // Initialise chip8 registers
  memset(chip8, 0, sizeof(chip8_t));

  chip8->pc = entry_point;

  FILE *rom = fopen(rom_name, "rb");

  if (rom == NULL) {
    printf("Could not open file/File does not exist.\n");
    return false;
  }

  // Calculate rom size and check if memory has enough space
  fseek(rom, 0, SEEK_END);
  unsigned long rom_size = ftell(rom);

  if (rom_size > RAM_SIZE - entry_point) {
    printf("ROM file too large\n");
    return false;
  }

  // Copy from ROM file to RAM
  fseek(rom, 0, SEEK_SET);
  if (fread(&chip8->memory[entry_point], 1, rom_size, rom) == 0) {
    printf("ROM could not be copied to memory\n");
    return false;
  }

  // Copy fonts into RAM
  memcpy(&chip8->memory[0], font, sizeof(font));

  fclose(rom);
  return true;
}

void sdl_close(sdl_t *sdl) {
  SDL_DestroyWindow(sdl->window);
  SDL_DestroyRenderer(sdl->renderer);
  SDL_Quit();
}
void init_config(config_t *config) {
  // Initialise defaults
  config->screen_h = SCREEN_HEIGHT;
  config->screen_w = SCREEN_WIDTH;

  config->scale_factor = 10;
}

void handle_user_input(sdl_t *sdl, chip8_t *chip8) {
  (void)sdl;
  SDL_Event event;

  while (SDL_PollEvent(&event) > 0) {
    switch (event.type) {
    case SDL_QUIT:
      chip8->state = QUIT;
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_SPACE:
        if (chip8->state == RUNNING) {
          chip8->state = PAUSED;
        } else if (chip8->state == PAUSED) {
          chip8->state = RUNNING;
        }
        break;

      case SDLK_ESCAPE:
        chip8->state = QUIT;
        break;

      case SDLK_1:
        chip8->key[0x1] = true;
        break;

      case SDLK_2:
        chip8->key[0x2] = true;
        break;

      case SDLK_3:
        chip8->key[0x3] = true;
        break;

      case SDLK_4:
        chip8->key[0xC] = true;
        break;

      case SDLK_q:
        chip8->key[0x4] = true;
        break;

      case SDLK_w:
        chip8->key[0x5] = true;
        break;

      case SDLK_e:
        chip8->key[0x6] = true;
        break;

      case SDLK_r:
        chip8->key[0xD] = true;
        break;

      case SDLK_a:
        chip8->key[0x7] = true;
        break;

      case SDLK_s:
        chip8->key[0x8] = true;
        break;

      case SDLK_d:
        chip8->key[0x9] = true;
        break;

      case SDLK_f:
        chip8->key[0xE] = true;
        break;

      case SDLK_z:
        chip8->key[0xA] = true;
        break;

      case SDLK_x:
        chip8->key[0x0] = true;
        break;

      case SDLK_c:
        chip8->key[0xB] = true;
        break;

      case SDLK_v:
        chip8->key[0xF] = true;
        break;
      }
      break;

    case SDL_KEYUP:
      switch (event.key.keysym.sym) {
      case SDLK_1:
        chip8->key[0x1] = false;
        break;

      case SDLK_2:
        chip8->key[0x2] = false;
        break;

      case SDLK_3:
        chip8->key[0x3] = false;
        break;

      case SDLK_4:
        chip8->key[0xC] = false;
        break;

      case SDLK_q:
        chip8->key[0x4] = false;
        break;

      case SDLK_w:
        chip8->key[0x5] = false;
        break;

      case SDLK_e:
        chip8->key[0x6] = false;
        break;

      case SDLK_r:
        chip8->key[0xD] = false;
        break;

      case SDLK_a:
        chip8->key[0x7] = false;
        break;

      case SDLK_s:
        chip8->key[0x8] = false;
        break;

      case SDLK_d:
        chip8->key[0x9] = false;
        break;

      case SDLK_f:
        chip8->key[0xE] = false;
        break;

      case SDLK_z:
        chip8->key[0xA] = false;
        break;

      case SDLK_x:
        chip8->key[0x0] = false;
        break;

      case SDLK_c:
        chip8->key[0xB] = false;
        break;

      case SDLK_v:
        chip8->key[0xF] = false;
        break;
      }
      break;

    default:
      break;
    }
  }
}

void clear_screen(chip8_t *chip8) {
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      chip8->display[y][x] = false;
    }
  }
}

void update_window(sdl_t *sdl, const chip8_t chip8) {
  for (int y = 0; y < SCREEN_HEIGHT; y++) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
      if (chip8.display[y][x] == true) {
        SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
      } else {
        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
      }
      SDL_RenderDrawPoint(sdl->renderer, x, y);
    }
  }

  SDL_RenderPresent(sdl->renderer);
}

void draw_sprite(uint16_t opcode, chip8_t *chip8) {
  int x_coord = chip8->V[((opcode & 0x0F00) >> 8)] % 64;
  int y_coord = chip8->V[((opcode & 0x00F0) >> 4)] % 32;
  int N = (opcode & 0x000F);
  chip8->V[0xF] = 0;

  for (int j = 0; j < N; j++) {
    uint8_t sprite_data = chip8->memory[chip8->I + j];
    for (int i = 0; i < 8; i++) {
      int x = x_coord + i;
      int y = y_coord + j;

      if (x > 63 || y > 31)
        continue;

      bool sprite_bit = (sprite_data >> (7 - i)) & 1;

      if (chip8->display[y][x] && sprite_bit) {
        chip8->V[0xF] = 1;
      }

      chip8->display[y][x] ^= sprite_bit;
    }
  }
}

void get_key(uint16_t opcode, chip8_t *chip8) {
  bool key_pressed = false;
  int X = (opcode & 0x0F00) >> 8;

  for (int i = 0; i < KEYS; i++) {
    if (chip8->key[i]) {
      key_pressed = true;
      chip8->V[X] = i;
      break;
    }
  }

  if (!key_pressed) {
    chip8->pc -= 2;
  }
}

void add(const uint16_t opcode, chip8_t *chip8) {
  int X = (opcode & 0x0F00) >> 8;
  int Y = (opcode & 0x00F0) >> 4;
  int temp = chip8->V[X] + chip8->V[Y];
  chip8->V[X] += chip8->V[Y];
  chip8->V[0xF] = temp > 255;
}

void subtract_x_y(const uint16_t opcode, chip8_t *chip8) {
  int X = (opcode & 0x0F00) >> 8;
  int Y = (opcode & 0x00F0) >> 4;
  int temp = chip8->V[X] - chip8->V[Y];
  chip8->V[X] = temp;
  chip8->V[0xF] = temp > 0;
}

void subtract_y_x(const uint16_t opcode, chip8_t *chip8) {
  int X = (opcode & 0x0F00) >> 8;
  int Y = (opcode & 0x00F0) >> 4;
  int temp = chip8->V[Y] - chip8->V[X];
  chip8->V[X] = temp;
  chip8->V[0xF] = temp > 0;
}

void emu_cycle(chip8_t *chip8) {
  chip8->opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
  chip8->pc += 2;

  int X = (chip8->opcode & 0x0F00) >> 8;
  int Y = (chip8->opcode & 0x00F0) >> 4;

  int N = chip8->opcode & 0x000F;
  int NN = chip8->opcode & 0x00FF;
  unsigned short NNN = chip8->opcode & 0x0FFF;

  // Specific opcodes
  switch (chip8->opcode) {
  case 0x00E0:
    clear_screen(chip8);
    break;

  case 0x00EE:
    chip8->sp -= 1;
    chip8->pc = chip8->stack[chip8->sp];
    break;

  default:
    break;
  }

  // Checking for first digit
  switch (chip8->opcode >> 12 | 0x0000) {
  case 1:
    chip8->pc = NNN;
    break;

  case 2:
    chip8->stack[chip8->sp] = chip8->pc;
    chip8->sp += 1;

    chip8->pc = NNN;
    break;

  case 3:
    if (chip8->V[X] == NN) {
      chip8->pc += 2;
    }
    break;

  case 4:
    if (chip8->V[X] != NN) {
      chip8->pc += 2;
    }
    break;

  case 5:
    if (N == 0) {
      if (chip8->V[X] == chip8->V[Y]) {
        chip8->pc += 2;
      }
    }
    break;

  case 6:
    chip8->V[X] = NN;
    break;

  case 7:
    chip8->V[X] += NN;
    break;

  case 8:
    switch (N) {
    case 0:
      chip8->V[X] = chip8->V[Y];
      break;

    case 1:
      chip8->V[X] |= chip8->V[Y];
      chip8->V[0xF] = 0;
      break;

    case 2:
      chip8->V[X] &= chip8->V[Y];
      chip8->V[0xF] = 0;
      break;

    case 3:
      chip8->V[X] ^= chip8->V[Y];
      chip8->V[0xF] = 0;
      break;

    case 4: {
      add(chip8->opcode, chip8);
      break;
    }

    case 5: {
      subtract_x_y(chip8->opcode, chip8);
      break;
    }

    case 6: {
      chip8->V[X] = chip8->V[Y];
      int temp = chip8->V[X] & 1;
      chip8->V[X] >>= 1;
      chip8->V[0xF] = temp;
      break;
    }

    case 7: {
      subtract_y_x(chip8->opcode, chip8);
      break;
    }

    case 0xE: {
      chip8->V[X] = chip8->V[Y];
      int temp = (chip8->V[X] >> 7) & 1;
      chip8->V[X] <<= 1;
      chip8->V[0xF] = temp;
      break;
    }

    default:
      break;
    }

  case 9:
    if (N == 0) {
      if (chip8->V[X] != chip8->V[Y]) {
        chip8->pc += 2;
      }
    }
    break;

  case 10:
    chip8->I = NNN;
    break;

  case 11:
    chip8->pc = NNN + chip8->V[0];
    break;

  case 12:
    chip8->V[X] = (rand() % 256) & NN;
    break;

  case 0xD: {
    draw_sprite(chip8->opcode, chip8);
  } break;

  case 0xE:
    switch (chip8->opcode & 0x00FF) {
    case 0x9E:
      if (chip8->key[chip8->V[X]]) {
        chip8->pc += 2;
      }
      break;

    case 0xA1:
      if (!chip8->key[chip8->V[X]]) {
        chip8->pc += 2;
      }
      break;

    default:
      break;
    }
    break;

  case 0xF:
    switch (NN) {
    case 0x07:
      chip8->V[X] = chip8->delay_timer;
      break;

    case 0x15:
      chip8->delay_timer = chip8->V[X];
      break;

    case 0x18:
      chip8->sound_timer = chip8->V[X];
      break;

    case 0x1E:
      chip8->I += chip8->V[X];
      break;

    case 0x0A: {
      get_key(chip8->opcode, chip8);
    }

    case 0x29:
      chip8->I = chip8->V[X] * 5;
      break;

    case 0x33: {
      chip8->memory[chip8->I] = chip8->V[X] / 100;
      chip8->memory[chip8->I + 1] = (chip8->V[X] % 100) / 10;
      chip8->memory[chip8->I + 2] = chip8->V[X] % 10;
      break;
    }

    case 0x55:
      for (int i = 0; i <= X; i++) {
        chip8->memory[chip8->I++] = chip8->V[i];
      }
      break;

    case 0x65:
      for (int i = 0; i <= X; i++) {
        chip8->V[i] = chip8->memory[chip8->I++];
      }
      break;
    }
    break;

  default:
    break;
  }
}
