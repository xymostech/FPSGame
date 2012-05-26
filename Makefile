CC=gcc

CFLAGS+=-Isrc/ `freetype-config --cflags`

UNAME := $(shell $(CC) -dumpmachine 2>&1 | grep -E -o "linux|darwin|win")

LIBS=-lpng -lz -lglfw -lm `freetype-config --libs`

ifeq ($(UNAME), linux)
	LIBS+=-lGL -lGLU
else ifeq ($(UNAME), darwin)
	LIBS+=-framework OpenGL -framework Cocoa
else
	$(error Cannot build on your OS)
endif

OBJ=main.o\
    key.o\
    mouse.o\
    window.o\
    cam.o\
    player.o\
    world.o\
    model.o\
    polygon.o\
    hud.o\
    font.o\
    texture.o\
    vector.o\
    server.o\
    data.o
HEAD=$(wildcard src/*.h)

PROD=FPS

BOBJ=$(addprefix build/,$(OBJ))

$(PROD): $(BOBJ)
	$(CC) -o $@ $^ $(LIBS)

build/%.o: src/%.c $(HEAD)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(PROD) build/*
