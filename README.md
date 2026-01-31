# AzxEngineGL

This is my _home made_ graphics engine library.

## How to use

Just add it in your project library like this.

```cmake
cmake_minimum_required(VERSION 4.0)
project(Example)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(AzxEngineGL)

target_link_libraries(Example PRIVATE AzxEngineGL)
```

## Current support

- [x] Multiple lights
- [ ] Lighing: Point and Spot
- [x] Normal mapping
- [ ] Parallax mapping
- [ ] Shadows
- [x] Using Framebuffers
- [ ] Bloom effect
- [ ] Motion Blur effect
- [ ] Fresnel effect
- [ ] SSAO
- [x] Grid floor
- [ ] Particle system

## My Projects using this

### [OpenGL Playground](https://github.com/Azrielx86/OpenGL_Playground)
This is the project where I develop and test it.
![OpenGL Playground Image](https://github.com/Azrielx86/OpenGL_Playground/raw/main/docs/img/window.png)

### [Advanced Computer Graphics Final Project](https://github.com/Azrielx86/ProyectoFinal_CGA)
An endless runner game.
![CGA Image](https://github.com/Azrielx86/ProyectoFinal_CGA/raw/main/docs/debug.png)
