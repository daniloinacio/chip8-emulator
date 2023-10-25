#ifndef _CHIP8_H_
#define _CHIP8_H_
#include <stdint.h>
#include "ioController.hpp"
#include <iostream>

class Chip8
{
  private:
    static const uint8_t fontset[80];
    uint32_t delayTimer;
    uint32_t soundTimer;
    uint8_t drawFlag;
    uint16_t opcode;
    uint16_t pc;
    uint16_t I;
    uint16_t sp;
    uint32_t seed;
    uint16_t stack[16];
    uint8_t V[16];
    uint8_t key[16];
    uint8_t memory[4096];
    uint8_t frameBuffer[64*32];
    uint8_t src, dst, kk;
    uint16_t  addr;
    IOController* ioController;

    void fetch();
    void decode();
    void execute();
    void updateTimers();
    void step();

  public:
    void initialize();
    void loadGame(std::string gameName);
    void run();
};

#endif