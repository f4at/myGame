
#include <stdlib.h>
#include "game.h"
#include "menu.h"
#include "video.h"
#include "events.h"
#include <math.h>
#include "audio.h"

container * game;
container * triangle;

void gameKeyboardHandler(Event event) {
    if (event->key.keysym.sym == SDLK_ESCAPE) {
        closeGame();
    }
}

void openGame() {
    menu->visible = 0;
    game->visible = 1;
    addCallback(EventsCallbacks,Events_Tick,&Game_Update);
    addCallback(EventsCallbacks,SDL_KEYDOWN,&gameKeyboardHandler);
    Mix_HaltMusic();
}

void closeGame() {
    menu->visible = 1;
    game->visible = 0;
    removeCallback(EventsCallbacks,Events_Tick,&Game_Update);
    removeCallback(EventsCallbacks,SDL_KEYDOWN,&gameKeyboardHandler);
    Mix_PlayMusic(menuMusic,-1);
}

void Game_Resize() {
    move(triangle,gameWidth-50,gameHeight-50);
}

void Game_Update() {
    rotate(triangle,round(.36*(SDL_GetTicks()%1000)));
}

void Game_Init() {
    game = newContainer();
    addChild(Context,game);
    game->visible = 0;

    triangle = newSprite(loadTexture("assets/imgs/triangle.png"));
    addChild(game,triangle);

    addCallback(EventsCallbacks,Events_GameResize,&Game_Resize);
    Game_Resize();
}

void Game_Quit() {
}