#ifndef EVENTS_H_
#define EVENTS_H_

#include "utils.h"
#include "SDL/SDL.h"

typedef SDL_Event * Event;
typedef void (*EventCallback)(Event);
typedef EventCallback (*EventCallbackWrapper)();

typedef struct {
    EventCallback *callbacks;
    fastUint8 n;
} EventCallbacks;

extern fastUint8 gameover;
extern EventCallbacks * EventsCallbacks;
extern fastUint8 Events_Tick;

void Events_Init();

Event newEvent();
void push(Event event);
void pushCustom(EventCallbacks * EventsCallbacks,Event event);
fastUint8 addEventType();
void addCallback(EventCallbacks * EventsCallbacks, fastUint8 type, EventCallback callback);
void removeCallback(EventCallbacks * EventsCallbacks, fastUint8 type, EventCallback callback);
void HandleEvents();
void Quit_game();

void Events_Quit();
#endif