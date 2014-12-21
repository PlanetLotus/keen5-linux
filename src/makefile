CC = g++
CFLAGS = -Wall -g
INCLUDES = -I/usr/local/include/SDL2 -D_REENTRANT
LFLAGS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib
LIBS = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lpthread
SRCS = globals.cpp Sprite.cpp MovingSprite.cpp Item.cpp Player.cpp Texture.cpp helpers.cpp main.cpp Timer.cpp Tile.cpp Controller.cpp BlasterShot.cpp TileCollisionInfo.cpp Camera.cpp Sparky.cpp Ampton.cpp Platform.cpp
OBJDIR = ../obj
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))
PROG = keen5

COMPILE_START_TIME := $(shell date +\(%j\*24\*60\*60+%H\*60\*60+%M\*60+%S\))
COMPILE_CURRENT_TIME = `date  +\(%j\*24\*60\*60+%H\*60\*60+%M\*60+%S\)`

define print_timer
@CT=$(1) && \
	printf "Compilation total time: %.2d:%.2d (min:sec)\n" \
	`echo "($$CT - $(2))/60" | bc` \
	`echo "($$CT - $(2))%60" | bc`
endef

.PHONY: depend clean

all: $(OBJDIR) $(PROG)
	$(call print_timer,$(COMPILE_CURRENT_TIME),$(COMPILE_START_TIME))

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROG) $(OBJ) $(LFLAGS) $(LIBS)

clean:
	@rm $(PROG)
	@rm -rf $(OBJDIR)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
