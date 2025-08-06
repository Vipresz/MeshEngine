FROM ubuntu:22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV MINGW_PREFIX=x86_64-w64-mingw32
ENV CC=${MINGW_PREFIX}-gcc
ENV CXX=${MINGW_PREFIX}-g++
ENV AR=${MINGW_PREFIX}-ar
ENV STRIP=${MINGW_PREFIX}-strip
ENV RANLIB=${MINGW_PREFIX}-ranlib
ENV PKG_CONFIG_PATH=/usr/${MINGW_PREFIX}/lib/pkgconfig

# Update and install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    unzip \
    tar \
    pkg-config \
    mingw-w64 \
    mingw-w64-tools \
    mingw-w64-x86-64-dev \
    python3 \
    && rm -rf /var/lib/apt/lists/*

# Create mingw directory and toolchain file
RUN mkdir -p /usr/share/mingw && \
    echo 'set(CMAKE_SYSTEM_NAME Windows)' > /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
    && echo 'set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)' >> /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
    && echo 'set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)' >> /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
    && echo 'set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)' >> /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
    && echo 'set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)' >> /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
    && echo 'set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)' >> /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
    && echo 'set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)' >> /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
    && echo 'set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)' >> /usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake

# Install GLM (header-only library)
RUN wget -O glm.zip https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip \
    && unzip glm.zip \
    && cp -r glm/glm /usr/${MINGW_PREFIX}/include/ \
    && rm -rf glm glm.zip

# Install GLFW for MinGW
RUN wget -O glfw.zip https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip \
    && unzip glfw.zip \
    && cd glfw-3.3.8 \
    && mkdir build && cd build \
    && cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=/usr/share/mingw/toolchain-${MINGW_PREFIX}.cmake \
        -DGLFW_BUILD_DOCS=OFF \
        -DGLFW_BUILD_TESTS=OFF \
        -DGLFW_BUILD_EXAMPLES=OFF \
        -DCMAKE_INSTALL_PREFIX=/usr/${MINGW_PREFIX} \
    && make -j$(nproc) \
    && make install \
    && cd ../.. \
    && rm -rf glfw-3.3.8 glfw.zip

# Install Dear ImGui
RUN wget -O imgui.zip https://github.com/ocornut/imgui/archive/refs/tags/v1.89.9.zip \
    && unzip imgui.zip \
    && cp -r imgui-1.89.9/*.h /usr/${MINGW_PREFIX}/include/ \
    && cp -r imgui-1.89.9/*.cpp /usr/${MINGW_PREFIX}/lib/ \
    && cp -r imgui-1.89.9/backends/imgui_impl_glfw.* /usr/${MINGW_PREFIX}/include/ \
    && cp -r imgui-1.89.9/backends/imgui_impl_opengl3.* /usr/${MINGW_PREFIX}/include/ \
    && rm -rf imgui-1.89.9 imgui.zip

# Set working directory
WORKDIR /workspace

# Copy build script
COPY build_docker.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/build_docker.sh

# Default command
CMD ["/bin/bash"] 