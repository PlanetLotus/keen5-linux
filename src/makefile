PROG = keen5
OBJS = globals.o Player.o Texture.o helpers.o main.o Timer.o
FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

$(PROG): $(OBJS)
	g++ -g -Wall -o $(PROG) $(OBJS) $(FLAGS)

clean:
	rm *.o keen5

# Dependencies
globals.o: globals.h
Player.o: Player.h
Texture.o: Texture.h
helpers.o: helpers.h
Timer.o: Timer.h
