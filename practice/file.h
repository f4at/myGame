#ifndef FILE_H_
#define FILE_H_

#include "SDL/SDL.h"

void init();
void quit();
int isIn(SDL_Rect pos, int mousex, int mousey);

#endif