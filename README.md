# ZEngine

![screenshot](./screenshot.png)

ZEngine is a hobby Game Engine. Initially undertaken as a university assignment and has since evolved into an expansive hobby project. The engine is developed primarily in C++, leveraging CMake as its build system. The project consists of three major components:

* ZEngine-Core: The core library that provides the fundamental functionalities of the engine.
* ZEngine-Editor: A user-friendly UI for map editing.
* ZEngine-Player: A runtime environment for playing the game.

## Prerequisites

ZEngine utilizes a variety of libraries, each serving specific purposes to ensure efficient and seamless functionality. These include:

* .NET 6.0: Scripting.
  * `coreclr`: .NET runtime.
  * `nethost`: Setup and launch .NET runtime.
* `assimp`: Model loading.
* `bgfx`: Platform agnostic rendering.
* `imgui`: UI rendering.
* `sdl2` + `sdl2_mixer`: Audio.
* `glfw`: Platform agnostic window & input management.
* `glm`: Math operations.
* `stb-image`: Image loading.
* `nlohmann-json`: For serializing and deserializing JSON data.
* `Nvidia PhysX 5`: 3D Physics engine.

The project incorporates git submodules for managing some of these libraries. Therefore, initialization of these submodules is necessary.

Otherwise you need to install the libraries using `brew` or `apt-get` or whatever package manager you use.

## Building the Project

First you need to build PhysX. To do this run the following commands:

```shell
cd 3rdparty/PhysX/physx
./generate_projects.sh

cmake --build . --config checked
```

Then to build the project, you will need to execute the following commands:

```shell
mkdir build
cd build
cmake ../
make -j12
```

## Platform Compatibility

ZEngine has primarily been developed and tested on MacOS.
Currently other platforms are not supported. However, the engine is designed to be platform agnostic and should be able to run on other platforms with some effort.
