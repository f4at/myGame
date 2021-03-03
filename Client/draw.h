#ifndef DRAW_H_
#define DRAW_H_

#include "utils.h"
#include "graphics.h"

typedef struct {
    float x;
    float y;
    float w;
    float h;
    RGBAColor color;
} Rectangle;

typedef struct {
    float x;
    float y;
    float r;
    RGBAColor color;
} Circle;

typedef struct {
    void ** content;
    int n;
} canvasContent;

container * newCanvas(fastUint16 w, fastUint16 h);
void ** addContent(container * c);

void drawRectangle(container * c, float x, float y, float w, float h, RGBAColor color);
void renderRectangle(Event event);

void drawCircle(container * c, float x, float y, float r, RGBAColor color);
void renderCircle(Event event);

void renderCanvas(Event event);
#endif