![build](https://github.com/mattiasgustavsson/newpixie/actions/workflows/main.yml/badge.svg)

Pixie Game Engine
=================

Experimenting with new version of my game engine Pixie


Build instructions
------------------

### Windows
From a Visual Studio command prompt, go to the `runtime` folder and do
```
cl ..\source\main.c
```

### Mac
Go to the `runtime` folder and do
```
clang ../source/main.c -lSDL2 -lGLEW -framework OpenGL
```

SDL2 and GLEW are required - if you don't have then installed you can do so by running
```
brew install sdl2 glew
```

### Linux
Go to the `runtime` folder and do
```
gcc ../source/main.c -lSDL2 -lGLEW -lGL -lm -lpthread
```

SDL2 and GLEW are required - if you don't have them installed you can do so by running
```
sudo apt-get install libsdl2-dev
sudo apt-get install libglew-dev
```

Demo game
---------
Alternatively, compile `stranded.c` instead of `main.c` for a more comprehensive demo project.


