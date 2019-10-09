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
  case 0x0000:
    switch (opcode & 0x000F) {
    case 0x0000: // 0x00E0: clears the screen
      for (int i = 0; i < 2064; i++)
        gfx[i] = 0x0;
      draw_flag = true;
      break;
    case 0x000E: // 0x00EE: returns from a subroutine
                 // TODO 0x00EE
      break;
    default:
      cout << "Invalid opcode";
      break;
    }
  case 0x1000: // 0x1NNN: Jumps to address NNN
    pc = opcode & 0x0FFF;
    break;
  case 0x2000:
    // TODO: 2NNN
    break;
  case 0x3000:
    // TODO: 3XNN
    break;
  case 0x4000:
    // TODO: 4XNN
    break;
  case 0x5000:
    // TODO: 5XY0
    break;
  case 0x6000:
    // TODO: 6XNN
    break;
  case 0x7000:
    // TODO: 7XNN
    break;
  case 0x8000:
    switch (opcode & 0x000F) {
    case 0x0000:
      // TODO: 8XY0
      break;
    case 0x0001:
      // TODO: 8XY1
      break;
    case 0x0002:
      // TODO: 8XY2
      break;
    case 0x0003:
      // TODO: 8XY3
      break;
    case 0x0004:
      // TODO: 8XY4
      break;
    case 0x0005:
      // TODO: 8XY5
      break;
    case 0x0006:
      // TODO: 8XY6
      break;
    case 0x0007:
      // TODO: 8XY7
      break;
    case 0x000E:
      // TODO: 8XYE
      break;

    default:
      cout << "Invalid opcode";
      break;
    }
  case 0x9000:
    // TODO: 9XY0
    break;
  case 0xA000: // 0xANNN: Sets I to the address NNN.
    I = opcode & 0x0FFF;
    pc += 2;
    break;
  case 0xB000:
    // TODO: BNNN
    break;
  case 0xC000:
    // TODO: CXNN
    break;
  case 0xD000:
    // TODO: DXYN
    break;
  case 0xE000:
    switch (opcode & 0x00FF) {
    case 0x009E:
      // TODO: EX9E
      break;
    case 0x00A1:
      // TODO: EXA1
      break;
    default:
      cout << "Invalid opcode";
      break;
    }
    break;
  case 0xF000:
    switch (opcode & 0x0FF) {
    case 0x0015:
      // TODO: FX15
      break;
    case 0x0018:
      // TODO: FX18
      break;
    case 0x001E:
      // TODO: FX1E
      break;
    case 0x0029:
      // TODO: FX29
      break;
    case 0x0033:
      // TODO: FX33
      break;
    case 0x0055:
      // TODO: FX55
      break;
    case 0x0065:
      // TODO: FX65
      break;
    default:
      cout << "Invaild opcode";
      break;
    }
  default:
    cout << "Invalid opcode";
    break;
  }
  if (delay_timer > 0)
    --delay_timer;

  if (sound_timer > 0) {
    if (sound_timer == 1)
      printf("BEEP!\n");
    --sound_timer;
  }
}