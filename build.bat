@echo off
echo Building MeshEngine for Windows using Docker...

REM Create output directory
if not exist output mkdir output

REM Build the Docker image
echo Building Docker image...
docker build -t meshengine-builder .

if %ERRORLEVEL% NEQ 0 (
    echo Docker build failed!
    pause
    exit /b 1
)

REM Run the build inside Docker
echo Running build inside Docker...
docker run --rm -v %cd%:/workspace -v %cd%/output:/workspace/output meshengine-builder /usr/local/bin/build_docker.sh

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)

REM Copy the executable to output directory
echo Copying executable to output directory...
docker run --rm -v %cd%:/workspace -v %cd%/output:/workspace/output meshengine-builder bash -c "cp build/MeshEngine.exe output/"

echo Build completed successfully!
echo Windows executable is available in the output directory:
dir output

pause 