#include "chip8.hpp"
#include "ioController.hpp"
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <random>
#include <fstream>
#include <SDL2/SDL.h>

const uint8_t Chip8::fontset[] =
{ 
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

void Chip8::initialize()
{
  pc     = 0x200;  // Program counter starts at 0x200
  opcode = 0;      // Reset current opcode	
  I      = 0;      // Reset index register
  sp     = 0;      // Reset stack pointer
 
  // Clear display	
  for(int i = 0; i < 64*32; i++){
    frameBuffer[i] = 0;
  }

  // Clear stack
  for(int i = 0; i < 16; i++)
    stack[i] = 0;

  // Clear registers V0-VF
  for(int i = 0; i < 16; i++)
    V[i] = 0;	
  
  // Clear memory
  for(int i = 0; i < 4096; i++)
    memory[i] = 0;	
  
  // Load fontset
  for(int i = 0; i < 80; i++)
    memory[i] = fontset[i];		
 
  // Reset timers
  delayTimer = 0;
  soundTimer = 0;

  drawFlag = false;

  ioController = new IOController();
  ioController->setupIOSystem();
}

void Chip8::fetch()
{
  // Fetch opcode
  opcode = memory[pc] << 8 | memory[pc + 1];
  pc += 2;
  // printf("opcode: 0x%.4X\n", opcode);
}

void Chip8::loadGame(std::string gameName)
{
  char buffer;
  std::ifstream gameCode;
  gameCode.open(gameName);
  uint16_t i = 0x200;

  while(gameCode.get(buffer))
  {
    memory[i] = buffer;
    // printf("0x%.4X ", buffer);
    i++;
  }
    
  gameCode.close();

}

void Chip8::decode()
{
  // Decode opcode
  kk = opcode & 0x00FF;
  addr = opcode & 0x0FFF;
  dst = (opcode & 0x0F00) >> 8;
  src = (opcode & 0x00F0) >> 4;
}

void Chip8::execute()
{
  std::mt19937 mt(42); //gerador mersene twist, inicializado com seed = 42
  //distribuição linear de inteiros entre 0 e 100
  std::uniform_int_distribution<int> linear(0, 255);
  uint8_t randNumber = linear( mt ); //inteiro pseudo-aleatório
  
  switch(opcode & 0xF000)
  {    
    case 0x0000:
      switch(opcode & 0x00F0)
      {
        case 0x00E0:         
          switch(opcode & 0x000F)
          {
            case 0x0000: // CLS
              for(int i = 0; i < 64*32; i++){
                frameBuffer[i] = 0;
              }
              drawFlag = true;
            break;
            case 0x000E: // RET
              sp--;
              pc = stack[sp];
            break;
            default:
            break;
          }
        break;
  
        case 0x00C0:        
          // Execute opcode
        break;

        case 0x00F0:
          switch (opcode & 0x000F)
          {
            case 0x000B:
            break;
            case 0x000C:
            break;            
            case 0x000D:
            break;
            case 0x000E:
            break;
            case 0x000F:
            break;                   
            default:
            break;
          }
  
        default:
          printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);      
        break;    
      }
    break;
    case 0x1000: // JP addr
      pc = addr;
    break;
    case 0x2000: // CALL addr
      stack[sp] = pc;
      ++sp;
      pc = addr;
    break;
    case 0x3000: // SE Vx, byte
      if(V[dst] == kk)
        pc += 2;
    break;
    case 0x4000: // SNE Vx, byte
      if(V[dst] != kk)
        pc += 2;
    break;
    case 0x5000: // SE Vx, Vy 
      if(V[dst] == V[src])
        pc += 2;
    break;
    case 0x6000: // LD Vx, byte
      V[dst] = opcode & 0x00FF;
    break;
    case 0x7000: // ADD Vx, byte
      V[dst] += opcode & 0x00FF;
    break;
    case 0x8000: 
      switch(opcode & 0x000F)
      {
        case 0x0000: // LD Vx, Vy
          V[dst] = V[src];
        break;
        case 0x0001: // OR Vx, Vy
          V[dst] |= V[src];
        break;
        case 0x0002: // AND Vx, Vy
          V[dst] &= V[src];
        break;
        case 0x0003: // XOR Vx, Vy
          V[dst] ^= V[src];        
        break;
        case 0x0004: // ADD Vx, Vy
          if(V[src] > (0xFF - V[dst]))
            V[0xF] = 1; //carry
          else
            V[0xF] = 0;
          V[dst] += V[src];          
        break;   
        case 0x0005: // SUB Vx, Vy
          if(V[src] > V[dst])
            V[0xF] = 1; //carry
          else
            V[0xF] = 0;
          V[dst] -= V[src];
        break;  
        case 0x0006: // SHR Vx {, Vy}
          if(V[dst] & 0x0001)
            V[0xF] = 1;
          V[dst] >>= 1;
        break;
        case 0x0007: // SUBN Vx, Vy
          if(V[src] > V[dst])
            V[0xF] = 1;
          else
            V[0xF] = 0;
        break;
        case 0x000E: // SHL Vx {, Vy
          if(V[dst] & 0x8000)
            V[0xF] = 1;
          V[dst] <<= 1;
        break;
        default:
          printf ("Unknown opcode [0x8000]: 0x%X\n", opcode);
        break;
      }
    break;
    case 0x9000: // SNE Vx, Vy
      if(V[dst] != V[src])
        pc += 2;
    break;
    case 0xA000: // ANNN: Sets I to the address NNN
      // Execute opcode
      I = addr;
    break;  
    case 0xB000:
      pc = addr + V[0];
    break;
    case 0xC000: // RND Vx, byte
      V[dst] = randNumber & kk;
    break;
    case 0xD000: 
      if((opcode & 0x000F) != 0) // DRW Vx, Vy, nibble
      {
        unsigned short x = V[dst];
        unsigned short y = V[src];
        unsigned short height = opcode & 0x000F;
        unsigned short pixel;
      
        V[0xF] = 0;
        for (int yline = 0; yline < height; yline++)
        {
          pixel = memory[I + yline];
          for(int xline = 0; xline < 8; xline++)
          {
            if((pixel & (0x80 >> xline)) != 0)
            {
              if(frameBuffer[(x + xline + ((y + yline) * 64))] == 1)
                V[0xF] = 1;                                 
              frameBuffer[x + xline + ((y + yline) * 64)] ^= 1;
            }
          }
        }
        drawFlag = true;
      }
      else // DRW Vx, Vy, 0
      {
        
      }
      
    break;
    case 0xE000: 
      switch (opcode & 0x00FF)
      {
        case 0x009E: // SKP Vx
          if(key[V[dst]])
            pc += 2;
        break; 
        case 0x00A1: // SKNP Vx
          if(!key[V[dst]])
            pc += 2;        
        break;    
        default:
          printf ("Unknown opcode [0xE000]: 0x%X\n", opcode);
        break;
      }
    break;
    case 0xF000:
      switch (opcode & 0x00FF)
      {
        case 0x0007: // LD Vx, DT
          V[dst] = delayTimer; 
        break;    
        case 0x000A: // LD Vx, K
          bool keyPressed;
          do
          {
            keyPressed = false;
            ioController->keyUpdate(key);
            for(uint8_t i = 0; i < 16; i++)
            {
              if(key[i])
              {
                keyPressed = true;
                V[dst] = i;
                break;
              }
            }
          }while(!keyPressed);

        break; 
        case 0x0015: // LD DT, Vx
          delayTimer = V[dst];
        break; 
        case 0x0018: // LD ST, Vx
          soundTimer = V[dst];
        break; 
        case 0x001E: // ADD I, Vx
          I += V[dst];
        break; 
        case 0x0029: // LD F, Vx
          I = V[dst] * 5;
        break; 
        case 0x0030:
        break;
        case 0x0033:
          memory[I]     = V[dst] / 100;
          memory[I + 1] = (V[dst] / 10) % 10;
          memory[I + 2] = (V[dst] % 100) % 10;
        break;   
        case 0x0055: // LD [I], Vx
          for(int i=0; i<16; i++)
          {
            memory[I + i] = V[i];
          }
        break;
        case 0x0065: // LD Vx, [I]
          for(int i=0; i<16; i++)
          {
            V[i] = memory[I + i];
          }
        break; 
        case 0x0075:
        break;
        case 0x0085:
        break;
        default:
          printf ("Unknown opcode [0xF000]: 0x%X\n", opcode);
        break;
      }
    break;                       
 
    default:
      printf ("Unknown opcode: 0x%X\n", opcode);
    break;
  } 
}

void Chip8::updateTimers()
{
  // Update timers
  if(delayTimer > 0)
    --delayTimer;
  if(soundTimer > 0)
    --soundTimer;
}

void Chip8::step()
{
  fetch();
  decode();
  execute();
  updateTimers();
}

void Chip8::run()
{
  while(1)
  {
    ioController->keyUpdate(key);
    step();
    if(drawFlag)
      ioController->screenUpdate(frameBuffer);
    
    //ioController->soundUpdate(soundTimer);

    if(ioController->getScreenState() == IOController::QUIT_SCREEN)
      break;
    SDL_Delay(5);
  }
  ioController->closeIOSystem();

}


 
