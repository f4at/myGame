#include <math.h>
#include "draw.h"
#include "graphics.h"
#include "events.h"

container * newCanvas(fastUint16 w, fastUint16 h) {
    container * c = malloc(sizeof(container));
    canvasContent * content = malloc(sizeof(canvasContent));
    content->content = NULL;
    content->n = 0;
    *c = (container){type:Canvas,id:declaredContainers++,content:content,pos:{x:0,y:0},size:{x:w,y:h},scale:{x:1,y:1},angle:0,tint:{r:1,g:1,b:1,a:1},visible:1,nChilds:0,interactive:0};
    addCallback(c->callbacks,Events_RenderContainer,&renderCanvas);
    return c;
}

void **addContent(container * c) {
    canvasContent * content = c->content;
    content->content = realloc(content->content,sizeof(void *)*++content->n);
    return content->content+content->n-1;
}

void * getContent(container * c, int i) {
    return ((canvasContent *)c->content)->content[i];
}


void drawRectangle(container * c, float x, float y, float w, float h, RGBAColor color) {
    Rectangle **rContent = (Rectangle **)addContent(c);
    *rContent = malloc(sizeof(Rectangle));
    **rContent = (Rectangle){x:x,y:y,w:w,h:h,color:color};

    addCallback(c->callbacks,Events_RenderContent,&renderRectangle);
}

void renderRectangle(Event event) {
    RGBAColor oTint = glGetTint();
    Rectangle rContent = *(Rectangle *)getContent(event->user.data1,*(int *)event->user.data2);
    glTintf(rContent.color,0);
    glBegin(GL_QUADS);
    glVertex2f(rContent.x-rContent.w/2,rContent.y-rContent.h/2);
    glVertex2f(rContent.x+rContent.w/2,rContent.y-rContent.h/2);
    glVertex2f(rContent.x+rContent.w/2,rContent.y+rContent.h/2);
    glVertex2f(rContent.x-rContent.w/2,rContent.y+rContent.h/2);
    glEnd();
    glFlush();
    glTintf(oTint,1);
}

void drawCircle(container * c, float x, float y, float r, RGBAColor color) {
    Circle **rContent = (Circle **)addContent(c);
    *rContent = malloc(sizeof(Circle));
    **rContent = (Circle){x:x,y:y,r:r,color:color};

    addCallback(c->callbacks,Events_RenderContent,&renderCircle);
}

void renderCircle(Event event) {
    RGBAColor oTint = glGetTint();
    Circle rContent = *(Circle *)getContent(event->user.data1,*(int *)event->user.data2);
    glTintf(rContent.color,0);

    GLfloat twicePi = 2.0f * M_PI;
    GLuint accruacy = ceil(256*rContent.r);
    
	glBegin(GL_TRIANGLE_FAN);
    glVertex2f(rContent.x, rContent.y);
    for(int i = 0; i <= accruacy;i++) { 
        glVertex2f(rContent.x+(rContent.r*cos(i*twicePi/accruacy)), rContent.y+(rContent.r*sin(i*twicePi/accruacy)));
    }
	glEnd();
    glFlush();
    glTintf(oTint,1);
}


void renderCanvas(Event event){
    RGBAColor oTint = glGetTint();
    container object = *(container*)event->user.data1;
    glTranslatef(object.pos.x,object.pos.y,0);
    glRotatef(object.angle,0,0,1);
    glScaled(object.scale.x,object.scale.y,1);
    glTintf(object.tint,0);
    glBindTexture(GL_TEXTURE_2D,0);
    for (int i=0;i<object.callbacks[Events_RenderContent].n;i++) {
        event->user.data2 = &i;
        object.callbacks[Events_RenderContent].callbacks[i](event);
    }
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
