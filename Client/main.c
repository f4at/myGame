#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "video.h"
#include "audio.h"
#include "settings.h"
#include "buttons.h"
#include "files.h"
#include "menu.h"
#include "game.h"

#ifdef  __EMSCRIPTEN__
#include "emscripten.h"
#endif

Event tickEvent, renderEvent;
void init() {
    IMG_Init(IMG_INIT_PNG);
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Files_Init();
    Events_Init();
    Settings_Init();
    Graphics_Init();
    Video_Init();
    Audio_Init();
    Buttons_Init();

    GLenum err = glewInit();
    
    if (err != GLEW_OK) {
        printf("glew Error.");
        exit(1);
    } else if (!GLEW_VERSION_2_1) {
        printf("The machine doesn't support the 2.1 API.");
        exit(1);
    }
    
    Menu_Init();
    Game_Init();
    
    gameover = 0;
    tickEvent = newEvent(); renderEvent = newEvent();
    tickEvent->type = SDL_USEREVENT; tickEvent->user.code = Events_Tick;
    renderEvent->type = SDL_USEREVENT; renderEvent->user.code = Events_Render;
}

void quit() {
    Game_Quit();
    Menu_Quit();
    Buttons_Quit();
    Audio_Quit();
    Events_Quit();
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();

    free(tickEvent);
    free(renderEvent);
}

int main() {
    init();
    while (!gameover) {
        push(renderEvent);
        HandleEvents();
        push(tickEvent);
    }
    quit();
    return 0;
}