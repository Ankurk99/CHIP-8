#include "SDL2/SDL.h"

class sdl {
public:
  SDL_Window *window;
  SDL_Renderer *renderer;

  void initSDL();
  void drawPixel(int x, int y);
  void present();
  void clearScreen();
  void cleanup();
};