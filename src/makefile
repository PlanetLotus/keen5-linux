CC = g++
CFLAGS = -Wall -g
INCLUDES = -I/usr/local/include/SDL2 -D_REENTRANT
LFLAGS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib
LIBS = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lpthread
SRCS = globals.cpp Sprite.cpp Player.cpp Texture.cpp helpers.cpp main.cpp Timer.cpp Tile.cpp Controller.cpp BlasterShot.cpp TileCollisionInfo.cpp Camera.cpp
OBJS = $(SRCS:.cpp=.o)
PROG = keen5

.PHONY: depend clean

all: $(PROG)
	@echo keen5 has been compiled

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROG) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm *.o $(PROG)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
