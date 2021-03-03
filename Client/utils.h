#ifndef UTILS_H_
#define UTILS_H_

#include <GL/glew.h>
#include "SDL/SDL.h"

extern const SDL_Color white;
extern const SDL_Color black;

typedef struct {
    float x;
    float y;
} vec2;

typedef struct {
    float r;
    float g;
    float b;
    float a;
} RGBAColor;

typedef uint_fast8_t fastUint8;
typedef uint_fast16_t fastUint16;
typedef uint_fast32_t fastUint32;

fastUint8 getbit(fastUint8 a, fastUint8 n);
void setbit(fastUint8 *a, fastUint8 n, fastUint8 b);
void setbit16(fastUint16 *a, fastUint8 n, fastUint8 b);

float min(float i1, float i2);
float max(float i1, float i2);

float degreeToRadian(float degree);
float radianToDegree(float radian);
float byteToDegree(fastUint8 byte);
fastUint8 degreeToByte(float degree);
float DistanceBetweenLineAndPoint(vec2 p1, vec2 p2, vec2 p0);
vec2 vecRotate(vec2 p, float angle);

char * split(char *s1, char *s2, fastUint8 x);
int splitWithPosition(char *s1, char *s2, fastUint8 x, char **r);
char * strcut(char *s1, fastUint16 begin, fastUint16 len);
char * concate(char *s1, char *s2);
char * copy(char *s1);
char * itoa(int n);
#endif