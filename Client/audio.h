#ifndef AUDIO_H_
#define AUDIO_H_

#include "SDL/SDL_mixer.h"
#include "utils.h"

extern fastUint16 audioRate;
extern fastUint16 audioFormat;
extern fastUint16 audioBuffer;
extern fastUint8 musicVolume;
extern fastUint8 GFXVolume;

Mix_Music * loadMusic(char* c);
Mix_Chunk * loadChunk(char* c);
int playChunk(Mix_Chunk *chunk, int loops);
void freeMusic(Mix_Music *music);
void freeChunk(Mix_Chunk *chunk);

void setMusicVolume(fastUint8 v);
void setGFXVolume(fastUint8 v);

void Audio_Init();
void Audio_Quit();

#endif