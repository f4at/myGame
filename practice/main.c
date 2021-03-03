#include "file.h"
#include "SDL/SDL_image.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <stdlib.h>


int main(void) {
    //begin init
    init();

    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    int bpp = info->vfmt->BitsPerPixel;

    int displayWidth = info->current_w;
    int displayHeight = info->current_h;

    int gameWidth=1280, gameHeight=720;
    int flags = SDL_DOUBLEBUF | SDL_HWSURFACE;
    SDL_Surface * screen = SDL_SetVideoMode(gameWidth,gameHeight,0,flags);

    // load imgs
    SDL_Surface * img1 = IMG_Load("imgs/rectangle.png");
    SDL_Surface * img2 = IMG_Load("imgs/rectangle2.png");
    SDL_Surface * img = img1;

    TTF_Font *police = NULL;
    police = TTF_OpenFont("arial-bold.ttf", 32); // load font
    SDL_Surface * text = TTF_RenderText_Blended(police,"HELLO!",(SDL_Color){r:255,g:255,b:255}); // render text
    TTF_CloseFont(police);

    //init pos
    SDL_Rect img_pos; img_pos.x = 0; img_pos.y = 0; 
    SDL_Rect text_pos; text_pos.x = 500; text_pos.y = 500; 
    
    //init music
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024)) { //open audio
        printf("Unable to open audio!\n");
        exit(1);
    }
    Mix_Chunk * chunk = Mix_LoadWAV("audio.wav"); //load sound effect
    Mix_Music * music = Mix_LoadMUS("audio.wav"); //load music

    Mix_AllocateChannels(32); //allocateChannels
    Mix_Volume(-1,128); //set sound effects volume
    Mix_VolumeMusic(128); //set music volume

    Mix_PlayMusic(music,-1); //play music

    SDL_Event myEvent;
    int lastTime;
    int gameover = 0;
    int in;
    // end init
    while (!gameover) {
        //begin affiachage
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(img,NULL,screen,&img_pos);
        SDL_BlitSurface(text,NULL,screen,&text_pos);
        SDL_Flip(screen);
        //end affichage
        while (SDL_PollEvent(&myEvent)) { //input
            //begin mise a jour
            switch (myEvent.type) {
                case SDL_QUIT:
                    gameover = 1; // close
                    break;
                case SDL_KEYDOWN:
                    switch (myEvent.key.keysym.sym) { // get the key
                        case SDLK_q:
                            gameover = 1; // close
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (isIn(img_pos,myEvent.motion.x,myEvent.motion.y)) {
                        img = img2;
                    } else {
                        img = img1; 
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (myEvent.button.button == SDL_BUTTON_LEFT) {
                        if (isIn(img_pos,myEvent.button.x,myEvent.button.y)) {
                            lastTime = SDL_GetTicks();
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (myEvent.button.button == SDL_BUTTON_LEFT) {
                        if (isIn(img_pos,myEvent.button.x,myEvent.button.y)) {
                            int difference = (SDL_GetTicks()-lastTime)/100;
                            img_pos.x += difference;
                            Mix_PlayChannel(-1, chunk, 0); // play sound effect
                        }
                    }
                    break;
            }
            //end mise a jour
        }
    }

    SDL_FreeSurface(img1);
    SDL_FreeSurface(img2);
    SDL_FreeSurface(text);
    Mix_FreeChunk(chunk);
    Mix_FreeMusic(music);
    quit();
    return 0;
}