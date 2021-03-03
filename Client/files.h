#ifndef FILES_H_
#define FILES_H_

#include "SDL/SDL.h"

typedef struct {
    SDL_RWops *file;
    char *name;
    char *mode;
} SDL_File;

extern SDL_File * Files;
extern int nFiles;

SDL_RWops * openFile(char *name, char *mode);
SDL_RWops * addFile(char *name, char *mode);
void closeFile(char *name);
void Files_Init();

#endif