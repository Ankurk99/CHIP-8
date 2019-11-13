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
      sp--;
      pc = stack[sp];
      break;
    default:
      cout << "Invalid opcode";
      break;
    }
    break;
  case 0x1000: // 0x1NNN: Jumps to address NNN
    pc = opcode & 0x0FFF;
    break;
  case 0x2000: // 2NNN: Calls subroutine at NNN
    stack[sp] = pc;
    sp++;
    pc = opcode & 0x0FFF;
    break;
  case 0x3000: // 0x3XNN: Skips the next instruction if VX equals NN. (Usually
               // the next instruction is a jump to skip a code block)
    if (V[(opcode & 0x0F00) >> 8] == opcode & 0x00FF)
      pc += 4;
    else
      pc += 2;
    break;
  case 0x4000: // 0x4XNN: Skips the next instruction if VX doesn't equal NN.
               // (Usually the next instruction is a jump to skip a code block)
    if (V[(opcode & 0x0F00) >> 8] != opcode & 0x00FF)
      pc += 4;
    else
      pc += 2;
    break;
  case 0x5000: // 0x5XY0: Skips the next instruction if VX equals VY. (Usually
               // the next instruction is a jump to skip a code block)
    if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
      pc += 4;
    else
      pc += 2;
    break;
  case 0x6000: // 6XNN: Set VX to NN
    V[(opcode & 0xFF00) >> 8] = opcode & 0x00FF;
    pc += 2;
    break;
  case 0x7000: // 7XNN: Adds NN to VX
    V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
    pc += 2;
    break;
  case 0x8000:
    switch (opcode & 0x000F) {
    case 0x0000: // 8XY0: Sets VX to the value of VY
      V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0001: // 8XY1: Sets VX to VX or VY
      V[(opcode & 0x0F00) >> 8] =
          V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
      pc += 2;
      break;
    case 0x0002: // 8XY2: Sets VX to VX and VY
      V[opcode & 0x0F00 >> 8] =
          V[opcode & 0x0F00 >> 8] & V[opcode & 0x00F0 >> 4];
      pc += 2;
      break;
    case 0x0003: // 8XY3: Sets VX to VX xor VY
      V[opcode & 0x0F00 >> 8] =
          V[opcode & 0x0F00 >> 8] ^ V[opcode & 0x00F0 >> 4];
      pc += 2;
      break;
    case 0x0004: // 8XY4: Adds VY to VX. VF is set to 1 when there's a carry,
                 // and to 0 when there isn't
      if (V[opcode & 0x0F00 >> 8] > (0xFF - V[opcode & 0x00F0 >> 4]))
        V[0xF] = 1;
      else
        V[0xF] = 0;
      V[opcode & 0x0F00 >> 8] += V[opcode & 0x00F0 >> 4];
      pc += 2;
      break;
    case 0x0005: // 8XY5: VY is subtracted from VX. VF is set to 0 when there's
                 // a borrow, and 1 when there isn't.
      if (V[opcode & 0x0F00 >> 8] < V[opcode & 0x00F0 >> 4])
        V[0xF] = 0;
      else
        V[0xF] = 1;
      V[opcode & 0x0F00 >> 8] -= V[opcode & 0x00F0 >> 4];
      pc += 2;
      break;
    case 0x0006: // 8XY6: Stores the least significant bit of VX in VF and then
                 // shifts VX to the right by 1
      V[0xF] = V[opcode & 0x0F00 >> 8] & 0x0001;
      V[opcode & 0x0F00 >> 8] <<= 1;
      pc += 2;
      break;
    case 0x0007:
      // TODO: 8XY7
      break;
    case 0x000E: // 8XYE: Stores the most significant bit of VX in VF and then
                 // shifts VX to the left by 1.
      V[0xF] = V[opcode & 0x0F00 >> 8] & 0x1000;
      V[opcode & 0x0F00 >> 8] >>= 1;
      pc += 2;
      break;

    default:
      cout << "Invalid opcode";
      break;
    }
  case 0x9000:
    // 9XY0: Skips the next instruction if VX doesn't equal VY. (Usually the
    // next instruction is a jump to skip a code block)
    if (V[(opcode & 0x0f00) >> 8] != V[(opcode & 0x0f00) >> 4])
      pc += 4;
    else
      pc += 2;
    break;
  case 0xA000: // 0xANNN: Sets I to the address NNN.
    I = opcode & 0x0FFF;
    pc += 2;
    break;
  case 0xB000: // BNNN: Jumps to the address NNN plus V0.
    pc = V[0] + (opcode & 0x0FFF);
    break;
  case 0xC000: // CXNN: Sets VX to the result of a bitwise and operation on a
               // random number (Typically: 0 to 255) and NN.
    V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
    pc += 2;
    break;
  case 0xD000:
    // DXYN: Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels
    // and a height of N pixels. Each row of 8 pixels is read as bit-coded
    // starting from memory location I; I value doesn’t change after the
    // execution of this instruction. As described above, VF is set to 1 if any
    // screen pixels are flipped from set to unset when the sprite is drawn, and
    // to 0 if that doesn’t happen
    unsigned short x = V[(opcode & 0x0F00) >> 8];
    unsigned short y = V[(opcode & 0x00F0) >> 4];
    unsigned short height = opcode & 0x000F;
    unsigned short pixel;

    V[0xF] = 0;
    for( int yline = 0; yline < height ; yline++){
      pixel = memory[I + yline];
      for(int xline = 0; xline < 8; xline++){
        if ((pixel & (0x80 >> xline)) != 0){
          if (gfx[(x + xline + ((y + yline) * 64))] == 1)
            V[0xF] = 1;
          gfx[x+ xline + ((y + yline) * 64)] ^= 1;
        }
      }
    }

    draw_flag = true;
    pc += 2;
    break;
  case 0xE000:
    switch (opcode & 0x00FF) {
    case 0x009E: // EX9E: Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block)

      break;
    case 0x00A1:
      // TODO: EXA1
      break;:
    default:
      cout << "Invalid opcode";
      break;
    }
    break;
  case 0xF000:
    switch (opcode & 0x0FF) {
    case 0x0007: // FX07: Sets VX to the value of the delay timer.
      V[opcode & 0x0F00 >> 8] = delay_timer;
      pc += 2;
      break;
    case 0x000A:
      // TODO: FX0A
      break;
    case 0x0015: // FX15: Sets the delay timer to VX.
      delay_timer = V[opcode & 0x0F00 >> 8];
      pc += 2;
      break;
    case 0x0018: // FX18: Sets the sound timer to VX.
      sound_timer = V[opcode & 0x0F00 >> 8];
      pc += 2;
      break;
    case 0x001E: // FX1E: Adds VX to I
      I += V[opcode & 0x0F00 >> 8];
      pc += 2;
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
