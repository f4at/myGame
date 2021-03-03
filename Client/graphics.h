#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "utils.h"
#include "events.h"
#include "fonts.h"

typedef enum {
    Events_RenderContainer,
    Events_RenderContent,
    Events_Interaction,
    Events_MouseIn,
    Events_MouseOut,
    Events_Select,
    Events_Deselect,
    Events_Activate,
} containerEvents;

typedef enum {
    Container,
    Sprite,
    Camera,
    FrameBuffer,
    Canvas
} containerType;

typedef GLuint Texture;
typedef GLuint Shader;
typedef GLuint Program;

typedef struct container {
    fastUint8 type;
    fastUint16 id;
    fastUint8 visible;
    vec2 pos;
    vec2 size;
    vec2 scale;
    float angle;
    RGBAColor tint;
    void * content;
    fastUint8 nChilds;
    struct container **childs;
    fastUint8 interactive;
    fastUint8 state;
    EventCallbacks callbacks[8]; 
} container;

extern container * Context;
extern fastUint16 declaredContainers;
extern fastUint8 Events_Render;

GLuint initShader(GLenum type, const char *filename);
Program initProram(const char *vertfilename, const char *fragfilename);
void changeShader(container* c, EventCallback e);

RGBAColor glGetTint();
void glTintf(RGBAColor tint,fastUint8 r);

void Graphics_Init();

GLuint loadTexture(char* c);
GLuint surfaceToTexture(SDL_Surface *s);
void updateTexture(SDL_Surface *s, GLuint texId);

container * newContainer();
container * newSprite(Texture texture);
container * newCamera();
container * newFrameBuffer(int w, int h);

void addChild(container *parent, container *child);

void renderTexture(Event event);
void renderContainer(Event event);
void renderSprite(Event event);
void renderCamera(Event event);
void renderGame();

void move(container *object,float x, float y);
void translate(container *object, float x, float y);
void rotate(container *object, float angle);
void scale(container *object, float f);
void scaleXY(container *object, float x, float y);
void setTint(container *object, float r, float g, float b, float a);
void multiplyTint(container *object, float r, float g, float b, float a);
void addTint(container *object, float r, float g, float b, float a);

GLuint textTexture(char *c, char *font, fastUint16 size, SDL_Color color);
void updateTextTexture(GLuint tex, char *c, char *font, fastUint16 size, SDL_Color color);
void updateTextContainer(container *object,char *c, char *font, fastUint16 size, SDL_Color color);
#endif