#ifndef GAME_H_
#define GAME_H_

#include "graphics.h"

extern container * game;

void openGame();
void closeGame();
void Game_Init();
void Game_Quit();
void Game_Update();
void Game_Resize();
#endif