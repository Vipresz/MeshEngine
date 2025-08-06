cmake_minimum_required(VERSION 3.10)
project(MeshEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find packages
find_package(PkgConfig REQUIRED)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/include)
include_directories(${CMAKE_SOURCE_DIR}/include/glm)
include_directories(${CMAKE_SOURCE_DIR}/include/glfw)
include_directories(${CMAKE_SOURCE_DIR}/include/imgui)

# Source files
set(SOURCES
    src/main.cpp
    src/Application.cpp
    src/Scene.cpp
    src/Camera.cpp
    src/Point.cpp
    src/Line.cpp
    src/Shader.cpp
    src/glad.c
    src/Renderer.cpp
    src/UIComponent.cpp
)

# Create executable
add_executable(MeshEngine ${SOURCES})

# Set output directory to avoid permission issues
set_target_properties(MeshEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Platform-specific settings
if(MINGW OR CMAKE_CROSSCOMPILING)
    # Windows-specific settings for MinGW cross-compilation
    target_compile_definitions(MeshEngine PRIVATE WIN32_LEAN_AND_MEAN)
    target_compile_options(MeshEngine PRIVATE -O3 -DNDEBUG)
    
    # Static linking for MinGW
    set_target_properties(MeshEngine PROPERTIES
        LINK_FLAGS "-static-libgcc -static-libstdc++"
    )
    
    # Windows-specific libraries
    target_link_libraries(MeshEngine
        -lglfw3
        -lopengl32
        -lgdi32
        -luser32
        -lkernel32
        -lwinmm
        -lws2_32
        -lpsapi
        -limm32
        -lole32
        -loleaut32
        -luuid
        -lcomctl32
        -lcomdlg32
        -ldwmapi
        -lsetupapi
        -lhid
        -lwininet
        -lwsock32
        -lwinmm
        -lversion
        -lole32
        -loleaut32
        -luuid
        -lcomctl32
        -lcomdlg32
        -ldwmapi
        -lsetupapi
        -lhid
        -lwininet
        -lwsock32
        -lwinmm
        -lversion
    )
else()
    # Native build (Linux/macOS)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    
    target_link_libraries(MeshEngine
        OpenGL::GL
        glfw
    )
endif() 