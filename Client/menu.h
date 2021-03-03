#ifndef MENU_H_
#define MENU_H_

#include "graphics.h"
#include "audio.h"

extern Mix_Music * menuMusic;
extern container * menu;
void Menu_Init();
void Menu_Quit();
void Menu_Resize();
#endif