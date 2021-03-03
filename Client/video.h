#ifndef VIDEO_H_
#define VIDEO_H_

#include "utils.h"
#include "events.h"

extern fastUint16 displayWidth;
extern fastUint16 displayHeight;
extern fastUint16 windowWidth;
extern fastUint16 windowHeight;
extern fastUint16 gameWidth;
extern fastUint16 gameHeight;
extern fastUint16 resolutionWidth;
extern fastUint16 resolutionHeight;

extern fastUint16 maxFps;
extern fastUint8 MSAASamples;
extern fastUint8 MSAAMaxSamples;
extern fastUint32 Video_Flags;
extern fastUint8 Events_GameResize;

void Video_Init();
void Video_Resize(Event event);
void Video_Fullscreen(fastUint8 o);
void Video_Quit();
#endif
