#ifndef _IO_CONTROLLER_H_
#define _IO_CONTROLLER_H_
#include "SDL2/SDL.h"

class IOController
{
  private:
    int screenState;
    const uint8_t *keyState;
    static const int KEY_1 = 0;
    static const int KEY_2 = 1;
    static const int KEY_3 = 2;
    static const int KEY_4 = 4;
    static const int KEY_5 = 5; 
    static const int KEY_6 = 6; 
    static const int KEY_7 = 8;
    static const int KEY_8 = 9;
    static const int KEY_9 = 10;
    static const int KEY_0 = 13;
    static const int KEY_A = 12;
    static const int KEY_B = 14;
    static const int KEY_C = 3;
    static const int KEY_D = 7;
    static const int KEY_E = 11;
    static const int KEY_F = 15;
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;    
  public:
    static const int QUIT_SCREEN = 1;
    void setupIOSystem();
    void screenUpdate(uint8_t *frameBuffer);
    int getScreenState(){
      return this->screenState;
    }
    void keyUpdate(uint8_t key[16]);
    void soundUpdate(uint8_t soundTimer);
    void closeIOSystem();
};

#endif