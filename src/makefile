CC = g++ -std=c++11
CFLAGS = -Wall -g
INCLUDES = -I/usr/local/include/SDL2 -D_REENTRANT -MMD
LFLAGS = -L/usr/local/lib -Wl,-rpath,/usr/local/lib
LIBS = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lpthread
SRCS = $(shell find . -name "*.cpp")
OBJDIR = ../obj
OBJ = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))
PROG = keen5
NODEPS := clean

COMPILE_START_TIME := $(shell date +\(%j\*24\*60\*60+%H\*60\*60+%M\*60+%S\))
COMPILE_CURRENT_TIME = `date  +\(%j\*24\*60\*60+%H\*60\*60+%M\*60+%S\)`

define print_timer
@CT=$(1) && \
	printf "Compilation total time: %.2d:%.2d (min:sec)\n" \
	`echo "($$CT - $(2))/60" | bc` \
	`echo "($$CT - $(2))%60" | bc`
endef

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
	-include $(OBJ:%.o=%.d)
endif

# I don't know why this defaults to ../obj/Ampton.o (the first .o dependency, but ONLY the first) instead of 'all'
.DEFAULT_GOAL := all

.PHONY: all depend clean

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
