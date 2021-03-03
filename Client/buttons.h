#ifndef BUTTONS_H_
#define BUTTONS_H_
#include "utils.h"
#include "events.h"
#include "graphics.h"

extern EventCallback defaultInCallback;
extern EventCallback defaultOutCallback;
extern EventCallback defaultSelectCallback;
extern EventCallback defaultDeselectCallback;
extern EventCallback defaultButtonCallback;

typedef struct {
    fastUint16 id;
    EventCallback Activate;
    EventCallback Select;
    EventCallback Deselect;
    EventCallback Out;
    EventCallback In;
} buttonCallbacks;


void toButton(container *object, EventCallback  mCallback,EventCallback activationCallback, EventCallback selectCallback, EventCallback deselectCallback,EventCallback inCallback, EventCallback outCallback, EventCallback init);
void Buttons_Init();
void Buttons_Quit();
#endif