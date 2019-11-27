#include "SDL2/SDL.h"
#include "sdl.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512

void sdl::initSDL(void) {
  int rendererFlags, windowFlags;
  rendererFlags = SDL_RENDERER_ACCELERATED;
  windowFlags = 0;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, windowFlags);

  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

  if (!renderer) {
    printf("Failed to create renderer: %s\n", SDL_GetError());
    exit(1);
  }
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

void sdl::drawPixel(uint32_t gfx[]) {

  // Store pixels in temporary buffer
  for (int i = 0; i < 2048; ++i) {
    uint8_t pixel = gfx[i];
    pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
  }
  // Update SDL texture
  SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(Uint32));
  // Clear screen and render
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void sdl::present() { SDL_RenderPresent(renderer); }

void sdl::clearScreen() { SDL_RenderClear(renderer); }

void sdl::cleanup() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
