# MeshEngine Build Instructions

This project uses Docker to cross-compile a Windows executable from any platform.

## Prerequisites

- Docker installed and running
- Git (to clone the repository)

## Building the Project

### On Windows:
```cmd
build.bat
```

### On Linux/macOS:
```bash
./build.sh
```

## What the Build Process Does

1. **Creates a Docker image** with MinGW toolchain and all required libraries:
   - GLFW (OpenGL framework)
   - GLAD (OpenGL loader)
   - Dear ImGui (GUI library)
   - GLM (math library)

2. **Runs the build inside Docker** using the `build_docker.sh` script which:
   - Configures CMake with MinGW cross-compilation toolchain
   - Builds the project with static linking
   - Creates a standalone Windows executable

3. **Copies the result** to the `output/` directory

## Output

The build process creates:
- `output/MeshEngine.exe` - A standalone Windows executable that can run on any Windows machine

## Troubleshooting

- **Docker not running**: Make sure Docker Desktop is started
- **Build fails**: Check that all source files are present in `src/` and `include/` directories
- **Permission denied**: On Linux/macOS, make sure `build.sh` is executable: `chmod +x build.sh`

## Project Structure

```
MeshEngine/
├── src/           # Source files
├── include/       # Header files
├── build_docker.sh # Script that runs inside Docker
├── build.sh       # Host script for Unix systems
├── build.bat      # Host script for Windows
├── Dockerfile     # Docker environment definition
└── CMakeLists.txt # CMake build configuration
``` 