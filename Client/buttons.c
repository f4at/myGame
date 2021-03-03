#include "buttons.h"
#include <stdlib.h>
#include <math.h>

EventCallback defaultInCallback;
EventCallback defaultOutCallback;
EventCallback defaultSelectCallback;
EventCallback defaultDeselectCallback;
EventCallback defaultButtonCallback;

void callback1(Event e) {
    multiplyTint(e->user.data2,1/0.9,1/0.9,1/0.9,1);
}

void callback2(Event e) {
    multiplyTint(e->user.data2,0.9,0.9,0.9,1);
}

void toButton(container *object, EventCallback mCallback,EventCallback activationCallback, EventCallback selectCallback, EventCallback deselectCallback,EventCallback inCallback, EventCallback outCallback, EventCallback init) {
    addCallback(object->callbacks,Events_Interaction,mCallback != NULL ? deselectCallback : defaultButtonCallback);
    addCallback(object->callbacks,Events_MouseIn,inCallback != NULL ? inCallback : defaultInCallback);
    addCallback(object->callbacks,Events_MouseOut,outCallback != NULL ? outCallback : defaultOutCallback);
    addCallback(object->callbacks,Events_Select,selectCallback != NULL ? selectCallback : defaultSelectCallback);
    addCallback(object->callbacks,Events_Activate,activationCallback);
    addCallback(object->callbacks,Events_Deselect,deselectCallback != NULL ? deselectCallback : defaultDeselectCallback);
    
    object->interactive = 1;
    Event e = newEvent();
    e->type = SDL_USEREVENT;
    e->user.data2 = object;
    if (init != NULL) {
        init(e);
    } else {
        e->user.code = Events_MouseOut;
        pushCustom(object->callbacks,e);
        e->user.code = Events_Deselect;
        pushCustom(object->callbacks,e);
    }
    free(e);
}

void buttonCallback1(Event event) {
    float *data = event->user.data1;
    Event mouse_event = *((Event*)((float*)event->user.data1+6));
    container *b = event->user.data2;
    if (data[5]) {
        if (mouse_event->type == SDL_MOUSEMOTION) {
            fastUint16 ox = abs(data[0]-mouse_event->motion.x)*2/data[3], oy = abs(data[1]-mouse_event->motion.y)*2/data[4];
            fastUint16 x = cosf(degreeToRadian(data[2]))*ox-sinf(degreeToRadian(data[2]))*oy;
            fastUint16 y = sinf(degreeToRadian(data[2]))*ox+cosf(degreeToRadian(data[2]))*oy;
            fastUint8 in = (x < b->size.x) && (y < b->size.y);
            if (getbit(b->state,0) != in){
                setbit(&b->state,0,in);
                if (in) {
                    event->user.code = Events_MouseIn;
                    pushCustom(b->callbacks,event);
                } else {
                    event->user.code = Events_MouseOut;
                    pushCustom(b->callbacks,event);
                    if (getbit(b->state,1)) {
                        setbit(&b->state,1,0);
                        event->user.code = Events_Deselect;
                        pushCustom(b->callbacks,event);
                    }
                }
            }      
        }
        else if (mouse_event->type == SDL_MOUSEBUTTONDOWN) {
            if (getbit(b->state,0) && mouse_event->button.button == SDL_BUTTON_LEFT) {
                setbit(&b->state,1,1);
                event->user.code = Events_Select;
                pushCustom(b->callbacks,event);
            }
        }
        else {
            if (mouse_event->button.button == SDL_BUTTON_LEFT) {
                if (getbit(b->state,1)) {
                    setbit(&b->state,1,0);
                    event->user.code = Events_Activate;
                    pushCustom(b->callbacks,event);
                    event->user.code = Events_Deselect;
                    pushCustom(b->callbacks,event);
                }
            }
        }
    } else {
        if (getbit(b->state,0)) {
            setbit(&b->state,0,0);
            event->user.code = Events_MouseOut;
            pushCustom(b->callbacks,event);
        }
        if (getbit(b->state,1)) {
            setbit(&b->state,1,0);
            event->user.code = Events_Deselect;
            pushCustom(b->callbacks,event);
        }
    }
    event->user.code = Events_Interaction;
}

void Buttons_Init() {
    defaultInCallback = &callback1;
    defaultOutCallback = &callback2;
    defaultSelectCallback = &callback1;
    defaultDeselectCallback = &callback2;
    defaultButtonCallback = &buttonCallback1;
}

void Buttons_Quit() {
}