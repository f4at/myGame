#include "SDL/SDL_image.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include "file.h"

void init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();
}

void quit() {
    SDL_Quit();
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
}

int isIn(SDL_Rect pos, int mousex, int mousey) {
    int inX = (mousex < pos.w+pos.x) && (mousex > pos.x);
    int inY = (mousey < pos.h+pos.y) && (mousey > pos.y);
    return inX && inY;
}