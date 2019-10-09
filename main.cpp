
#include "CHIP8.h"

int main(int argc, char **argv) {
  CHIP8 core("path to the code");

  for (;;) {
    core.emulate_cycle();
  }
  return 0;
}