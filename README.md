# MeshEngine - 3D Point & Line Editor

A C++ application for creating and editing 3D points and lines through an intuitive GUI interface.

## Features

- **3D Point Creation**: Click in 3D space to spawn points
- **Line Connection**: Connect points with lines to create 3D meshes
- **Interactive GUI**: Left toolbar with tool selection
- **Menu System**: File and Tools menus at the top
- **Camera Controls**: 
  - Right-click and drag to rotate camera
  - Mouse wheel to zoom in/out
- **Real-time Rendering**: OpenGL-based 3D rendering

## Controls

- **Left Mouse Button**: Place points (when Point tool is selected)
- **Right Mouse Button**: Rotate camera
- **Mouse Wheel**: Zoom in/out
- **Toolbar Buttons**: Switch between Select, Point, and Line tools

## Building the Project

### Option 1: Docker Build (Recommended)

This approach builds a Windows executable from Linux using MinGW cross-compilation.

#### Prerequisites
- Docker installed on your system
- Git

#### Building with Docker

1. Clone the repository:
```bash
git clone <repository-url>
cd MeshEngine
```

2. Build using Docker:

**Linux/macOS:**
```bash
chmod +x build_docker_host.sh
./build_docker_host.sh
```

**Windows:**
```bash
build_docker_host.bat
```

3. The Windows executable will be available in the `output/` directory.

### Option 2: Native Build

#### Prerequisites
- CMake 3.16 or higher
- C++17 compatible compiler
- OpenGL 3.3 or higher
- GLFW3
- GLM (OpenGL Mathematics)

#### Dependencies

**Windows (using vcpkg):**
```bash
vcpkg install glfw3 glm glad
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install libglfw3-dev libglm-dev
```

**macOS (using Homebrew):**
```bash
brew install glfw glm
```

#### Building

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure with CMake:
```bash
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

4. Run the application:
```bash
./MeshEngine
```

## Project Structure

```
MeshEngine/
├── CMakeLists.txt          # Build configuration
├── include/                # Header files
│   ├── Application.h       # Main application class
│   ├── Scene.h            # 3D scene management
│   ├── Point.h            # 3D point representation
│   ├── Line.h             # 3D line representation
│   ├── Camera.h           # Camera controls
│   └── Shader.h           # OpenGL shader management
├── src/                   # Source files
│   ├── main.cpp           # Application entry point
│   ├── Application.cpp    # Main application implementation
│   ├── Scene.cpp          # Scene management
│   ├── Point.cpp          # Point rendering
│   ├── Line.cpp           # Line rendering
│   ├── Camera.cpp         # Camera implementation
│   └── Shader.cpp         # Shader management
└── external/              # External libraries (ImGui)
    └── imgui/
```

## GUI Layout

- **Top Menu Bar**: File, Tools, and View menus
- **Left Toolbar**: Tool selection buttons and scene information
- **3D Viewport**: Main rendering area for 3D scene

## Tools

- **Select**: Default tool for selecting existing points and lines
- **Point**: Click in 3D space to create new points
- **Line**: Connect existing points to create lines

## Future Enhancements

- File I/O for saving/loading scenes
- More advanced selection and manipulation tools
- Grid and coordinate system display
- Undo/Redo functionality
- Multiple viewport support
- Export to common 3D formats

## License

This project is open source and available under the MIT License. 