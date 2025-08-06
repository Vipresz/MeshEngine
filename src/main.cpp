#include "Application.h"
#include <iostream>

int main() {
    Application app(1200, 800, "MeshEngine - 3D Point & Line Editor");
    
    if (!app.Initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    
    app.Run();
    app.Shutdown();
    
    return 0;
} 