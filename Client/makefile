game: main.o game.o menu.o video.o audio.o settings.o utils.o buttons.o graphics.o events.o files.o fonts.o draw.o
	gcc -Og ./*.o -o game -lGL -lGLEW -lm -lSDL -lSDL_mixer -lSDL_image -lSDL_ttf -g
	rm ./*.o
main.o:main.c
	gcc -Og main.c -c -g
game.o:game.c
	gcc -Og game.c -c -g
menu.o:menu.c
	gcc -Og menu.c -c -g
video.o:video.c
	gcc -Og video.c -c -g
audio.o:audio.c
	gcc -Og audio.c -c -g
buttons.o:buttons.c
	gcc -Og buttons.c -c -g
graphics.o:graphics.c
	gcc -Og graphics.c -c -g
fonts.o:fonts.c
	gcc -Og fonts.c -c -g
events.o:events.c
	gcc -Og events.c -c -g
settings.o:settings.c
	gcc -Og settings.c -c -g
files.o:files.c
	gcc -Og files.c -c -g
utils.o:settings.c
	gcc -Og utils.c -c -g
draw.o:draw.c
	gcc -Og draw.c -c -g