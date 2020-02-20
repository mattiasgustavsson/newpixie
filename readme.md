![build](https://github.com/mattiasgustavsson/newpixie/workflows/build/badge.svg)

Pixie Game Engine
=================

Experimenting with new version of my game engine Pixie


Build instructions
------------------

Windows: from a Visual Studio command prompt, go to the `runtime` folder and do
```
cl ..\source\main.c
```

Mac: go to the `runtime` folder and do
```
clang ../source/main.c -lSDL2 -lGLEW -framework OpenGL
```

Linux: go to the `runtime` folder and do
```
gcc ../source/main.c -lSDL2 -lGLEW -lGL -lm -lpthread
```

Alternatively, compile `stranded.c` instead of `main.c` for a more comprehensive demo project.
