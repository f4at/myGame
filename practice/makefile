game: main.o file.o
	gcc ./*.o -o game -lSDL -lSDL_mixer -lSDL_image -lSDL_ttf
	rm ./*.o
main.o: main.c
	gcc -c main.c
file.o: file.c
	gcc -c file.c