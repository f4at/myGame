#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settings.h"
#include "utils.h"

char * settings;
FILE * settingsFile;

char * getSetting(char *key) {
    char *h,*r;
    int i=0;
    do {
        h = split(settings,"\n",i);
        if (h!=NULL) {
            if (strcmp(split(h,":",0),key)==0) {
                r = split(h,":",1);
                free(h);
                return r;
            }
            free(h);
        } else {
            free(h);
            return NULL;
        }
        i++;
    } while(1);
}

int getSettingInt(char *key) {
    char * c = getSetting(key);
    if (c==NULL) {
        return -1;
    }
    int r = atoi(c);
    free(c);
    return r;
}

void setSetting(char *key, char *value) {
    char *h,*r,*o;
    fastUint8 i=0,klen=strlen(key);
    fastUint16 p,slen=strlen(settings);
    do {
        p = splitWithPosition(settings,"\n",i,&h);
        if (p != -1) {
            if (strcmp(split(h,":",0),key)==0) {
                r = strcut(settings,p+strlen(h),slen);
                free(h);
                h = strcut(settings,0,p+klen+1);
                o = concate(h,value);
                free(h);
                h = concate(o,r);
                free(r);
                free(settings);
                settings = h;
                settingsFile = fopen("data/settings.cfg","w");
                fwrite(settings,strlen(settings),1,settingsFile);
                fclose(settingsFile);
                break;
            }
            free(h);
        } else {
            free(h);
            break;
        }
    } while(1);
}

void Settings_Init() {
    settings = calloc(1024,1);
    settingsFile = fopen("data/settings.cfg","r");
    int l = fread(settings, 1024, 1, settingsFile);
    fclose(settingsFile);
}