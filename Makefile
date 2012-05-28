CC=gcc

CFLAGS+=-Isrc/ `freetype-config --cflags`

UNAME := $(shell $(CC) -dumpmachine 2>&1 | grep -E -o "linux|darwin|win")

LIBS=-lpng -lz -lglfw -lm `freetype-config --libs`
RELEASELIBS=libs/libfreetype.a -lbz2 libs/libpng15.a -lz libs/libglfw.a -lm

ifeq ($(UNAME), linux)
	LIBS+=-lGL -lGLU
	RELEASELIBS+=-lGL -lGLU
else ifeq ($(UNAME), darwin)
	LIBS+=-framework OpenGL -framework Cocoa
	RELEASELIBS+=-framework OpenGL -framework Cocoa
else
	$(error Cannot build on your OS)
endif

OBJ=game.o\
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

.PHONY: release
release: $(PROD)-release

$(PROD)-release: $(BOBJ)
	$(CC) -o $@ $^ $(RELEASELIBS)

.PHONY: release-app
release-app: $(PROD).app $(PROD).app.zip

$(PROD).app.zip: $(PROD).app
	zip -qr $@ $^

$(PROD).app: $(PROD)-release
	mkdir -p $@/Contents
	cp buildres/Info.plist $@/Contents/
	mkdir -p $@/Contents/MacOS
	cp $(PROD)-release $@/Contents/MacOS/$(PROD)
	mkdir -p $@/Contents/Resources
	cp -r res $@/Contents/Resources/

.PHONY: clean
clean:
	rm -rf $(PROD) $(PROD)-release $(PROD).app* build/*
