cmake_minimum_required(VERSION 4.0)
project(AzxEngineGL)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLM_ENABLE_EXPERIMENTAL")

if (ENABLE_LOG)
    message(STATUS "Adding extra logs.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_LOG")
endif ()

if (USE_DEBUG_ASSETS)
    message(STATUS "Using debug assets (not copying to binary dir).")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_DEBUG_ASSETS")
endif ()

if (ENABLE_GL_DEBUG)
    message(STATUS "Adding GL Debug.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLE_GL_DEBUG")
endif ()

if (NOT UNIX)
    message(STATUS "Compiling Windows version")
    add_compile_definitions(-DWIN32)
endif ()

if (VCPKG_TOOLCHAIN)
    message(STATUS "Using vcpkg Package Manager")

    set(OpenGL_GL_PREFERENCE GLVND)

    find_package(glm CONFIG REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(assimp CONFIG REQUIRED)
    find_package(Stb REQUIRED)
    find_package(imgui CONFIG REQUIRED)

    include_directories(${Stb_INCLUDE_DIR})
else ()
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(glm REQUIRED)

    # If Assimp from your package manager is bugged and u dont want to compile it and install system-wide.
    # Also useful if your debugger is not loading the debug symbols.
    if (FETCH_EXTERNAL_ASSIMP)
        message(STATUS "Using external Assimp.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nontrivial-memcall")
        FetchContent_Declare(assimp
                GIT_REPOSITORY https://github.com/assimp/assimp.git
                GIT_TAG v5.4.3)
        set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
        set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(assimp)
    else ()
        find_package(assimp REQUIRED)
    endif ()

    # imgui
    FetchContent_Declare(imgui_external
            URL https://github.com/ocornut/imgui/archive/refs/tags/v1.92.3.tar.gz
            EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(imgui_external)

    add_library(imgui
            ${imgui_external_SOURCE_DIR}/imgui.cpp
            ${imgui_external_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_external_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_external_SOURCE_DIR}/imgui_widgets.cpp
            ${imgui_external_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_external_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )

    target_include_directories(imgui PUBLIC ${imgui_external_SOURCE_DIR})
    include_directories(${imgui_external_SOURCE_DIR})
    include_directories(${imgui_external_SOURCE_DIR}/backends)
    include_directories("/usr/include/stb")
endif ()

add_compile_options(-Wall -Wextra -Wconversion -Wdouble-promotion -Wno-sign-conversion -Wno-unknown-pragmas -Wuninitialized)
add_link_options(-Wall -Wextra -Wconversion -Wdouble-promotion -Wno-sign-conversion -Wno-unknown-pragmas -Wuninitialized)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Compiling Debug version.")
    add_compile_options(-O0 -gfull -DDEBUG -fno-inline -fstack-protector-all -fsanitize=address,undefined -fsanitize-trap)
    add_link_options(-O0 -gfull -DDEBUG -fno-inline -fstack-protector-all -fsanitize=address,undefined -fsanitize-trap)
else ()
    add_compile_options(-O3)
endif ()

add_library(AzxEngineGL STATIC
        src/Window.cpp
        src/Shader.cpp
        src/Shader.h
        src/Camera.cpp
        src/Input/Keyboard.cpp
        src/Input/Keyboard.h
        src/Mesh.cpp
        src/Mesh.h
        src/Model.cpp
        src/Model.h
        src/Input/Mouse.cpp
        src/Input/Mouse.h
        src/GlobalDefines.h
        src/Resources/ResourceManager.cpp
        src/Resources/ResourceManager.h
        src/Resources/Material.cpp
        src/Resources/Material.h
        src/Skybox.cpp
        src/Skybox.h
        src/Resources/Texture.cpp
        src/Resources/Texture.h
        src/Framebuffer.cpp
        src/Framebuffer.h
        src/Lights/PointLight.h
        src/StorageBufferDynamicArray.cpp
        src/StorageBufferDynamicArray.h
        src/Gizmos/GizmoBase.h
        src/ECS/Registry.cpp
        src/ECS/Registry.h
        src/ECS/Types.h
        src/ECS/Components/Transform.h
        src/ECS/Components/MeshRenderer.h
        src/ECS/ISystem.h
        src/TimeManager.cpp
        src/TimeManager.h
        src/ECS/ComponentArray.cpp
        src/ECS/ComponentArray.h
        src/ECS/IComponentArray.h
        src/ECS/Systems/RenderSystem.h
        src/ECS/Systems/RenderSystem.cpp
        src/ECS/SystemManager.cpp
        src/ECS/SystemManager.h
        src/ECS/Systems/PlayerControlSystem.cpp
        src/ECS/Systems/PlayerControlSystem.h
        src/ECS/Components/PlayerController.h
        src/Utils.h
        src/Bone.cpp
        src/Bone.h
        src/Assimp2Glm.cpp
        src/Assimp2Glm.h
        src/ECS/Components/SkinnedMeshRenderer.h
        src/ECS/Components/Skeleton.h
        src/SkinnedAnimation.cpp
        src/SkinnedAnimation.h
        src/SkinnedAnimator.cpp
        src/SkinnedAnimator.h
        src/BoneInfo.h
        src/ECS/Components/Collider.h
        src/ECS/Components/Collider.cpp
        src/ECS/Systems/CollisionSystem.cpp
        src/ECS/Systems/CollisionSystem.h
        src/Primitives/AbstractPrimitive.cpp
        src/Primitives/AbstractPrimitive.h
        src/Primitives/Plane.cpp
        src/Primitives/Plane.h
        src/Primitives/Cube.cpp
        src/Primitives/Cube.h
)

target_include_directories(AzxEngineGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (VCPKG_TOOLCHAIN)
    target_link_libraries(AzxEngineGL PUBLIC OpenGL::GL)
    target_include_directories(AzxEngineGL PUBLIC ${Stb_INCLUDE_DIR})
    target_link_libraries(AzxEngineGL PUBLIC imgui::imgui)
else ()
    target_link_libraries(AzxEngineGL PUBLIC OpenGL)
    target_link_libraries(AzxEngineGL PUBLIC imgui)
endif ()
target_link_libraries(AzxEngineGL PUBLIC glfw)
target_link_libraries(AzxEngineGL PUBLIC GLEW::GLEW)
target_link_libraries(AzxEngineGL PUBLIC glm::glm)
target_link_libraries(AzxEngineGL PUBLIC assimp::assimp)
