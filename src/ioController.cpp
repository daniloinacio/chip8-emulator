#include <iostream>
#include "SDL2/SDL.h"
#include "ioController.hpp"

void IOController::setupIOSystem(){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(640, 320, SDL_WINDOW_OPENGL, &window, &renderer);
  // window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320 , SDL_WINDOW_OPENGL);
  // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_RenderSetScale(renderer, 10, 10);

  keyState = SDL_GetKeyboardState(NULL);
}

void IOController::screenUpdate(uint8_t *frameBuffer){
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (int i = 0; i < 32; i++){
    for (int j = 0; j < 64; j++){
      if(frameBuffer[i*64 + j]){
        SDL_RenderDrawPoint(renderer, j, i);
      }
    }
  }
  SDL_RenderPresent(renderer); 
  if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
    this->screenState = IOController::QUIT_SCREEN;
}

void IOController::closeIOSystem(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void IOController::keyUpdate(uint8_t *key){
  SDL_PumpEvents();
  key[KEY_1] = keyState[SDL_SCANCODE_1];
  key[KEY_2] = keyState[SDL_SCANCODE_2];
  key[KEY_3] = keyState[SDL_SCANCODE_3];
  key[KEY_C] = keyState[SDL_SCANCODE_4];
  key[KEY_4] = keyState[SDL_SCANCODE_Q];
  key[KEY_5] = keyState[SDL_SCANCODE_W];
  key[KEY_6] = keyState[SDL_SCANCODE_E];
  key[KEY_D] = keyState[SDL_SCANCODE_R];
  key[KEY_7] = keyState[SDL_SCANCODE_A];
  key[KEY_8] = keyState[SDL_SCANCODE_S];
  key[KEY_9] = keyState[SDL_SCANCODE_D];
  key[KEY_E] = keyState[SDL_SCANCODE_F];
  key[KEY_A] = keyState[SDL_SCANCODE_Z];
  key[KEY_0] = keyState[SDL_SCANCODE_X];
  key[KEY_B] = keyState[SDL_SCANCODE_C];
  key[KEY_F] = keyState[SDL_SCANCODE_V];
}


