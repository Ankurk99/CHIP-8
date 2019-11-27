#include "chip8.h"
#include "sdl.h"
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
sdl screen;

void drawGraphics(uint32_t gfx[]) {
  screen.drawPixel(gfx);
  screen.present();
}

uint8_t keymap[16] = {
    SDLK_x, SDLK_1, SDLK_2, SDLK_3, SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c, SDLK_4, SDLK_r, SDLK_f, SDLK_v,
};

// setKeys set's the key[] when a key is pressed
void setKeys(unsigned char key[]) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      exit(0);

    // Process keydown events
    if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == SDLK_ESCAPE)
        exit(0);

      for (int i = 0; i < 16; ++i) {
        if (e.key.keysym.sym == keymap[i]) {
          key[i] = 1;
        }
      }
    }
    // Process keyup events
    if (e.type == SDL_KEYUP) {
      for (int i = 0; i < 16; ++i) {
        if (e.key.keysym.sym == keymap[i]) {
          key[i] = 0;
        }
      }
    }
  }
}

int main(int argc, char **argv) {
  chip8 core;
  core.load_program(argv[1]);
  screen.initSDL();
  for (;;) {
    core.emulate_cycle();
    if (core.draw_flag) {
      core.draw_flag = false;
      drawGraphics(core.gfx);
    }
    setKeys(core.key);
    std::this_thread::sleep_for(std::chrono::microseconds(2000));
  }
  screen.cleanup();
  return 0;
}
