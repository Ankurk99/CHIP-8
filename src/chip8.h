#include <stdint.h>

class chip8 {
  // chip8 have 35 opcodes, each of one is 2 bytes long
  // an unsigned short has the length of 2 bytes
  unsigned short opcode; // current opcode
  // chip8 has total of 4K memory
  unsigned char memory[4096];
  // CPU Registers: the chip8 hhas 15 8bit registers (16th register is carry
  // flag)
  unsigned char V[16]; // V-regs (V0-VF)
  unsigned short I;    // Index register
  unsigned short pc;   // program counter
  unsigned char delay_timer;
  unsigned char sound_timer;
  unsigned short stack[16];
  unsigned short sp;

public:
  bool draw_flag;
  unsigned char key[16]; // key state
  uint32_t gfx[64 * 32]; // screen resolution

  chip8();
  ~chip8();
  void init();
  void emulate_cycle();
  void load_program(char filepath[]);
};
