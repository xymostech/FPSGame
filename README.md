#Unnamed FPS Game V0.1
##First Person Shooter Game

A project to build an open-source FPS game. Currently being written in C, using
OpenGL for graphics.

### Building

Scons is the build system, so building is as easy as

    $ scons

The game is located at `build/game/FPS`.

To build everything, run:

    $ scons .

This builds the game, the server, and builds a Mac app.

### Usage

You need to have a server running to run the game. If you run the server
locally, you can run everything normally. Currently, customization of servers
isn't very easy, and has to be changed in-code. It's under `server.c`. This will
be changed soon.

Run game:

    $ ./build/game/FPS

Run server:

    $ ./build/serv/serv
