
#include "chip8.h"

int main(int argc, char **argv) {
  chip8 core;
  core.load_program("path to file");

  for (;;) {
    core.emulate_cycle();
  }
  return 0;
}