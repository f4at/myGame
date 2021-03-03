#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "utils.h"

const SDL_Color white = {r:255,g:255,b:255};
const SDL_Color black = {r:0,g:0,b:0};

fastUint8 getbit(fastUint8 a, fastUint8 n) {
    return (a & ( 1 << n )) >> n;
}

void setbit(fastUint8 *a, fastUint8 n, fastUint8 b) {
    *a = (*a & ~(1UL << n)) | (1UL << n)*b;
}

void setbit16(fastUint16 *a, fastUint8 n, fastUint8 b) {
    *a = (*a & ~(1UL << n)) | (1UL << n)*b;
}

float min(float i1, float i2) {
    return i1 > i2 ? i2 : i1;
}

float max(float i1, float i2) {
    return i1 < i2 ? i2 : i1;
}

float degreeToRadian(float degree) {
    return degree/360*M_PI;
}

float radianToDegree(float radian) {
    return radian*M_PI/360;
}

float byteToDegree(fastUint8 byte) {
    return byte*360/255.0;
}

fastUint8 degreeToByte(float degree) {
    return round(degree*360/255);
}

vec2 vecRotate(vec2 p, float angle) {
    vec2 r;
    r.x = cosf(angle)*p.x-sinf(angle)*p.y;
    r.y = sinf(angle)*p.x+cosf(angle)*p.y;
    return r;
}
/*
Args:
p1: point in the line.
p2: another point in the line.
p0: the point.
*/
float DistanceBetweenLineAndPoint(vec2 p1, vec2 p2, vec2 p0) {
    float e=(p2.x-p1.x),e2=(p2.y-p1.y);
    return abs(e*(p1.y-p0.y)-(p1.x-p0.x)*e2)/sqrt(e*e+e2*e2);
}

char * split(char *s1, char *s2, fastUint8 x) {
    char *t = malloc(strlen(s1)+1), *rt, *r;
    strcpy(t,s1);
    rt = strtok(t, s2);
    while(x>0) {
        x--;
        if (t!=NULL) {
            rt = strtok(NULL, s2);
        } else {
            r = NULL;
            free(t);
            return r;
        }
    }
    r = malloc(strlen(rt)+1);
    strcpy(r,rt);
    free(t);
    return r;
}

int splitWithPosition(char *s1, char *s2, fastUint8 x, char **r) {
    fastUint16 l = strlen(s2);
    fastUint16 pos = x*l;
    char* t = malloc(l+1);
    strcpy(t,s1);
    char* rt;
    rt = strtok(t, s2);
    while(x>0) {
        x--;
        pos += strlen(rt);
        if (t != NULL) 
            rt = strtok(NULL, s2);
        else {
            *r = NULL;
            free(t);
            return -1;
        }
    }
    *r = malloc(strlen(rt)+1);
    strcpy(*r,rt);
    free(t);
    return pos;
}

char * strcut(char *s1, fastUint16 begin, fastUint16 len) {
    len = min(len,strlen(s1)-begin);
    char *r = malloc(len+1);
    for (fastUint16 i=0;i<len;i++) {
        r[i] = s1[begin+i];
    }
    r[len] = '\0';
    return r;
}

char * concate(char *s1, char *s2) {
    fastUint16 l1=strlen(s1), l2=strlen(s2);
    char *r = malloc(sizeof(char)*(l1+l2+1));
    for (int i=0;i<l1;i++) {
        r[i] = s1[i];
    }
    for (int i=0;i<l2;i++) {
        r[i+l1] = s2[i];
    }
    r[l1+l2] = '\0';
    return r;
}

char * copy(char *s1) {
    char * r = malloc(strlen(s1)+1);
    strcpy(r,s1);
    return r;
}

char *itoa(int n) {
    char *r = malloc(sizeof(char)*(n ? (int)log10(abs(n))+2+(n<0) : 2));
    sprintf(r, "%d", n);
    return r;
}