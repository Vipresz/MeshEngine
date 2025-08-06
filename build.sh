#!/bin/bash

echo "Building MeshEngine for Windows using Docker..."

# Create output directory
mkdir -p output

# Build the Docker image
echo "Building Docker image..."
docker build -t meshengine-builder .

if [ $? -ne 0 ]; then
    echo "Docker build failed!"
    exit 1
fi

# Run the build inside Docker
echo "Running build inside Docker..."
docker run --rm -v $(pwd):/workspace -v $(pwd)/output:/workspace/output meshengine-builder /usr/local/bin/build_docker.sh

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

# Copy the executable to output directory
echo "Copying executable to output directory..."
docker run --rm -v $(pwd):/workspace -v $(pwd)/output:/workspace/output meshengine-builder bash -c "cp build/MeshEngine.exe output/"

echo "Build completed successfully!"
echo "Windows executable is available in the output directory:"
ls -la output/ 