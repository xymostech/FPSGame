CC=gcc

CFLAGS+=-Isrc/
LFLAGS+=

OBJ=main.o\
    key.o\
    mouse.o\
    window.o
HEAD=$(wildcard src/*.h)

PROD=FPS

.PHONY: none mac linux
none:
	@echo "Use 'make mac' or 'make linux' to build"

.PHONY: clean
clean:
	rm -rf $(PROD) build/*

#===== MAC BUILD =====#

MACOBJ=$(addprefix build/mac/,$(OBJ))

MACCFLAGS=$(CLFAGS)
MACLFLAGS=$(LFLAGS) -framework OpenGL -framework Cocoa -lglfw -lm

mac: $(PROD)-mac

$(PROD)-mac: $(MACOBJ)
	$(CC) -o $@ $^ $(MACLFLAGS)

build/mac/%.o: src/%.c $(HEAD)
	$(CC) -c -o $@ $< $(MACCFLAGS)

#===== LINUX BUILD =====#

LINOBJ=$(addprefix build/linux/,$(OBJ))

MACCFLAGS=$(CLFAGS)
MACLFLAGS=$(LFLAGS) -lGL -lGLU -lglfw -lm

linux: $(PROD)-linux

$(PROD)-linux: $(LINOBJ)
	$(CC) -o $@ $^ $(MACLFLAGS)

build/linux/%.o: src/%.c $(HEAD)
	$(CC) -c -o $@ $< $(MACCFLAGS)
