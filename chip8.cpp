#include "chip8.h"

using namespace std;

// file variables
#define carryFlag V[15]
#define spriteCollision V[15]

unsigned char chip8_fontset[80] = {
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

// constructor
Chip8::Chip8() { /* do nothing */ }

// destructor
Chip8::~Chip8() { /* do nothing */ }

// initialize chip8's registers & memory
void Chip8::initialize() {
    // clear variables
    pc 	   = 512;
    opcode = 0;
    I      = 0;
    sp     = 0;

    // clear stack
    for (int i = 0; i < 16; i++)
        stack[i] = 0;

    // clear registers
    for (int i = 0; i < 16; i++)
        V[i] = 0;

    // initialize memory & float in fontset
    for (int i = 0; i < 4096; i++)
        memory[i] = 0;

    for (int i = 0; i < 80; i++)
        memory[i] = chip8_fontset[i];

    // clear display
    for (int i = 0; i < (64*32); i++)
        gfx[i] = 0;

    // prepare opTables
    populateOpTables();

    return;
}

void Chip8::loadGame(string gameName) {
    return;
}

// fetch, decode, & execute opcode, then update timers
void Chip8::emulateCycle() {
    // fetch opcode instruction by combining blocks at two consecutive 8-bit blocks of memory starting at pc
    opcode = memory[pc] << 8 | memory[pc+1];

    executeOpcode();
    updateTimers();

    return;
}

void Chip8::drawGraphics() {
    return;
}

void Chip8::setKeys() {
    return;
}

/* ---- ---- helper functions ---- ---- */

// updates both timers & if sound hits 0, makes a sound; called by emulateCycle()
void Chip8::updateTimers() {
    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0) {
        sound_timer--;

        if (sound_timer == 0)
            printf("BEEP!\n");
    }

    return;	
}

void Chip8::fetchOpcode() {

}

// decode & execute opcode by looking it up in the table
void Chip8::executeOpcode() {
    unsigned char opcode_p1 = (opcode & 0xF000) >> 12;
    unsigned char opcode_p2 = (opcode & 0x0F00) >> 8;
    unsigned char opcode_p3 = (opcode & 0x00F0) >> 4;
    unsigned char opcode_p4 = (opcode & 0x000F);

    ((*this).*(opTable[(opcode & 0xF000) >> 12]))();
}

void Chip8::populateOpTables() {
    opTable[16] = {
        &Chip8::opTable0, &Chip8::op_0x1NNN, &Chip8::op_0x2NNN, &Chip8::op_0x3XKK,
        &Chip8::op_0x4XKK, &Chip8::op_0x5XY0, &Chip8::op_0x6XKK, &Chip8::op_0x7XKK, 
        &Chip8::opTable8, &Chip8::op_0x9XY0, &Chip8::op_0xANNN, &Chip8::op_0xBNNN,
        &Chip8::op_0xCXKK, &Chip8::op_0xDXYN, &Chip8::opTableE, &Chip8::opTableF
    };

    opTable0[0x0] = &Chip8::op_0x00E0;
    opTable0[0xE] = &Chip8::op_0x00EE;

    opTable8[0x0] = &Chip8::op_0x8XY0;
    opTable8[0x1] = &Chip8::op_0x8XY1;
    opTable8[0x2] = &Chip8::op_0x8XY2;
    opTable8[0x3] = &Chip8::op_0x8XY3;
    opTable8[0x4] = &Chip8::op_0x8XY4;
    opTable8[0x5] = &Chip8::op_0x8XY5;
    opTable8[0x6] = &Chip8::op_0x8XY6;
    opTable8[0x7] = &Chip8::op_0x8XY7;
    opTable8[0xE] = &Chip8::op_0x8XYE;

    opTableE[0x1] = &Chip8::op_0xEXA1;
    opTableE[0xE] = &Chip8::op_0xEX9E;

    opTableF[0x07] = &Chip8::op_0xFX07;
    opTableF[0x0A] = &Chip8::op_0xFX0A;
    opTableF[0x15] = &Chip8::op_0xFX15;
    opTableF[0x18] = &Chip8::op_0xFX18;
    opTableF[0x1E] = &Chip8::op_0xFX1E;
    opTableF[0x29] = &Chip8::op_0xFX29;
    opTableF[0x33] = &Chip8::op_0xFX33;
    opTableF[0x55] = &Chip8::op_0xFX55;
    opTableF[0x65] = &Chip8::op_0xFX65;

}

// get a random 8-bit number
uint8_t randByte() {
    return rand() % 0xFF;
}

/* ---- ---- opcode instruction functions ---- ---- */
void Chip8::op_null() {
    printf("Unknown / Unhandled opcode: 0x%X\n", opcode);
};

// CLS := clear display
void Chip8::op_0x00E0() {
    for (int i = 0; i < (64*32); i++)
        gfx[i] = 0x0;
    
    drawFlag = true;
    pc += 2;
}

// RET := return from sub-routine
void Chip8::op_0x00EE() {
    sp--;
    pc = stack[sp];
    pc +=2;
}

// 1NNN := jump to address
void Chip8::op_0x1NNN() {
    pc = opcode & 0x0FFF;
}

// 2NNN := call address
void Chip8::op_0x2NNN() {
    stack[sp] = pc;
    sp++;
    pc = opcode & 0x0FFF;
}

// 3XKK := skip next if Vx = kk
void Chip8::op_0x3XKK() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    if (V[x] == kk)
        pc += 4;
    else
        pc += 2;
}

// 4XKK := skip next if Vx != kk
void Chip8::op_0x4XKK() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    if (V[x] != kk)
        pc += 4;
    else
        pc += 2;
}

// 5XY0 := skip next if Vx == Vy
void Chip8::op_0x5XY0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (V[x] == V[y])
        pc += 4;
    else
        pc += 2;
}

// 6XKK := Vx = kk
void Chip8::op_0x6XKK() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    V[x] = kk;
    pc += 2;
}

// 7XKK := Vx = Vx + kk
void Chip8::op_0x7XKK() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    V[x] += kk;
    pc += 2;
}

// 8XY0 := Vx = Vy
void Chip8::op_0x8XY0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[y];
    pc += 2;
}

// 8XY1 := set Vx | Vy
void Chip8::op_0x8XY1() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[x] | V[y];
    pc += 2;
}

// 8XY2 := set Vx & Vy
void Chip8::op_0x8XY2() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[x] & V[y];
    pc += 2;
}

// 8XY3 := Vx = Vx ^ Vy
void Chip8::op_0x8XY3() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    V[x] = V[x] ^ V[y];
    pc += 2;
}

// 8XY4 := Vx = Vx + Vy, set VF = 1 if carry
void Chip8::op_0x8XY4() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    uint16_t sum = V[x] + V[y];

    if (sum > 255)
        carryFlag = 1;
    else
        carryFlag = 0;
    
    V[x] = sum & 0xFFu;
    pc += 2;
}

// 8XY5 := Vx = Vx - Vy, VF = 1 if "not borrow"
void Chip8::op_0x8XY5() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (V[y] > V[x])
        carryFlag = 0;
    else
        carryFlag = 1;
    
    V[x] = V[x] - V[y];
    pc += 2;
}

// 8XY6 := Vx = Vx >> 1 {ie Vx/2}, VF = least sig. byte
void Chip8::op_0x8XY6() {
    uint8_t x = (opcode & 0x0F00) >> 8;

    carryFlag = (V[x] & 0x1);
    V[x] = V[x] >> 1;
    pc += 2;
}

// 8XY7 := Vx = Vy - Vx, VF = 1 if "not borrow"
void Chip8::op_0x8XY7() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (V[x] > V[y])
        carryFlag = 0;
    else
        carryFlag = 1;
    
    V[x] = V[y] - V[x];
    pc += 2;
}

// 8XYE := Vx = Vx >> 1 {ie Vx*2}, VF = most sig. byte
void Chip8::op_0x8XYE() {
    uint8_t x = (opcode & 0x0F00) >> 8;

    carryFlag = (V[x] & 0x80u) >> 7;
    V[x] = V[x] << 1;
    pc += 2;
}

// 9XY0 := if Vx != Vy, skip next instruction
void Chip8::op_0x9XY0() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    if (V[x] != V[y])
        pc += 4;
    else
        pc += 2;
}

// ANNN := I = NNN
void Chip8::op_0xANNN() {
    I = opcode & 0x0FFF;
    pc += 2;
};

// BNNN := Jump to NNN + V0
void Chip8::op_0xBNNN() {
    pc = V[0] + (opcode & 0x0FFF);
}

// CXKK := Vx = RAND & kk
void Chip8::op_0xCXKK() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t kk = opcode & 0x00FF;

    V[x] = randByte() & kk;
    pc += 2;
}

void Chip8::op_0xDXYN() {

}

// EX9E := if key_Vx pressed, skip next instruction 
void Chip8::op_0xEX9E() {
    uint8_t x = (opcode & 0x0F00) >> 8;

    if (key[V[x]] != 0)
        pc += 4;
    else
        pc += 2;
}

// EXA1 := if key_Vx not pressed, skip next instruction 
void Chip8::op_0xEXA1() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    
    if (key[V[x]] == 0)
        pc += 4;
    else
        pc += 2;
}

// FX15 := Vx = delay_timer
void Chip8::op_0xFX07() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    V[x] = delay_timer;
    pc += 2;
}

// FX0A := wait for key-press, then store that key's value into Vx
void Chip8::op_0xFX0A() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    bool keyPressed = false;

    for (int i = 0; i < 16; i++) {
        if (key[i] != 0) {
            V[x] = i;
            keyPressed = true;
        }
    }

    if(keyPressed)
        pc += 2;
}

// FX15 := delay_timer = Vx
void Chip8::op_0xFX15() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    delay_timer = V[x];
    pc += 2;
}

// FX18 := sound_timer = Vx
void Chip8::op_0xFX18() {
    uint8_t x = (opcode & 0x0F00) >> 8;
    sound_timer = V[x];
    pc += 2;
}

// FX1E := I = I + Vx
void Chip8::op_0xFX1E() {
    uint8_t x = (opcode & 0x0F00) >> 8;



    I = I + V[x];
    pc += 2;
}

void Chip8::op_0xFX29() {

}

void Chip8::op_0xFX33() {

}

void Chip8::op_0xFX55() {

}

void Chip8::op_0xFX65() {

}