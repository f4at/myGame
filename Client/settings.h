#ifndef SETTINGS_H_
#define SETTINGS_H_

void Settings_Init();
char * getSetting(char *key);
void setSetting(char *key, char *value);
int getSettingInt(char *key);

#endif