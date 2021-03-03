#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "graphics.h"
#include "video.h"
#include "fonts.h"


fastUint16 declaredContainers = 0;
container * Context;
fastUint8 Events_Render;

fastUint16 currentTime,lastRenderTime;

RGBAColor Tint={r:1,g:1,b:1,a:1};

Shader initShader(GLenum type, const char *filename) {
    GLuint shader = glCreateShader(type);
    FILE * f = fopen(filename,"r");  
    if (f != NULL) {
        char * shaderIn = NULL, c;    
        int i = 0;
        while (1) {
            c = fgetc(f);
            if (c == EOF) {
                break;
            } else {
                shaderIn = realloc(shaderIn,++i);
                shaderIn[i-1] = c;
            }
        }

        const char * glShaderIn = shaderIn;
        glShaderSource(shader,1,&glShaderIn,NULL);
        glCompileShader(shader);

        GLint compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            printf("Can't compile shader.\n");
            exit(1);
        }
        free(shaderIn);
    } else {
        printf("Can't load shader file.\n");
        exit(1);
    }
    return shader;
}

Program initProram(const char *vertfilename, const char *fragfilename) {
    Program program = glCreateProgram();

    Shader fragShader = initShader(GL_FRAGMENT_SHADER,fragfilename);
    Shader vertShader = initShader(GL_VERTEX_SHADER,vertfilename);
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    
    GLint program_linked;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &program_linked);
    if (!program_linked) {
        printf("Error program not linked.");
        exit(1);
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    return program;
}

void changeShader(container* c, EventCallback e) {
    c->callbacks[Events_RenderContent].callbacks[0] = e;
}

RGBAColor glGetTint() {
    return Tint;
}

void glTintf(RGBAColor tint,fastUint8 r) {
    if (r) {
        Tint = tint;
    } else {
        Tint.r*=tint.r;
        Tint.g*=tint.g;
        Tint.b*=tint.b;
        Tint.a*=tint.a;
    }
    glColor4f(Tint.r, Tint.g, Tint.b, Tint.a);
}

Texture loadTexture(char* c) {
    SDL_Surface* s = IMG_Load(c);
    return surfaceToTexture(s);
}

Texture surfaceToTexture(SDL_Surface *s) {
    Texture texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    int format;
    if (s->format->BytesPerPixel == 4) {
        if (s->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
            format = GL_BGRA;
    } else if (s->format->BytesPerPixel == 3) {
        if (s->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
    } else {
        return 0;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, s->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, s->w, s->h, 0, format, GL_UNSIGNED_BYTE, s->pixels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SDL_FreeSurface(s);
    return texId;
}

void updateTexture(SDL_Surface *s, Texture texId) {
    glBindTexture(GL_TEXTURE_2D, texId);
    int format;
    if (s->format->BytesPerPixel == 4) {
        if (s->format->Rmask == 0x000000ff)
            format = GL_RGBA;
        else
            format = GL_BGRA;
    } else if (s->format->BytesPerPixel == 3) {
        if (s->format->Rmask == 0x000000ff)
            format = GL_RGB;
        else
            format = GL_BGR;
    } else {
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, s->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, s->w, s->h, 0, format, GL_UNSIGNED_BYTE, s->pixels);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    SDL_FreeSurface(s);
}

container * newContainer() {
    container * c = malloc(sizeof(container));
    *c = (container){type:Container,id:declaredContainers++,content:NULL,pos:{x:0,y:0},size:{x:0,y:0},scale:{x:1,y:1},angle:0,tint:{r:1,g:1,b:1,a:1},visible:1,nChilds:0,interactive:0};
    addCallback(c->callbacks,Events_RenderContainer,&renderContainer);
    return c;
}

container * newSprite(Texture texture) {
    int w,h;
    glBindTexture(GL_TEXTURE_2D,texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    container * c = malloc(sizeof(container));
    Texture * tex = malloc(sizeof(Texture)); *tex = texture;
    *c = (container){type:Sprite,id:declaredContainers++,content:tex,pos:{x:0,y:0},size:{x:w,y:h},scale:{x:1,y:1},angle:0,tint:{r:1,g:1,b:1,a:1},visible:1,nChilds:0,interactive:0};
    addCallback(c->callbacks,Events_RenderContainer,&renderSprite);
    addCallback(c->callbacks,Events_RenderContent,&renderTexture);
    return c;
}

container * newCamera() {
    container * c = malloc(sizeof(container));
    Texture * tex = malloc(sizeof(Texture));
    *c = (container){type:Camera,id:declaredContainers++,content:NULL,pos:{x:0,y:0},size:{x:0,y:0},scale:{x:1,y:1},angle:0,tint:{r:1,g:1,b:1,a:1},visible:1,nChilds:0,interactive:0};
    addCallback(c->callbacks,Events_RenderContainer,&renderCamera);
    return c;
}

container * newFrameBuffer(int w, int h) {
    container * c = malloc(sizeof(container));
    *c = (container){type:FrameBuffer,id:declaredContainers++,content:NULL,pos:{x:0,y:0},size:{x:w,y:h},scale:{x:1,y:1},angle:0,tint:{r:1,g:1,b:1,a:1},visible:1,nChilds:0,interactive:0};
    addCallback(c->callbacks,Events_RenderContainer,&renderContainer);
    return c;
}

void addChild(container *parent, container *child) {
    parent->childs = realloc(parent->childs,sizeof(container *)*++parent->nChilds);
    parent->childs[parent->nChilds-1] = child;
}

void renderTexture(Event event) {
    container object =  *(container *)event->user.data1;
    glBindTexture(GL_TEXTURE_2D, *(Texture*)object.content);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glVertex2f(-object.size.x/2,-object.size.y/2);
    glTexCoord2f(1,0);
    glVertex2f(object.size.x/2,-object.size.y/2);
    glTexCoord2f(1,1);
    glVertex2f(object.size.x/2,object.size.y/2);
    glTexCoord2f(0,1);
    glVertex2f(-object.size.x/2,object.size.y/2);
    glEnd();
    glFlush();
}

void renderSprite(Event event) {
    RGBAColor oTint = Tint;
    container object = *(container*)event->user.data1;
    glTranslatef(object.pos.x,object.pos.y,0);
    glRotatef(object.angle,0,0,1);
    glScaled(object.scale.x,object.scale.y,1);
    glTintf(object.tint,0);
    event->user.code = Events_RenderContent;
    pushCustom(object.callbacks,event);
    event->user.code = Events_RenderContainer;
    for (int i=0;i<object.nChilds;i++) {
        if (object.childs[i]->visible) {
            event->user.data1 = object.childs[i];
            pushCustom(object.childs[i]->callbacks,event);
        }
    }
    glTintf(oTint,1);
    glScaled(1/object.scale.x,1/object.scale.y,1);
    glRotatef(-object.angle,0,0,1);
    glTranslatef(-object.pos.x,-object.pos.y,0);
}

void renderContainer(Event event) {
    RGBAColor oTint = Tint;
    container object = *(container*)event->user.data1;
    glTranslatef(object.pos.x,object.pos.y,0);
    glRotatef(object.angle,0,0,1);
    glScaled(object.scale.x,object.scale.y,1);
    glTintf(object.tint,0);
    for (int i=0;i<object.nChilds;i++) {
        if (object.childs[i]->visible) {
            event->user.data1 = object.childs[i];
            pushCustom(object.childs[i]->callbacks,event);
        }
    }
    glTintf(oTint,1);
    glScaled(1/object.scale.x,1/object.scale.y,1);
    glRotatef(-object.angle,0,0,1);
    glTranslatef(-object.pos.x,-object.pos.y,0);
}

void renderCamera(Event event) {
    RGBAColor oTint = Tint;
    container object = *(container*)event->user.data1;
    glTranslatef(-object.pos.x,-object.pos.y,0);
    glRotatef(-object.angle,0,0,1);
    glScaled(1/object.scale.x,1/object.scale.y,1);
    glTintf(object.tint,0);
    for (int i=0;i<object.nChilds;i++) {
        if (object.childs[i]->visible) {
            event->user.data1 = object.childs[i];
            pushCustom(object.childs[i]->callbacks,event);
        }
    }
    glTintf(oTint,1);
    glScaled(object.scale.x,object.scale.y,1);
    glRotatef(object.angle,0,0,1);
    glTranslatef(object.pos.x,object.pos.y,0);
}

Event containerRenderEvent;
void renderGame() {
    currentTime = SDL_GetTicks();
    if (currentTime-lastRenderTime >= 1000/(float)maxFps) {
        glClear(GL_COLOR_BUFFER_BIT);
        containerRenderEvent->user.data1 = Context;
        pushCustom(Context->callbacks,containerRenderEvent);
        SDL_GL_SwapBuffers();
        lastRenderTime = currentTime;
    } else {
        SDL_Delay(1000/maxFps+lastRenderTime-currentTime);
    }
}

void move(container *object, float x, float y) {
    object->pos.x = x;
    object->pos.y = y;
}

void translate(container *object, float x, float y) {
    object->pos.x += x;
    object->pos.y += y;
}

void rotate(container *object, float angle) {
    object->angle = angle;
}

void scale(container *object, float f) {
    object->scale.x = f;
    object->scale.y = f;
}

void scaleXY(container *object, float x, float y) {
    object->scale.x = x;
    object->scale.y = y;
}

void setTint(container *object, float r, float g, float b, float a) {
    object->tint.r = r;
    object->tint.g = g;
    object->tint.b = b;
    object->tint.a = a;
}

void multiplyTint(container *object, float r, float g, float b, float a) {
    object->tint.r *= r;
    object->tint.g *= g;
    object->tint.b *= b;
    object->tint.a *= a;
}

void addTint(container *object, float r, float g, float b, float a) {
    object->tint.r += r;
    object->tint.g += g;
    object->tint.b += b;
    object->tint.a += a;
}

Texture textTexture(char *c, char *font, fastUint16 size, SDL_Color color) {
    TTF_Font *ttf = loadTTF(font,size);
    SDL_Surface *s = TTF_RenderText_Blended(ttf, c, color);
    TTF_CloseFont(ttf);
    return surfaceToTexture(s);
}

void updateTextTexture(Texture tex, char *c, char *font, fastUint16 size, SDL_Color color) {
    TTF_Font *ttf = loadTTF(font,size);
    SDL_Surface *s = TTF_RenderText_Blended(ttf, c, color);
    TTF_CloseFont(ttf);
    updateTexture(s, tex);
}

void updateTextContainer(container *object,char *c, char *font, fastUint16 size, SDL_Color color) {
    updateTextTexture(*(Texture *)object->content,c,font,size,color);

    int h,w;
    glBindTexture(GL_TEXTURE_2D,*(Texture *)object->content);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
    object->size.x = w;
    object->size.y = h;
}

void Graphics_Init() {
    Events_Render = addEventType();
    addCallback(EventsCallbacks,Events_Render,&renderGame);

    containerRenderEvent = newEvent();
    containerRenderEvent->type = SDL_USEREVENT;
    containerRenderEvent->user.code = Events_RenderContainer;

    Context = newContainer();
    Context->interactive = 1;

    lastRenderTime = SDL_GetTicks();
}

void Graphics_Quit() {
    free(containerRenderEvent);
}   
