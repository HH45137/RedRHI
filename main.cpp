#include <iostream>
#include <SDL3/SDL.h>
#include <stb_image.h>
#include <OBJ_Loader.h>

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
    auto adapter_info = rhi_device->GetAdapterInfo();

    // Ready
    RedRHIBuffer *vertex_buffer = nullptr;
    RedRHITexture *texture = nullptr;
    {
        std::vector<float> vertices = {

        };

        {
            const std::string ASSETS_ROOT = "../assets/";

            objl::Loader loader;
            loader.LoadFile(ASSETS_ROOT + "6Hz/6Hz.obj");
            std::cout << loader.LoadedMeshes[0].MeshName << std::endl;
        }

        vertex_buffer = rhi_device->CreateBuffer(
            RED_RHI_BUFFER_USAGE_VERTEX,
            RED_RHI_MEMORY_TYPE_DEVICE,
            vertices.size(),
            3,
            vertices.data()
        );

        texture = rhi_device->CreateTexture(
            RED_RHI_TEXTURE_FORMAT_RGB_8,
            RED_RHI_TEXTURE_SAMPLER_TYPE_LINEAR,
            RED_RHI_TEXTURE_ADDRESS_TYPE_REPEAT,
            1024,
            1024,
            8
        );
    }

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        // Draw
        {
            rhi_device->DrawFrame();
        }

        SDL_GL_SwapWindow(window);
    }

    rhi_device->Destroy();

    SDL_Quit();
    return 0;
}
