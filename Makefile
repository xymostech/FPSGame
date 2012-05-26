CC=gcc

CFLAGS+=-Isrc/ `freetype-config --cflags`
LIBS+=`freetype-config --libs` -lpng -lz -lglfw

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

.PHONY: none mac linux
none:
	@echo "Use 'make mac' or 'make linux' to build"

.PHONY: clean
clean:
	rm -rf $(PROD)-* build/mac/* build/linux/*

#===== MAC BUILD =====#

MACOBJ=$(addprefix build/mac/,$(OBJ))

MACCFLAGS=$(CFLAGS)
MACLIBS=$(LIBS) -framework OpenGL -framework Cocoa -lm

mac: $(PROD)-mac

$(PROD)-mac: $(MACOBJ)
	$(CC) -o $@ $^ $(MACLIBS)

build/mac/%.o: src/%.c $(HEAD)
	$(CC) -c -o $@ $< $(MACCFLAGS)

#===== LINUX BUILD =====#

LINOBJ=$(addprefix build/linux/,$(OBJ))

LINCFLAGS=$(CFLAGS)
LINLIBS=$(LIBS) -lGL -lGLU -lm

linux: $(PROD)-linux

$(PROD)-linux: $(LINOBJ)
	$(CC) -o $@ $^ $(LINLIBS)

build/linux/%.o: src/%.c $(HEAD)
	$(CC) -c -o $@ $< $(LINCFLAGS)
