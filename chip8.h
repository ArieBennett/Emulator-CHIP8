#ifndef CHIP8_H
#define CHIP8_H

#include <string>

class Chip8 {
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

    // arrays of method pointers
    typedef void (Chip8::*Chip8Inst)();
    static Chip8Inst opTable[0xF + 1];
    static Chip8Inst opTable0[0xE + 1];
    static Chip8Inst opTable8[0xE + 1];
    static Chip8Inst opTableE[0xE + 1];
    static Chip8Inst opTableF[0x65 + 1];

    // helper functions
        // opcode helper functions
        void op_null();

        void op_0x1NNN();
        void op_0x2NNN();
        void op_0x3XKK();
        void op_0x4XKK();
        void op_0x5XY0();
        void op_0x6XKK();
        void op_0x7XKK();
        void op_0x9XY0();
        void op_0xANNN();
        void op_0xBNNN();
        void op_0xCXKK();
        void op_0xDXYN();

        void op_0x00E0();   void op_0x00EE();

        void op_0x8XY0();   void op_0x8XY1();   void op_0x8XY2();
        void op_0x8XY3();   void op_0x8XY4();   void op_0x8XY5();
        void op_0x8XY6();   void op_0x8XY7();   void op_0x8XYE();

        void op_0xEXA1();   void op_0xEX9E();

        void op_0xFX07();   void op_0xFX0A();   void op_0xFX15();
        void op_0xFX18();   void op_0xFX1E();   void op_0xFX29();
        void op_0xFX33();   void op_0xFX55();   void op_0xFX65();

    void populateOpTables();
    void fetchOpcode();
    void executeOpcode();
    void updateTimers();

public:
    // other variables
    unsigned char drawFlag;         // controls whether to update the screen or not

    Chip8();
    ~Chip8();

    void initialize();
    void loadGame(std::string);
    void emulateCycle();
    void drawGraphics();
    void setKeys();
};

#endif