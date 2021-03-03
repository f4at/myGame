#include <stdlib.h>
#include <math.h>
#include "SDL/SDL.h"
#include "video.h"
#include "settings.h"
#include "events.h"

fastUint16 displayWidth;
fastUint16 displayHeight;
fastUint16 windowWidth;
fastUint16 windowHeight;
fastUint16 gameWidth;
fastUint16 gameHeight;
fastUint16 resolutionWidth;
fastUint16 resolutionHeight;
fastUint16 maxFps;
fastUint8 MSAASamples;
fastUint8 MSAAMaxSamples = 4;

fastUint32 Video_Flags;
fastUint8 Events_GameResize;

void Video_Init() { 
    Events_GameResize = addEventType();

    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    fastUint8 bpp = info->vfmt->BitsPerPixel;

    char *title = getSetting("title");
    char *icon = getSetting("iconText");
    SDL_WM_SetCaption(title, icon);
    free(title);free(icon);

    Video_Flags = SDL_OPENGL;// | SDL_RESIZABLE

    displayWidth = info->current_w;
    displayHeight = info->current_h;

    resolutionWidth = getSettingInt("resolutionWidth");
    resolutionHeight = getSettingInt("resolutionHeight");
    resolutionWidth = resolutionWidth ? resolutionWidth : displayWidth;
    resolutionHeight = resolutionHeight ? resolutionHeight : displayHeight; 

    if (getSettingInt("fullScreen")) {
        Video_Flags |= SDL_FULLSCREEN;
    }

    MSAASamples = getSettingInt("MSAASamples");
    MSAASamples = min(MSAASamples,MSAAMaxSamples);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MSAASamples);
    SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, getSettingInt("sync"));
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);



    Event event = newEvent();
    event->type = SDL_VIDEORESIZE;
    event->resize.w = resolutionWidth;
    event->resize.h = resolutionHeight;
    Video_Resize(event);
    free(event);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
}

void Video_Resize(Event event) {
    resolutionWidth = event->resize.w;
    resolutionHeight = event->resize.h;
    maxFps = getSettingInt("maxFPS");
    if (Video_Flags & SDL_FULLSCREEN) {
        windowWidth = displayWidth;
        windowHeight = displayHeight;
        float f = ((float)resolutionWidth/(float)resolutionHeight)/((float)displayWidth/(float)displayHeight);
        gameWidth = round((float)resolutionWidth*min(1,1/f));
        gameHeight = round((float)resolutionHeight*min(1,f));
    }
    else {
        resolutionWidth = event->resize.w;
        resolutionHeight = event->resize.h;
        windowWidth = resolutionWidth;
        windowHeight = resolutionHeight;
        gameWidth = resolutionWidth;
        gameHeight = resolutionHeight;
    }
    SDL_Surface* screen = SDL_SetVideoMode(windowWidth, windowHeight, 32, Video_Flags);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, gameWidth, gameHeight, 0.0f, 0.0f, 1.0f);
    glViewport(0,0, windowWidth,windowHeight);

    event->type = Events_GameResize;
    push(event);
    event->type = SDL_VIDEORESIZE;
}

void Video_Fullscreen(fastUint8 o) {
    setbit16(&Video_Flags,log2f(SDL_FULLSCREEN),o);
}

void Video_Quit() {
}