#include <stdlib.h>
#include <stdio.h>
#include "events.h"
#include "utils.h"
#include "graphics.h"
#include "video.h"
#include <math.h>

fastUint8 gameover, nEventsTypes;
EventCallbacks * EventsCallbacks = NULL;
fastUint8 Events_Tick;
Event myEvent;

Event newEvent() {
    return malloc(sizeof(SDL_Event));
}

void push(Event event) {
    EventCallbacks ec = EventsCallbacks[event->type==SDL_USEREVENT?event->user.code:event->type];
    for (int i=0;i<ec.n;i++) {
        ec.callbacks[i](event);
    }
}

void pushCustom(EventCallbacks * EventsCallbacks,Event event) {
    EventCallbacks ec = EventsCallbacks[event->type==SDL_USEREVENT?event->user.code:event->type];
    for (int i=0;i<ec.n;i++) {
        ec.callbacks[i](event);
    }
}

void addCallback(EventCallbacks * EventsCallbacks, fastUint8 type, EventCallback callback) {
    EventsCallbacks[type].callbacks = realloc(EventsCallbacks[type].callbacks, ++EventsCallbacks[type].n*sizeof(EventCallback));
    EventsCallbacks[type].callbacks[EventsCallbacks[type].n-1] = callback;
}

void removeCallback(EventCallbacks * EventsCallbacks, fastUint8 type, EventCallback callback) {
    for (int i=0;i<EventsCallbacks[type].n;i++)
        if (callback==EventsCallbacks[type].callbacks[i]) {
            for (int j=i+1;j<EventsCallbacks[type].n;j++)
                EventsCallbacks[type].callbacks[j-1] = EventsCallbacks[type].callbacks[j];
            break;
        }
    EventsCallbacks[type].callbacks = realloc(EventsCallbacks[type].callbacks, --EventsCallbacks[type].n*sizeof(EventCallback)); 
}

fastUint8 addEventType() {
    EventsCallbacks = realloc(EventsCallbacks,++nEventsTypes*sizeof(EventCallbacks));
    EventsCallbacks[nEventsTypes-1].n = 0;
    EventsCallbacks[nEventsTypes-1].callbacks = NULL;
    return nEventsTypes-1;
}

void Quit_game() {
    gameover = 1;
}

void HandleKeyDown(Event event) {
    switch(event->key.keysym.sym) {
        case SDLK_q:
            Quit_game(event);
            break;
    }
}

void HandleKeyUP(Event event) {
}

void HandleButtonsInteractions(Event event) {
    float *data = event->user.data1;
    container *object = event->user.data2;
    float odata[6];
    for (int i=0;i<6;i++)
        odata[i] = data[i];
    if (object->type == Camera){
        data[5] = data[5] && object->visible && object->interactive;
        pushCustom(object->callbacks,event);
        vec2 p = {x:object->pos.x,y:object->pos.y};
        p = vecRotate(p,degreeToRadian(-data[2]));
        data[0] -= p.x/data[3];
        data[1] -= p.y/data[4];
        data[2] -= object->angle;
        data[3] /= object->scale.x;
        data[4] /= object->scale.y;
        for (int i=0;i<object->nChilds;i++) {
            event->user.data2 = object->childs[i];
            HandleButtonsInteractions(event);
        }
            
    }
    else {
        vec2 p = {x:object->pos.x,y:object->pos.y};
        p = vecRotate(p,degreeToRadian(data[2]));
        data[0] += p.x*data[3];
        data[1] += p.y*data[4];
        data[2] += object->angle;
        data[3] *= object->scale.x;
        data[4] *= object->scale.y;
        data[5] = data[5] && object->visible && object->interactive;
        pushCustom(object->callbacks,event);
        for (int i=0;i<object->nChilds;i++) {
            event->user.data2 = object->childs[i];
            HandleButtonsInteractions(event);
        }
    }
    for (int i=0;i<6;i++)
        data[i] = odata[i];
}

Event InteractionEvent;
void HandleMouseInput(Event event) {
    if (event->type==SDL_MOUSEMOTION) {
        event->motion.x = round((float)event->motion.x*(float)gameWidth/(float)windowWidth);
        event->motion.y = round((float)event->motion.y*(float)gameHeight/(float)windowHeight);
    }
    *((Event*)((float*)InteractionEvent->user.data1+6)) = event;
    InteractionEvent->user.data2 = Context;
    HandleButtonsInteractions(InteractionEvent);
}

void HandleEvents() {
    while (SDL_PollEvent(myEvent)) {
        push(myEvent);
    }
}

void Events_Init() {
    nEventsTypes = 0;
    
    for (int i=0;i<25;i++)
        addEventType();
    
    Events_Tick = addEventType();
    
    myEvent = newEvent();
    InteractionEvent = newEvent();
    InteractionEvent->type = SDL_USEREVENT;
    InteractionEvent->user.code = Events_Interaction;
    InteractionEvent->user.data1 = malloc(sizeof(float)*6+sizeof(Event));
    float* data = InteractionEvent->user.data1; data[0]=0;data[1]=0;data[2]=0;data[3]=1;data[4]=1;data[5]=1;

    addCallback(EventsCallbacks,SDL_MOUSEBUTTONUP,&HandleMouseInput);
    addCallback(EventsCallbacks,SDL_MOUSEBUTTONDOWN,&HandleMouseInput);
    addCallback(EventsCallbacks,SDL_MOUSEMOTION,&HandleMouseInput);

    addCallback(EventsCallbacks,SDL_QUIT,&Quit_game);
    addCallback(EventsCallbacks,SDL_KEYDOWN,&HandleKeyDown);
    addCallback(EventsCallbacks,SDL_VIDEORESIZE,&Video_Resize);
}

void Events_Quit() {
    free(myEvent);
}