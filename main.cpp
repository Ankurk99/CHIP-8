
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

int main(int argc, char **argv) {
  chip8 core;
  core.load_program("path to file");
  screen.initSDL();
  for (;;) {
    core.emulate_cycle();
    if (core.draw_flag)
      drawGraphics(core.gfx);
  }
  screen.cleanup();
  return 0;
}
