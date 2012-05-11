CC=gcc

CFLAGS+=-Isrc/
LFLAGS+=-lGL -lGLU -lglfw -lm

OBJ=main.o\
    key.o\
    mouse.o\
    window.o
BOBJ=$(addprefix build/,$(OBJ))
HEAD=$(wildcard src/*.h)

PROD=FPS

$(PROD): $(BOBJ)
	$(CC) -o $@ $^ $(LFLAGS)

build/%.o: src/%.c $(HEAD)
	$(CC) -c -o $@ $< $(CFLAGS)
