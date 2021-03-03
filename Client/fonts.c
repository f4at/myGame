#include "fonts.h"
#include "files.h"
#include "graphics.h"

TTF_Font * loadTTF(char *c, fastUint16 size) {
    return TTF_OpenFontRW(openFile(c,"rb"),0,size);
}