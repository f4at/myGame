#include "settings.h"
#include "audio.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

fastUint16 audioRate;
fastUint16 audioFormat;
fastUint16 audioBuffer;
fastUint8 musicVolume;
fastUint8 GFXVolume;

Mix_Music **musics = NULL;
Mix_Chunk **chunks = NULL;
fastUint16 musicsLoaded=0;
fastUint16 chunksLoaded=0;

Mix_Music * loadMusic(char* c) {
    Mix_Music * music = Mix_LoadMUS(c);
    if (!music) {
        printf("error failed to load music %s\n",c);
        exit(1);
    }
    musics = realloc(musics,sizeof(Mix_Music*)*++musicsLoaded);
    musics[musicsLoaded-1] = music;
    return music;
}

Mix_Chunk * loadChunk(char* c) {
    Mix_Chunk * chunk = Mix_LoadWAV(c);
    if (!chunk) {
        printf("error failed to load chunk %s\n",c);
        exit(1);
    }
    chunks = realloc(chunks,sizeof(Mix_Chunk*)*++chunksLoaded);
    chunks[chunksLoaded-1] = chunk;
    return chunk;
}

int playChunk(Mix_Chunk *chunk, int loops) {
    if (loops)
        return Mix_PlayChannel(-1, chunk, loops == -1 ? -1 : loops-1);
}

void freeMusic(Mix_Music *music) {
    for (int i=0;i<musicsLoaded;i++) {
        if (musics[i] == music) {
            Mix_FreeMusic(music);
            for (int j=i;j<musicsLoaded-1;j++) {
                musics[j] = musics[j+1];
            }
            break;
        }
    }
    musics = realloc(musics,sizeof(Mix_Music*)*--musicsLoaded);
}

void freeChunk(Mix_Chunk *chunk) {
    for (int i=0;i<chunksLoaded;i++) {
        if (chunks[i] == chunk) {
            Mix_FreeChunk(chunk);
            for (int j=i;j<chunksLoaded-1;j++) {
                chunks[j] = chunks[j+1];
            }
            break;
        }
    }
    chunks = realloc(chunks,sizeof(Mix_Chunk*)*--chunksLoaded);
}

void setMusicVolume(fastUint8 v) {
    musicVolume = v;
    Mix_VolumeMusic(round(1.28*v));
}

void setGFXVolume(fastUint8 v) {
    GFXVolume = v;
    Mix_Volume(-1,round(1.28*v));
}

void Audio_Init() {
    Mix_Init(MIX_INIT_MP3);
    audioRate = getSettingInt("audioRate");
    audioFormat = getSettingInt("audioFormat");
    audioBuffer = getSettingInt("audioBuffer");
    if (Mix_OpenAudio(audioRate, audioFormat, 2, audioBuffer)) {
        printf("Unable to open audio!\n");
        exit(1);
    }
    setMusicVolume(getSettingInt("musicVolume"));
    setGFXVolume(getSettingInt("GFXVolume"));
    Mix_AllocateChannels(128);
}

void Audio_Quit() {
    Mix_CloseAudio();
    for (int i=0;i<chunksLoaded;i++) {
        Mix_FreeChunk(chunks[i]);
    }
    for (int i=0;i<musicsLoaded;i++) {
        Mix_FreeMusic(musics[i]);
    }
    free(musics);
    free(chunks);
    Mix_Quit();
}