#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_error.h"
#include "include/SDL2/SDL_events.h"
#include "include/SDL2/SDL_keycode.h"
#include "include/SDL2/SDL_render.h"
#include "include/SDL2/SDL_video.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define RAM_SIZE 4096
#define REGISTERS 16
#define STACK_SIZE 16
#define KEYS 16

// Typedefs
typedef enum { RUNNING, PAUSED, QUIT } emu_state_t;

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} rgb_t;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
} sdl_t;

typedef struct {
  int screen_w;
  int screen_h;
  int scale_factor;
} config_t;

typedef struct {
  emu_state_t state;
  unsigned short stack[STACK_SIZE];
  unsigned short opcode;
  unsigned short I;
  unsigned short pc;
  unsigned short sp;
  unsigned char memory[RAM_SIZE];
  unsigned char V[REGISTERS];
  bool display[SCREEN_HEIGHT][SCREEN_WIDTH];
  unsigned char key[KEYS];
  unsigned char delay_timer, sound_timer;
} chip8_t;

// Function prototypes
void draw_sprite(uint16_t opcode, chip8_t *chip8);
bool init_sdl(sdl_t *sdl, const config_t config);
bool init_chip(chip8_t *chip8, const char rom_name[]);
void sdl_close(sdl_t *sdl);
void init_config(config_t *config);
void handle_user_input(sdl_t *sdl, chip8_t *chip8);
void clear_screen(chip8_t *chip8);
void update_window(sdl_t *sdl, const chip8_t chip8);
void subtract_x_y(const uint16_t opcode, chip8_t *chip8);
void emu_cycle(chip8_t *chip8);
void add(const uint16_t opcode, chip8_t *chip8);
void get_key(uint16_t opcode, chip8_t *chip8);
void subtract_x_y(const uint16_t opcode, chip8_t *chip8);
void subtract_y_x(const uint16_t opcode, chip8_t *chip8);
