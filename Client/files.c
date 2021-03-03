#include <string.h>
#include <stdlib.h>
#include "files.h"
#include "utils.h"

SDL_File * Files;
int nFiles;

SDL_RWops * openFile(char *name, char *mode) {
    for (int i=0;i<nFiles;i++) {
        if (strcmp(Files[i].name,name) == 0) {
            if (strcmp(Files[i].mode,mode) == 0) {
                return Files[i].file;
            } else {
                closeFile(name);
                break;
            }
        }
    }
    return addFile(name,mode);
}

SDL_RWops * addFile(char *name, char *mode) {
    Files = realloc(Files, sizeof(SDL_File)*++nFiles);
    Files[nFiles-1].file=SDL_RWFromFile(name,mode);
    Files[nFiles-1].name=copy(name);
    Files[nFiles-1].mode=copy(mode);
    return Files[nFiles-1].file;
}

void closeFile(char *name) {
    for (int i=0;i<nFiles;i++) {
        if (strcmp(Files[i].name,name) == 0) {
            SDL_RWclose(Files[i].file);
            free(Files[i].name);
            free(Files[i].mode);
            nFiles--;
            Files[i] = Files[nFiles];
            Files = realloc(Files, sizeof(SDL_File)*nFiles);
            break;
        }
    }
    
}

void Files_Init() {
    Files = NULL;
    nFiles = 0;
}