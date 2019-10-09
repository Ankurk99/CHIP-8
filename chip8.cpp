#include "chip8.h"
#include <fstream>
#include <iostream>

using namespace std;

unsigned char font_set[80]{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

chip8::chip8() {
  // TODO: initialise the variables
  pc = 0x200;
  opcode = 0;
  I = 0;
  sp = 0;

  for (int i = 0; i < 64 * 32; i++) {
    gfx[i] = 0; // clear the screen
  }

  for (int i = 0; i < 16; i++) {
    stack[i] = 0; // clear the stack
  }

  for (int i = 0; i < 16; i++) {
    key[i] = V[i] = 0; // clear the registers
  }

  for (int i = 0; i < 4096; ++i)
    memory[i] = 0; // clear memory

  for (int i = 0; i < 80; i++) {
    // load fontset
    memory[i] = font_set[i]; // The first 80 places are filled with the font set
  }
}

void chip8::load_program(char filepath[]) {
  streampos size;
  char *memblock;

  ifstream program;
  program.open(filepath, ios::in | ios::binary | ios::ate);
  if (program.is_open()) {
    size =
        program.tellg(); // size of the file, as the file is opened in ios::ate
    program.seekg(0, ios::beg);
    memblock = new char[size];
    program.read(memblock, size);
    if ((4096 - 512) > size) {
      for (int i = 0; i < size; i++) {
        memory[i + 512] = memblock[i];
      }
    } else
      cout << "ROM is too big!!";
    program.close();
    delete (memblock);
  } else
    cout << "Unable to open file";
}

int chip8::emulate_cycle() {
  opcode = memory[pc] << 8 | memory[pc + 1];
  switch (opcode & 0xF000) {
  // TODO: 0NNN
  // TODO: 00E0
  // TODO: 00EE
  // TODO: 1NNN
  // TODO: 2NNN
  // TODO: 3XNN
  // TODO: 4XNN
  // TODO: 5XY0
  // TODO: 6XNN
  // TODO: 7XNN
  // TODO: 8XY0
  // TODO: 8XY1
  // TODO: 8XY2
  // TODO: 8XY3
  // TODO: 8XY4
  // TODO: 8XY5
  // TODO: 8XY6
  // TODO: 8XY7
  // TODO: 8XYE
  // TODO: 9XY0
  // TODO: ANNN
  // TODO: BNNN
  // TODO: CXNN
  // TODO: DXYN
  // TODO: EX9E
  // TODO: EXA1
  // TODO: FX15
  // TODO: FX18
  // TODO: FX1E
  // TODO: FX29
  // TODO: FX33
  // TODO: FX55
  // TODO: FX65
  default:
    cout << "Invalid opcode";
    break;
  }
  // TODO: delay timer
  // TODO: sound timer
}