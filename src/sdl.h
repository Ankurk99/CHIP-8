#include "SDL2/SDL.h"
#include <stdint.h>

class sdl {
public:
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;

  uint32_t pixels[2048];
  void initSDL();
  void drawPixel(uint32_t gfx[]);
  void present();
  void clearScreen();
  void cleanup();
};