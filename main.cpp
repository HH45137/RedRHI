#include <iostream>
#include <SDL3/SDL.h>
#include "RedRHI/RedRHI.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *window = SDL_CreateWindow(
        "RHI Renderer Window",
        1920,
        1080,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("Failed to create window.");
    }

    SDL_GLContext opengl_context = SDL_GL_CreateContext(window);
    if (!opengl_context) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Failed to create OpenGL context.");
    }

    RedRHIDevice *rhi_device = new RedOpenGLDevice();
    rhi_device->Init();

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        rhi_device->DrawFrame();

        SDL_GL_SwapWindow(window);
    }

    rhi_device->Destroy();

    SDL_Quit();
    return 0;
}
