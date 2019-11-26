
#include "chip8.h"
#include "sdl.cpp"

sdl screen;

void drawGraphics(unsigned char gfx[]) {
  screen.clearScreen();
  for (int x = 0; x < 64; x++) {
    for (int y = 0; y < 32; y++) {
      if (gfx[x + y * 64] == 1)
        screen.drawPixel(x, y);
    }
  }
  screen.present();
}

// setKeys set's the key[] when a key is pressed
void setKeys(unsigned char keys[]) {
  // clear keys
  for (int i = 0; i < 16; i++) {
    key[i] = 0;
  }
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      exit(0);
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysm.sym) {
      case SDLK_1:
        key[0x1] = 1;
        break;
      case SDLK_2:
        key[0x2] = 1;
        break;
      case SDLK_3:
        key[0x3] = 1;
        break;
      case SDLK_4:
        key[0xC] = 1;
        break;
      case SDLK_q:
        key[0x4] = 1;
        break;
      case SDLK_w:
        key[0x5] = 1;
        break;
      case SDLK_e:
        key[0x6] = 1;
        break;
      case SDLK_r:
        key[0xD] = 1;
        break;
      case SDLK_a:
        key[0x7] = 1;
        break;
      case SDLK_s:
        key[0x8] = 1;
        break;
      case SDLK_d:
        key[0x9] = 1;
        break;
      case SDLK_e:
        key[0xE] = 1;
        break;
      case SDLK_a:
        key[0xA] = 1;
        break;
      case SDLK_0:
        key[0x0] = 1;
        break;
      case SDLK_c:
        key[0xB] = 1;
        break;
      case SDLK_v:
        key[0xF] = 1;
        break;
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
}

int main(int argc, char **argv) {
  chip8 core;
  core.load_program("path to file");
  screen.initSDL();
  for (;;) {
    core.emulate_cycle();
    if (core.draw_flag)
      drawGraphics(core.gfx);
    setKeys(core.keys);
  }
  screen.cleanup();
  return 0;
}
