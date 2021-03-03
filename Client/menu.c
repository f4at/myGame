#include <stdlib.h>
#include "SDL/SDL.h"
#include "video.h"
#include "settings.h"
#include "buttons.h"
#include "menu.h"
#include "game.h"
#include "utils.h"
#include "draw.h"

container * menu;
container * settingsWindow;
container * background;
container * menuButtons;
container * optionsButton;
container * quitButton;
container * playButton;
container * closeButton;
container * fullscreenButton;
container * musicVolumeController;
container * GFXVolumeController;
Mix_Chunk * menuButtonsSound;
Mix_Music * menuMusic;

Texture closeButtonTexture;
Texture plusButton,minusButton;

const SDL_Color shadowColor = (SDL_Color){r:240,g:230,b:220};
const SDL_Color FontColor = (SDL_Color){r:16,g:16,b:16};
const SDL_Color optionsTextColor = (SDL_Color){r:0,g:0,b:0};

fastUint8 selected = 0;
Event keyboard_event;
void menuKeyboardHandler(Event event) {
    if (menu->visible) {
        if (menuButtons->interactive) {
            if (event->key.keysym.sym == SDLK_UP) {
                container * c = menuButtons->childs[selected > 1? selected - 2 : 2];
                keyboard_event->user.code = Events_MouseIn;
                keyboard_event->user.data2 = c;
                pushCustom(c->callbacks,keyboard_event);
            } else if (event->key.keysym.sym == SDLK_DOWN) {
                container * c = menuButtons->childs[selected == 3 ? 0 : selected];
                keyboard_event->user.code = Events_MouseIn;
                keyboard_event->user.data2 = c;
                pushCustom(c->callbacks,keyboard_event);
            } else if (event->key.keysym.sym == SDLK_RETURN && selected) {
                container * c = menuButtons->childs[selected-1];
                keyboard_event->user.code = Events_Activate;
                keyboard_event->user.data2 = c;
                pushCustom(c->callbacks,keyboard_event);
            }
        } else if (event->key.keysym.sym == SDLK_RETURN) {
            close_settings();
        }
        
    }
}

void open_settings() {
    menuButtons->interactive = 0;
    settingsWindow->visible = 1;
    Mix_VolumeMusic(musicVolume/4);
}

void close_settings() {
    menuButtons->interactive = 1;
    settingsWindow->visible = 0;
    Mix_VolumeMusic(musicVolume);
}

void menuButtonsInCallback(Event event) {
    defaultInCallback(event);
    playChunk(menuButtonsSound,1);
}

void menuButtonsInCallback_(Event event) {
    defaultInCallback(event);
    playChunk(menuButtonsSound,1);
    translate(event->user.data2,8,0);

    if (selected) {
        keyboard_event->user.code = Events_MouseOut;
        keyboard_event->user.data2 = menuButtons->childs[selected -1];
        defaultOutCallback(keyboard_event);
        translate(keyboard_event->user.data2,-8,0);
    }
}

void menuButtonsOutCallback_() {
}

void menuButtonsInCallback_1(Event event) {
    if (selected != 1) {
        menuButtonsInCallback_(event);
        selected = 1;
    }
}

void menuButtonsInCallback_2(Event event) {
    if (selected != 2) {
        menuButtonsInCallback_(event);
        selected = 2;
    }
}

void menuButtonsInCallback_3(Event event) {
    if (selected != 3) {
        menuButtonsInCallback_(event);
        selected = 3;
    }
}

void fullscreenButtonCallback() {
    if (Video_Flags & SDL_FULLSCREEN) {
        Video_Fullscreen(0);
        updateTextContainer(fullscreenButton->childs[0],"Fullscreen: Off","assets/fonts/arial-bold.ttf",36,optionsTextColor);
    } else {
        Video_Fullscreen(1);
        updateTextContainer(fullscreenButton->childs[0],"Fullscreen: On","assets/fonts/arial-bold.ttf",36,optionsTextColor);
    }
    Event event = newEvent();
    event->type = SDL_VIDEORESIZE;
    event->resize.h = resolutionHeight;
    event->resize.w = resolutionWidth;
    push(event);
    free(event);
}

void gfxPlusButtonCallback() {
    setGFXVolume(min(100,GFXVolume+5));
    char *v = itoa(GFXVolume);char *t = concate("GFX Volume: ",v);free(v);
    updateTextContainer(GFXVolumeController->childs[0],t,"assets/fonts/arial-bold.ttf",34,optionsTextColor);free(t);
}

void gfxMinusButtonCallback() {
    setGFXVolume(max(0,GFXVolume-5));
    char *v = itoa(GFXVolume);char *t = concate("GFX Volume: ",v);free(v);
    updateTextContainer(GFXVolumeController->childs[0],t,"assets/fonts/arial-bold.ttf",34,optionsTextColor);free(t);
}

void musicPlusButtonCallback() {
    musicVolume = min(100,musicVolume+5);
    char *v = itoa(musicVolume);char *t = concate("Music Volume: ",v);free(v);
    updateTextContainer(musicVolumeController->childs[0],t,"assets/fonts/arial-bold.ttf",32,optionsTextColor);free(t);
    Mix_VolumeMusic(musicVolume/4);
}

void musicMinusButtonCallback() {
    musicVolume = max(0,musicVolume-5);
    char *v = itoa(musicVolume);char *t = concate("Music Volume: ",v);free(v);
    updateTextContainer(musicVolumeController->childs[0],t,"assets/fonts/arial-bold.ttf",32,optionsTextColor);free(t);
    Mix_VolumeMusic(musicVolume/4);
}

Shader bgProgram;
GLuint VAO,VBO,EBO;

void drawBg() {
    glUseProgram(bgProgram);
    glBindVertexArray(VAO);
    
    glBindTexture(GL_TEXTURE_2D, *(Texture*)background->content);
    glUniform1f(glGetUniformLocation(bgProgram, "time"),SDL_GetTicks());
    glUniform1f(glGetUniformLocation(bgProgram, "ourTexture"),0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void mainMenu_Init() {
    bgProgram = initProram("assets/shaders/defaultVert.glsl","assets/shaders/defaultFrag.glsl");
    glBindAttribLocation(bgProgram, 0, "vertexBuf");
    
    float vertex_data[] = {
        1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    background = newSprite(loadTexture("assets/imgs/bg.png"));
    addChild(menu,background);
    changeShader(background, &drawBg);
    
    menuButtons = newContainer();
    menuButtons->interactive = 1;
    setTint(menuButtons,0.9,0.9,0.9,1);
    addChild(menu, menuButtons);

    playButton = newCanvas(152,60);
    drawCircle(playButton,-45,0,30,(RGBAColor){r:1,g:1,b:1,a:1});
    drawCircle(playButton,45,0,30,(RGBAColor){r:1,g:1,b:1,a:1});
    drawRectangle(playButton,0,0,90,60,(RGBAColor){r:1,g:1,b:1,a:1});

    addChild(playButton,newSprite(textTexture("Play","assets/fonts/arial-bold.ttf",44,FontColor)));
    addChild(menuButtons, playButton);
    toButton(playButton,NULL,&openGame,NULL,NULL,&menuButtonsInCallback_1,&menuButtonsOutCallback_,NULL);

    optionsButton = newCanvas(192,50);
    drawCircle(optionsButton,-70,0,25,(RGBAColor){r:1,g:1,b:1,a:1});
    drawCircle(optionsButton,70,0,25,(RGBAColor){r:1,g:1,b:1,a:1});
    drawRectangle(optionsButton,0,0,140,50,(RGBAColor){r:1,g:1,b:1,a:1});

    addChild(optionsButton,newSprite(textTexture("Options","assets/fonts/arial-bold.ttf",40,FontColor)));
    addChild(menuButtons, optionsButton);
    toButton(optionsButton,NULL,&open_settings,NULL,NULL,&menuButtonsInCallback_2,&menuButtonsOutCallback_,NULL);

    quitButton = newCanvas(128,50);
    drawCircle(quitButton,-38,0,25,(RGBAColor){r:1,g:1,b:1,a:1});
    drawCircle(quitButton,38,0,25,(RGBAColor){r:1,g:1,b:1,a:1});
    drawRectangle(quitButton,0,0,76,50,(RGBAColor){r:1,g:1,b:1,a:1});

    addChild(quitButton,newSprite(textTexture("Quit","assets/fonts/arial-bold.ttf",40,FontColor)));
    addChild(menuButtons, quitButton);
    toButton(quitButton,NULL,&Quit_game,NULL,NULL,&menuButtonsInCallback_3,&menuButtonsOutCallback_,NULL);
}

void settingsMenu_Init() {
    plusButton = loadTexture("assets/imgs/plusButton.png");
    minusButton = loadTexture("assets/imgs/minusButton.png");
    closeButtonTexture = loadTexture("assets/imgs/closeButton.png");
    Texture optionsButton = loadTexture("assets/imgs/optionsButton2.png");
    
    //settingsWindow = newCanvas(800,600);
    //drawRectangle(settingsWindow,0,0,800,600,(RGBAColor){r:0.5,g:0.5,b:0.5,a:0.5});
    settingsWindow = newSprite(loadTexture("assets/imgs/optionsWindow2.png"));
    addChild(menu, settingsWindow);
    settingsWindow->visible = 0;
    settingsWindow->interactive = 1;
    
    closeButton = newSprite(closeButtonTexture);
    addChild(settingsWindow,closeButton);
    scale(closeButton,0.25);
    toButton(closeButton,NULL,&close_settings,NULL,NULL,&menuButtonsInCallback,NULL,NULL);
    move(closeButton,(settingsWindow->size.x-closeButton->size.x*0.4)/2,(closeButton->size.y*0.4-settingsWindow->size.y)/2);

    fullscreenButton = newSprite(optionsButton);
    container * fullscreenText = newSprite(textTexture(Video_Flags&SDL_FULLSCREEN ? "Fullscreen: On" : "Fullscreen: Off","assets/fonts/arial-bold.ttf",36,optionsTextColor));
    addChild(fullscreenButton,fullscreenText);
    addChild(settingsWindow,fullscreenButton);
    toButton(fullscreenButton,NULL,&fullscreenButtonCallback,NULL,NULL,&menuButtonsInCallback,NULL,NULL);
    move(fullscreenButton,0,-70);

    GFXVolumeController = newSprite(optionsButton); GFXVolumeController->interactive = 1;
    char *v = itoa(GFXVolume);char *t = concate("GFX Volume: ",v);free(v);
    container * gfxText = newSprite(textTexture(t,"assets/fonts/arial-bold.ttf",34,optionsTextColor));free(t);
    container * gfxminusButton = newSprite(minusButton);
    container * gfxplusButton = newSprite(plusButton);
    move(gfxminusButton,-0.5*(GFXVolumeController->size.x+gfxminusButton->size.x)-5,0);
    move(gfxplusButton,0.5*(GFXVolumeController->size.x+gfxplusButton->size.x)+5,0);
    addChild(GFXVolumeController,gfxText);
    addChild(GFXVolumeController,gfxminusButton);
    addChild(GFXVolumeController,gfxplusButton);
    addChild(settingsWindow,GFXVolumeController);
    move(GFXVolumeController,0,20);
    setTint(GFXVolumeController,0.9,0.9,0.9,1);
    toButton(gfxminusButton,NULL,&gfxMinusButtonCallback,NULL,NULL,&menuButtonsInCallback,NULL,defaultDeselectCallback);
    toButton(gfxplusButton,NULL,&gfxPlusButtonCallback,NULL,NULL,&menuButtonsInCallback,NULL,defaultDeselectCallback);

    musicVolumeController = newSprite(optionsButton); musicVolumeController->interactive = 1;
    v = itoa(musicVolume);t = concate("Music Volume: ",v);free(v);
    container * musicText = newSprite(textTexture(t,"assets/fonts/arial-bold.ttf",32,optionsTextColor));free(t);
    container * musicminusButton = newSprite(minusButton);
    container * musicplusButton = newSprite(plusButton);
    move(musicminusButton,-0.5*(musicVolumeController->size.x+musicminusButton->size.x)-5,0);
    move(musicplusButton,0.5*(musicVolumeController->size.x+musicplusButton->size.x)+5,0);
    addChild(musicVolumeController,musicText);
    addChild(musicVolumeController,musicminusButton);
    addChild(musicVolumeController,musicplusButton);
    addChild(settingsWindow,musicVolumeController);
    move(musicVolumeController,0,110);
    setTint(musicVolumeController,0.9,0.9,0.9,1);
    toButton(musicminusButton,NULL,&musicMinusButtonCallback,NULL,NULL,&menuButtonsInCallback,NULL,defaultDeselectCallback);
    toButton(musicplusButton,NULL,&musicPlusButtonCallback,NULL,NULL,&menuButtonsInCallback,NULL,defaultDeselectCallback);
}

void Menu_Init() {
    addCallback(EventsCallbacks,Events_GameResize,&Menu_Resize);
    menuButtonsSound = loadChunk("assets/sounds/button-1.wav");
    menuMusic = loadMusic("assets/sounds/music.wav");
    Mix_PlayMusic(menuMusic,-1);
    menu = newContainer();
    menu->interactive = 1;
    addChild(Context,menu);
    mainMenu_Init();
    settingsMenu_Init();
    Menu_Resize();

    addCallback(EventsCallbacks,SDL_KEYDOWN,&menuKeyboardHandler);
    keyboard_event = newEvent();
    keyboard_event->type = SDL_USEREVENT;
}

void Menu_Quit() {
    free(keyboard_event);
}

void Menu_Resize() {
    scale(background,max(gameWidth/background->size.x,gameHeight/background->size.y));
    move(background,gameWidth/2,gameHeight/2);

    move(playButton,playButton->size.x/2+25,gameHeight-quitButton->size.y*2.5-140);
    move(optionsButton,optionsButton->size.x/2+25,gameHeight-quitButton->size.y*1.5-70);
    move(quitButton,quitButton->size.x/2+25,gameHeight-quitButton->size.y/2-50);

    if (selected) {
        keyboard_event->user.code = Events_MouseOut;
        keyboard_event->user.data2 = menuButtons->childs[selected -1];
        defaultOutCallback(keyboard_event);
        selected = 0;
    }
    
    move(settingsWindow,gameWidth/2,gameHeight/2);
}