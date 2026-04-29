#include <iostream>
#include <SDL3/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
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
    RedRHIBuffer *index_buffer = nullptr;
    RedRHITexture *texture = nullptr;
    RedRHIShader *shader_program = nullptr;
    {
        const std::string ASSETS_ROOT = "../Assets/";

        /*objl::Loader loader;
        loader.LoadFile(ASSETS_ROOT + "6Hz/6Hz.obj");
        std::cout << loader.LoadedMeshes[0].MeshName << std::endl;*/

        std::vector<float> vertices = {
            0.5f, 0.5f, 0.0f, // top right
            0.5f, -0.5f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f // top left
        };

        std::vector<float> indices = {
            0, 1, 3, // first Triangle
            1, 2, 3 // second Triangle
        };

        vertex_buffer = rhi_device->CreateBuffer(
            RED_RHI_BUFFER_USAGE_VERTEX,
            RED_RHI_MEMORY_TYPE_DEVICE,
            vertices.size(),
            vertices.data()
        );

        index_buffer = rhi_device->CreateBuffer(
            RED_RHI_BUFFER_USAGE_INDEX,
            RED_RHI_MEMORY_TYPE_DEVICE,
            indices.size(),
            indices.data()
        );

        int32_t width, height, channels;
        unsigned char *image_data = stbi_load(
            (ASSETS_ROOT + "6Hz/6Hz.png").c_str(),
            &width, &height, &channels, 0
        );
        texture = rhi_device->CreateTexture(
            RED_RHI_TEXTURE_FORMAT_RGB_8,
            RED_RHI_TEXTURE_SAMPLER_TYPE_LINEAR,
            RED_RHI_TEXTURE_ADDRESS_TYPE_REPEAT,
            width,
            height,
            8
        );
        stbi_image_free(image_data);

        std::string vertex_shader_src = "#version 460 core\n"
                "layout (location = 0) in vec3 aPos;\n"
                "void main()\n"
                "{\n"
                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                "}\0";
        std::string fragment_shader_src = "#version 460 core\n"
                "out vec4 FragColor;\n"
                "void main()\n"
                "{\n"
                "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                "}\n\0";
        RedRHIShader *vertex_shader = rhi_device->CreateShader(vertex_shader_src, RED_RHI_SHADER_STAGE_VERTEX);
        RedRHIShader *fragment_shader = rhi_device->CreateShader(fragment_shader_src, RED_RHI_SHADER_STAGE_FRAGMENT);
        shader_program = rhi_device->CreateShader(vertex_shader, fragment_shader);
    }

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        // Draw
        {
            rhi_device->ClearColor(0.2, 0.4, 0.6);
            rhi_device->ClearFrameBuffer();
        }

        SDL_GL_SwapWindow(window);
    }

    rhi_device->DestroyShader(shader_program);
    rhi_device->DestroyBuffer(vertex_buffer);
    rhi_device->DestroyBuffer(index_buffer);
    rhi_device->Destroy();

    SDL_Quit();
    return 0;
}
