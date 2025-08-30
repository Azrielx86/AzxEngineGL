# AzxEngineGL

This is my _home made_ graphics engine library.

Based on my [OpenGL Playground](https://github.com/Azrielx86/OpenGL_Playground_Base) project to use it on multiple programs.

## How to use

Just add it in your project library like this.

```cmake
cmake_minimum_required(VERSION 4.0)
project(OpenGL_Playground VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(AzxEngineGL)
add_subdirectory(OpenGL_Playground)
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
