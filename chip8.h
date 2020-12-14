#ifndef CHIP8_H
#define CHIP8_H

#include <string>

class chip8 {
    // memory variables
    unsigned char memory[4096];     // 4k-bit block of memory
    unsigned char V[16];            // 16 8-bit CPU registers; V0..VE, VF is flag for arithmetic carry / sprite collision
    unsigned short opcode;          // 16-bit currently loaded opcode
    unsigned short I;               // address register
    unsigned short pc;              // program counter; 0x000..0xFFF
    unsigned short stack[16];       // the stack w/ 16 levels
    unsigned short sp;              // stack pointer

    // i/o variables & timers
    unsigned char gfx[64*32];       // 2048 b/w pixels
    unsigned char key[16];          // keypad; 0x0..0xF
    unsigned char delay_timer;      // 60Hz timer
    unsigned char sound_timer;      // 60Hz timer; counts 60..0, sounds when hits 0

public:
    // other variables
    unsigned char drawFlag;         // controls whether to update the screen or not


    void initialize();
    void loadGame(std::string);
    void emulateCycle();
    void drawGraphics();
    void setKeys();
};

#endif