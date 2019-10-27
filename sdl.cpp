#include "SDL2/SDL.h"
#include "sdl.h"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

void sdl::initSDL(void) {
  int rendererFlags, windowFlags;
  rendererFlags = SDL_RENDERER_ACCELERATED;
  windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_Window *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, windowFlags);

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, rendererFlags);

  if (!renderer) {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }
}

void sdl::drawPixel(int x, int y) { SDL_RenderDrawPoint(renderer, x, y); }

void sdl::present() { SDL_RenderPresent(renderer); }

void sdl::clearScreen() { SDL_RenderClear(renderer); }

void sdl::cleanup() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}